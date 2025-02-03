/*
 * led_states.c
 *
 *  Created on: Oct 30, 2024
 *      Author: vinhd
 */


#include "led_states.h"
#include "global.h"



void state_init(){
	HAL_GPIO_WritePin(GPIOB, D3_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, D4_Pin, 0);
}

void state_red(){
	HAL_GPIO_WritePin(GPIOB, D3_Pin, 1);
	HAL_GPIO_WritePin(GPIOB, D4_Pin, 1);
}

void state_green(){
	HAL_GPIO_WritePin(GPIOB, D3_Pin, 1);
	HAL_GPIO_WritePin(GPIOB, D4_Pin, 0);


}

void state_yellow(){
	HAL_GPIO_WritePin(GPIOB, D3_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, D4_Pin, 1);
}


void state_init1(){
	HAL_GPIO_WritePin(GPIOB, D5_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, D6_Pin, 0);
}


void state_red1(){
	HAL_GPIO_WritePin(GPIOB, D5_Pin, 1);
	HAL_GPIO_WritePin(GPIOB, D6_Pin, 1);
}
void state_green1(){
	HAL_GPIO_WritePin(GPIOB, D5_Pin, 1);
	HAL_GPIO_WritePin(GPIOB, D6_Pin, 0);
}
void state_yellow1(){
	HAL_GPIO_WritePin(GPIOB, D5_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, D6_Pin, 1);
}

void blinkind_led_traffic_x(){
	switch(status){
		case AUTO_RED:
			HAL_GPIO_TogglePin(GPIOB, D3_Pin);
			HAL_GPIO_TogglePin(GPIOB, D4_Pin);


			break;
		case AUTO_YELLOW:
			HAL_GPIO_WritePin(GPIOB, D3_Pin, 0);
			HAL_GPIO_TogglePin(GPIOB, D4_Pin);


			break;
		case AUTO_GREEN:
			HAL_GPIO_WritePin(GPIOB, D4_Pin, 0);
			HAL_GPIO_TogglePin(GPIOB, D3_Pin);
			break;
		default:
			break;
		}
}

void blinkind_led_traffic_y(){
	switch(status1){
			case AUTO_RED:
				HAL_GPIO_TogglePin(GPIOB, D5_Pin);
				HAL_GPIO_TogglePin(GPIOB, D6_Pin);


			break;
			case AUTO_YELLOW:
				HAL_GPIO_WritePin(GPIOB, D5_Pin, 0);
				HAL_GPIO_TogglePin(GPIOB, D6_Pin);
			break;
			case AUTO_GREEN:
				HAL_GPIO_WritePin(GPIOB, D6_Pin, 0);
				HAL_GPIO_TogglePin(GPIOB, D5_Pin);
			break;
			default:
			break;
		}
}


