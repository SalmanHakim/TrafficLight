//source file that has a function that initialise the ports
#include "gpio_init.h"

void initGpio(void)
{

	XGpio_Initialize(&REGION_0_COLOUR, 7);
	XGpio_Initialize(&REGION_1_COLOUR, 10);
	XGpio_Initialize(&REGION_2_COLOUR, 11);
	XGpio_Initialize(&REGION_4_COLOUR, 13);
	XGpio_Initialize(&REGION_6_COLOUR, 15);
	XGpio_Initialize(&REGION_7_COLOUR, 16);
	XGpio_Initialize(&REGION_8_COLOUR, 17);
	XGpio_Initialize(&SEG7_HEX_OUT, 0);
	XGpio_Initialize(&SEG7_SEL_OUT, 1);
	XGpio_Initialize(&LED_OUT, 2);
	XGpio_Initialize(&BTNU_IN, 6);
}
