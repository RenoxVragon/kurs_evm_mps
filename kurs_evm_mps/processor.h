#pragma once
#include <iostream>
using namespace std;

class Processor
{
public:
	Processor(int* com, int _size, int f_mem, int s_mem, int t_mem, int four_mem);
	~Processor();
	void print_commands();
	friend void bus_work(Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
	friend void switch_work(Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
	friend void set_states_no_c(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4);
	friend void set_states_1_c(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
	friend void set_states_2_c(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
	friend void set_states_3_c(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
	friend void set_states_4_c(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
	friend void start_next_queuec(Processor& p1, Processor& p2, Processor& p3, Processor& p4);
	friend int get_command(Processor& p, int index);
	friend bool check_call(Processor& p);
	friend void start_set_states(Processor& p1, Processor& p2, Processor& p3, Processor& p4);
	friend void set_states_all_reg(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4);
	friend void set_states_one_p(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
	friend void set_states_one_w(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
	friend void set_states_two_w(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
	friend void set_states_three_w(int c1, int c2, int c3, int c4, Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m);
	friend void set_active(Processor& p, int next_p);
	friend void start_next_queue(Processor& p1, Processor& p2, Processor& p3, Processor& p4);
	friend int get_commandc(Processor& p, int index);
	friend int comparate_statesc(int s1, int s2, int s3, int s4);
	friend int min_comparate_statesc(int s1, int s2, int s3, int s4);
	friend void off_processorsc(int state);
	friend void off_memoryc(int state);
	friend void set_activec(Processor& p, int next_p);
	friend void start_set_statesc(Processor& p1, Processor& p2, Processor& p3, Processor& p4);
private:
	int appeals[4];
	int* commands;
	int size_list;
};