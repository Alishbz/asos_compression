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
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include "compression_test_app.h"
#include "asos_compression.h"

 

#define CONFIQ_QUANTIZATION_LEVEL   11//17 // 17


static asos_comp_t comp_object;
 

static void _buffer_visualize(const char* name, const unsigned char* buffer, unsigned int size)
{
	printf(name); printf(" ");
	int i = 0;
	for (i = 0; i < size; i++) {
		printf("%d, " , buffer[i]);
	} 
	printf("\n");
}

static void _buffer_random_load(  unsigned char* buffer, unsigned int size)
{
	srand(time(NULL));
	int i = 0;

	for (i = 0; i < size; i++) {
		buffer[i] = (unsigned char)(rand() % 15); // FOR SÝMULATE ACC
		buffer[i] = (unsigned char)(buffer[i] * 15);
	}
}


void comp_test_singleton_init(void)
{
	asos_compression_create(&comp_object);

	asos_comp_config_t comp_cfg; 
	comp_cfg.quantization_value = CONFIQ_QUANTIZATION_LEVEL;
	asos_compression_config_load(&comp_object , &comp_cfg);
}

void comp_test_1_run(void)
{
	printf(" \n\nTest1 Starting...\n\n");

	#define OUTPUT_BUF_SIZE    (20)
	#define INPUT_BUF_SIZE     (20)

	unsigned char in_buf[INPUT_BUF_SIZE] = { 
		15 , 16 , 14 , 15 , 17 , 14 , 13 , 19 , 25 , 30 , 
		32 , 33 , 25 , 24 , 15 , 15 , 12 , 11 , 13 , 14 
	};

	//_buffer_random_load((unsigned char*)in_buf, INPUT_BUF_SIZE);
	 
	_buffer_visualize("input buffer          " , (const unsigned char*)in_buf , INPUT_BUF_SIZE);

	unsigned char out_buf[OUTPUT_BUF_SIZE]; memset(out_buf , 0 , OUTPUT_BUF_SIZE);

	asos_compress_byte(&comp_object, (const unsigned char*)in_buf, INPUT_BUF_SIZE, (unsigned char*)out_buf, OUTPUT_BUF_SIZE);

	_buffer_visualize("asos comp out buffer  ", (const unsigned char*)out_buf, OUTPUT_BUF_SIZE);
	 
	memset(in_buf, 0, INPUT_BUF_SIZE);

	asos_uncompress_byte(&comp_object, (const unsigned char*)out_buf, OUTPUT_BUF_SIZE, (unsigned char*)in_buf, INPUT_BUF_SIZE);

	_buffer_visualize("asos uncomp out buffer", (const unsigned char*)in_buf, INPUT_BUF_SIZE);

	printf(" \nTest1 end...\n\n\n");
}

void comp_test_2_run(void)
{
	printf(" \n\nTest2 Starting...\n\n");

	#define OUTPUT_BUF_SIZE    (50)
	#define INPUT_BUF_SIZE     (50)

	unsigned char in_buf[INPUT_BUF_SIZE];

	_buffer_random_load((unsigned char*)in_buf, INPUT_BUF_SIZE);

	_buffer_visualize("input buffer          ", (const unsigned char*)in_buf, INPUT_BUF_SIZE);

	unsigned char out_buf[OUTPUT_BUF_SIZE]; memset(out_buf, 0, OUTPUT_BUF_SIZE);

	asos_compress_byte(&comp_object, (unsigned char*)in_buf, INPUT_BUF_SIZE, (unsigned char*)out_buf, OUTPUT_BUF_SIZE);

	_buffer_visualize("asos comp out buffer  ", (const unsigned char*)out_buf, OUTPUT_BUF_SIZE);

	memset(in_buf, 0, INPUT_BUF_SIZE);

	asos_uncompress_byte(&comp_object, (const unsigned char*)out_buf, OUTPUT_BUF_SIZE, (unsigned char*)in_buf, INPUT_BUF_SIZE);

	_buffer_visualize("asos uncomp out buffer", (const unsigned char*)in_buf, INPUT_BUF_SIZE);

	printf(" \nTest2 end...\n\n\n");
}

void comp_test_3_run(void)
{
	printf(" \n\nTest3 Starting...\n\n");

	#define OUTPUT_BUF_SIZE    (100)
	#define INPUT_BUF_SIZE     (100)

	unsigned char in_buf[INPUT_BUF_SIZE];

	_buffer_random_load((unsigned char*)in_buf, INPUT_BUF_SIZE);

	_buffer_visualize("input buffer           ", (const unsigned char*)in_buf, INPUT_BUF_SIZE);

	unsigned char out_buf[OUTPUT_BUF_SIZE]; memset(out_buf, 0, OUTPUT_BUF_SIZE);

	asos_compress_byte(&comp_object, (unsigned char*)in_buf, INPUT_BUF_SIZE, (unsigned char*)out_buf, OUTPUT_BUF_SIZE);

	_buffer_visualize("asos comp out buffer   ", (const unsigned char*)out_buf, OUTPUT_BUF_SIZE);

	memset(in_buf, 0, INPUT_BUF_SIZE);

	asos_uncompress_byte(&comp_object, (const unsigned char*)out_buf, OUTPUT_BUF_SIZE, (unsigned char*)in_buf, INPUT_BUF_SIZE);

	_buffer_visualize("asos uncomp out buffer ", (const unsigned char*)in_buf, INPUT_BUF_SIZE);

	printf(" \nTest3 end...\n\n\n");
}


void comp_test_delete(void) 
{
	asos_compression_delete(&comp_object);

	printf("\n\n\nCOMP TEST END...\n\n\n");
}