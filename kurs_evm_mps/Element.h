#pragma once
#include <iostream>

struct Element { // структура элемента очереди
	int number_proc;
	int number_com;
	Element* prev;
};
