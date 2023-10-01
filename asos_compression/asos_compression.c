/****************************************************************************************
* ALI SAHBAZ
*
*
* SIMPLE AND FAST COMPRESSION - ASOS
*
* Date          : 30.09.2023
* By            : Ali Sahbaz
* e-mail        : ali_sahbaz@outlook.com
*/
#include <stdio.h>
#include <string.h>
#include "asos_compression.h"

#define QUANTIZATION_DEFAULT_VALUE		COMP_ASCII_Q_VAL

#define MAX_ALLOCATION_SIZE				(1000)  // ? TODO
 
#define OBJECT_STM_CHECK(STM , out)		do{ if( STM == 1 ){ return out; }   }while(0);  /** STATEMENT CHECK for SPECIAL THIS API */

#define MAX_BYTE						(0xFF)



/** @proAPIConstructorsFunctions *********************************/

asos_comp_fout_e asos_compression_create(asos_comp_t* me) 
{
	asos_comp_fout_e fout = ASOS_COMP_SUCCESS;

	OBJECT_STM_CHECK(me == NULL , ASOS_COMP_FAIL);

	me->is_init = 1;

	me->config.quantization_value = QUANTIZATION_DEFAULT_VALUE;

	return fout;
}

asos_comp_fout_e asos_compression_delete(asos_comp_t* me)
{
	asos_comp_fout_e fout = ASOS_COMP_SUCCESS;

	OBJECT_STM_CHECK(me->is_init != 1 , ASOS_COMP_FAIL);

	me->is_init = 0;

	return fout;
}

asos_comp_fout_e asos_compression_config_load(asos_comp_t* me, const asos_comp_config_t* config)
{
	asos_comp_fout_e fout = ASOS_COMP_SUCCESS;

	OBJECT_STM_CHECK((me == NULL) || (config == NULL), ASOS_COMP_FAIL);
	  
	OBJECT_STM_CHECK(me->is_init != 1, ASOS_COMP_FAIL);

	me->config.quantization_value = config->quantization_value;

	if (me->config.quantization_value == 0) {
		me->config.quantization_value = 1; 
		return ASOS_COMP_WARNING;
	}

	return fout;
} 

static unsigned char _get_max_count(asos_comp_t* me) {
	return (unsigned char)(MAX_BYTE / me->config.quantization_value);
}

static unsigned char _get_shifter_val(asos_comp_t* me) {

	unsigned char count = _get_max_count(me);

	unsigned char finder = 8 , shifter = 0x80;

	do {  
		if ((count & shifter) == shifter) { 
			return finder;
		}
		shifter = shifter >> 1;
		finder--;
	} while (finder);

	return finder;
}


static unsigned char _get_val_protection(asos_comp_t* me) {

	unsigned char shifter = _get_shifter_val(me);
	 
	unsigned char out = 0x00 , cntcross = 0x80;

	int i = 0;

	for (i = 0; i < shifter; i++) {

		out |= cntcross;

		cntcross = cntcross >> 1;
	}
	  
	return out;
}


static unsigned char _get_count_protection(asos_comp_t* me) {

	unsigned char shifter = (unsigned char)(8 - _get_shifter_val(me));

	unsigned char out = 0x00, cntcross = 0x01;

	int i = 0;

	for (i = 0; i < shifter; i++) {

		out |= cntcross;

		cntcross = cntcross << 1;
	}

	return out;
}



static void _quantization_process(asos_comp_t* me,
									const unsigned char* input_buffer, unsigned int input_buffer_size,
									unsigned char* output_buffer, unsigned int output_buffer_size)
{
	int i = 0;
	for (i = 0; i < input_buffer_size; i++) {
		if (i > output_buffer_size-1) { 
			break; 
		}
		output_buffer[i] = (unsigned char)(input_buffer[i] / me->config.quantization_value);
	}
}

 
 

/** return is lenght for comp buffer ***/
static unsigned int _run_length_encoding_reverse(asos_comp_t* me,
												 const unsigned char* input_buffer, unsigned int input_buffer_size,
												 unsigned char* output_buffer, unsigned int output_buffer_size)
{
	unsigned int i = 0, t = 0 , k = 0;

	for (i = 0; i < input_buffer_size; i++) {

		unsigned char repeat_count = (unsigned char)(input_buffer[i] & _get_count_protection(me) );

		unsigned short val = (unsigned short)((input_buffer[i] >> (8-_get_shifter_val(me))) * me->config.quantization_value);

		if (val > MAX_BYTE) {
			val = MAX_BYTE;
		} 

		for (k = 0; k < repeat_count; k++) {

			output_buffer[t] = (unsigned char)val;

			t++;

			if (t > output_buffer_size - 1) { 
				goto exit_l; 
			}
		}

		if (repeat_count == 0) {
			output_buffer[t] = (unsigned char)val;

			t++;

			if (t > output_buffer_size - 1) {
				goto exit_l;
			}
		}
	}
	exit_l:
	return t;
}




