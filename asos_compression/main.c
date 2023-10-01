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
#include "compression_test_app.h"

int main() {

    printf("30.09.2023 - Ali Þahbaz \n\nCompression Test Starting...\n\n");

    comp_test_singleton_init();

    comp_test_1_run();

    comp_test_2_run();

    comp_test_3_run();

    comp_test_delete();

    return 0;
}