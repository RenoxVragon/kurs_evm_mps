#pragma once
#include "Element.h"
#include <iostream>


class Queue { // ����� �������
public:
	Queue(); // ����������� 
	~Queue(); // �������������
	void push(int proc, int com_n); // �������� ������� � �������
	int pop(); // ������� ������� �� �������
	void print(); // ������� ������� �� �����
	bool check_empty(); // �������� �� �������
	int get_size(); // ������ �������
protected:
	void clear(); // �������� ������������� �������
	Element* get_tail(); // ��������� ��������� �� �����
private:
	Element* tail;
};
