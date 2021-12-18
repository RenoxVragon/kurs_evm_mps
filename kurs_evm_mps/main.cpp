#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include "memoryslot.h"
#include "processor.h"
#include "bus.h"
#include "switch.h"
using namespace std;

int kr = 0, k = 0, pr = 0, m = 0; // ���-�� ������, �� ��������� ���������; ����� ���-�� ������; ������������� ������; �������� ���������

int process_input(int min_range, int max_range)
{
	string str;
	while (1)
	{
		cin >> str;
		if (str.find_first_not_of("0123456789", 0) == string::npos)
		{
			int choice = stoi(str);
			if (choice < min_range || choice > max_range)
				cout << "������������ ����. ���������� ��� ���: " << endl;
			else
				return choice;
		}
		else
			cout << "������������ ����. ���������� ��� ���: " << endl;
	}
}

void set_info()
{
	cout << "������������� ���������� ����������������� �������, ��������� �� 4 ����������� � 4 ����� ������." << endl;
	int choice;
	cout << "������� ���������� ������ � ���������: " << endl;
	k = process_input(0, INT32_MAX);
	system("cls");
	cout << "�������� ������� ������, ������� �� ������� ��������� � ������:\n1. 90 % ������\n2. 80 % ������\n3. 60 % ������" << endl;
	choice = process_input(1, 3);
	switch (choice)
	{
	case 1:
		kr = 90;
		break;
	case 2:
		kr = 80;
		break;
	case 3:
		kr = 60;
		break;
	}
	system("cls");
	cout << "�������� ������� ������, ������� ����� ������������ ������ � 1 ������ ������:\n1. 90 % ������\n2. 70 % ������" << endl;
	choice = process_input(1, 2);
	if (choice == 1)
		pr = 90;
	else
		pr = 70;
	system("cls");
	cout << "�������� ����� ��������� ���������� � ������ ������:\n1. 2 �����\n2. 5 ������\n3. 10 ������" << endl;
	choice = process_input(1, 3);
	switch (choice)
	{
	case 1:
		m = 2;
		break;
	case 2:
		m = 5;
		break;
	case 3:
		m = 10;
		break;
	}
	system("cls");
}

void generate_list(int* _command_list)
{
	srand(time(0));
	float count, krr;
	count = k; // ���-�� ���� ������
	krr = kr; // ������� � ���������
	int kr_count = 0, k_count = 0;
	count = k * (kr * 0.01); // ������� ���-�� � ���������, ����� ����������� ������� �����
	if (count - int(count) < 0.5)
		kr_count = int(count);
	else
		kr_count = int(count) + 1; 
	k_count = k - kr_count; // ������� ������� ������, ��������� ��������� � ������.
	for (int i = 0; i < k; i++)
		_command_list[i] = 0;
	for (int i = 0; i < k_count; i++)
	{
		int j = rand() % k;
		if (_command_list[j] == 0)
			_command_list[j] = 1;
		else
		{
			int j = rand() % k;
			_command_list[j] = 1;
		}
	}
}

int* separate_commands()
{
	int sizes[4];
	switch (k % 4)
	{
	case 0:
		sizes[0] = k / 4;
		sizes[1] = k / 4;
		sizes[2] = k / 4;
		sizes[3] = k / 4;
		break;
	case 1:
		sizes[0] = k / 4 + 1;
		sizes[1] = k / 4;
		sizes[2] = k / 4;
		sizes[3] = k / 4;
		break;
	case 2:
		sizes[0] = k / 4 + 1;
		sizes[1] = k / 4 + 1;
		sizes[2] = k / 4;
		sizes[3] = k / 4;
		break;
	case 3:
		sizes[0] = k / 4 + 1;
		sizes[1] = k / 4 + 1;
		sizes[2] = k / 4 + 1;
		sizes[3] = k / 4;
		break;
	}
	return sizes;
}

void parralel_commands(int *com1, int* com2, int* com3, int* com4, const int* _command_list)
{
	const int* sizes = separate_commands();
	int j = 0;
	for (int i = 0; i < sizes[0]; i++)
	{
		com1[i] = _command_list[j];
		j++;
	}
	for (int i = 0; i < sizes[1]; i++)
	{
		com2[i] = _command_list[j];
		j++;
	}
	for (int i = 0; i < sizes[2]; i++)
	{
		com3[i] = _command_list[j];
		j++;
	}
	for (int i = 0; i < sizes[3]; i++)
	{
		com4[i] = _command_list[j];
		j++;
	}
}

