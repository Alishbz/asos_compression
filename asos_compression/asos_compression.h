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
#pragma once
#ifndef asos_compression_H
#define asos_compression_H

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum {
		ASOS_COMP_SUCCESS = 0,
		ASOS_COMP_WARNING,
		ASOS_COMP_FAIL
	}asos_comp_fout_e;


#define COMP_ASCII_Q_VAL   (17)  
	


	/** @superConfigObject **/
	typedef struct {
		  
		unsigned char quantization_value; /** '0' or '1' means not use **/

		// todo , add more filter options

	}asos_comp_config_t;




	/** @superAPIObject **/
	typedef struct {
		  
		asos_comp_config_t config;

        char is_init;

	}asos_comp_t;




	/** @proAPIConstructorsFunctions *********************************/
	 
	asos_comp_fout_e asos_compression_create(asos_comp_t*  me);

	asos_comp_fout_e asos_compression_delete(asos_comp_t* me);
	 
	asos_comp_fout_e asos_compression_config_load(asos_comp_t* me, const asos_comp_config_t* config);





	/** @proProcessFunction ***************************************/
	 
	// every return are  COMPRESSION PROSES LENGH SIZE

	// byte to byte
	unsigned int asos_compress_byte(asos_comp_t* me , 
		                            const unsigned char* input_buffer , unsigned int input_buffer_size,
		                            unsigned char* output_buffer, unsigned int output_buffer_size);
	 
	// byte to byte
	unsigned int asos_uncompress_byte(asos_comp_t* me,
								        const unsigned char* input_buffer, unsigned int input_buffer_size,
									    unsigned char* output_buffer, unsigned int output_buffer_size);
	 

	// NOT if you wanna compress ASCII than you must just config =>quantization_value = COMP_ASCII_Q_VAL

	// ascii to byte
	//unsigned int asos_compress_ascii(asos_comp_t* me,
	//								const char* input_ascii_buffer, unsigned int input_buffer_size,
	//								unsigned char* output_char_buffer, unsigned int output_buffer_size);


	// byte to ascii
	//unsigned int asos_uncompress_ascii(asos_comp_t* me,
	//									const unsigned char* input_char_buffer, unsigned int input_buffer_size,
	//									char* output_ascii_buffer, unsigned int output_buffer_size);


#ifdef __cplusplus
}
#endif

#endif