/** return is lenght for comp buffer ***/
static unsigned int _run_length_encoding(asos_comp_t* me,
										 const unsigned char* input_buffer, unsigned int input_buffer_size,
										 unsigned char* output_buffer, unsigned int output_buffer_size)
{
	unsigned int i = 0 , t = 0;

	unsigned char same_count = 1;

	unsigned char max_count = (unsigned char)(8 - _get_shifter_val(me));

	unsigned char same_holder = input_buffer[0];

	for (i = 1; i < input_buffer_size; i++) {

		if (i > output_buffer_size-1) { 
			break; 
		}

		if ((same_holder == input_buffer[i])  && (same_count != max_count)) {
			same_count++;
		}
		else  {

			unsigned char holdx = input_buffer[i];  /** maybe input_buffer and output_buffer are in same adress **/
			 
			printf("RLE for val: %d , record count: %d \n ", same_holder, same_count); // test

			output_buffer[t] = ((same_holder << (8- _get_shifter_val(me))) & _get_val_protection(me) ) |     
							   (same_count & _get_count_protection(me));    

			t++; 
			if (i == input_buffer_size - 1) { 
				break; 
			}
			same_holder = holdx;
			same_count = 1;
		}
		
	}

	output_buffer[t] = ((input_buffer[input_buffer_size-1] << (8- _get_shifter_val(me))) & _get_val_protection(me)) |
						(same_count & _get_count_protection(me));
	 
	return t;
}



// test  
static void _buffer_visualize(const char* name, const unsigned char* buffer, unsigned int size)
{
	printf(name); printf(" ");
	int i = 0;
	for (i = 0; i < size; i++) {
		printf("%d, ", buffer[i]);
	}
	printf("\n");
}
 

/** @proProcessFunction ***************************************/

unsigned int asos_compress_byte(asos_comp_t* me,
							   const unsigned char* input_buffer, unsigned int input_buffer_size,
							   unsigned char* output_buffer, unsigned int output_buffer_size)
{ 
	OBJECT_STM_CHECK((me == NULL) || (input_buffer == NULL ) || (output_buffer == NULL) , 0);

	OBJECT_STM_CHECK(me->is_init != 1, 0);

	OBJECT_STM_CHECK( (input_buffer_size < 2) || (output_buffer_size < 2), 0);

	OBJECT_STM_CHECK((input_buffer_size > MAX_ALLOCATION_SIZE) || (output_buffer_size > MAX_ALLOCATION_SIZE), 0);

	unsigned char q_buf[MAX_ALLOCATION_SIZE];

	memset(q_buf, 0, MAX_ALLOCATION_SIZE);
	 
	_quantization_process(me, input_buffer, input_buffer_size, (unsigned char*)q_buf, MAX_ALLOCATION_SIZE);
	  
	_buffer_visualize("Q               " , q_buf , input_buffer_size);

	return _run_length_encoding(me, (const unsigned char*)q_buf, input_buffer_size, ( unsigned char*)output_buffer, output_buffer_size);
}

unsigned int asos_uncompress_byte(asos_comp_t* me,
								 const unsigned char* input_buffer, unsigned int input_buffer_size,
								 unsigned char* output_buffer, unsigned int output_buffer_size)
{ 
	OBJECT_STM_CHECK((me == NULL) || (input_buffer == NULL) || (output_buffer == NULL), 0);

	OBJECT_STM_CHECK(me->is_init != 1, 0);

	OBJECT_STM_CHECK((input_buffer_size < 2) || (output_buffer_size < 2), 0);
	 
	OBJECT_STM_CHECK( (input_buffer_size > MAX_ALLOCATION_SIZE )|| (output_buffer_size > MAX_ALLOCATION_SIZE), 0);
	  
	return _run_length_encoding_reverse(me, (const unsigned char*)input_buffer, input_buffer_size, (unsigned char*)output_buffer, output_buffer_size);
}


