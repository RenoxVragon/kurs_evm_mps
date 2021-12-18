#pragma once
#include <iostream>
#include <windows.h>
#include "processor.h"
#include "Queue.h"

void switch_work(Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
void print_switch();
int check_switch_status();
bool check_call(Processor& p);
int get_commandc(Processor& p, int index);
int comparate_statesc(int s1, int s2, int s3, int s4);
int min_comparate_statesc(int s1, int s2, int s3, int s4);
void off_processorsc(int state);
void off_memoryc(int state);
void set_activec(Processor& p, int next_p);
void start_next_queuec(Processor& p1, Processor& p2, Processor& p3, Processor& p4);
void start_set_statesc(Processor& p1, Processor& p2, Processor& p3, Processor& p4);
void set_states_no_c(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4);
void set_states_1_c(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
void set_states_2_c(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
void set_states_3_c(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
void set_states_4_c(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);