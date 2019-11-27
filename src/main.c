/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * main.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "gpio_init.h"							//Added for the use of ports name and ISR initialisation
#include "seg7_display.h"						//Added for the use of displayNumber function

volatile u8 interruptServiced = FALSE;

volatile u16 gr1;								//green light TR1
volatile u16 ye1;								//yellow light TR1
volatile u16 re1;								//red light TR1

volatile u16 gr2;								//green light TR2
volatile u16 ye2;								//yellow light TR2
volatile u16 re2;								//red light TR2

volatile u16 pd;								//pedestrian light

volatile u8 pdLED;								//LED output indicating pedestrian button has been pressed
volatile u16 LED;								//LED to show traffic light sequence

volatile u8 countDisplay;						//counter for countdown of traffic light and pedestrian light

volatile u8 pdTrigg;							//trigger for pedestrian light


int main()
{
    init_platform();

    initGpio();											//initialise all ports used
    setUpInterruptSystem();								//initialise interrupt system routine

    u8 btnUIn = 0;										//pedestrian push buttons input

    while (1) {
    	displayNumber(countDisplay);					//display countdown on 7-segment display

    	//pedestrian push button input and LED
    	LED = LED + pdLED;								//LED that is displayed is the traffic light sequence and the pedestrian indicator
    	btnUIn = XGpio_DiscreteRead(&BTNU_IN, 1);		//assign the pedestrian button to a variable
    	XGpio_DiscreteWrite(&LED_OUT, 1, LED);			//display LED

    	if (btnUIn) {									//detect pedestrian button pressed
    		while (btnUIn) {							//detect the release of the button
    	    	btnUIn = XGpio_DiscreteRead(&BTNU_IN, 1);
    			XGpio_DiscreteWrite(&LED_OUT, 1, LED);
    			displayNumber(countDisplay);			//avoid flickering
    			pdLED = 8;								//turns the pedestrian button indicator on
    			pdTrigg = TRUE;							//set pedestrian light trigger to true
    		}
    	}

    	//display all TR2
    	XGpio_DiscreteWrite(&REGION_0_COLOUR, 1, re2);	//red
    	XGpio_DiscreteWrite(&REGION_1_COLOUR, 1, ye2);	//yellow
    	XGpio_DiscreteWrite(&REGION_2_COLOUR, 1, gr2);	//green

    	//display all TR1
    	XGpio_DiscreteWrite(&REGION_6_COLOUR, 1, re1);	//red
    	XGpio_DiscreteWrite(&REGION_7_COLOUR, 1, ye1);	//yellow
    	XGpio_DiscreteWrite(&REGION_8_COLOUR, 1, gr1);	//green

    	//display pedestrian light
    	XGpio_DiscreteWrite(&REGION_4_COLOUR, 1, pd);

    	while (interruptServiced == FALSE);
    }

    cleanup_platform();
    return 0;
}
