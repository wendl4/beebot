#include "perform.h"
#include "stepper.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "esp_log.h"

static const char *TAG = "Perform";

void prepair_program(char program[]) {
    Cmd cmds[strlen(program)];
    for (int i = 0; i < strlen(program); i++) {
        int b = program[i] - '0';
        Cmd command = {b, NULL, NULL};
        cmds[i] = command;    
    }
    Program prog = {&cmds[0], strlen(program)};
    Perform p = {&prog,0};
    //printf("cmd %d",p.prog->cmds[p.pc].cmd);
    interpret(&p,0); 
    //show_cmd(&p);
}

void show_cmd(Perform *p) {
    printf("cmd %d",p->prog->cmds[p->pc].cmd);
    ESP_LOGI(TAG,"AA");
}

int find_right_braces(Perform *p, int position) {
    int count = 0;
    while (position < p->prog->len) {
        int cmd = p->prog->cmds[position].cmd;
        if (cmd == RightBraces) {
            if (count == 0) {
                return position;
            }
            else count--;
        }
        if (cmd == LeftBraces) {
            count++;
        }
        position++;
    }
    return -1;
}


void interpret(Perform *p, int pc) {
    int position = pc;
    int count = 0;
    uint32_t cmd;
    //uint32_t arg1 = p->prog->cmds[p->pc].arg1;
    //uint32_t arg2 = p->prog->cmds[p->pc].arg2;
    while (position < p->prog->len) {
        cmd = p->prog->cmds[position].cmd;
        ESP_LOGI(TAG,"command %d",cmd);
        switch (cmd) {
            case None:
                break;
            case StepperFwd:
                setDir(-1,1);
                makeStep();
                break;
            case StepperBack:
                setDir(1,-1);
                makeStep();
                break;
            case StepperRight:
                setDir(-1,-1);
                makeStep();
                break;
            case StepperLeft:
                setDir(1,1);
                makeStep();
                break;
            case LeftBraces:
                while(count > 0) {
                    interpret(p,position+1);
                    --count;
                }
                position = find_right_braces(p,position+1);
                break;
            case RightBraces:
                return;
            case TimesTwo:
                count = 2;
                break;
            case TimesThree:
                count = 3;
                break;
            default:
                printf("Unknown cmd %d at position %d\n", cmd, p->pc);
                return;
        }
        position++;
    }
}
