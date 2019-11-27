#include "timer.h"								// Added for the macros and functions declared
#include "xil_types.h"							// Added for integer type definitions
#include "seg7_display.h"						// Added for the use of displayDigit function

volatile u8 interruptServiced;

volatile u16 gr1;								//green light TR1
volatile u16 ye1;								//yellow light TR1
volatile u16 re1;								//red light TR1

volatile u16 gr2;								//green light TR2
volatile u16 ye2;								//yellow light TR2
volatile u16 re2;								//red light TR2

volatile u16 pd;								//pedestrian light

volatile u8 pdLED = 0;							//LED output indicating pedestrian button has been pressed
volatile u16 LED;								//LED to show traffic light sequence

volatile u8 state = 0;							//state number
volatile u8 count1s = 0;						//counter for interrupt every 1 second
volatile u8 countDisplay = TIME_TL - 1;			//counter for countdown of traffic light and pedestrian light
volatile u8 countBlink = 0;						//counter for blinking

volatile u8 chooseColour = 0;					//indicator for which colour to display when blinking
volatile u8 road = FALSE;						//indicator to choose which road sequence goes next after state 0
volatile u8 pdTrigg = FALSE;					//trigger for pedestrian light


void hwTimerISR(void *CallbackRef)					//hardware timer start
{
	displayDigit();									//function for displaying number at 7-segment display

	//sequential part of the process
	if (count1s == 250) {							//make the interrupt occur every 1 second
		if (countDisplay == 0) {					//wait for countdown to finish

			if (state == 0) {						//state 0
				if (road)							//choose which state and road sequence are happening after
					state = 4;
				else
					state = 1;

				countDisplay = TIME_TL - 1;			//reset countdown for next state
			}

			else if (state == 1) {					//state 1
				state = 2;
				countDisplay = TIME_TL - 1;
			}

			else if (state == 2) {					//state 2
				state = 3;
				countDisplay = TIME_TL - 1;
			}

			else if (state == 3) {					//state 3
				state = 0;
				countDisplay = TIME_TL - 1;
				road = TRUE;						//set road to TRUE for change in sequence for road 2
			}

			else if (state == 4) {					//state 4
				state = 5;
				countDisplay = TIME_TL - 1;
			}

			else if (state == 5) {					//state 5
				state = 6;
				countDisplay = TIME_TL - 1;
			}

			else if (state == 6) {					//state 6
				state = 7;
				countDisplay = TIME_TL - 1;
			}

			else if (state == 7) {					//state 7
				if (pdTrigg) {						//choose which state to go depending on pedestrian trigger
					state = 8;
					countDisplay = TIME_PL - 1;		//reset countdown to next state which is pedestrian light 'ON'
				}
				else {
					state = 1;
					countDisplay = TIME_TL - 1;
				}
			}

			else if (state == 8) {					//state 8
				state = 0;
				countDisplay = TIME_TL - 1;
				road = FALSE;						//set road to FALSE for change in sequence for road 1
				chooseColour = 0;					//set to 0 for initialisation
				countBlink = 0;						//reset counter for consistency
			}
		}
		else
			countDisplay = countDisplay - 1;

		count1s = 0;								//reset "1 second" counter
		}
	else
		count1s = count1s + 1;


	//combinational part of the process
	switch (state) {
		case 0 :							//state 0; TR1 shows red & TR2 shows red
			gr1 = OFF;
			ye1 = OFF;
			re1 = RED;

			gr2 = OFF;
			ye2 = OFF;
			re2 = RED;

			pd = RED;

			LED = 0b1000001000000000;
		break;

		case 1 :							//state 1; TR1 shows red and yellow & TR2 shows red
			gr1 = OFF;
			ye1 = YELLOW;
			re1 = RED;

			gr2 = OFF;
			ye2 = OFF;
			re2 = RED;

			pd = RED;

			LED = 0b1000001100000000;
		break;

		case 2 :							//state 2; TR1 shows green & TR2 shows red
			gr1 = GREEN;
			ye1 = OFF;
			re1 = OFF;

			gr2 = OFF;
			ye2 = OFF;
			re2 = RED;

			pd = RED;

			LED = 0b1000000010000000;
		break;

		case 3 :							//state 3; TR1 shows yellow & TR2 shows red
			gr1 = OFF;
			ye1 = YELLOW;
			re1 = OFF;

			gr2 = OFF;
			ye2 = OFF;
			re2 = RED;

			pd = RED;

			LED = 0b1000000100000000;
		break;

		case 4 :							//state 4; TR1 shows red & TR2 shows red and yellow
			gr1 = OFF;
			ye1 = OFF;
			re1 = RED;

			gr2 = OFF;
			ye2 = YELLOW;
			re2 = RED;

			pd = RED;

			LED = 0b1100001000000000;
		break;

		case 5 :							//state 5; TR1 shows red & TR2 shows green
			gr1 = OFF;
			ye1 = OFF;
			re1 = RED;

			gr2 = GREEN;
			ye2 = OFF;
			re2 = OFF;

			pd = RED;

			LED = 0b0010001000000000;
		break;

		case 6 :							//state 6; TR1 red & TR2 shows yellow
			gr1 = OFF;
			ye1 = OFF;
			re1 = RED;

			gr2 = OFF;
			ye2 = YELLOW;
			re2 = OFF;

			pd = RED;

			LED = 0b0100001000000000;
		break;

		case 7 :							//state 7; TR1 red & TR2 red before pedestrian light
			gr1 = OFF;
			ye1 = OFF;
			re1 = RED;

			gr2 = OFF;
			ye2 = OFF;
			re2 = RED;

			pd = RED;

			LED = 0b1000001000000000;
		break;

		case 8 :										//state 8; Pedestrian light turns green
			if (countDisplay < 2) {						//set blinking effect for the last 2 seconds
				if (countBlink == 45) {					//every 0.18 seconds it will switch colours
					if (chooseColour == 0) {			//0 for OFF
						gr1 = OFF;
						ye1 = OFF;
						re1 = RED;

						gr2 = OFF;
						ye2 = OFF;
						re2 = RED;

						pd = OFF;
						chooseColour = 1;
						LED = 0b1000001000000000;
					}

					else if (chooseColour == 1) {		//1 for GREEN
						gr1 = OFF;
						ye1 = OFF;
						re1 = RED;

						gr2 = OFF;
						ye2 = OFF;
						re2 = RED;

						pd = GREEN;
						chooseColour = 0;
						LED = 0b1000101000000000;
					}

					countBlink = 0;						//reset blinking counter
				}
				countBlink = countBlink + 1;
			}

			else {										//the first 3 seconds of pedestrian light
				gr1 = OFF;
				ye1 = OFF;
				re1 = RED;

				gr2 = OFF;
				ye2 = OFF;
				re2 = RED;

				pd = GREEN;

				LED = 0b1000101000000000;
			}
			pdLED = 0;										//turn the pedestrian indicator light off;
			pdTrigg = FALSE;								//set the pedestrian trigger to false;
		break;												//these will ensure no pedestrian button pressed at this period will be taken into account

		default :
			gr1 = OFF;
			ye1 = OFF;
			re1 = RED;

			gr2 = OFF;
			ye2 = OFF;
			re2 = RED;

			pd = RED;

			LED = 0b1000101000000000;
		break;
	}

	interruptServiced = TRUE;
	return;
}


