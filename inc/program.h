#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <stdbool.h>

int exe_program(struct arguments args);
void signal_handler(int signal);
int start_deamon(bool value);

#endif