#pragma once
#include <iostream>
#include <windows.h>
#include "processor.h"

void bus_work(Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
void print_bus();
int get_command(Processor &p, int index);
int check_bus_status();
void set_states_all_reg(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4);
void set_states_one_p(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
void set_states_one_w(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
void set_states_two_w(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
void set_states_three_w(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
void start_set_states(Processor& p1, Processor& p2, Processor& p3, Processor& p4);
int comparate_states(int s1, int s2, int s3, int s4);
void off_processors(int state);
void off_memory(int state);
void set_active(Processor& p, int next_p);
void start_next_queue(Processor& p1, Processor& p2, Processor& p3, Processor& p4);