//header file for declaration of the name of the ports
#ifndef __GPIO_INIT_H_
#define __GPIO_INIT_H_

#include "xgpio.h"				//xgpio object definitions

void initGpio(void);			//function for initialising the ports
int setUpInterruptSystem();		//function for initialising ISR

XGpio REGION_0_COLOUR;
XGpio REGION_1_COLOUR;
XGpio REGION_2_COLOUR;
XGpio REGION_4_COLOUR;
XGpio REGION_6_COLOUR;
XGpio REGION_7_COLOUR;
XGpio REGION_8_COLOUR;
XGpio SEG7_HEX_OUT;
XGpio SEG7_SEL_OUT;
XGpio LED_OUT;
XGpio BTNU_IN;


#endif
