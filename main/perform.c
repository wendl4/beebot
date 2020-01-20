#include "perform.h"
#include "translator.h"
#include "stepper.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "esp_log.h"

static const char *TAG = "Perform";

void run(cmd * program) {
    int registers[16]; 
    int position = 0;
    cmd current_command;
    while (strcmp(program[position].name,"HLT") != 0) {
        current_command = program[position];
        //ESP_LOGI(TAG,"command %s",current_command.name);
        if (strcmp(program[position].name,"FWD") == 0) {
            setDir(-1,1);
            makeStep();
            position++;
        }
        else if (strcmp(program[position].name,"BCK") == 0) {
            setDir(1,-1);
            makeStep();
            position++;
        }
        else if (strcmp(program[position].name,"LFT") == 0) {
            setDir(1,1);
            makeStep();
            position++;
        }
        else if (strcmp(program[position].name,"RGHT") == 0) {
            setDir(-1,-1);
            makeStep();
            position++;
        }
        else if (strcmp(program[position].name,"MOV") == 0) {
            registers[program[position].arg1] = program[position].arg2;
            position++;
        }
        else if (strcmp(program[position].name,"JMP") == 0) {
            position = program[position].arg1;
        }
        else if (strcmp(program[position].name,"JZ") == 0) {
            if (registers[program[position].arg1] == 0) {
                position = program[position].arg2;
            }
            else position++;
        }
        else if (strcmp(program[position].name,"DECR") == 0) {
            registers[program[position].arg1]--;
        }
    }
}

void prepair_program(char code[]) {
    cmd program[512];
    translate(code,program);
    run(program);
}
