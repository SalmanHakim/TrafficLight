//header file that contain all functions and macros for timer.c
#ifndef TIMER_H_
#define TIMER_H_

//Declaration of functions used in timer.c
void hwTimerISR(void *CallbackRef);

//Definitions for timing of traffic light and pedestrian light
#define		TIME_TL		2
#define		TIME_PL		5

//Definitions for traffic light colour codes
#define		RED			0b111100000000
#define		GREEN		0b000011110000
#define		YELLOW		0b111111110000
#define 	OFF			0b001100110011


#endif /* TIMER_H_ */
