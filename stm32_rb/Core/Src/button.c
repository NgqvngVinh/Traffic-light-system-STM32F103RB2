/*
 * button.c
 *
 *  Created on: Oct 30, 2024
 *      Author: vinhd
 */

//
//#include "button.h"
//
//int button1_flag = 0;
//
//int KeyReg0 = NORMAL_STATE;
//int KeyReg1 = NORMAL_STATE;
//
//int KeyReg2 = NORMAL_STATE; //State hien tai
//int KeyReg3 = NORMAL_STATE; //State da xu ly tu KeyReg2
//int TimerForKeyPress = 200;
//
//
//
//int isButton1Pressed(){
//	if(button1_flag == 1){
//		button1_flag = 0;
//		return 1;
//	}
//	else
//		return 0;
//}
//
//
//void subKeyProcess(){
//	button1_flag = 1;
//}
//
//
//void getKeyInput(){
//	KeyReg0 = KeyReg1;
//	KeyReg1 = KeyReg2;
//	KeyReg2 = HAL_GPIO_ReadPin(A0_GPIO_Port, A0_Pin);
//	if( (KeyReg0 == KeyReg1) && (KeyReg1 == KeyReg2)){
//		if(KeyReg3 != KeyReg2){
//			KeyReg3 = KeyReg2;
//			if(KeyReg2 == PRESSED_STATE){
//				//TODO
//				subKeyProcess();
//				TimerForKeyPress = 200;
//			}
//		}
//		else{
//			TimerForKeyPress--;
//			if(TimerForKeyPress == 0){
//				//TODO
//				KeyReg3 = NORMAL_STATE;
//			}
//		}
//	}
//}


#include <led_7_seg.h>
#include "button.h"
#include "i2c-lcd.h"


static GPIO_TypeDef* buttonPort[NO_OF_BUTTONS] = {A0_GPIO_Port, A1_GPIO_Port, A2_GPIO_Port}; //lưu trữ cổng
static uint16_t buttonPin[NO_OF_BUTTONS] = {A0_Pin, A1_Pin, A2_Pin};							//lưu trữ chân


// Bộ đệm cho việc chống dội
static GPIO_PinState debounceButtonBuffer0[NO_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS];
static GPIO_PinState buttonBuffer[NO_OF_BUTTONS]; // Bộ đệm lưu trạng thái nút sau khi chống dội


//flag
static uint8_t flagForButtonPress[NO_OF_BUTTONS]; // Cờ cho việc nhấn nút hơn 2 giây
static uint8_t flagForButtonPress_2s[NO_OF_BUTTONS]; // Cờ cho việc nhấn nút hơn 2 giây


//counter
static uint16_t counterForButtonPress_2s[NO_OF_BUTTONS] = {0};// Bộ đếm thời gian nhấn nút
//int time_for_button_press = 200;


//vari
enum ButtonState buttonState[NO_OF_BUTTONS];
extern int mode;





void button_init(){
    for(int i = 0; i < NO_OF_BUTTONS; i++){
        buttonState[i] = BUTTON_RELEASED;
        flagForButtonPress[i] = 0;
        flagForButtonPress_2s[i] = 0;
        counterForButtonPress_2s[i] = 0;
        buttonBuffer[i] = BUTTON_IS_RELEASED;
        debounceButtonBuffer0[i] = BUTTON_IS_RELEASED;
        debounceButtonBuffer1[i] = BUTTON_IS_RELEASED;
        debounceButtonBuffer2[i] = BUTTON_IS_RELEASED;
    }
}




//function
int is_button_pressed(uint8_t index){
	if(index >= NO_OF_BUTTONS) return 0;
	if(flagForButtonPress[index]){
		lcd_clear_display();
		flagForButtonPress[index] = 0;
		return 1;
	}
	return 0;
}



void button_reading(void){
    for(uint8_t i = 0; i < NO_OF_BUTTONS; i++) {
        debounceButtonBuffer0[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = debounceButtonBuffer2[i];
        debounceButtonBuffer2[i] = HAL_GPIO_ReadPin(buttonPort[i], buttonPin[i]);

        if(debounceButtonBuffer0[i] == debounceButtonBuffer1[i] && debounceButtonBuffer1[i] == debounceButtonBuffer2[i]){
        	// Trạng thái nút nhấn đã ổn định
            if(buttonBuffer[i] != debounceButtonBuffer2[i]){
                // Trạng thái nút nhấn đã thay đổi
                buttonBuffer[i] = debounceButtonBuffer2[i];

                if(debounceButtonBuffer2[i] == BUTTON_IS_PRESSED){
                    // Nút nhấn chuyển từ thả sang nhấn
                    flagForButtonPress[i] = 1;
                    counterForButtonPress_2s[i] = DURATION_FOR_AUTO_INCREASING; // Đặt bộ đếm
                } else {
                    // Nút nhấn chuyển từ nhấn sang thả
                    counterForButtonPress_2s[i] = 0;
                    flagForButtonPress[i] = 0;
                    flagForButtonPress_2s[i] = 0;
                }
            }
				else if(debounceButtonBuffer2[i] == BUTTON_IS_PRESSED && counterForButtonPress_2s[i] > 0){
								counterForButtonPress_2s[i]--;
								if(counterForButtonPress_2s[i] == 0){
//									flagForButtonPress_2s[i] = 1;
									flagForButtonPress[i] = 1;
									counterForButtonPress_2s[i] = DURATION_FOR_AUTO_INCREASING;

								}
							}
        }
    }
}





