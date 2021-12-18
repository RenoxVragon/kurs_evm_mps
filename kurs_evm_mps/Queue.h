#pragma once
#include "Element.h"
#include <iostream>


class Queue { // класс очереди
public:
	Queue(); // конструктор 
	~Queue(); // деконструктор
	void push(int proc, int com_n); // добавить элемент в очередь
	int pop(); // извлечь элемент из очереди
	void print(); // вывести очередь на экран
	bool check_empty(); // проверка на пустоту
	int get_size(); // размер очереди
protected:
	void clear(); // удаление скопированной очереди
	Element* get_tail(); // получение указателя на хвост
private:
	Element* tail;
};
