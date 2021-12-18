#include "processor.h"

Processor::Processor(int* com, int _size, int f_mem, int s_mem, int t_mem, int four_mem)
{
	this->commands = com;
	this->size_list = _size;
	this->appeals[0] = f_mem;
	this->appeals[1] = s_mem;
	this->appeals[2] = t_mem;
	this->appeals[3] = four_mem;
}

Processor::~Processor()
{
	delete this->commands;
	this->size_list = 0;
}

void Processor::print_commands()
{
	for (int i = 0; i < size_list; i++)
	{
		if (this->commands[i] == 1)
			cout << "Команда с обращением в память: " << i << endl;
		else
			cout << "Команда с регистром: " << i << endl;
	}
	cout << "Количество обращений в 1 ячейку памяти: " << appeals[0] << endl;
	cout << "Количество обращений в 2 ячейку памяти: " << appeals[1] << endl;
	cout << "Количество обращений в 3 ячейку памяти: " << appeals[2] << endl;
	cout << "Количество обращений в 4 ячейку памяти: " << appeals[3] << endl;
}