#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_system.h"
#include "stepper.h"

const bool FULL_STEP_MOTOR_SEQUENCE[][4] = {
		{ 1,  0,  1,  0 },
		{ 0,  1,  1,  0 },
		{ 0,  1,  0,  1 },
		{ 1,  0,  0,  1 }
};

const bool HALF_STEP_MOTOR_SEQUENCE[][4] = {
		{ 1,  0,  0,  0 },
		{ 1,  1,  0,  0 },
		{ 0,  1,  0,  0 },
		{ 0,  1,  1,  0 },
		{ 0,  0,  1,  0 },
		{ 0,  0,  1,  1 },
		{ 0,  0,  0,  1 },
		{ 1,  0,  0,  1 }
};


const int PINS1[4] = {GPIO_NUM_16,GPIO_NUM_4,GPIO_NUM_0,GPIO_NUM_2};
const int PINS2[4] = {GPIO_NUM_17,GPIO_NUM_5,GPIO_NUM_18,GPIO_NUM_19};

void writeSequence() {
	int sequenceLength = sizeof(FULL_STEP_MOTOR_SEQUENCE) / sizeof(FULL_STEP_MOTOR_SEQUENCE[0]);
	for (int sequenceNo = 0; sequenceNo < sequenceLength; sequenceNo++) {
		for (int i = 0; i < 4; i++) {
			gpio_set_level(PINS1[i], FULL_STEP_MOTOR_SEQUENCE[sequenceNo][i]);
			gpio_set_level(PINS2[i], FULL_STEP_MOTOR_SEQUENCE[sequenceLength-sequenceNo][sequenceLength-i]);
		}
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}

void initStepper() {
	for (int i = 0; i < 4; i++) {
    	gpio_set_direction(PINS1[i], GPIO_MODE_OUTPUT);
		gpio_set_direction(PINS2[i], GPIO_MODE_OUTPUT);
    }
}

void makeStep() {

    int sequenceNo = 0;

    for (int i = 0; i < 511; i++) {
        writeSequence();
    }
}
