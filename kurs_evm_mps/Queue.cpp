#include "Queue.h"

using namespace std;

Queue::Queue()
{
	tail = NULL;
}

Queue::~Queue()
{
	this->clear();
}

void Queue::push(int proc, int com_n) // добавление элемента в конец очереди
{
	Element* e = new Element; // формирование нового элемента
	e->number_proc = proc;
	e->number_com = com_n;
	e->prev = NULL;
	if (tail) // если очередь Ќ≈ пуста
	{
		e->prev = tail;
		tail = e;
	}
	else // если очередь пуста
		tail = e;
}

int Queue::pop() // извлечение первого элемента очереди
{
	int data = 0;
	Element* e = tail;
	if (tail == NULL) // если очередь пуста
	{
		cout << "Queue is empty." << endl;
		return data;
	}
	if (NULL == e->prev) // если в очереди 1 элемент
	{
		data = e->number_proc;
		delete tail;
		tail = NULL;
		return data;
	}
	while (NULL != e->prev->prev) // если в очереди больше 1 элемента
	{
		e = e->prev; // продвижение по очереди
	}
	if (NULL == e->prev->prev) // 2 и более элементов, извлечение первого.
	{
		data = e->prev->number_proc;
		delete e->prev->prev;
		e->prev = NULL;
		return data;
	}
}

void Queue::print() // вывод очереди на экран
{
	Element* e = tail;
	int count = 1; // минимальное количество элементов в очереди
	if (tail == NULL) // очередь пуста
	{
		cout << "Queue is empty." << endl;
	}
	else
	{
		while (e->prev != NULL) // очередь не пуста
		{
			count++; // подсчет количества элементов
			e = e->prev;
		}
		int* data = new int[count]; // создание динаммического массива по количеству элементов
		e = tail;
		int j = count;
		while (e->prev != NULL) // записываем с конца числа с очереди в массив
		{
			j--;
			data[j] = e->number_proc;
			e = e->prev;
		}
		data[0] = e->number_proc; // в первый элемент массива записываем значение первого элемента очереди
		cout << "Queue: ";
		for (int i = 0; i < count; i++)
		{
			cout << " " << data[i]; // выводим массив на экран
		}
		cout << endl;
		delete[] data; // удал€ем массив
	}
}

bool Queue::check_empty()
{
	if (this->tail == NULL)
		return true;
	else
		return false;
}

int Queue::get_size()
{
	Element* e = tail;
	int count = 1; // минимум в очереди
	if (this->tail == NULL)
		return 0;
	else
	{
		while (e->prev != NULL) // очередь не пуста
		{
			count++; // подсчет количества элементов
			e = e->prev;
		}
		return count;
	}
}

void Queue::clear() // очищение скопированной очереди
{
	Element* e = tail;
	if (tail != NULL)
	{
		while (tail != NULL)
		{
			tail = tail->prev;
			delete e;
			e = tail;
		}
		delete e;
		this->tail = NULL;
	}
}

Element* Queue::get_tail() // получение указател€ на хвост очереди
{
	return tail;
}