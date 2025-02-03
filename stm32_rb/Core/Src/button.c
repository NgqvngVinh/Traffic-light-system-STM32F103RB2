/*
 * button.c
 *
 *  Created on: Oct 30, 2024
 *      Author: vinhd
 */


#include <led_7_seg.h>
#include "button.h"
#include "i2c-lcd.h"


static GPIO_TypeDef* buttonPort[NO_OF_BUTTONS] = {A0_GPIO_Port, A1_GPIO_Port, A2_GPIO_Port}; 	// Store GPIO button port
static uint16_t buttonPin[NO_OF_BUTTONS] = {A0_Pin, A1_Pin, A2_Pin};						 	// Store button pin


// Buffers used for debouncing the three different button inputs
static GPIO_PinState debounceButtonBuffer0[NO_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS];
static GPIO_PinState buttonBuffer[NO_OF_BUTTONS]; 												// Stores the stable (debounced) button state


//Flag
static uint8_t flagForButtonPress[NO_OF_BUTTONS];
//static uint8_t flagForButtonPress_2s[NO_OF_BUTTONS];


//Counter for tracking the button press duration (2 seconds)
static uint16_t counterForButtonPress_2s[NO_OF_BUTTONS] = {0};


//Variabes
enum ButtonState buttonState[NO_OF_BUTTONS];
extern int mode;





void button_init(){
    for(int i = 0; i < NO_OF_BUTTONS; i++){
        buttonState[i] = BUTTON_RELEASED;
        flagForButtonPress[i] = 0;
        counterForButtonPress_2s[i] = 0;
        buttonBuffer[i] = BUTTON_IS_RELEASED;
        debounceButtonBuffer0[i] = BUTTON_IS_RELEASED;
        debounceButtonBuffer1[i] = BUTTON_IS_RELEASED;
        debounceButtonBuffer2[i] = BUTTON_IS_RELEASED;
    }
}




//Function
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
    	// Shift the debounce buffer through 3 filters (buffers)
        debounceButtonBuffer0[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = debounceButtonBuffer2[i];
        debounceButtonBuffer2[i] = HAL_GPIO_ReadPin(buttonPort[i], buttonPin[i]);
        // Check if the i button state has stabilized
        if(debounceButtonBuffer0[i] == debounceButtonBuffer1[i] && debounceButtonBuffer1[i] == debounceButtonBuffer2[i]){
        	// If the stable state differs from the previously stored state (last filter)
            if(buttonBuffer[i] != debounceButtonBuffer2[i]){
            	// Update the latest state to the stable state buffer
                buttonBuffer[i] = debounceButtonBuffer2[i];
                // Button changed from 'released' to 'pressed'
                if(debounceButtonBuffer2[i] == BUTTON_IS_PRESSED){
                    // Turn on the Button 'Press' flag
                    flagForButtonPress[i] = 1;
                    // Initialize the counter for Button 'Press'
                    counterForButtonPress_2s[i] = DURATION_FOR_AUTO_INCREASING;
                } else {
                	// Button changed from 'pressed' to 'released', free all the counter and flag
                    counterForButtonPress_2s[i] = 0;
                    flagForButtonPress[i] = 0;
                	}
            	}
            	// If the button is still pressed and the counter is running
				else if(debounceButtonBuffer2[i] == BUTTON_IS_PRESSED && counterForButtonPress_2s[i] > 0){
								counterForButtonPress_2s[i]--;
								// Button press more than 2 seconds, turn on the Button 'Press' flag
								if(counterForButtonPress_2s[i] == 0){
									flagForButtonPress[i] = 1;
									counterForButtonPress_2s[i] = DURATION_FOR_AUTO_INCREASING;

				}
			}
        }
    }
}





