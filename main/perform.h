#include <stdbool.h>

enum CmdType {
    None = 0,
    StepperFwd,
    StepperBack,
    StepperRight,
    StepperLeft,
    LeftBraces,
    RightBraces,
    TimesTwo,
    TimesThree
};

typedef struct _Cmd {
    int cmd;
    int arg1;
    int arg2;
} Cmd;

typedef struct _Program {
    const Cmd *cmds;
    int len;
} Program;

typedef struct _Perform {
    const Program *prog;
    int pc;
} Perform;

void interpret(Perform *p,int pc);
void prepair_program(char program[]);
void show_cmd(Perform *p);