void print_lists(const int* com1, const int* com2, const int* com3, const int* com4, const int* sizes)
{
	cout << "����� ������ 1 ����������:" << endl;
	for (int i = 0; i < sizes[0]; i++)
	{
		if (com1[i] == 1)
			cout << "������� � ���������� � ������: " << i << endl;
		else
			cout << "������� � ���������: " << i << endl;
	}
	cout << "����� ������ 2 ����������:" << endl;
	for (int i = 0; i < sizes[1]; i++)
	{
		if (com2[i] == 1)
			cout << "������� � ���������� � ������: " << i << endl;
		else
			cout << "������� � ���������: " << i << endl;
	}
	cout << "����� ������ 3 ����������:" << endl;
	for (int i = 0; i < sizes[2]; i++)
	{
		if (com3[i] == 1)
			cout << "������� � ���������� � ������: " << i << endl;
		else
			cout << "������� � ���������: " << i << endl;
	}
	cout << "����� ������ 4 ����������:" << endl;
	for (int i = 0; i < sizes[3]; i++)
	{
		if (com4[i] == 1)
			cout << "������� � ���������� � ������: " << i << endl;
		else
			cout << "������� � ���������: " << i << endl;
	}
}

int* separate_memory_appeal(const int* com1, const int* com2, const int* com3, const int* com4, const int* sizes)
{
	int appeal[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	float count = 0, krr;
	krr = pr; // ������� � � ���������� � ������
	int kr_count = 0, k_count = 0;
	for (int i = 0; i < sizes[0]; i++)
	{
		if (com1[i] == 1)
			count++;
	}
	k_count = int(count); // ����� ���������� ������ � ����������
	count = count * (krr * 0.01);
	if (count - int(count) < 0.5) // ������� ������� ������ � 1 ������
		kr_count = int(count);
	else
		kr_count = int(count) + 1;
	k_count = k_count - kr_count; // ��������� ������� � 1 ������, �������� ������� ��� �������������
	appeal[0] = kr_count;
	if (k_count > 0) // 1 ���������
	{
		switch(k_count % 3)
		{
		case 0:
			appeal[1] = k_count / 3;
			appeal[2] = k_count / 3;
			appeal[3] = k_count / 3;
			break;
		case 1:
			appeal[1] = k_count / 3 + 1;
			appeal[2] = k_count / 3;
			appeal[3] = k_count / 3;
			break;
		case 2:
			appeal[1] = k_count / 3 + 1;
			appeal[2] = k_count / 3 + 1;
			appeal[3] = k_count / 3;
			break;
		}
	}
	count = 0; k_count = 0; kr_count = 0;  // 2 ���������
	for (int i = 0; i < sizes[1]; i++)
	{
		if (com2[i] == 1)
			count++;
	}
	k_count = int(count); // ����� ���������� ������ � ����������
	count = count * (krr * 0.01);
	if (count - int(count) < 0.5) // ������� ������� ������ � 1 ������
		kr_count = int(count);
	else
		kr_count = int(count) + 1;
	k_count = k_count - kr_count; // ��������� ������� � 1 ������, �������� ������� ��� �������������
	appeal[5] = kr_count;
	if (k_count > 0)
	{
		switch (k_count % 3)
		{
		case 0:
			appeal[4] = k_count / 3;
			appeal[6] = k_count / 3;
			appeal[7] = k_count / 3;
			break;
		case 1:
			appeal[4] = k_count / 3 + 1;
			appeal[6] = k_count / 3;
			appeal[7] = k_count / 3;
			break;
		case 2:
			appeal[4] = k_count / 3 + 1;
			appeal[6] = k_count / 3 + 1;
			appeal[7] = k_count / 3;
			break;
		}
	}
	count = 0; k_count = 0; kr_count = 0;  // 3 ���������
	for (int i = 0; i < sizes[2]; i++)
	{
		if (com3[i] == 1)
			count++;
	}
	k_count = int(count); // ����� ���������� ������ � ����������
	count = count * (krr * 0.01);
	if (count - int(count) < 0.5) // ������� ������� ������ � 1 ������
		kr_count = int(count);
	else
		kr_count = int(count) + 1;
	k_count = k_count - kr_count; // ��������� ������� � 1 ������, �������� ������� ��� �������������
	appeal[10] = kr_count;
	if (k_count > 0)
	{
		switch (k_count % 3)
		{
		case 0:
			appeal[8] = k_count / 3;
			appeal[9] = k_count / 3;
			appeal[11] = k_count / 3;
			break;
		case 1:
			appeal[8] = k_count / 3 + 1;
			appeal[9] = k_count / 3;
			appeal[11] = k_count / 3;
			break;
		case 2:
			appeal[8] = k_count / 3 + 1;
			appeal[9] = k_count / 3 + 1;
			appeal[11] = k_count / 3;
			break;
		}
	}
	count = 0; k_count = 0; kr_count = 0;  // 4 ���������
	for (int i = 0; i < sizes[3]; i++)
	{
		if (com4[i] == 1)
			count++;
	}
	k_count = int(count); // ����� ���������� ������ � ����������
	count = count * (krr * 0.01);
	if (count - int(count) < 0.5) // ������� ������� ������ � 1 ������
		kr_count = int(count);
	else
		kr_count = int(count) + 1;
	k_count = k_count - kr_count; // ��������� ������� � 1 ������, �������� ������� ��� �������������
	appeal[15] = kr_count;
	if (k_count > 0)
	{
		switch (k_count % 3)
		{
		case 0:
			appeal[12] = k_count / 3;
			appeal[13] = k_count / 3;
			appeal[14] = k_count / 3;
			break;
		case 1:
			appeal[12] = k_count / 3 + 1;
			appeal[13] = k_count / 3;
			appeal[14] = k_count / 3;
			break;
		case 2:
			appeal[12] = k_count / 3 + 1;
			appeal[13] = k_count / 3 + 1;
			appeal[14] = k_count / 3;
			break;
		}
	}
	return appeal;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	bool bus_w = false, commutator_w = false;
	set_info();
	int *command_list = new int[k]; // ��������� �� k �������
	cout << "��������� ��������� �� K �������, �� ������� Kr �� ������� ��������� � ������." << endl;
	generate_list(command_list);
	for (int i = 0; i < k; i++)
	{
		if (command_list[i] == 1)
			cout << "������� � ���������� � ������: " << i << endl;
		else 
			cout << "������� � ���������: " << i << endl;
	}
	const int *sizes_list;
	sizes_list = separate_commands();
	int sizes[4]; for (int i = 0; i < 4; i++) sizes[i] = sizes_list[i];
	int* com_l1 = new int[sizes_list[0]], * com_l2 = new int[sizes_list[1]], * com_l3 = new int[sizes_list[2]], * com_l4 = new int[sizes_list[3]];
	Sleep(5000);
	system("cls");
	parralel_commands(com_l1, com_l2, com_l3, com_l4, command_list);
	print_lists(com_l1, com_l2, com_l3, com_l4, sizes);
	Sleep(5000);
	const int* get_appeals = separate_memory_appeal(com_l1, com_l2, com_l3, com_l4, sizes);
	int appeal_memory[16]; for (int i = 0; i < 16; i++) appeal_memory[i] = get_appeals[i];
	cout << "������������� ��������� � ������ ������:" << endl;
	for (int i = 0; i < 16; i++)
	{
		switch (i % 4)
		{
		case 0:
			cout << "1 ������: " << appeal_memory[i] << endl;
			break;
		case 1:
			cout << "2 ������: " << appeal_memory[i] << endl;
			break;
		case 2:
			cout << "3 ������: " << appeal_memory[i] << endl;
			break;
		case 3:
			cout << "4 ������: " << appeal_memory[i] << endl;
			break;
		}
	}
	Sleep(5000);
	system("cls");
	cout << "������������� ����������� � ����� ������." << endl;
	Processor P1(com_l1, sizes[0], appeal_memory[0], appeal_memory[1], appeal_memory[2], appeal_memory[3]),
		P2(com_l2, sizes[1], appeal_memory[4], appeal_memory[5], appeal_memory[6], appeal_memory[7]), 
		P3(com_l3, sizes[2], appeal_memory[8], appeal_memory[9], appeal_memory[10], appeal_memory[11]),
		P4(com_l4, sizes[3], appeal_memory[12], appeal_memory[13], appeal_memory[14], appeal_memory[15]);
	P1.print_commands();
	cout << "\n";
	P2.print_commands();
	cout << "\n";
	P3.print_commands();
	cout << "\n";
	P4.print_commands();
	cout << "\n";
	cout << "�������� ���� ����������������� �������:\n1. ����� ����.\n2. ����������." << endl;
	int choice = process_input(1, 2);
	if (choice == 1)
		bus_w = true;
	else
		commutator_w = true;
	if (bus_w)
	{
		print_bus();
		start_set_states(P1, P2, P3, P4);
		bus_work(P1, P2, P3, P4, m);
	}
	if (commutator_w)
	{
		print_switch();
		start_set_statesc(P1, P2, P3, P4);
		switch_work(P1, P2, P3, P4, m);
	}
}