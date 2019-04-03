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
    perform_step(&p); 
    //show_cmd(&p);

    p.pc++;
    makeStep();
}

void show_cmd(Perform *p) {
    printf("cmd %d",p->prog->cmds[p->pc].cmd);
    ESP_LOGI(TAG,"AA");
}


bool perform_step(Perform *p) {
    uint32_t cmd = p->prog->cmds[p->pc].cmd;
    //uint32_t arg1 = p->prog->cmds[p->pc].arg1;
    //uint32_t arg2 = p->prog->cmds[p->pc].arg2;
    switch (cmd) {
        case None:
            return false;
        case StepperFwd:
            setDir(-1,1);
            return true;
        case StepperBack:
            setDir(1,-1);
            return true;
        case StepperRight:
            setDir(-1,-1);
            return true;
        case StepperLeft:
            setDir(1,1);
            return true;
        default:
            printf("Unknown cmd %d at position %d\n", cmd, p->pc);
            return false;
    }
}
