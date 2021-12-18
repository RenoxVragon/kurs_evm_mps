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

void Queue::push(int proc, int com_n) // ���������� �������� � ����� �������
{
	Element* e = new Element; // ������������ ������ ��������
	e->number_proc = proc;
	e->number_com = com_n;
	e->prev = NULL;
	if (tail) // ���� ������� �� �����
	{
		e->prev = tail;
		tail = e;
	}
	else // ���� ������� �����
		tail = e;
}

int Queue::pop() // ���������� ������� �������� �������
{
	int data = 0;
	Element* e = tail;
	if (tail == NULL) // ���� ������� �����
	{
		cout << "Queue is empty." << endl;
		return data;
	}
	if (NULL == e->prev) // ���� � ������� 1 �������
	{
		data = e->number_proc;
		delete tail;
		tail = NULL;
		return data;
	}
	while (NULL != e->prev->prev) // ���� � ������� ������ 1 ��������
	{
		e = e->prev; // ����������� �� �������
	}
	if (NULL == e->prev->prev) // 2 � ����� ���������, ���������� �������.
	{
		data = e->prev->number_proc;
		delete e->prev->prev;
		e->prev = NULL;
		return data;
	}
}

void Queue::print() // ����� ������� �� �����
{
	Element* e = tail;
	int count = 1; // ����������� ���������� ��������� � �������
	if (tail == NULL) // ������� �����
	{
		cout << "Queue is empty." << endl;
	}
	else
	{
		while (e->prev != NULL) // ������� �� �����
		{
			count++; // ������� ���������� ���������
			e = e->prev;
		}
		int* data = new int[count]; // �������� �������������� ������� �� ���������� ���������
		e = tail;
		int j = count;
		while (e->prev != NULL) // ���������� � ����� ����� � ������� � ������
		{
			j--;
			data[j] = e->number_proc;
			e = e->prev;
		}
		data[0] = e->number_proc; // � ������ ������� ������� ���������� �������� ������� �������� �������
		cout << "Queue: ";
		for (int i = 0; i < count; i++)
		{
			cout << " " << data[i]; // ������� ������ �� �����
		}
		cout << endl;
		delete[] data; // ������� ������
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
	int count = 1; // ������� � �������
	if (this->tail == NULL)
		return 0;
	else
	{
		while (e->prev != NULL) // ������� �� �����
		{
			count++; // ������� ���������� ���������
			e = e->prev;
		}
		return count;
	}
}

void Queue::clear() // �������� ������������� �������
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

Element* Queue::get_tail() // ��������� ��������� �� ����� �������
{
	return tail;
}