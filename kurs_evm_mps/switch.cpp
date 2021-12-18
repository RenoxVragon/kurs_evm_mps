#include "switch.h"
enum { passive, active, register_work, wait, process, start }; // состояния
enum { no_calls = 1, one_call, two_calls, three_calls, four_calls }; // состояния коммутатора
// m - время ожидания доступа
// p1 - p4, m1 - m4 - процессоры и ячейки памяти
int a = 0, s = 0, d = 0, f = 0; // индексы команд процессоров
int p1c = passive, p2c = passive, p3c = passive, p4c = passive; // состояние каждого процессора
int b1c = start, b2c = start, b3c = start, b4c = start; // операция процессора в коммутаторе
int m1c = passive, m2c = passive, m3c = passive, m4c = passive; // состояние каждой ячейки памяти
int clkc = 0; // такты
int time_waitc = 0;  // общее время ожидания доступа 
int time_mem1 = 0, time_mem2 = 0, time_mem3 = 0, time_mem4 = 0; // время ожжидания обращения к ячейкам памяти
int time_p1 = 0, time_p2 = 0, time_p3 = 0, time_p4 = 0; // время работы процессора с ячейкой памяти
int ccp1, ccp2, ccp3, ccp4; // команды процессоров
int cstatus = 0; // состояние коммутатора
Queue cq; // очередь команд

void print_switch()
{
	system("cls");
	cout << "P1 " << p1c << " | P2 " << p2c << " | P3 " << p3c << " | P4 " << p4c << endl;
	cout << "-------------------------" << endl;
	cout << "  " << b1c << "  |   " << b2c << "  |   " << b3c << "  |  " << b4c << endl;
	cout << "-------------------------" << endl;
	cout << "M1 " << m1c << " | M2 " << m2c << " | M3 " << m3c << " | M4 " << m4c << endl;
	cout << "Такт: " << clkc << endl;
	cout << "Общее время ожидания доступа к ячейкам памяти: " << time_waitc << endl;
	cout << "Текущий номер команды 1 процессора: " << a << endl;
	cout << "Текущий номер команды 2 процессора: " << s << endl;
	cout << "Текущий номер команды 3 процессора: " << d << endl;
	cout << "Текущий номер команды 4 процессора: " << f << endl;
	cout << "0 - не активен, 1 - активен, 2 - работа с регистром, 3 - ожидание, 4 - обращение в память, 5 - старт." << endl;
	cout << "Очередь обращений по процессорам: ";
	cq.print();
	cout << endl;
	Sleep(1000);
}

int check_switch_status()
{
	switch (m1c + m2c + m3c + m4c)
	{
	case 0:
		return no_calls;
		break;
	case 1:
		return one_call;
		break;
	case 2:
		return two_calls;
		break;
	case 3:
		return three_calls;
		break;
	case 4:
		return four_calls;
		break;
	}
}

bool check_call(Processor& p)
{
	int max = 0;
	int index_m = 0;
	int ind = 0; //
	int ind_c = 1; // индекс второго обращения
	int ind_t = 2; // индекс третьего обращения
	int ind_f = 3; // индекс четвертого обращения
	for (int r = 0; r < 4; r++)
	{
		if (p.appeals[r] > max)
		{
			max = p.appeals[r];
			index_m = r;
		}
	}
	switch (index_m)
	{
	case 0:
		if ((m1c == 1) && (index_m == 0))
			return false;
		else
			return true;
		break;
	case 1:
		if ((m2c == 1) && (index_m == 1))
			return false;
		else
			return true;
		break;
	case 2:
		if ((m3c == 1) && (index_m == 2))
			return false;
		else
			return true;
		break;
	case 3:
		if ((m4c == 1) && (index_m == 3))
			return false;
		else
			return true;
		break;
	}
}

int get_commandc(Processor& p, int index)
{
	return p.commands[index];
}

int comparate_statesc(int s1, int s2, int s3, int s4)
{
	int index = 0;
	int max = 0;
	if (s1 > max)
	{
		max = s1;
		index = 1;
	}
	if (s2 > max)
	{
		max = s2;
		index = 2;
	}
	if (s3 > max)
	{
		max = s3;
		index = 3;
	}
	if (s4 > max)
	{
		max = s4;
		index = 4;
	}
	return index;
}

int min_comparate_statesc(int s1, int s2, int s3, int s4)
{
	int index = 0;
	int min = 2;
	if (s1 < min)
	{
		min = s1;
		index = 1;
	}
	if (s2 < min)
	{
		min = s2;
		index = 2;
	}
	if (s3 < min)
	{
		min = s3;
		index = 3;
	}
	if (s4 < min)
	{
		min = s4;
		index = 4;
	}
	return index;
}

void off_processorsc(int state)
{
	switch (state)
	{
	case 1:
		p1c = passive;
		time_p1 = 0;
		b1c = register_work;
		break;
	case 2:
		p2c = passive;
		time_p2 = 0;
		b2c = register_work;
		break;
	case 3:
		p3c = passive;
		time_p3 = 0;
		b3c = register_work;
		break;
	case 4:
		p4c = passive;
		time_p4 = 0;
		b4c = register_work;
		break;
	}
}

void off_memoryc(int state)
{
	switch (state)
	{
	case 1:
		m1c = passive;
		time_mem1 = 0;
		break;
	case 2:
		m2c = passive;
		time_mem2 = 0;
		break;
	case 3:
		m3c = passive;
		time_mem3 = 0;
		break;
	case 4:
		m4c = passive;
		time_mem4 = 0;
		break;
	}
}

void set_activec(Processor& p, int next_p)
{
	switch (next_p)
	{
	case 1:
		p1c = active;
		b1c = process;
		break;
	case 2:
		p2c = active;
		b2c = process;
		break;
	case 3:
		p3c = active;
		b3c = process;
		break;
	case 4:
		p4c = active;
		b4c = process;
		break;
	}
	int max = 0;
	int index_m = 0;
	for (int r = 0; r < 4; r++)
	{
		if (p.appeals[r] > max)
		{
			max = p.appeals[r];
			index_m = r;
		}
	}
	p.appeals[index_m]--;
	switch (index_m)
	{
	case 0:
		m1c = active;
		break;
	case 1:
		m2c = active;
		break;
	case 2:
		m3c = active;
		break;
	case 3:
		m4c = active;
		break;
	}
}

void start_next_queuec(Processor& p1, Processor& p2, Processor& p3, Processor& p4)
{
	int data = cq.pop();
	switch (data)
	{
	case 1: 
		if (check_call(p1)) 
			set_activec(p1, 1); 
		else 
		{ 
			cq.push(1, a); 
			b1c = wait; 
		} 
		break;
	case 2: 
		if (check_call(p2)) 
		set_activec(p2, 2); 
		  else 
		{ 
			cq.push(2, s);
			b2c = wait; 
		}		
		  break;
	case 3: 
		if (check_call(p3)) 
			set_activec(p3, 3); 
		  else 
	{ 
		cq.push(3, d); 
		b3c = wait;
	} 
		  break;
	case 4: 
		if (check_call(p4))
		set_activec(p4, 4); 
		  else { 
		cq.push(4, f);
		b4c = wait;
	} 
		  break;
	}
}

void start_set_statesc(Processor& p1, Processor& p2, Processor& p3, Processor& p4) // установка стартового состояния коммутатора
{
	ccp1 = get_commandc(p1, a);
	ccp2 = get_commandc(p2, s);
	ccp3 = get_commandc(p3, d);
	ccp4 = get_commandc(p4, f);
	int start_status = ccp1 + ccp2 + ccp3 + ccp4; // 0, 1, 2, 3 или 4, для check_call надо c 1 - 4
	cstatus = start_status;
	switch (ccp1 + ccp2 + ccp3 + ccp4) // проверка входа команд
	{
	case 0: // нет обращений
		b1c = register_work; b2c = register_work; b3c = register_work; b4c = register_work;
		a++; s++; d++; f++;
		break;
	case 1: // есть 1 обращение
		if (ccp1 == 1) // если 1 проц
		{
			set_activec(p1, 1);
			b2c = register_work; b3c = register_work; b4c = register_work;
			s++; d++; f++;
		}
		else if (ccp2 == 1) // если второй проц
		{
			set_activec(p2, 2);
			b1c = register_work; b3c = register_work; b4c = register_work;
			a++; d++; f++;
		}
		else if (ccp3 == 1)
		{
			set_activec(p3, 3);
			b1c = register_work; b2c = register_work;  b4c = register_work;
			a++; s++; f++;
		}
		else
		{
			set_activec(p4, 4);
			b1c = register_work; b2c = register_work; b3c = register_work;
			a++; s++; d++;
		} // если 4 проц
		break;
	case 2: // есть 2 обращения
		if (ccp1 == 1)
		{
			set_activec(p1, 1);
			b2c = register_work; b3c = register_work; b4c = register_work;
			int next_c = comparate_statesc(0, ccp2, ccp3, ccp4);
			switch (next_c)
			{
			case 2:
				if (check_call(p2))
					set_activec(p2, 2);
				else
				{
					cq.push(2, s);
					b2c = wait;
				}
				d++; f++;
				break;
			case 3:
				if (check_call(p3))
					set_activec(p3, 3);
				else
				{
					cq.push(3, d);
					b3c = wait;
				}
				s++; f++;
				break;
			case 4:
				if (check_call(p4))
					set_activec(p4, 4);
				else
				{
					cq.push(4, f);
					b4c = wait;
				}
				s++; d++;
				break;
			}
		}
		else if (ccp2 == 1)
		{
			set_activec(p2, 2);
			b1c = register_work; b3c = register_work; b4c = register_work;
			int next_c = comparate_statesc(ccp1, 0, ccp3, ccp4);
			switch (next_c)
			{
			case 1:
				if (check_call(p1))
					set_activec(p1, 1);
				else
				{
					cq.push(1, a);
					b1c = wait;
				}
				d++; f++;
				break;
			case 3:
				if (check_call(p3))
					set_activec(p3, 3);
				else
				{
					cq.push(3, d);
					b3c = wait;
				}
				a++; f++;
				break;
			case 4:
				if (check_call(p4))
					set_activec(p4, 4);
				else
				{
					cq.push(4, f);
					b4c = wait;
				}
				a++; d++;
				break;
			}
		}
		else if (ccp3 == 1)
		{
			set_activec(p3, 3);
			b2c = register_work; b1c = register_work; b4c = register_work;
			if (check_call(p4))
				set_activec(p4, 4);
			else
			{
				cq.push(4, f);
				b4c = wait;
			}
			a++; s++;
		}
		break;
	case 3: // есть 3 обращения
		if (ccp1 == 1)
		{
			set_activec(p1, 1);
			b2c = register_work; b3c = register_work; b4c = register_work;
			if (ccp2 == 1 && ccp3 == 1)
			{
				if (check_call(p2))
					set_activec(p2, 2);
				else
				{
					cq.push(2, s);
					b2c = wait;
				}
				if (check_call(p3))
					set_activec(p3, 3);
				else
				{
					cq.push(3, d);
					b3c = wait;
				}
				f++;
			}
			if (ccp2 == 1 && ccp4 == 1)
			{
				if (check_call(p2))
					set_activec(p2, 2);
				else
				{
					cq.push(2, s);
					b2c = wait;
				}
				if (check_call(p4))
					set_activec(p4, 4);
				else
				{
					cq.push(4, f);
					b4c = wait;
				}
				d++;
			}
			if (ccp3 == 1 && ccp4 == 1)
			{
				if (check_call(p3))
					set_activec(p3, 3);
				else
				{
					cq.push(3, d);
					b3c = wait;
				}
				if (check_call(p4))
					set_activec(p4, 4);
				else
				{
					cq.push(4, f);
					b4c = wait;
				}
				s++;
			}
		}
		else
		{
			set_activec(p2, 2);
			b3c = register_work; b4c = register_work; b1c = register_work;
			if (check_call(p3))
				set_activec(p3, 3);
			else
			{
				cq.push(3, d);
				b3c = wait;
			}
			if (check_call(p4))
				set_activec(p4, 4);
			else
			{
				cq.push(4, f);
				b4c = wait;
			}
			a++;
		}
		break;
	case 4: // есть 4 обращения
		set_activec(p1, 1);
		if (check_call(p2))
			set_activec(p2, 2);
		else
		{
			cq.push(2, s);
			b2c = wait;
		}
		if (check_call(p3))
			set_activec(p3, 3);
		else
		{
			cq.push(3, d);
			b3c = wait;
		}
		if (check_call(p4))
			set_activec(p4, 4);
		else
		{
			cq.push(4, f);
			b4c = wait;
		}
		break;
	}
}

void set_states_no_c(int c1, int c2, int c3, int c4, 
	Processor& p1, Processor& p2, Processor& p3, Processor& p4)
{
	int next_c = 0;
	switch (c1 + c2 + c3 + c4)
	{
	case 0: // нет обращений
		a++; s++; d++; f++;
		break;
	case 1: // 1 обращение
		next_c = comparate_statesc(c1, c2, c3, c4); // находим обращение
		switch (next_c) // по команде находим проц
		{
		case 1:
			set_activec(p1, 1);
			s++; d++; f++;
			break;
		case 2:
			set_activec(p2, 2);
			a++; d++; f++;
			break;
		case 3:
			set_activec(p3, 3);
			a++; s++; f++;
			break;
		case 4:
			set_activec(p4, 4);
			a++; s++; d++;
			break;
		}
		break;
	case 2: // 2 обращения
		next_c = comparate_statesc(c1, c2, c3, c4); // находим обращение
		switch (next_c)
		{
		case 1:
			set_activec(p1, 1);
			next_c = comparate_statesc(0, c2, c3, c4); // находим 2 обращение
			switch (next_c)
			{
			case 2:
				cstatus = 3;
				if (check_call(p2)) 
					set_activec(p2, 2);
				else { 
					cq.push(2, s); 
					b2c = wait; 
				}
				d++; f++;
				break;
			case 3:
				cstatus+=2;
				if (check_call(p3)) 
					set_activec(p3, 3); 
				else { 
					cq.push(3, d); 
					b3c = wait; 
				}
				s++; f++;
				break;
			case 4:
				cstatus = 3;
				if (check_call(p4)) 
					set_activec(p4, 4); 
				else { 
					cq.push(4, f); 
					b4c = wait; 
				}
				d++; s++;
				break;
			}
			break;
		case 2:
			set_activec(p2, 2);
			next_c = comparate_statesc(0, 0, c3, c4); // находим 2 обращение
			switch (next_c)
			{
			case 3:
				cstatus = 3;
				if (check_call(p3)) 
					set_activec(p3, 3); 
				else 
				{ 
					cq.push(3, d); 
					b3c = wait; 
				}
				a++; f++;
				break;
			case 4:
				cstatus = 3;
				if (check_call(p4)) 
					set_activec(p4, 4); 
				else { 
					cq.push(4, f); 
					b4c = wait; 
				}
				d++; a++;
				break;
			}
			break;
		case 3:
			set_activec(p3, 3);
			cstatus = 3;
			if (check_call(p4)) 
				set_activec(p4, 4); 
			else { 
				cq.push(4, f); 
				b4c = wait; 
			}
			a++; s++;
			break;
		}
		break;
	case 3: // 3 обращения
		next_c = comparate_statesc(c1, c2, c3, c4); // проверяем, 1 обращается или нет
		if (next_c == 1)
		{
			set_activec(p1, 1);
			next_c = comparate_statesc(0, c2, c3, c4); // проверяем, 2 обращается или нет
			cstatus += 3;
			if (next_c == 2)
			{
				if (check_call(p2)) set_activec(p2, 2); else cq.push(2, s);
				next_c = comparate_statesc(0, 0, c3, c4); // проверяем 3
				if (next_c == 3)
				{
					if (check_call(p3)) 
						set_activec(p3, 3); 
					else { 
						cq.push(3, d); 
						b3c = wait; 
					}
					f++;
				}
				else
				{
					if (check_call(p4))
						set_activec(p4, 4); 
					else { 
						cq.push(4, f);
						b4c = wait; 
					}
					d++;
				}
			}
			else
			{
				if (check_call(p3)) 
					set_activec(p3, 3);
				else { 
					cq.push(3, d);
					b3c = wait; 
				}
				if (check_call(p4)) 
					set_activec(p4, 4); 
				else { 
					cq.push(4, f); 
					b4c = wait; 
				}
				s++;
			}
		}
		else
		{
			set_activec(p2, 2);
			cstatus += 3;
			if (check_call(p3))
				set_activec(p3, 3); 
			else { 
				cq.push(3, d); 
				b3c = wait; 
			}
			if (check_call(p4)) 
				set_activec(p4, 4);
			else { 
				cq.push(4, f); 
				b4c = wait; 
			}
			a++;
		}
		break;
	case 4: // 4 обращения
		set_activec(p1, 1);
		cstatus += 4;
		if (check_call(p2)) 
			set_activec(p2, 2); 
		else { 
			cq.push(2, s);
			b2c = wait; 
		}
		if (check_call(p3))
			set_activec(p3, 3); 
		else { 
			cq.push(3, d); 
			b3c = wait; 
		}
		if (check_call(p4))
			set_activec(p4, 4); 
		else { 
			cq.push(4, f);
		b4c = wait; 
		}
		break;
	}
}

void set_states_1_c(int c1, int c2, int c3, int c4,
	Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m)
{
	switch (c1+c2+c3+c4)
	{
	case 1: // нет новых обращений
		if ((time_mem1) >= m || (time_mem2 >= m) || (time_mem3 >= m) || (time_mem4 >= m))
		{
			int act_m = comparate_statesc(m1c, m2c, m3c, m4c); // находим активную память
			int act_p = comparate_statesc(p1c, p2c, p3c, p4c);
			switch (act_m)
			{
			case 1:
				time_mem1 = 0;
				off_processorsc(act_p);
				off_memoryc(act_m);
				a++; s++; d++; f++;
				break;
			case 2:
				time_mem2 = 0;
				off_processorsc(act_p);
				off_memoryc(act_m);
				a++; s++; d++; f++;
				break;
			case 3:
				time_mem3 = 0;
				off_processorsc(act_p);
				off_memoryc(act_m);
				a++; s++; d++; f++;
				break;
			case 4:
				time_mem4 = 0;
				off_processorsc(act_p);
				off_memoryc(act_m);
				a++; s++; d++; f++;
				break;
			}
		}
		else
		{
			int act_c = comparate_statesc(m1c, m2c, m3c, m4c);
			int act_p = comparate_statesc(p1c, p2c, p3c, p4c);
			switch (act_c)
			{
			case 1:
				time_mem1++;
				switch (act_p)
				{
				case 1: 
					s++; d++; f++; time_p1++; 
					break;
				case 2: 
					a++; d++; f++; time_p2++; 
					break;
				case 3: 
					a++; s++; f++; time_p3++; 
					break;
				case 4: 
					a++; s++; d++; time_p4++; 
					break;
				}
				break;
			case 2:
				time_mem2++;
				switch (act_p)
				{
				case 1:
					s++; d++; f++; time_p1++; 
					break;
				case 2: 
					a++; d++; f++; time_p2++; 
					break;
				case 3: 
					a++; s++; f++; time_p3++; 
					break;
				case 4: 
					a++; s++; d++; time_p4++; 
					break;
				}
				break;
			case 3:
				time_mem3++;
				switch (act_p)
				{
				case 1: 
					s++; d++; f++; time_p1++; 
					break;
				case 2:
					a++; d++; f++; time_p2++;
					break;
				case 3: 
					a++; s++; f++; time_p3++; 
					break;
				case 4: 
					a++; s++; d++; time_p4++; 
					break;
				}
				break;
			case 4:
				time_mem4++;
				switch (act_p)
				{
				case 1: 
					s++; d++; f++; time_p1++; 
					break;
				case 2:
					a++; d++; f++; time_p2++;
					break;
				case 3:
					a++; s++; f++; time_p3++;
					break;
				case 4:
					a++; s++; d++; time_p4++; 
					break;
				}
				break;
			}
		}
		break;
	case 2: // есть одно новое обращение, становится активным или в очередь
		if ((time_mem1 >= m) || (time_mem2 >= m) || (time_mem3 >= m) || (time_mem4 >= m))
		{
			int act_m = comparate_statesc(m1c, m2c, m3c, m4c); // находим активную память
			int act_p = comparate_statesc(p1c, p2c, p3c, p4c);
			int next_c = 0;
			switch (act_m)
			{
			case 1:
				time_mem1 = 0;
				off_processorsc(act_p);
				off_memoryc(act_m);
				next_c = comparate_statesc(0, c2, c3, c4);
				switch (next_c)
				{
				case 2: 
					if (cq.check_empty()) 
				{ 
					a++; 
					set_activec(p2, 2); 
					d++; f++; 
				}
					  else { 
					start_next_queuec(p1, p2, p3, p4);
					a++; d++; f++; 
				}
					  break;
				case 3: 
					if (cq.check_empty()) 
				{ 
					a++; s++; 
					set_activec(p3, 3); f++; 
				}
					  else 
				{ 
					start_next_queuec(p1, p2, p3, p4); 
					a++; s++; f++;
				} break;
				case 4: 
					if (cq.check_empty()) { 
					a++; s++; d++; 
					set_activec(p4, 4); 
				}
					  else { 
					start_next_queuec(p1, p2, p3, p4);
					a++; s++; d++; }
					  break;
				}
				break;
			case 2:
				time_mem2 = 0;
				off_processorsc(act_p);
				off_memoryc(act_m);
				next_c = comparate_statesc(c1, 0, c3, c4);
				switch (next_c)
				{
				case 1: 
					if (cq.check_empty()) 
				{ 
					set_activec(p1, 1); 
					s++; d++; f++; }
					  else { 
					start_next_queuec(p1, p2, p3, p4);
					s++; d++; f++; } 
					  break;
				case 3:
					if (cq.check_empty()) { 
					a++; s++; 
					set_activec(p3, 3); 
					f++; }
					  else { 
					start_next_queuec(p1, p2, p3, p4); 
					a++; s++; f++; 
				} 
					  break;
				case 4:
					if (cq.check_empty()) { 
					a++; s++; d++; 
					set_activec(p4, 4); 
				}
					  else { 
					start_next_queuec(p1, p2, p3, p4); 
					a++; s++; d++; } 
					  break;
				}
				break;
			case 3:
				time_mem3 = 0;
				off_processorsc(act_p);
				off_memoryc(act_m);
				next_c = comparate_statesc(c1, c2, 0, c4);
				switch (next_c)
				{
				case 1: 
					if (cq.check_empty()) { 
					set_activec(p1, 1); 
					s++; d++; f++; }
					  else { 
					start_next_queuec(p1, p2, p3, p4); 
					s++; d++; f++; } 
					  break;
				case 2: 
					if (cq.check_empty()) { 
					a++; 
					set_activec(p2, 2); 
					d++; f++; }
					  else { 
					start_next_queuec(p1, p2, p3, p4); 
					a++; d++; f++; } 
					  break;
				case 4: 
					if (cq.check_empty()) { 
					a++; s++; d++; 
					set_activec(p4, 4); }
					  else { 
					start_next_queuec(p1, p2, p3, p4); 
					a++; s++; d++; } 
					  break;
				}
				break;
			case 4:
				time_mem4 = 0;
				off_processorsc(act_p);
				off_memoryc(act_m);
				next_c = comparate_statesc(c1, c2, c3, 0);
				switch (next_c)
				{
				case 1:
					if (cq.check_empty()) { 
					set_activec(p1, 1); 
					s++; d++; f++; }
					  else {
					start_next_queuec(p1, p2, p3, p4); 
					s++; d++; f++; } 
					  break;
				case 2:
					if (cq.check_empty()) { 
					a++; 
					set_activec(p2, 2); d++; f++; 
				}
					  else { 
					start_next_queuec(p1, p2, p3, p4); 
					a++; d++; f++; } 
					  break;
				case 3:
					if (cq.check_empty()) { 
					a++; s++; 
					set_activec(p3, 3); 
					f++; }
					  else {
					start_next_queuec(p1, p2, p3, p4); 
					a++; s++; f++; } 
					  break;
				}
				break;
			}
		}
		else
		{
			time_waitc++;
			cstatus++;
			int act_m = comparate_statesc(m1c, m2c, m3c, m4c); // находим активную память
			int act_p = comparate_statesc(p1c, p2c, p3c, p4c); // активный проц
			int next_c = 0;
			switch (act_m)
			{
			case 1:
				time_mem1++;
				switch (act_p)
				{
				case 1:
					next_c = comparate_statesc(0, c2, c3, c4); // находим, кто следующий обращается
					time_p1++;
					switch (next_c)
					{
					case 2:
						if (cq.check_empty()) {
						if (check_call(p2)) 
							set_activec(p2, 2); 
						else { 
							cq.push(2, s); 
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						d++; f++; 
						break;
					case 3:
						if (cq.check_empty()) { 
						if (check_call(p3)) 
							set_activec(p3, 3); 
						else { 
							cq.push(3, d); 
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						s++; f++; 
						break;
					case 4:
						if (cq.check_empty()) { 
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f); 
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						d++; s++; 
						break;
					}
					break;
				case 2:
					next_c = comparate_statesc(c1, 0, c3, c4); // находим, кто следующий обращается
					time_p2++;
					switch (next_c)
					{
					case 1:
						if (cq.check_empty()) { 
						if (check_call(p1)) 
							set_activec(p1, 1); 
						else { 
							cq.push(1, a); 
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						d++; f++; 
						break;
					case 3: 
						if (cq.check_empty()) {
						if (check_call(p3)) 
							set_activec(p3, 3); 
						else { 
							cq.push(3, d); 
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						a++; f++; 
						break;
					case 4: 
						if (cq.check_empty()) { 
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f); 
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						d++; a++; 
						break;
					}
					break;
				case 3:
					next_c = comparate_statesc(c1, c2, 0, c4); // находим, кто следующий обращается
					time_p3++;
					switch (next_c)
					{
					case 2:
						if (cq.check_empty()) {
						if (check_call(p2)) 
							set_activec(p2, 2); 
						else { 
							cq.push(2, s); 
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						a++; f++;
						break;
					case 1:
						if (cq.check_empty()) { 
						if (check_call(p1)) 
							set_activec(p1, 1); 
						else { 
							cq.push(1, a); 
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						s++; f++; 
						break;
					case 4:
						if (cq.check_empty()) { 
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f); 
							b2c = wait; 
						}
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						a++; s++; 
						break;
					}
					break;
				case 4:
					next_c = comparate_statesc(c1, c2, c3, 0); // находим, кто следующий обращается
					time_p4++;
					switch (next_c)
					{
					case 2:
						if (cq.check_empty()) { 
						if (check_call(p2)) 
							set_activec(p2, 2); 
						else { 
							cq.push(2, s); 
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						d++; a++; 
						break;
					case 3:
						if (cq.check_empty()) { 
						if (check_call(p3)) 
							set_activec(p3, 3); 
						else { 
							cq.push(3, d);
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						s++; a++; 
						break;
					case 1: 
						if (cq.check_empty()) { 
						if (check_call(p1)) 
							set_activec(p1, 1); 
						else {
							cq.push(1, a);
							b2c = wait;
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						d++; s++; 
						break;
					}
					break;
				}
				break;
			case 2:
				time_mem2++;
				switch (act_p)
				{
				case 1:
					next_c = comparate_statesc(0, c2, c3, c4); // находим, кто следующий обращается
					time_p1++;
					switch (next_c)
					{
					case 2:
						if (cq.check_empty()) {
						if (check_call(p2)) 
							set_activec(p2, 2); 
						else { 
							cq.push(2, s);
							b2c = wait;
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						d++; f++; 
						break;
					case 3:
						if (cq.check_empty()) { 
						if (check_call(p3)) 
							set_activec(p3, 3);
						else { 
							cq.push(3, d); 
							b2c = wait;
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						s++; f++; 
						break;
					case 4:
						if (cq.check_empty()) { 
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f); 
							b2c = wait;
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						d++; s++;
						break;
					}
					break;
				case 2:
					next_c = comparate_statesc(c1, 0, c3, c4); // находим, кто следующий обращается
					time_p2++;
					switch (next_c)
					{
					case 1: 
						if (cq.check_empty()) {
						if (check_call(p1)) 
							set_activec(p1, 1);
						else { 
							cq.push(1, a);
							b2c = wait; 
						} 
					}
						  else
						start_next_queuec(p1, p2, p3, p4);
						d++; f++;
						break;
					case 3:
						if (cq.check_empty()) {
						if (check_call(p3)) 
							set_activec(p3, 3); 
						else {
							cq.push(3, d);
							b2c = wait;
						}
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						a++; f++;
						break;
					case 4:
						if (cq.check_empty()) { 
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f); 
							b2c = wait;
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						d++;
						a++;
						break;
					}
					break;
				case 3:
					next_c = comparate_statesc(c1, c2, 0, c4); // находим, кто следующий обращается
					time_p3++;
					switch (next_c)
					{
					case 2:
						if (cq.check_empty()) { 
						if (check_call(p2)) 
							set_activec(p2, 2); 
						else {
							cq.push(2, s);
							b2c = wait;
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						a++; f++; 
						break;
					case 1:
						if (cq.check_empty()) { 
						if (check_call(p1))
							set_activec(p1, 1); 
						else {
							cq.push(1, a); 
							b2c = wait;
						}
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						s++;
						f++; 
						break;
					case 4:
						if (cq.check_empty()) { 
						if (check_call(p4)) 
							set_activec(p4, 4);
						else { 
							cq.push(4, f); 
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						a++; s++;
						break;
					}
					break;
				case 4:
					next_c = comparate_statesc(c1, c2, c3, 0); // находим, кто следующий обращается
					time_p4++;
					switch (next_c)
					{
					case 2:
						if (cq.check_empty()) { 
						if (check_call(p2)) 
							set_activec(p2, 2); 
						else {
							cq.push(2, s);
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						d++; a++; 
						break;
					case 3:
						if (cq.check_empty()) { 
						if (check_call(p3)) 
							set_activec(p3, 3); 
						else { 
							cq.push(3, d);
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						s++; a++; 
						break;
					case 1: 
						if (cq.check_empty()) {
						if (check_call(p1))
							set_activec(p1, 1); 
						else { 
							cq.push(1, a);
							b2c = wait;
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						d++; s++; 
						break;
					}
					break;
				}
				break;
			case 3:
				time_mem3++;
				switch (act_p)
				{
				case 1:
					next_c = comparate_statesc(0, c2, c3, c4); // находим, кто следующий обращается
					time_p1++;
					switch (next_c)
					{
					case 2: 
						if (cq.check_empty()) { 
						if (check_call(p2)) 
							set_activec(p2, 2); 
						else { 
							cq.push(2, s);
							b2c = wait;
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						d++; f++; 
						break;
					case 3: 
						if (cq.check_empty()) {
						if (check_call(p3))
							set_activec(p3, 3); 
						else {
							cq.push(3, d);
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						s++; f++; 
						break;
					case 4:
						if (cq.check_empty()) { 
						if (check_call(p4))
							set_activec(p4, 4);
						else { 
							cq.push(4, f); 
							b2c = wait; 
						}
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						d++; s++; 
						break;
					}
					break;
				case 2:
					next_c = comparate_statesc(c1, 0, c3, c4); // находим, кто следующий обращается
					time_p2++;
					switch (next_c)
					{
					case 1: 
						if (cq.check_empty()) { 
						if (check_call(p1)) 
							set_activec(p1, 1);
						else { 
							cq.push(1, a);
							b2c = wait;
						}
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						d++; f++; 
						break;
					case 3:
						if (cq.check_empty()) { 
						if (check_call(p3))
							set_activec(p3, 3);
						else { 
							cq.push(3, d); 
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						a++; f++;
						break;
					case 4: 
						if (cq.check_empty()) { 
						if (check_call(p4)) 
							set_activec(p4, 4); else { 
							cq.push(4, f); 
							b2c = wait;
						}
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						d++; a++;
						break;
					}
					break;
				case 3:
					next_c = comparate_statesc(c1, c2, 0, c4); // находим, кто следующий обращается
					time_p3++;
					switch (next_c)
					{
					case 2:
						if (cq.check_empty()) { 
						if (check_call(p2))
							set_activec(p2, 2); 
						else { 
							cq.push(2, s);
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						a++; f++; 
						break;
					case 1: 
						if (cq.check_empty()) { 
						if (check_call(p1)) 
							set_activec(p1, 1);
						else { 
							cq.push(1, a);
							b2c = wait; }
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						s++; f++; 
						break;
					case 4:
						if (cq.check_empty()) { 
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f); 
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						a++; s++;
						break;
					}
					break;
				case 4:
					next_c = comparate_statesc(c1, c2, c3, 0); // находим, кто следующий обращается
					time_p4++;
					switch (next_c)
					{
					case 2: 
						if (cq.check_empty()) {
						if (check_call(p2))
							set_activec(p2, 2); 
						else { 
							cq.push(2, s); 
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4);
						d++; a++; 
						break;
					case 3: 
						if (cq.check_empty()) { 
						if (check_call(p3))
							set_activec(p3, 3); 
						else {
							cq.push(3, d); 
							b2c = wait; 
						}
					}
						  else
						start_next_queuec(p1, p2, p3, p4);
						s++; a++;
						break;
					case 1: 
						if (cq.check_empty()) { 
						if (check_call(p1)) 
							set_activec(p1, 1); 
						else { 
							cq.push(1, a); 
							b2c = wait; 
						}
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						d++; s++; 
						break;
					}
					break;
				}
				break;
			case 4:
				time_mem4++;
				switch (act_p)
				{
				case 1:
					next_c = comparate_statesc(0, c2, c3, c4); // находим, кто следующий обращается
					time_p1++;
					switch (next_c)
					{
					case 2:
						if (cq.check_empty()) { 
						if (check_call(p2)) 
							set_activec(p2, 2); 
						else { 
							cq.push(2, s); 
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						d++; f++; 
						break;
					case 3: 
						if (cq.check_empty()) { 
						if (check_call(p3)) 
							set_activec(p3, 3); 
						else { 
							cq.push(3, d); 
							b2c = wait; 
						} 
					}
						  else 
						start_next_queuec(p1, p2, p3, p4); 
						s++; f++; 
						break;
					case 4: 
						if (cq.check_empty()) { 
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { cq.push(4, f); 
						b2c = wait; } }
						  else start_next_queuec(p1, p2, p3, p4); d++; s++; break;
					}
					break;
				case 2:
					next_c = comparate_statesc(c1, 0, c3, c4); // находим, кто следующий обращается
					time_p2++;
					switch (next_c)
					{
					case 1:
						if (cq.check_empty()) { 
							if (check_call(p1)) 
								set_activec(p1, 1);
							else { 
								cq.push(1, a);
								b2c = wait;
							} 
						}
						  else 
							start_next_queuec(p1, p2, p3, p4); 
						d++; f++; 
						break;
					case 3:
						if (cq.check_empty()) { 
							if (check_call(p3)) 
								set_activec(p3, 3);
							else { 
								cq.push(3, d);
								b2c = wait;
							} 
						}
						  else 
							start_next_queuec(p1, p2, p3, p4); 
						a++; f++;
						break;
					case 4:
						if (cq.check_empty()) {
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f);
								b2c = wait; 
							} 
						}
						  else 
							start_next_queuec(p1, p2, p3, p4); 
						d++; a++; 
						break;
					}
					break;
				case 3:
					next_c = comparate_statesc(c1, c2, 0, c4); // находим, кто следующий обращается
					time_p3++;
					switch (next_c)
					{
					case 2: 
						if (cq.check_empty()) { 
							if (check_call(p2)) 
								set_activec(p2, 2); 
							else {
								cq.push(2, s);
								b2c = wait;
							} 
						}
						  else 
							start_next_queuec(p1, p2, p3, p4); 
						a++; f++; 
						break;
					case 1: 
						if (cq.check_empty()) {
							if (check_call(p1)) 
								set_activec(p1, 1);
							else { 
								cq.push(1, a);
								b2c = wait;
							} 
						}
						  else
							start_next_queuec(p1, p2, p3, p4); 
						s++; f++; 
						break;
					case 4: 
						if (cq.check_empty()) {
							if (check_call(p4)) 
								set_activec(p4, 4); 
							else { 
								cq.push(4, f); 
								b2c = wait; } }
						  else 
							start_next_queuec(p1, p2, p3, p4);
						a++; s++;
						break;
					}
					break;
				case 4:
					next_c = comparate_statesc(c1, c2, c3, 0); // находим, кто следующий обращается
					time_p4++;
					switch (next_c)
					{
					case 2: 
						if (cq.check_empty()) { 
							if (check_call(p2))
								set_activec(p2, 2);
							else { 
								cq.push(2, s);
								b2c = wait;
							} 
						}
						  else 
							start_next_queuec(p1, p2, p3, p4);
						d++; a++;
						break;
					case 3: 
						if (cq.check_empty()) {
							if (check_call(p3)) 
								set_activec(p3, 3); 
							else { 
								cq.push(3, d); 
								b2c = wait;
							} 
						}
						  else 
							start_next_queuec(p1, p2, p3, p4);
						s++; a++; 
						break;
					case 1: 
						if (cq.check_empty()) {
							if (check_call(p1)) 
								set_activec(p1, 1); 
							else { 
								cq.push(1, a);
								b2c = wait; 
							} 
						}
						  else 
							start_next_queuec(p1, p2, p3, p4); 
						d++; s++; 
						break;
					}
					break;
				}
				break;
			}
		}
		break;
	case 3: // есть два новых обращения, 2/1 становятся активным или в очередь
		cstatus = 4;
		if ((time_mem1 >= m) || (time_mem2 >= m) || (time_mem3 >= m) || (time_mem4 >= m))
		{
			int act_m = comparate_statesc(m1c, m2c, m3c, m4c);
			int act_p = 0;
			int next_c = 0;
			switch (act_m)
			{
			case 1:
				time_mem1 = 0;
				off_memoryc(act_m);
				act_p = comparate_statesc(p1c, p2c, p3c, p4c);
				off_processorsc(act_p);
				next_c = comparate_statesc(0, c2, c3, c4); // находим следующее обращение
				switch (next_c)
				{
				case 2:
					if (c3 == 1)
					{
						if (cq.check_empty()) 
						{ 
							if (check_call(p2)) set_activec(p2, 2); else { cq.push(2, s); b2c = wait; }
							if (check_call(p3)) set_activec(p3, 3); else { cq.push(3, d); b3c = wait; }
							a++; f++;
						}
						else
						{
							cq.push(2, s); b2c = wait;
							cq.push(3, d); b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							a++; f++;
						}
					}
					else
					{
						if (cq.check_empty())
						{
							if (check_call(p2)) set_activec(p2, 2); else { cq.push(2, s); b2c = wait; }
							if (check_call(p4)) set_activec(p4, 4); else { cq.push(4, f); b4c = wait; }
							a++; d++;
						}
						else
						{
							cq.push(2, s); b2c = wait;
							cq.push(4, f); b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							a++; d++;
						}
					}
					break;
				case 3:
					if (cq.check_empty())
					{
						if (check_call(p3)) set_activec(p3, 3); else { cq.push(3, d); b3c = wait; }
						if (check_call(p4)) set_activec(p4, 4); else { cq.push(4, f); b4c = wait; }
						a++; s++;
					}
					else
					{
						cq.push(2, s); b2c = wait;
						cq.push(3, d); b3c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
						a++; s++;
					}
					break;
				}
				break;
			case 2:
				time_mem2 = 0;
				off_memoryc(act_m);
				act_p = comparate_statesc(p1c, p2c, p3c, p4c);
				off_processorsc(act_p);
				next_c = comparate_statesc(c1, 0, c3, c4);
				switch (next_c)
				{
				case 1:
					if (c3 == 1)
					{
						if (cq.check_empty())
						{
							if (check_call(p1)) set_activec(p1, 1); else { cq.push(1, a); b1c = wait; }
							if (check_call(p3)) set_activec(p3, 3); else { cq.push(3, d); b3c = wait; }
							s++; f++;
						}
						else
						{
							cq.push(1, a); b1c = wait;
							cq.push(3, d); b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++; f++;
						}
					}
					else
					{
						if (cq.check_empty())
						{
							if (check_call(p1)) set_activec(p1, 1); else { cq.push(1, a); b1c = wait; }
							if (check_call(p4)) set_activec(p4, 4); else { cq.push(4, f); b4c = wait; }
							s++; d++;
						}
						else
						{
							cq.push(1, 1); b1c = wait;
							cq.push(4, f); b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++; d++;
						}
					}
					break;
				case 3:
					if (cq.check_empty())
					{
						if (check_call(p3)) set_activec(p3, 3); else { cq.push(3, d); b3c = wait; }
						if (check_call(p4)) set_activec(p4, 4); else { cq.push(4, f); b4c = wait; }
						a++; s++;
					}
					else
					{
						cq.push(3, d); b3c = wait;
						cq.push(4, f); b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
						a++; s++;
					}
				}
				break;
			case 3:
				time_mem3 = 0;
				off_memoryc(act_m);
				act_p = comparate_statesc(p1c, p2c, p3c, p4c);
				off_processorsc(act_p);
				next_c = comparate_statesc(c1, c2, 0, c4);
				switch (next_c)
				{
				case 1:
					if (c2 == 1)
					{
						if (cq.check_empty())
						{
							if (check_call(p1)) set_activec(p1, 1); else { cq.push(1, a); b1c = wait; }
							if (check_call(p2)) set_activec(p2, 2); else { cq.push(2, s); b2c = wait; }
							d++; f++;
						}
						else
						{
							cq.push(1, a); b1c = wait;
							cq.push(2, s); b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							d++; f++;
						}
					}
					else
					{
						if (cq.check_empty())
						{
							if (check_call(p1)) set_activec(p1, 1); else { cq.push(1, a); b1c = wait; }
							if (check_call(p4)) set_activec(p4, 4); else { cq.push(4, f); b4c = wait; }
							s++; d++;
						}
						else
						{
							cq.push(1, 1); b1c = wait;
							cq.push(4, f); b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++; d++;
						}
					}
					break;
				case 2:
					if (cq.check_empty())
					{
						if (check_call(p2)) set_activec(p2, 2); else { cq.push(2, s); b2c = wait; }
						if (check_call(p4)) set_activec(p4, 4); else { cq.push(4, f); b4c = wait; }
						a++; d++;
					}
					else
					{
						cq.push(2, s); b2c = wait;
						cq.push(4, f); b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
						a++; d++;
					}
				}
				break;
			case 4:
				time_mem4 = 0;
				off_memoryc(act_m);
				act_p = comparate_statesc(p1c, p2c, p3c, p4c);
				off_processorsc(act_p);
				next_c = comparate_statesc(c1, c2, c3, 0);
				switch (next_c)
				{
				case 1:
					if (c3 == 1)
					{
						if (cq.check_empty())
						{
							if (check_call(p1)) set_activec(p1, 1); else { cq.push(1, a); b1c = wait; }
							if (check_call(p3)) set_activec(p3, 3); else { cq.push(3, d); b3c = wait; }
							s++; f++;
						}
						else
						{
							cq.push(1, a); b1c = wait;
							cq.push(3, d); b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++; f++;
						}
					}
					else
					{
						if (cq.check_empty())
						{
							if (check_call(p1)) set_activec(p1, 1); else { cq.push(1, a); b1c = wait; }
							if (check_call(p2)) set_activec(p2, 2); else { cq.push(2, s); b2c = wait; }
							f++; d++;
						}
						else
						{
							cq.push(1, 1); b1c = wait;
							cq.push(2, s); b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							f++; d++;
						}
					}
					break;
				case 2:
					if (cq.check_empty())
					{
						if (check_call(p2)) set_activec(p2, 2); else { cq.push(2, s); b2c = wait; }
						if (check_call(p3)) set_activec(p3, 3); else { cq.push(3, d); b3c = wait; }
						a++; f++;
					}
					else
					{
						cq.push(2, s); b2c = wait;
						cq.push(3, d); b3c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
						a++; f++;
					}
				}
				break;
			}
		}
		else
		{
			int act_m = comparate_statesc(m1c, m2c, m3c, m4c);
			time_waitc++;
			int act_p = 0;
			int no_c = 0;
			switch (act_m)
			{
			case 1:
				time_mem1++;
				act_p = comparate_statesc(p1c, p2c, p3c, p4c);
				switch (act_p)
				{
				case 1:
					time_p1++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 2:
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3);
							else {
								cq.push(3, d);
								b3c = wait; 
							}
							if (check_call(p4)) 
								set_activec(p4, 4);
							else {
								cq.push(4, f);
								b4c = wait; 
							}
							s++;
						}
						else
						{
							cq.push(3, d); 
							b3c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++;
						}
						break;
					case 3:
						if (cq.check_empty())
						{
							if (check_call(p2))
								set_activec(p2, 2);
							else { 
								cq.push(2, s);
								b2c = wait; 
							}
							if (check_call(p4)) 
								set_activec(p4, 4);
							else {
								cq.push(4, f); 
								b4c = wait; }
							d++;
						}
						else
						{
							cq.push(2, s); 
							b2c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					case 4:
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2);
							else { 
								cq.push(2, s); 
								b2c = wait; }
							if (check_call(p3)) 
								set_activec(p3, 3);
							else { 
								cq.push(3, d);
								b3c = wait; }
							f++;
						}
						else
						{
							cq.push(2, s); 
							b2c = wait;
							cq.push(3, d);
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						break;
					}
					break;
				case 2:
					time_p2++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 1:
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3);
							else
							{ 
								cq.push(3, d);
								b3c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f);
								b4c = wait; }
							a++;
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					case 3:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1); 
							else {
								cq.push(1, a);
								b1c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else {
								cq.push(4, f);
								b4c = wait; }
							d++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					case 4:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1);
							else {
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p3))
								set_activec(p3, 3);
							else {
								cq.push(3, d); 
								b3c = wait; }
							f++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(3, d);
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						break;
					}
					break;
				case 3:
					time_p3++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 2:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1); 
							else { 
								cq.push(1, a);
								b1c = wait; }
							if (check_call(p4))
								set_activec(p4, 4);
							else { 
								cq.push(4, f); 
								b4c = wait; }
							s++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++;
						}
						break;
					case 1:
						if (cq.check_empty())
						{
							if (check_call(p2))
								set_activec(p2, 2);
							else { 
								cq.push(2, s);
								b2c = wait; }
							if (check_call(p4))
								set_activec(p4, 4);
							else {
								cq.push(4, f);
								b4c = wait; }
							a++;
						}
						else
						{
							cq.push(2, s);
							b2c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					case 4:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1);
							else { 
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p2)) 
								set_activec(p2, 2); 
							else { 
								cq.push(2, s); 
								b2c = wait; }
							f++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(2, s); 
							b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						break;
					}
					break;
				case 4:
					time_p4++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 2:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1); 
							else { 
								cq.push(1, a);
								b1c = wait; }
							if (check_call(p3)) 
								set_activec(p3, 3);
							else { 
								cq.push(3, d);
								b3c = wait; }
							s++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(3, d);
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++;
						}
						break;
					case 1:
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2); 
							else { 
								cq.push(2, s); 
								b2c = wait; }
							if (check_call(p3)) 
								set_activec(p3, 3); 
							else { 
								cq.push(3, d); 
								b3c = wait; }
							a++;
						}
						else
						{
							cq.push(2, s);
							b2c = wait;
							cq.push(3, d);
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					case 3:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1);
							else { 
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p2))
								set_activec(p2, 2); 
							else { 
								cq.push(2, s);
								b2c = wait; }
							d++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(2, s);
							b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					}
					break;
				}
				break;
			case 2:
				time_mem2++;
				act_p = comparate_statesc(p1c, p2c, p3c, p4c);
				switch (act_p)
				{
				case 1:
					time_p1++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 2:
						if (cq.check_empty())
						{
							if (check_call(p3))
								set_activec(p3, 3);
							else {
								cq.push(3, d); 
								b3c = wait; }
							if (check_call(p4))
								set_activec(p4, 4); 
							else {
								cq.push(4, f);
								b4c = wait; }
							s++;
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++;
						}
						break;
					case 3:
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2);
							else { 
								cq.push(2, s);
								b2c = wait; }
							if (check_call(p4))
								set_activec(p4, 4);
							else { 
								cq.push(4, f);
								b4c = wait; }
							d++;
						}
						else
						{
							cq.push(2, s); 
							b2c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					case 4:
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2);
							else { 
								cq.push(2, s);
								b2c = wait; }
							if (check_call(p3)) 
								set_activec(p3, 3);
							else { 
								cq.push(3, d);
								b3c = wait; }
							f++;
						}
						else
						{
							cq.push(2, s);
							b2c = wait;
							cq.push(3, d);
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						break;
					}
					break;
				case 2:
					time_p2++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 1:
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3); 
							else { 
								cq.push(3, d); 
								b3c = wait; }
							if (check_call(p4))
								set_activec(p4, 4);
							else { 
								cq.push(4, f); 
								b4c = wait; }
							a++;
						}
						else
						{
							cq.push(3, d); 
							b3c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					case 3:
						if (cq.check_empty())
						{
							if (check_call(p1))
								set_activec(p1, 1); 
							else { 
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f); 
								b4c = wait; }
							d++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					case 4:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1);
							else {
								cq.push(1, a);
								b1c = wait; }
							if (check_call(p3)) 
								set_activec(p3, 3); 
							else {
								cq.push(3, d); 
								b3c = wait; }
							f++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(3, d); 
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						break;
					}
					break;
				case 3:
					time_p3++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 2:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1); 
							else {
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p4))
								set_activec(p4, 4);
							else { 
								cq.push(4, f);
								b4c = wait; }
							s++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++;
						}
						break;
					case 1:
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2); 
							else { 
								cq.push(2, s);
								b2c = wait; }
							if (check_call(p4))
								set_activec(p4, 4); 
							else { 
								cq.push(4, f);
								b4c = wait; }
							a++;
						}
						else
						{
							cq.push(2, s);
							b2c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					case 4:
						if (cq.check_empty())
						{
							if (check_call(p1))
								set_activec(p1, 1); 
							else {
								cq.push(1, a);
								b1c = wait; }
							if (check_call(p2))
								set_activec(p2, 2);
							else { 
								cq.push(2, s);
								b2c = wait; }
							f++;
						}
						else
						{
							cq.push(1, a); 
							b1c = wait;
							cq.push(2, s);
							b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						break;
					}
					break;
				case 4:
					time_p4++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 2:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1);
							else {
								cq.push(1, a);
								b1c = wait; }
							if (check_call(p3))
								set_activec(p3, 3);
							else {
								cq.push(3, d); 
								b3c = wait; }
							s++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(3, d); 
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++;
						}
						break;
					case 1:
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2);
							else { 
								cq.push(2, s);
								b2c = wait; }
							if (check_call(p3)) 
								set_activec(p3, 3);
							else { 
								cq.push(3, d);
								b3c = wait; }
							a++;
						}
						else
						{
							cq.push(2, s);
							b2c = wait;
							cq.push(3, d); 
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					case 3:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1); 
							else { 
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p2)) 
								set_activec(p2, 2);
							else { 
								cq.push(2, s); 
								b2c = wait; }
							d++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(2, s); 
							b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					}
					break;
				}
				break;
			case 3:
				time_mem3++;
				act_p = comparate_statesc(p1c, p2c, p3c, p4c);
				switch (act_p)
				{
				case 1:
					time_p1++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 2:
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3);
							else { 
								cq.push(3, d); 
								b3c = wait; }
							if (check_call(p4))
								set_activec(p4, 4);
							else { 
								cq.push(4, f);
								b4c = wait; }
							s++;
						}
						else
						{
							cq.push(3, d); 
							b3c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++;
						}
						break;
					case 3:
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2); 
							else { 
								cq.push(2, s);
								b2c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4); 
							else { 
								cq.push(4, f); 
								b4c = wait; }
							d++;
						}
						else
						{
							cq.push(2, s); 
							b2c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					case 4:
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2);
							else { 
								cq.push(2, s); 
								b2c = wait; }
							if (check_call(p3)) 
								set_activec(p3, 3); 
							else { 
								cq.push(3, d); 
								b3c = wait; }
							f++;
						}
						else
						{
							cq.push(2, s); 
							b2c = wait;
							cq.push(3, d); 
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						break;
					}
					break;
				case 2:
					time_p2++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 1:
						if (cq.check_empty())
						{
							if (check_call(p3))
								set_activec(p3, 3); 
							else { 
								cq.push(3, d); 
								b3c = wait; }
							if (check_call(p4))
								set_activec(p4, 4); 
							else { 
								cq.push(4, f); 
								b4c = wait; }
							a++;
						}
						else
						{
							cq.push(3, d); 
							b3c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					case 3:
						if (cq.check_empty())
						{
							if (check_call(p1))
								set_activec(p1, 1);
							else { 
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4); 
							else { 
								cq.push(4, f); 
								b4c = wait; }
							d++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					case 4:
						if (cq.check_empty())
						{
							if (check_call(p1))
								set_activec(p1, 1);
							else { 
								cq.push(1, a);
								b1c = wait; }
							if (check_call(p3)) 
								set_activec(p3, 3);
							else { 
								cq.push(3, d);
								b3c = wait; }
							f++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(3, d); 
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						break;
					}
					break;
				case 3:
					time_p3++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 2:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1); 
							else { 
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f); 
								b4c = wait; }
							s++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++;
						}
						break;
					case 1:
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2);
							else { 
								cq.push(2, s); 
								b2c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4); 
							else { 
								cq.push(4, f);
								b4c = wait; }
							a++;
						}
						else
						{
							cq.push(2, s);
							b2c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					case 4:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1);
							else {
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p2)) 
								set_activec(p2, 2);
							else { 
								cq.push(2, s); 
								b2c = wait; }
							f++;
						}
						else
						{
							cq.push(1, a); 
							b1c = wait;
							cq.push(2, s); 
							b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						break;
					}
					break;
				case 4:
					time_p4++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 2:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1); 
							else { 
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p3)) 
								set_activec(p3, 3); 
							else { 
								cq.push(3, d);
								b3c = wait; }
							s++;
						}
						else
						{
							cq.push(1, a); 
							b1c = wait;
							cq.push(3, d); 
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++;
						}
						break;
					case 1:
						if (cq.check_empty())
						{
							if (check_call(p2))
								set_activec(p2, 2);
							else { 
								cq.push(2, s);
								b2c = wait; }
							if (check_call(p3))
								set_activec(p3, 3);
							else { 
								cq.push(3, d);
							b3c = wait; }
							a++;
						}
						else
						{
							cq.push(2, s); 
							b2c = wait;
							cq.push(3, d); 
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					case 3:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1);
							else { 
								cq.push(1, a);
								b1c = wait; }
							if (check_call(p2))
								set_activec(p2, 2);
							else {
								cq.push(2, s); 
								b2c = wait; }
							d++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(2, s);
							b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					}
					break;
				}
				break;
			case 4:
				time_mem4++;
				act_p = comparate_statesc(p1c, p2c, p3c, p4c);
				int no_c = 0;
				switch (act_p)
				{
				case 1:
					time_p1++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 2:
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3);
							else {
								cq.push(3, d);
								b3c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4); 
							else {
								cq.push(4, f);
								b4c = wait; }
							s++;
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++;
						}
						break;
					case 3:
						if (cq.check_empty())
						{
							if (check_call(p2))
								set_activec(p2, 2);
							else { 
								cq.push(2, s); 
								b2c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f); 
								b4c = wait; }
							d++;
						}
						else
						{
							cq.push(2, s); 
							b2c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					case 4:
						if (cq.check_empty())
						{
							if (check_call(p2))
								set_activec(p2, 2); 
							else { 
								cq.push(2, s); 
								b2c = wait; }
							if (check_call(p3)) 
								set_activec(p3, 3); 
							else { 
								cq.push(3, d); 
								b3c = wait; }
							f++;
						}
						else
						{
							cq.push(2, s); 
							b2c = wait;
							cq.push(3, d); 
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						break;
					}
					break;
				case 2:
					time_p2++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 1:
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3); 
							else { 
								cq.push(3, d); 
								b3c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4); 
							else { 
								cq.push(4, f); 
								b4c = wait; }
							a++;
						}
						else
						{
							cq.push(3, d); 
							b3c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					case 3:
						if (cq.check_empty())
						{
							if (check_call(p1))
								set_activec(p1, 1); 
							else { 
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f); 
								b4c = wait; }
							d++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					case 4:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1);
							else { 
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p3)) 
								set_activec(p3, 3); else { 
								cq.push(3, d); 
								b3c = wait; }
							f++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(3, d); 
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						break;
					}
					break;
				case 3:
					time_p3++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 2:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1); 
							else { 
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4); 
							else { 
								cq.push(4, f); 
								b4c = wait; }
							s++;
						}
						else
						{
							cq.push(1, a); 
							b1c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++;
						}
						break;
					case 1:
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2); 
							else { 
								cq.push(2, s); 
								b2c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4); 
							else { 
								cq.push(4, f); 
								b4c = wait; }
							a++;
						}
						else
						{
							cq.push(2, s); 
							b2c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					case 4:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1);
							else { 
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p2)) 
								set_activec(p2, 2); 
							else { 
								cq.push(2, s); 
								b2c = wait; }
							f++;
						}
						else
						{
							cq.push(1, a); 
							b1c = wait;
							cq.push(2, s); 
							b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						break;
					}
					break;
				case 4:
					time_p4++;
					no_c = min_comparate_statesc(c1, c2, c3, c4); // находим не обращающийся процессор
					switch (no_c)
					{
					case 2:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1); 
							else { 
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p3)) 
								set_activec(p3, 3); 
							else { 
								cq.push(3, d); 
								b3c = wait; }
							s++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(3, d); 
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							s++;
						}
						break;
					case 1:
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2); 
							else { 
								cq.push(2, s); 
								b2c = wait; }
							if (check_call(p3)) 
								set_activec(p3, 3); 
							else { 
								cq.push(3, d); 
								b3c = wait; }
							a++;
						}
						else
						{
							cq.push(2, s); 
							b2c = wait;
							cq.push(3, d); 
							b3c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					case 3:
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1); 
							else { 
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p2)) 
								set_activec(p2, 2); 
							else { 
								cq.push(2, s); 
								b2c = wait; }
							d++;
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(2, s); 
							b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					}
					break;
				}
				break;
			}
		}
		break;
	case 4: // есть три новых обращения, 3/2/1 становятся активными или в очередь
		cstatus = 5;
		if ((time_mem1 >= m) || (time_mem2 >= m) || (time_mem3 >= m) || (time_mem4 >= m))
		{
			int act_m = comparate_statesc(m1c, m2c, m3c, m4c);
			int act_p = comparate_statesc(p1c, p2c, p3c, p4c);
			switch (act_m)
			{
			case 1:
				time_mem1 = 0;
				off_memoryc(act_m);
				switch (act_p)
				{
				case 1:
					a++;
					if (cq.check_empty())
					{
						if (check_call(p2))
							set_activec(p2, 2); 
						else { 
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p3)) 
							set_activec(p3, 3); 
						else { 
							cq.push(3, d); 
							b3c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4);
						else { 
							cq.push(4, f);
							b4c = wait; }
					}
					else
					{
						cq.push(2, s); 
						b2c = wait;
						cq.push(3, d); 
						b3c = wait;
						cq.push(4, f); 
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				case 2:
					s++;
					if (cq.check_empty())
					{
						if (check_call(p1)) 
							set_activec(p1, 1);
						else { 
							cq.push(1, a); 
							b1c = wait; }
						if (check_call(p3)) 
							set_activec(p3, 3); 
						else { 
							cq.push(3, d);
							b3c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f); 
							b4c = wait; }
					}
					else
					{
						cq.push(1, a);
						b1c = wait;
						cq.push(3, d);
						b3c = wait;
						cq.push(4, f); 
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				case 3:
					d++;
					if (cq.check_empty())
					{
						if (check_call(p2))
							set_activec(p2, 2);
						else {
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p1)) 
							set_activec(p1, 1); 
						else { 
							cq.push(1, a); 
							b1c = wait; }
						if (check_call(p4))
							set_activec(p4, 4);
						else { 
							cq.push(4, f);
							b4c = wait; }
					}
					else
					{
						cq.push(2, s);
						b2c = wait; 
						cq.push(1, a);
						b1c = wait;
						cq.push(4, f);
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				case 4:
					f++;
					if (cq.check_empty())
					{
						if (check_call(p2))
							set_activec(p2, 2);
						else {
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p3))
							set_activec(p3, 3); 
						else { 
							cq.push(3, d);
							b3c = wait; }
						if (check_call(p1))
							set_activec(p1, 1);
						else { 
							cq.push(1, a); 
							b1c = wait; }
					}
					else
					{
						cq.push(2, s);
						b2c = wait; 
						cq.push(3, d); 
						b3c = wait; 
						cq.push(1, a);
						b1c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				}
				break;
			case 2:
				time_mem2 = 0;
				off_memoryc(act_m);
				switch (act_p)
				{
				case 1:
					a++;
					if (cq.check_empty())
					{
						if (check_call(p2)) 
							set_activec(p2, 2);
						else { 
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p3))
							set_activec(p3, 3); 
						else { 
							cq.push(3, d); 
							b3c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f);
							b4c = wait; }
					}
					else
					{
						cq.push(2, s); 
						b2c = wait; 
						cq.push(3, d);
						b3c = wait;
						cq.push(4, f);
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				case 2:
					s++;
					if (cq.check_empty())
					{
						if (check_call(p1))
							set_activec(p1, 1);
						else { 
							cq.push(1, a); 
							b1c = wait; }
						if (check_call(p3))
							set_activec(p3, 3);
						else { 
							cq.push(3, d); 
							b3c = wait; }
						if (check_call(p4))
							set_activec(p4, 4);
						else {
							cq.push(4, f);
							b4c = wait; }
					}
					else
					{
						cq.push(1, a);
						b1c = wait; 
						cq.push(3, d); 
						b3c = wait; 
						cq.push(4, f); 
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				case 3:
					d++;
					if (cq.check_empty())
					{
						if (check_call(p2))
							set_activec(p2, 2);
						else { 
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p1))
							set_activec(p1, 1);
						else { 
							cq.push(1, a);
							b1c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4);
						else { 
							cq.push(4, f); 
							b4c = wait; }
					}
					else
					{
						cq.push(2, s);
						b2c = wait; 
						cq.push(1, a); 
						b1c = wait; 
						cq.push(4, f);
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				case 4:
					f++;
					if (cq.check_empty())
					{
						if (check_call(p2)) 
							set_activec(p2, 2); 
						else { 
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p3))
							set_activec(p3, 3);
						else { 
							cq.push(3, d);
							b3c = wait; }
						if (check_call(p1))
							set_activec(p1, 1);
						else { 
							cq.push(1, a);
							b1c = wait; }
					}
					else
					{
						cq.push(2, s);
						b2c = wait;
						cq.push(3, d);
						b3c = wait; 
						cq.push(1, a); 
						b1c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				}
				break;
			case 3:
				time_mem3 = 0;
				off_memoryc(act_m);
				switch (act_p)
				{
				case 1:
					a++;
					if (cq.check_empty())
					{
						if (check_call(p2))
							set_activec(p2, 2);
						else { 
							cq.push(2, s); 
							b2c = wait; }
						if (check_call(p3)) 
							set_activec(p3, 3);
						else { 
							cq.push(3, d); 
							b3c = wait; }
						if (check_call(p4))
							set_activec(p4, 4); 
						else { 
							cq.push(4, f); 
							b4c = wait; }
					}
					else
					{
						cq.push(2, s); 
						b2c = wait; 
						cq.push(3, d);
						b3c = wait; 
						cq.push(4, f);
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				case 2:
					s++;
					if (cq.check_empty())
					{
						if (check_call(p1)) 
							set_activec(p1, 1); 
						else { 
							cq.push(1, a); 
							b1c = wait; }
						if (check_call(p3))
							set_activec(p3, 3);
						else { 
							cq.push(3, d);
							b3c = wait; }
						if (check_call(p4))
							set_activec(p4, 4);
						else { 
							cq.push(4, f); 
							b4c = wait; }
					}
					else
					{
						cq.push(1, a); 
						b1c = wait; 
						cq.push(3, d); 
						b3c = wait; 
						cq.push(4, f); 
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				case 3:
					d++;
					if (cq.check_empty())
					{
						if (check_call(p2)) 
							set_activec(p2, 2);
						else {
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p1)) 
							set_activec(p1, 1); 
						else { 
							cq.push(1, a); 
							b1c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f); 
							b4c = wait; }
					}
					else
					{
						cq.push(2, s);
						b2c = wait;
						cq.push(1, a); 
						b1c = wait;
						cq.push(4, f);
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				case 4:
					f++;
					if (cq.check_empty())
					{
						if (check_call(p2))
							set_activec(p2, 2);
						else {
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p3)) 
							set_activec(p3, 3);
						else {
							cq.push(3, d);
							b3c = wait; }
						if (check_call(p1)) 
							set_activec(p1, 1);
						else { 
							cq.push(1, a); 
							b1c = wait; }
					}
					else
					{
						cq.push(2, s); 
						b2c = wait;
						cq.push(3, d); 
						b3c = wait; 
						cq.push(1, a);
						b1c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				}
				break;
			case 4:
				time_mem4 = 0;
				off_memoryc(act_m);
				switch (act_p)
				{
				case 1:
					a++;
					if (cq.check_empty())
					{
						if (check_call(p2)) 
							set_activec(p2, 2); 
						else { 
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p3))
							set_activec(p3, 3);
						else { 
							cq.push(3, d);
							b3c = wait; }
						if (check_call(p4))
							set_activec(p4, 4);
						else { 
							cq.push(4, f);
							b4c = wait; }
					}
					else
					{
						cq.push(2, s); 
						b2c = wait; 
						cq.push(3, d);
						b3c = wait; 
						cq.push(4, f); 
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				case 2:
					s++;
					if (cq.check_empty())
					{
						if (check_call(p1))
							set_activec(p1, 1); 
						else {
							cq.push(1, a);
							b1c = wait; }
						if (check_call(p3))
							set_activec(p3, 3); 
						else { 
							cq.push(3, d); 
							b3c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f);
							b4c = wait; }
					}
					else
					{
						cq.push(1, a); 
						b1c = wait;
						cq.push(3, d);
						b3c = wait; 
						cq.push(4, f);
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				case 3:
					d++;
					if (cq.check_empty())
					{
						if (check_call(p2))
							set_activec(p2, 2);
						else { 
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p1))
							set_activec(p1, 1);
						else { 
							cq.push(1, a); 
							b1c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f); 
							b4c = wait; }
					}
					else
					{
						cq.push(2, s); 
						b2c = wait; 
						cq.push(1, a); 
						b1c = wait; 
						cq.push(4, f);
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				case 4:
					f++;
					if (cq.check_empty())
					{
						if (check_call(p2)) 
							set_activec(p2, 2);
						else { 
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p3)) 
							set_activec(p3, 3);
						else { 
							cq.push(3, d); 
						b3c = wait; }
						if (check_call(p1)) 
							set_activec(p1, 1);
						else { 
							cq.push(1, a);
							b1c = wait; }
					}
					else
					{
						cq.push(2, s); 
						b2c = wait; 
						cq.push(3, d);
						b3c = wait; 
						cq.push(1, a); 
						b1c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					off_processorsc(act_p);
					break;
				}
				break;
			}
		}
		else
		{
			time_waitc++;
			int act_m = comparate_statesc(m1c, m2c, m3c, m4c);
			int act_p = comparate_statesc(p1c, p2c, p3c, p4c);
			switch (act_m)
			{
			case 1:
				time_mem1++;
				switch (act_p)
				{
				case 1:
					time_p1++;
					if (cq.check_empty())
					{
						if (check_call(p2))
							set_activec(p2, 2); 
						else { 
							cq.push(2, s);
						b2c = wait; }
						if (check_call(p3))
							set_activec(p3, 3); 
						else { 
							cq.push(3, d); 
							b3c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f); 
							b4c = wait; }
					}
					else
					{
						cq.push(2, s);
						b2c = wait; 
						cq.push(3, d); 
						b3c = wait; 
						cq.push(4, f); 
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 2:
					time_p2++;
					if (cq.check_empty())
					{
						if (check_call(p1)) 
							set_activec(p1, 1);
						else { 
							cq.push(1, a);
							b1c = wait; }
						if (check_call(p3))
							set_activec(p3, 3); 
						else { 
							cq.push(3, d);
							b3c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4);
						else { 
							cq.push(4, f);
							b4c = wait; }
					}
					else
					{
						cq.push(1, a); 
						b1c = wait; 
						cq.push(3, d);
						b3c = wait; 
						cq.push(4, f);
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 3:
					time_p3++;
					if (cq.check_empty())
					{
						if (check_call(p2))
							set_activec(p2, 2);
						else { 
							cq.push(2, s); 
							b2c = wait; }
						if (check_call(p1)) 
							set_activec(p1, 1);
						else { 
							cq.push(1, a); 
							b1c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4);
						else { 
							cq.push(4, f);
							b4c = wait; }
					}
					else
					{
						cq.push(2, s); 
						b2c = wait; 
						cq.push(1, a);
						b1c = wait; 
						cq.push(4, f); 
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 4:
					time_p4++;
					if (cq.check_empty())
					{
						if (check_call(p2))
							set_activec(p2, 2); 
						else { 
							cq.push(2, s); 
							b2c = wait; }
						if (check_call(p3)) 
							set_activec(p3, 3);
						else { 
							cq.push(3, d); 
							b3c = wait; }
						if (check_call(p1)) 
							set_activec(p1, 1);
						else { 
							cq.push(1, a); 
							b1c = wait; }
					}
					else
					{
						cq.push(2, s); 
						b2c = wait; 
						cq.push(3, d);
						b3c = wait; 
						cq.push(1, a);
						b1c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				}
				break;
			case 2:
				time_mem2++;
				switch (act_p)
				{
				case 1:
					time_p1++;
					if (cq.check_empty())
					{
						if (check_call(p2))
							set_activec(p2, 2);
						else {
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p3)) 
							set_activec(p3, 3); 
						else { 
							cq.push(3, d); 
							b3c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4);
						else { 
							cq.push(4, f);
							b4c = wait; }
					}
					else
					{
						cq.push(2, s); 
						b2c = wait; 
						cq.push(3, d); 
						b3c = wait;
						cq.push(4, f);
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 2:
					time_p2++;
					if (cq.check_empty())
					{
						if (check_call(p1)) 
							set_activec(p1, 1);
						else {
							cq.push(1, a);
							b1c = wait; }
						if (check_call(p3)) 
							set_activec(p3, 3); 
						else { 
							cq.push(3, d);
							b3c = wait; }
						if (check_call(p4))
							set_activec(p4, 4); 
						else { 
							cq.push(4, f);
							b4c = wait; }
					}
					else
					{
						cq.push(1, a); 
						b1c = wait; 
						cq.push(3, d); 
						b3c = wait; 
						cq.push(4, f); 
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 3:
					time_p3++;
					if (cq.check_empty())
					{
						if (check_call(p2))
							set_activec(p2, 2);
						else { 
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p1)) 
							set_activec(p1, 1);
						else {
							cq.push(1, a);
							b1c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f);
							b4c = wait; }
					}
					else
					{
						cq.push(2, s); 
						b2c = wait; 
						cq.push(1, a);
						b1c = wait; 
						cq.push(4, f); 
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 4:
					time_p4++;
					if (cq.check_empty())
					{
						if (check_call(p2))
							set_activec(p2, 2); 
						else { 
							cq.push(2, s); 
							b2c = wait; }
						if (check_call(p3))
							set_activec(p3, 3); 
						else { 
							cq.push(3, d); 
							b3c = wait; }
						if (check_call(p1)) 
							set_activec(p1, 1);
						else { 
							cq.push(1, a); 
							b1c = wait; }
					}
					else
					{
						cq.push(2, s); 
						b2c = wait; 
						cq.push(3, d); 
						b3c = wait;
						cq.push(1, a);
						b1c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				}
				break;
			case 3:
				time_mem3++;
				switch (act_p)
				{
				case 1:
					time_p1++;
					if (cq.check_empty())
					{
						if (check_call(p2))
							set_activec(p2, 2);
						else { 
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p3)) 
							set_activec(p3, 3); 
						else { 
							cq.push(3, d);
							b3c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else {
							cq.push(4, f);
							b4c = wait; }
					}
					else
					{
						cq.push(2, s); 
						b2c = wait; 
						cq.push(3, d);
						b3c = wait; 
						cq.push(4, f); 
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 2:
					time_p2++;
					if (cq.check_empty())
					{
						if (check_call(p1)) 
							set_activec(p1, 1); 
						else { 
							cq.push(1, a);
							b1c = wait; }
						if (check_call(p3)) 
							set_activec(p3, 3); 
						else { 
							cq.push(3, d); 
							b3c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4);
						else { 
							cq.push(4, f); 
							b4c = wait; }
					}
					else
					{
						cq.push(1, a); 
						b1c = wait; 
						cq.push(3, d); 
						b3c = wait; 
						cq.push(4, f);
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 3:
					time_p3++;
					if (cq.check_empty())
					{
						if (check_call(p2)) 
							set_activec(p2, 2);
						else {
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p1)) 
							set_activec(p1, 1);
						else {
							cq.push(1, a); 
							b1c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f); 
							b4c = wait; }
					}
					else
					{
						cq.push(2, s);
						b2c = wait; 
						cq.push(1, a); 
						b1c = wait; 
						cq.push(4, f);
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 4:
					time_p4++;
					if (cq.check_empty())
					{
						if (check_call(p2)) 
							set_activec(p2, 2);
						else { 
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p3)) 
							set_activec(p3, 3); 
						else { 
							cq.push(3, d); 
							b3c = wait; }
						if (check_call(p1)) 
							set_activec(p1, 1); 
						else { 
							cq.push(1, a); 
							b1c = wait; }
					}
					else
					{
						cq.push(2, s);
						b2c = wait; 
						cq.push(3, d);
						b3c = wait;
						cq.push(1, a);
						b1c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				}
				break;
			case 4:
				time_mem4++;
				switch (act_p)
				{
				case 1:
					time_p1++;
					if (cq.check_empty())
					{
						if (check_call(p2)) 
							set_activec(p2, 2);
						else {
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p3)) 
							set_activec(p3, 3);
						else { 
							cq.push(3, d); 
							b3c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4);
						else { 
							cq.push(4, f);
							b4c = wait; }
					}
					else
					{
						cq.push(2, s);
						b2c = wait; 
						cq.push(3, d);
						b3c = wait; 
						cq.push(4, f);
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 2:
					time_p2++;
					if (cq.check_empty())
					{
						if (check_call(p1)) 
							set_activec(p1, 1); 
						else { 
							cq.push(1, a);
							b1c = wait; }
						if (check_call(p3))
							set_activec(p3, 3);
						else { 
							cq.push(3, d);
							b3c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f); 
							b4c = wait; }
					}
					else
					{
						cq.push(1, a);
						b1c = wait;
						cq.push(3, d);
						b3c = wait; 
						cq.push(4, f);
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 3:
					time_p3++;
					if (cq.check_empty())
					{
						if (check_call(p2)) 
							set_activec(p2, 2);
						else { 
							cq.push(2, s);
							b2c = wait; }
						if (check_call(p1)) 
							set_activec(p1, 1); 
						else { 
							cq.push(1, a); 
							b1c = wait; }
						if (check_call(p4)) 
							set_activec(p4, 4); 
						else { 
							cq.push(4, f);
							b4c = wait; }
					}
					else
					{
						cq.push(2, s); 
						b2c = wait; 
						cq.push(1, a);
						b1c = wait;
						cq.push(4, f);
						b4c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 4:
					time_p4++;
					if (cq.check_empty())
					{
						if (check_call(p2))
							set_activec(p2, 2);
						else { 
							cq.push(2, s); 
							b2c = wait; }
						if (check_call(p3))
							set_activec(p3, 3);
						else { 
							cq.push(3, d);
						b3c = wait; }
						if (check_call(p1)) 
							set_activec(p1, 1);
						else { 
							cq.push(1, a);
						b1c = wait; }
					}
					else
					{
						cq.push(2, s); 
						b2c = wait;
						cq.push(3, d);
						b3c = wait; 
						cq.push(1, a); 
						b1c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				}
				break;
			}
		}
		break;
	}
}

void set_states_2_c(int c1, int c2, int c3, int c4,
	Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m)
{
	switch (c1 + c2 + c3 + c4)
	{
	case 2: // нет новых обращений
		if ((time_mem1 >= m) || (time_mem2 >= m) || (time_mem3 >= m) || (time_mem4 >= m))
		{
			int act_m1 = comparate_statesc(m1c, m2c, m3c, m4c), act_m2 = 0; // находим 1 активную
			int act_p1 = comparate_statesc(p1c, p2c, p3c, p4c), act_p2 = 0;
			switch (act_p1) // смотрим по 1 процессору
			{
			case 1: // 1 проц
				act_p2 = comparate_statesc(0, p2c, p3c, p4c);
				switch (act_m1) // смотрим по 1 памяти
				{
				case 1: // 1 память
					act_m2 = comparate_statesc(0, m2c, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 2:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1); 
								a++; }
							else if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2);
								a++; }
							if (time_p2 >= m && time_mem1 >= m) {
								off_processorsc(act_p2);
								off_memoryc(1);
								s++; }
							else if (time_p2 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2); 
								s++; }
							break;
						case 3:
							if (time_p1 >= m && time_mem1 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(1);
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) {
								off_processorsc(act_p1);
								off_memoryc(3);
								a++; }
							if (time_p2 >= m && time_mem1 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(1);
								s++; }
							else if (time_p2 >= m && time_mem3 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(3); 
								s++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem1 >= m) {
								off_processorsc(act_p1);
								off_memoryc(1); 
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(4); 
								a++; }
							if (time_p2 >= m && time_mem1 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(1); 
								s++; }
							else if (time_p2 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(4);
								s++; }
							break;
						}
						d++; f++;
						break;
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1);
								a++; }
							else if (time_p1 >= m && time_mem2 >= m) {
								off_processorsc(act_p1);
								off_memoryc(2); 
								a++; }
							if (time_p3 >= m && time_mem1 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(1);
								d++; }
							else if (time_p3 >= m && time_mem2 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(2);
								d++; }
							break;
						case 3:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(1);
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(3);
								a++; }
							if (time_p3 >= m && time_mem1 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(1); 
								d++; }
							else if (time_p3 >= m && time_mem3 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(3);
								d++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem1 >= m) {
								off_processorsc(act_p1);
								off_memoryc(1);
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(4); 
								a++; }
							if (time_p3 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(1);
								d++; }
							else if (time_p3 >= m && time_mem4 >= m) {
								off_processorsc(act_p2);
								off_memoryc(4); 
								d++; }
							break;
						}
						s++; f++;
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(1);
								a++; }
							else if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(2);
								a++; }
							if (time_p4 >= m && time_mem1 >= m) {
								off_processorsc(act_p2);
								off_memoryc(1);
								f++; }
							else if (time_p2 >= m && time_mem2 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(2); 
								f++; }
							break;
						case 3:
							if (time_p1 >= m && time_mem1 >= m) {
								off_processorsc(act_p1);
								off_memoryc(1); 
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(3); 
								a++; }
							if (time_p4 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); 
								f++; }
							else if (time_p4 >= m && time_mem3 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(3); 
								f++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1); 
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(4);
								a++; }
							if (time_p4 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(1); 
								f++; }
							else if (time_p4 >= m && time_mem4 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(4); 
								f++; }
							break;
						}
						s++; d++;
						break;
					}
					break;
				case 2: // 2 mem
					act_m2 = comparate_statesc(0, 0, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 2:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(2);
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(3); 
								a++; }
							if (time_p2 >= m && time_mem2 >= m) {
								off_processorsc(act_p2);
								off_memoryc(2);
								s++; }
							else if (time_p2 >= m && time_mem3 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(3); 
								s++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(2); 
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(4);
								a++; }
							if (time_p2 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2); 
								s++; }
							else if (time_p2 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(4); 
								s++; }
							break;
						}
						d++; f++;
						break;
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2);
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(3); 
								a++; }
							if (time_p3 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(2); 
								d++; }
							else if (time_p3 >= m && time_mem3 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(3); 
								d++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2); 
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(4); 
								a++; }
							if (time_p3 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2);
								d++; }
							else if (time_p3 >= m && time_mem4 >= m) {
								off_processorsc(act_p2);
								off_memoryc(4);
								d++; }
							break;
						}
						s++; f++;
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2); 
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(3); 
								a++; }
							if (time_p4 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(2);
								f++; }
							else if (time_p4 >= m && time_mem3 >= m) {
								off_processorsc(act_p2);
								off_memoryc(3); 
								f++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem2 >= m) {
								off_processorsc(act_p1);
								off_memoryc(2); 
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(4); 
								a++; }
							if (time_p4 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2); 
								f++; }
							else if (time_p4 >= m && time_mem4 >= m) {
								off_processorsc(act_p2);
								off_memoryc(4); 
								f++; }
							break;
						}
						s++; d++;
						break;
					}
					break;
				case 3: // 3 mem
					switch (act_p2) // смотрим по 2 процессору
					{
					case 2:
						if (time_p1 >= m && time_mem3 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(3); 
							a++; }
						else if (time_p1 >= m && time_mem4 >= m) { 
							off_processorsc(act_p1);
							off_memoryc(4); 
							a++; }
						if (time_p2 >= m && time_mem3 >= m) { 
							off_processorsc(act_p2); 
							off_memoryc(3);
							s++; }
						else if (time_p2 >= m && time_mem4 >= m) { 
							off_processorsc(act_p2); 
							off_memoryc(4); 
							s++; }
						d++; f++;
						break;
					case 3:
						if (time_p1 >= m && time_mem3 >= m) {
							off_processorsc(act_p1);
							off_memoryc(3); 
							a++; }
						else if (time_p1 >= m && time_mem4 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(4); 
							a++; }
						if (time_p3 >= m && time_mem3 >= m) {
							off_processorsc(act_p2); 
							off_memoryc(3); 
							d++; }
						else if (time_p3 >= m && time_mem4 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(4); 
							d++; }
						s++; f++;
						break;
					case 4:
						if (time_p1 >= m && time_mem3 >= m) {
							off_processorsc(act_p1);
							off_memoryc(3); 
							a++; }
						else if (time_p1 >= m && time_mem4 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(4); 
							a++; }
						if (time_p4 >= m && time_mem3 >= m) { 
							off_processorsc(act_p2); 
							off_memoryc(3); 
							f++; }
						else if (time_p4 >= m && time_mem4 >= m) {
							off_processorsc(act_p2);
							off_memoryc(4); 
							f++; }
						s++; d++;
						break;
					}
					break;
				}
				break;
			case 2: // 2 proc
				act_p2 = comparate_statesc(0, 0, p3c, p4c);
				switch (act_m1) // смотрим по 1 памяти
				{
				case 1: // 1 память
					act_m2 = comparate_statesc(0, m2c, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							if (time_p2 >= m && time_mem1 >= m) {
								off_processorsc(act_p1);
								off_memoryc(1); 
								s++; }
							else if (time_p2 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2);
								s++; }
							if (time_p3 >= m && time_mem1 >= m) {
								off_processorsc(act_p2);
								off_memoryc(1); 
								d++; }
							else if (time_p3 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(2); 
								d++; }
							break;
						case 3:
							if (time_p2 >= m && time_mem1 >= m) {
								off_processorsc(act_p1);
								off_memoryc(1); 
								s++; }
							else if (time_p2 >= m && time_mem3 >= m) {
								off_processorsc(act_p1);
								off_memoryc(3);
								s++; }
							if (time_p3 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); 
								d++; }
							else if (time_p3 >= m && time_mem3 >= m) {
								off_processorsc(act_p2);
								off_memoryc(3);
								d++; }
							break;
						case 4:
							if (time_p2 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1);
								s++; }
							else if (time_p2 >= m && time_mem4 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(4);
								s++; }
							if (time_p3 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1);
								d++; }
							else if (time_p3 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(4);
								d++; }
							break;
						}
						a++; f++;
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							if (time_p2 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(1);
								s++; }
							else if (time_p2 >= m && time_mem2 >= m) {
								off_processorsc(act_p1);
								off_memoryc(2);
								s++; }
							if (time_p4 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); 
								f++; }
							else if (time_p4 >= m && time_mem2 >= m) {
								off_processorsc(act_p2);
								off_memoryc(2); 
								f++; }
							break;
						case 3:
							if (time_p2 >= m && time_mem1 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(1); 
								s++; }
							else if (time_p2 >= m && time_mem3 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(3); 
								s++; }
							if (time_p4 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1);
								f++; }
							else if (time_p4 >= m && time_mem3 >= m) {
								off_processorsc(act_p2);
								off_memoryc(3);
								f++; }
							break;
						case 4:
							if (time_p2 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1); 
								s++; }
							else if (time_p2 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(4); 
								s++; }
							if (time_p4 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); 
								f++; }
							else if (time_p4 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(4); 
								f++; }
							break;
						}
						a++; d++;
						break;
					}
					break;
				case 2: // 2 mem
					act_m2 = comparate_statesc(0, 0, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							if (time_p2 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2); 
								s++; }
							else if (time_p2 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(3); 
								s++; }
							if (time_p3 >= m && time_mem2 >= m) {
								off_processorsc(act_p2);
								off_memoryc(2);
								d++; }
							else if (time_p3 >= m && time_mem3 >= m) {
								off_processorsc(act_p2);
								off_memoryc(3); 
								d++; }
							break;
						case 4:
							if (time_p2 >= m && time_mem2 >= m) {
								off_processorsc(act_p1);
								off_memoryc(2); 
								s++; }
							else if (time_p2 >= m && time_mem4 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(4); 
								s++; }
							if (time_p3 >= m && time_mem2 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(2); 
								d++; }
							else if (time_p3 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(4); 
								d++; }
							break;
						}
						a++; f++;
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							if (time_p2 >= m && time_mem2 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(2);
								s++; }
							else if (time_p2 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(3); 
								s++; }
							if (time_p4 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2); 
								f++; }
							else if (time_p4 >= m && time_mem3 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(3); 
								f++; }
							break;
						case 4:
							if (time_p2 >= m && time_mem2 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(2); 
								s++; }
							else if (time_p2 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(4); 
								s++; }
							if (time_p4 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2); 
								f++; }
							else if (time_p4 >= m && time_mem4 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(4); 
								f++; }
							break;
						}
						a++; d++;
						break;
					}
					break;
				case 3: // 3 mem
					switch (act_p2) // смотрим по 2 процессору
					{
					case 3:
						if (time_p2 >= m && time_mem3 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(3); 
							s++; }
						else if (time_p2 >= m && time_mem4 >= m) {
							off_processorsc(act_p1);
							off_memoryc(4);
							s++; }
						if (time_p3 >= m && time_mem3 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(3);
							d++; }
						else if (time_p3 >= m && time_mem4 >= m) {
							off_processorsc(act_p2);
							off_memoryc(4); 
							d++; }
						a++; f++;
						break;
					case 4:
						if (time_p2 >= m && time_mem3 >= m) { 
							off_processorsc(act_p1);
							off_memoryc(3);
							s++; }
						else if (time_p2 >= m && time_mem4 >= m) {
							off_processorsc(act_p1); 
							off_memoryc(4); 
							s++; }
						if (time_p4 >= m && time_mem3 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(3);
							f++; }
						else if (time_p4 >= m && time_mem4 >= m) {
							off_processorsc(act_p2);
							off_memoryc(4); 
							f++; }
						a++; d++;
						break;
					}
					break;
				}
				break;
			case 3: // 3 proc
				act_p2 = 4;
				switch (act_m1) // смотрим по 1 памяти
				{
				case 1: // 1 память
					act_m2 = comparate_statesc(0, m2c, m3c, m4c);
					switch (act_m2) // смотрим по 2 памяти
					{
					case 2:
						if (time_p4 >= m && time_mem1 >= m) { 
							off_processorsc(act_p1);
							off_memoryc(1);
							f++; }
						else if (time_p4 >= m && time_mem2 >= m) {
							off_processorsc(act_p1); 
							off_memoryc(2); 
							f++; }
						if (time_p3 >= m && time_mem1 >= m) {
							off_processorsc(act_p2);
							off_memoryc(1); 
							d++; }
						else if (time_p3 >= m && time_mem2 >= m) { 
							off_processorsc(act_p2); 
							off_memoryc(2); 
							d++; }
						a++; s++;
						break;
					case 3:
						if (time_p4 >= m && time_mem1 >= m) {
							off_processorsc(act_p1); 
							off_memoryc(1); 
							f++; }
						else if (time_p4 >= m && time_mem3 >= m) {
							off_processorsc(act_p1); 
							off_memoryc(3);
							f++; }
						if (time_p3 >= m && time_mem1 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(1); 
							d++; }
						else if (time_p3 >= m && time_mem3 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(3); 
							d++; }
						a++; s++;
						break;
					case 4:
						if (time_p4 >= m && time_mem1 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(1);
							f++; }
						else if (time_p4 >= m && time_mem4 >= m) {
							off_processorsc(act_p1);
							off_memoryc(4); 
							f++; }
						if (time_p3 >= m && time_mem1 >= m) {
							off_processorsc(act_p2); 
							off_memoryc(1); 
							d++; }
						else if (time_p3 >= m && time_mem4 >= m) { 
							off_processorsc(act_p2); 
							off_memoryc(4);
							d++; }
						a++; s++;
						break;
					}
					break;
				case 2: // 2 mem
					act_m2 = comparate_statesc(0, 0, m3c, m4c);
					switch (act_m2) // смотрим по 2 памяти
					{
					case 3:
						if (time_p4 >= m && time_mem2 >= m) {
							off_processorsc(act_p1);
							off_memoryc(2);
							f++; }
						else if (time_p4 >= m && time_mem3 >= m) { 
							off_processorsc(act_p1);
							off_memoryc(3); 
							f++; }
						if (time_p3 >= m && time_mem2 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(2);
							d++; }
						else if (time_p3 >= m && time_mem3 >= m) {
							off_processorsc(act_p2);
							off_memoryc(3);
							d++; }
						a++; s++;
						break;
					case 4:
						if (time_p4 >= m && time_mem2 >= m) {
							off_processorsc(act_p1); 
							off_memoryc(2); 
							f++; }
						else if (time_p4 >= m && time_mem4 >= m) {
							off_processorsc(act_p1);
							off_memoryc(4);
							f++; }
						if (time_p3 >= m && time_mem2 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(2); 
							d++; }
						else if (time_p3 >= m && time_mem4 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(4); 
							d++; }
						a++; s++;
						break;
					}
					break;
				case 3: // 3 mem
					if (time_p4 >= m && time_mem3 >= m) { 
						off_processorsc(act_p1); 
						off_memoryc(3);
						f++; }
					else if (time_p4 >= m && time_mem4 >= m) {
						off_processorsc(act_p1); 
						off_memoryc(4);
						f++; }
					if (time_p3 >= m && time_mem3 >= m) {
						off_processorsc(act_p2); 
						off_memoryc(3);
						d++; }
					else if (time_p3 >= m && time_mem4 >= m) {
						off_processorsc(act_p2); 
						off_memoryc(4);
						d++; }
					a++; s++;
					break;
				}
				break;
			}
		}
		else
		{
		int act_m1 = comparate_statesc(m1c, m2c, m3c, m4c), act_m2 = 0; // находим 1 активную
		int act_p1 = comparate_statesc(p1c, p2c, p3c, p4c), act_p2 = 0;
		switch (act_p1) // смотрим по 1 процессору
		{
		case 1: // 1 проц
			act_p2 = comparate_statesc(0, p2c, p3c, p4c);
			switch (act_m1) // смотрим по 1 памяти
			{
			case 1: // 1 память
				act_m2 = comparate_statesc(0, m2c, m3c, m4c);
				switch (act_p2) // смотрим по 2 процессору
				{
				case 2:
					switch (act_m2) // смотрим по 2 памяти
					{
					case 2:
						time_p1++; time_p2++; time_mem1++; time_mem2++;
						break;
					case 3:
						time_p1++; time_p2++; time_mem1++; time_mem3++;
						break;
					case 4:
						time_p1++; time_p2++; time_mem1++; time_mem4++;
						break;
					}
					d++; f++;
					break;
				case 3:
					switch (act_m2) // смотрим по 2 памяти
					{
					case 2:
						time_p1++; time_p3++; time_mem1++; time_mem2++;
						break;
					case 3:
						time_p1++; time_p3++; time_mem1++; time_mem3++;
						break;
					case 4:
						time_p1++; time_p3++; time_mem1++; time_mem4++;
						break;
					}
					s++; f++;
					break;
				case 4:
					switch (act_m2) // смотрим по 2 памяти
					{
					case 2:
						time_p1++; time_p4++; time_mem1++; time_mem2++;
						break;
					case 3:
						time_p1++; time_p4++; time_mem1++; time_mem3++;
						break;
					case 4:
						time_p1++; time_p4++; time_mem1++; time_mem4++;
						break;
					}
					s++; d++;
					break;
				}
				break;
			case 2: // 2 mem
				act_m2 = comparate_statesc(0, 0, m3c, m4c);
				switch (act_p2) // смотрим по 2 процессору
				{
				case 2:
					switch (act_m2) // смотрим по 2 памяти
					{
					case 3:
						time_p1++; time_p2++; time_mem2++; time_mem3++;
						break;
					case 4:
						time_p1++; time_p2++; time_mem4++; time_mem4++;
						break;
					}
					d++; f++;
					break;
				case 3:
					switch (act_m2) // смотрим по 2 памяти
					{
					case 3:
						time_p1++; time_p3++; time_mem2++; time_mem3++;
						break;
					case 4:
						time_p1++; time_p3++; time_mem4++; time_mem4++;
						break;
					}
					s++; f++;
					break;
				case 4:
					switch (act_m2) // смотрим по 2 памяти
					{
					case 3:
						time_p1++; time_p4++; time_mem2++; time_mem3++;
						break;
					case 4:
						time_p1++; time_p4++; time_mem4++; time_mem4++;
						break;
					}
					s++; d++;
					break;
				}
				break;
			case 3: // 3 mem
				switch (act_p2) // смотрим по 2 процессору
				{
				case 2:
					time_p1++; time_p2++; time_mem3++; time_mem4++;
					d++; f++;
					break;
				case 3:
					time_p1++; time_p3++; time_mem3++; time_mem4++;
					s++; f++;
					break;
				case 4:
					time_p1++; time_p4++; time_mem3++; time_mem4++;
					s++; d++;
					break;
				}
				break;
			}
			break;
		case 2: // 2 proc
			act_p2 = comparate_statesc(0, 0, p3c, p4c);
			switch (act_m1) // смотрим по 1 памяти
			{
			case 1: // 1 память
				act_m2 = comparate_statesc(0, m2c, m3c, m4c);
				switch (act_p2) // смотрим по 2 процессору
				{
				case 3:
					switch (act_m2) // смотрим по 2 памяти
					{
					case 2:
						time_p2++; time_p3++; time_mem1++; time_mem2++;
						break;
					case 3:
						time_p2++; time_p3++; time_mem1++; time_mem3++;
						break;
					case 4:
						time_p2++; time_p3++; time_mem1++; time_mem4++;
						break;
					}
					a++; f++;
					break;
				case 4:
					switch (act_m2) // смотрим по 2 памяти
					{
					case 2:
						time_p2++; time_p4++; time_mem1++; time_mem2++;
						break;
					case 3:
						time_p2++; time_p4++; time_mem1++; time_mem3++;
						break;
					case 4:
						time_p2++; time_p4++; time_mem1++; time_mem4++;
						break;
					}
					a++; d++;
					break;
				}
				break;
			case 2: // 2 mem
				act_m2 = comparate_statesc(0, 0, m3c, m4c);
				switch (act_p2) // смотрим по 2 процессору
				{
				case 3:
					switch (act_m2) // смотрим по 2 памяти
					{
					case 3:
						time_p2++; time_p3++; time_mem2++; time_mem3++;
						break;
					case 4:
						time_p2++; time_p3++; time_mem2++; time_mem4++;
						break;
					}
					a++; f++;
					break;
				case 4:
					switch (act_m2) // смотрим по 2 памяти
					{
					case 3:
						time_p2++; time_p4++; time_mem2++; time_mem3++;
						break;
					case 4:
						time_p2++; time_p4++; time_mem2++; time_mem4++;
						break;
					}
					a++; d++;
					break;
				}
				break;
			case 3: // 3 mem
				switch (act_p2) // смотрим по 2 процессору
				{
				case 3:
					time_p2++; time_p3++; time_mem3++; time_mem4++;
					a++; f++;
					break;
				case 4:
					time_p2++; time_p4++; time_mem3++; time_mem4++;
					a++; d++;
					break;
				}
				break;
			}
			break;
		case 3: // 3 proc
			act_p2 = 4;
			switch (act_m1) // смотрим по 1 памяти
			{
			case 1: // 1 память
				act_m2 = comparate_statesc(0, m2c, m3c, m4c);
				switch (act_m2) // смотрим по 2 памяти
				{
				case 2:
					time_p4++; time_p3++; time_mem1++; time_mem2++;
					a++; s++;
					break;
				case 3:
					time_p4++; time_p3++; time_mem1++; time_mem3++;
					a++; s++;
					break;
				case 4:
					time_p4++; time_p3++; time_mem1++; time_mem4++;
					a++; s++;
					break;
				}
				break;
			case 2: // 2 mem
				act_m2 = comparate_statesc(0, 0, m3c, m4c);
				switch (act_m2) // смотрим по 2 памяти
				{
				case 3:
					time_p4++; time_p3++; time_mem2++; time_mem3++;
					a++; s++;
					break;
				case 4:
					time_p4++; time_p3++; time_mem4++; time_mem2++;
					a++; s++;
					break;
				}
				break;
			case 3: // 3 mem
				time_p4++; time_p3++; time_mem3++; time_mem4++;
				a++; s++;
				break;
			}
			break;
		}
		}
		break;
	case 3: // есть одно новое
		cstatus++;
		if ((time_mem1 >= m) || (time_mem2 >= m) || (time_mem3 >= m) || (time_mem4 >= m))
		{
			int act_m1 = comparate_statesc(m1c, m2c, m3c, m4c), act_m2 = 0; // находим 1 активную
			int act_p1 = comparate_statesc(p1c, p2c, p3c, p4c), act_p2 = 0;
			int next_c = 0; 
			switch (act_p1) // смотрим по 1 процессору
			{
			case 1: // 1 проц
				act_p2 = comparate_statesc(0, p2c, p3c, p4c);
				switch (act_m1) // смотрим по 1 памяти
				{
				case 1: // 1 память
					act_m2 = comparate_statesc(0, m2c, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 2:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1);
								a++; }
							else if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2);
								a++; }
							if (time_p2 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(1);
								s++; }
							else if (time_p2 >= m && time_mem2 >= m) {
								off_processorsc(act_p2);
								off_memoryc(2);
								s++; }
							break;
						case 3:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1);
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(3);
								a++; }
							if (time_p2 >= m && time_mem1 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(1);
								s++; }
							else if (time_p2 >= m && time_mem3 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(3); 
								s++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1); 
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(4); 
								a++; }
							if (time_p2 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); 
								s++; }
							else if (time_p2 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(4); 
								s++; }
							break;
						}
						next_c = comparate_statesc(0, 0, c3, c4);
						if (next_c == 3) 
						{ 
							if (cq.check_empty()) 
							{ 
								f++; 
								if (check_call(p3)) 
									set_activec(p3, 3); 
								else {
									cq.push(3, d);
									b3c = wait;
								}
							}
							else { 
								start_next_queuec(p1, p2, p3, p4); 
								f++; 
							}
						}
						else { 
							if (cq.check_empty()) 
							{
								d++;
								if (check_call(p4)) 
									set_activec(p4, 4);
								else { 
									cq.push(4, f); 
									b4c = wait; }
						} 
							else { 
								start_next_queuec(p1, p2, p3, p4);
								d++;
							} 
						}
						break;
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							if (time_p1 >= m && time_mem1 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(1); 
								a++; }
							else if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2); 
								a++; }
							if (time_p3 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); 
								d++; }
							else if (time_p3 >= m && time_mem2 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(2);
								d++; }
							break;
						case 3:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(1);
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(3); 
								a++; }
							if (time_p3 >= m && time_mem1 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(1); 
								d++; }
							else if (time_p3 >= m && time_mem3 >= m) {
								off_processorsc(act_p2);
								off_memoryc(3);
								d++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1);
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(4); 
								a++; }
							if (time_p3 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); 
								d++; }
							else if (time_p3 >= m && time_mem4 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(4); 
								d++; }
							break;
						}
						next_c = comparate_statesc(0, c1, 0, c4);
						if (next_c == 2) 
						{ 
							if (cq.check_empty()) 
							{ 
								f++;
								if (check_call(p2)) 
									set_activec(p2, 2); 
								else { 
									cq.push(2, s);
									b2c = wait; 
								}
						} 
							else 
							{ 
								start_next_queuec(p1, p2, p3, p4); 
								f++; 
							} 
						}
						else 
						{ 
							if (cq.check_empty()) 
							{ 
								s++; 
								if (check_call(p4)) 
									set_activec(p4, 4); 
								else { 
									cq.push(4, f);
									b4c = wait; 
								}
						} 
							else 
							{ 
								start_next_queuec(p1, p2, p3, p4);
								s++; 
							} 
						}
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1);
								a++; }
							else if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2);
								a++; }
							if (time_p4 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); 
								f++; }
							else if (time_p2 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2);
								f++; }
							break;
						case 3:
							if (time_p1 >= m && time_mem1 >= m) {
								off_processorsc(act_p1);
								off_memoryc(1);
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(3); 
								a++; }
							if (time_p4 >= m && time_mem1 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(1); 
								f++; }
							else if (time_p4 >= m && time_mem3 >= m) {
								off_processorsc(act_p2);
								off_memoryc(3); 
								f++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1); 
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(4);
								a++; }
							if (time_p4 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(1); 
								f++; }
							else if (time_p4 >= m && time_mem4 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(4);
								f++; }
							break;
						}
						next_c = comparate_statesc(0, c2, c3, 0);
						if (next_c == 2) 
						{ 
							if (cq.check_empty()) 
							{ 
								d++; 
								if (check_call(p2)) 
									set_activec(p2, 2);
								else { 
									cq.push(2, s); 
									b2c = wait; }
						} 
							else
							{ 
								start_next_queuec(p1, p2, p3, p4);
								d++;
							}
						}
						else { 
							if (cq.check_empty()) 
							{ 
								s++; 
								if (check_call(p3)) 
									set_activec(p3, 3);
								else { 
									cq.push(3, d); 
									b3c = wait; }
						} 
							else
							{ 
								start_next_queuec(p1, p2, p3, p4);
								s++; 
							} 
						}
						break;
					}
					break;
				case 2: // 2 mem
					act_m2 = comparate_statesc(0, 0, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 2:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2); 
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(3); 
								a++; }
							if (time_p2 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2); 
								s++; }
							else if (time_p2 >= m && time_mem3 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(3);
								s++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2);
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(4); 
								a++; }
							if (time_p2 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2); 
								s++; }
							else if (time_p2 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(4); 
								s++; }
							break;
						}
						next_c = comparate_statesc(0, 0, c3, c4);
						if (next_c == 3) 
						{ 
							if (cq.check_empty()) 
							{
								f++; 
								if (check_call(p3))
									set_activec(p3, 3);
								else {
									cq.push(3, d);
									b3c = wait; }
						} 
							else 
							{ 
								start_next_queuec(p1, p2, p3, p4); 
								f++; 
							} 
						}
						else 
						{
							if (cq.check_empty()) 
							{
								d++; 
								if (check_call(p4)) 
									set_activec(p4, 4); 
								else
								{
									cq.push(4, f);
									b3c = wait;
								}
						} 
							else 
							{ 
								start_next_queuec(p1, p2, p3, p4);
								d++;
							} 
						}
						break;
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(2); 
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(3);
								a++; }
							if (time_p3 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2); 
								d++; }
							else if (time_p3 >= m && time_mem3 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(3);
								d++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2);
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(4); 
								a++; }
							if (time_p3 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(2); 
								d++; }
							else if (time_p3 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(4); 
								d++; }
							break;
						}
						next_c = comparate_statesc(0, c2, 0, c4);
						if (next_c == 2)
						{ 
							if (cq.check_empty())
							{
								f++; 
								if (check_call(p2))
									set_activec(p2, 2);
								else { 
									cq.push(2, s);
									b2c = wait; }
						} 
							else 
							{ 
								start_next_queuec(p1, p2, p3, p4); 
								f++;
							} 
						}
						else 
						{
							if (cq.check_empty()) 
							{
								s++;
								if (check_call(p4)) 
									set_activec(p4, 4);
								else { 
									cq.push(4, f);
									b4c = wait; }
						} 
							else 
							{ 
								start_next_queuec(p1, p2, p3, p4); 
								s++; 
							} 
						}
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							if (time_p1 >= m && time_mem2 >= m) {
								off_processorsc(act_p1);
								off_memoryc(2); 
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(3);
								a++; }
							if (time_p4 >= m && time_mem2 >= m) {
								off_processorsc(act_p2);
								off_memoryc(2); 
								f++; }
							else if (time_p4 >= m && time_mem3 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(3);
								f++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2);
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(4); 
								a++; }
							if (time_p4 >= m && time_mem2 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(2); 
								f++; }
							else if (time_p4 >= m && time_mem4 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(4); 
								f++; }
							break;
						}
						next_c = comparate_statesc(0, c2, c3, 0);
						if (next_c == 3)
						{ 
							if (cq.check_empty()) 
							{ 
								s++; 
								if (check_call(p3))
									set_activec(p3, 3); 
								else
								{
									cq.push(3, d);
									b3c = wait;
								}
						} 
							else
							{ 
								start_next_queuec(p1, p2, p3, p4);
								s++; 
							} 
						}
						else { 
							if (cq.check_empty())
							{ 
								d++;
								if (check_call(p2)) 
									set_activec(p2, 2);
								else { 
									cq.push(2, s); 
									b2c = wait; }
						} 
							else 
							{ 
								start_next_queuec(p1, p2, p3, p4);
								d++; 
							} 
						}
						break;
					}
					break;
				case 3: // 3 mem
					switch (act_p2) // смотрим по 2 процессору
					{
					case 2:
						if (time_p1 >= m && time_mem3 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(3);
							a++; }
						else if (time_p1 >= m && time_mem4 >= m) {
							off_processorsc(act_p1); 
							off_memoryc(4); 
							a++; }
						if (time_p2 >= m && time_mem3 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(3);
							s++; }
						else if (time_p2 >= m && time_mem4 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(4);
							s++; }
						next_c = comparate_statesc(0, 0, c3, c4);
						if (next_c == 3)
						{ 
							if (cq.check_empty()) 
							{ 
								f++;
								if (check_call(p3)) 
									set_activec(p3, 3);
								else { 
									cq.push(3, d);
									b3c = wait; }
						} 
							else 
							{ 
								start_next_queuec(p1, p2, p3, p4);
								f++;
							} 
						}
						else 
						{ 
							if (cq.check_empty())
							{
								d++; 
								if (check_call(p4))
									set_activec(p4, 4);
								else { 
									cq.push(4, f); 
									b4c = wait; }
						}
							else 
							{ 
								start_next_queuec(p1, p2, p3, p4);
								d++;
							} 
						}
						break;
					case 3:
						if (time_p1 >= m && time_mem3 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(3);
							a++; }
						else if (time_p1 >= m && time_mem4 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(4); 
							a++; }
						if (time_p3 >= m && time_mem3 >= m) { 
							off_processorsc(act_p2); 
							off_memoryc(3); 
							d++; }
						else if (time_p3 >= m && time_mem4 >= m) {
							off_processorsc(act_p2); 
							off_memoryc(4);
							d++; }
						next_c = comparate_statesc(0, c2, 0, c4);
						if (next_c == 2)
						{ 
							if (cq.check_empty()) 
							{
								f++;
								if (check_call(p2)) 
									set_activec(p2, 2);
								else { 
									cq.push(2, s);
									b2c = wait; }
						}
							else { 
								start_next_queuec(p1, p2, p3, p4); 
								f++;
							} 
						}
						else 
						{ 
							if (cq.check_empty()) {
								s++; 
								if (check_call(p4))
									set_activec(p4, 4);
								else {
									cq.push(4, f);
									b4c = wait; }
						} 
							else { 
								start_next_queuec(p1, p2, p3, p4);
								s++; 
							} 
						}
						break;
					case 4:
						if (time_p1 >= m && time_mem3 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(3); 
							a++; }
						else if (time_p1 >= m && time_mem4 >= m) {
							off_processorsc(act_p1); 
							off_memoryc(4); 
							a++; }
						if (time_p4 >= m && time_mem3 >= m) {
							off_processorsc(act_p2); 
							off_memoryc(3); 
							f++; }
						else if (time_p4 >= m && time_mem4 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(4);
							f++; }
						next_c = comparate_statesc(0, c2, c3, 0);
						if (next_c == 3) 
						{ 
							if (cq.check_empty()) {
								s++; 
								if (check_call(p3))
									set_activec(p3, 3);
								else { 
									cq.push(3, d); 
									b3c = wait; }
						} 
							else { 
								start_next_queuec(p1, p2, p3, p4);
								s++; 
							} 
						}
						else { 
							if (cq.check_empty()) { 
								d++; 
								if (check_call(p2)) 
									set_activec(p2, 2);
								else { 
									cq.push(2, s);
									b2c = wait; }
						} 
							else { 
								start_next_queuec(p1, p2, p3, p4);
								d++; 
							} 
						}
						break;
					}
					break;
				}
				break;
			case 2: // 2 proc
				act_p2 = comparate_statesc(0, 0, p3c, p4c);
				switch (act_m1) // смотрим по 1 памяти
				{
				case 1: // 1 память
					act_m2 = comparate_statesc(0, m2c, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							if (time_p2 >= m && time_mem1 >= m) { off_processorsc(act_p1); off_memoryc(1); s++; }
							else if (time_p2 >= m && time_mem2 >= m) { off_processorsc(act_p1); off_memoryc(2); s++; }
							if (time_p3 >= m && time_mem1 >= m) { off_processorsc(act_p2); off_memoryc(1); d++; }
							else if (time_p3 >= m && time_mem2 >= m) { off_processorsc(act_p2); off_memoryc(2); d++; }
							break;
						case 3:
							if (time_p2 >= m && time_mem1 >= m) { off_processorsc(act_p1); off_memoryc(1); s++; }
							else if (time_p2 >= m && time_mem3 >= m) { off_processorsc(act_p1); off_memoryc(3); s++; }
							if (time_p3 >= m && time_mem1 >= m) { off_processorsc(act_p2); off_memoryc(1); d++; }
							else if (time_p3 >= m && time_mem3 >= m) { off_processorsc(act_p2); off_memoryc(3); d++; }
							break;
						case 4:
							if (time_p2 >= m && time_mem1 >= m) { off_processorsc(act_p1); off_memoryc(1); s++; }
							else if (time_p2 >= m && time_mem4 >= m) { off_processorsc(act_p1); off_memoryc(4); s++; }
							if (time_p3 >= m && time_mem1 >= m) { off_processorsc(act_p2); off_memoryc(1); d++; }
							else if (time_p3 >= m && time_mem4 >= m) { off_processorsc(act_p2); off_memoryc(4); d++; }
							break;
						}
						next_c = comparate_statesc(0, 0, c3, c4);
						if (next_c == 1) { if (cq.check_empty()) { f++; if (check_call(p1)) set_activec(p1, 1); else cq.push(1, a);
						} else { start_next_queuec(p1, p2, p3, p4); f++; } }
						else { if (cq.check_empty()) { a++; if (check_call(p4)) set_activec(p4, 4); else cq.push(4, f);
						} else { start_next_queuec(p1, p2, p3, p4); a++; } }
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							if (time_p2 >= m && time_mem1 >= m) { off_processorsc(act_p1); off_memoryc(1); s++; }
							else if (time_p2 >= m && time_mem2 >= m) { off_processorsc(act_p1); off_memoryc(2); s++; }
							if (time_p4 >= m && time_mem1 >= m) { off_processorsc(act_p2); off_memoryc(1); f++; }
							else if (time_p4 >= m && time_mem2 >= m) { off_processorsc(act_p2); off_memoryc(2); f++; }
							break;
						case 3:
							if (time_p2 >= m && time_mem1 >= m) { off_processorsc(act_p1); off_memoryc(1); s++; }
							else if (time_p2 >= m && time_mem3 >= m) { off_processorsc(act_p1); off_memoryc(3); s++; }
							if (time_p4 >= m && time_mem1 >= m) { off_processorsc(act_p2); off_memoryc(1); f++; }
							else if (time_p4 >= m && time_mem3 >= m) { off_processorsc(act_p2); off_memoryc(3); f++; }
							break;
						case 4:
							if (time_p2 >= m && time_mem1 >= m) { off_processorsc(act_p1); off_memoryc(1); s++; }
							else if (time_p2 >= m && time_mem4 >= m) { off_processorsc(act_p1); off_memoryc(4); s++; }
							if (time_p4 >= m && time_mem1 >= m) { off_processorsc(act_p2); off_memoryc(1); f++; }
							else if (time_p4 >= m && time_mem4 >= m) { off_processorsc(act_p2); off_memoryc(4); f++; }
							break;
						}
						next_c = comparate_statesc(c1, 0, c3, 0);
						if (next_c == 3) { if (cq.check_empty()) { a++; if (check_call(p3)) set_activec(p3, 3); else cq.push(3, d);
						} else { start_next_queuec(p1, p2, p3, p4); a++; } }
						else { if (cq.check_empty()) { d++; if (check_call(p1)) set_activec(p1, 1); else cq.push(1, a);
						} else { start_next_queuec(p1, p2, p3, p4); d++; } }
						break;
					}
					break;
				case 2: // 2 mem
					act_m2 = comparate_statesc(0, 0, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							if (time_p2 >= m && time_mem2 >= m) { off_processorsc(act_p1); off_memoryc(2); s++; }
							else if (time_p2 >= m && time_mem3 >= m) { off_processorsc(act_p1); off_memoryc(3); s++; }
							if (time_p3 >= m && time_mem2 >= m) { off_processorsc(act_p2); off_memoryc(2); d++; }
							else if (time_p3 >= m && time_mem3 >= m) { off_processorsc(act_p2); off_memoryc(3); d++; }
							break;
						case 4:
							if (time_p2 >= m && time_mem2 >= m) { off_processorsc(act_p1); off_memoryc(2); s++; }
							else if (time_p2 >= m && time_mem4 >= m) { off_processorsc(act_p1); off_memoryc(4); s++; }
							if (time_p3 >= m && time_mem2 >= m) { off_processorsc(act_p2); off_memoryc(2); d++; }
							else if (time_p3 >= m && time_mem4 >= m) { off_processorsc(act_p2); off_memoryc(4); d++; }
							break;
						}
						next_c = comparate_statesc(c1, 0, 0, c4);
						if (next_c == 1) { if (cq.check_empty()) { f++; if (check_call(p1)) set_activec(p1, 1); else cq.push(1, a);
						} else { start_next_queuec(p1, p2, p3, p4); f++; } }
						else { if (cq.check_empty()) { a++; if (check_call(p4)) set_activec(p4, 4); else cq.push(4, f);
						} else { start_next_queuec(p1, p2, p3, p4); a++; } }
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							if (time_p2 >= m && time_mem2 >= m) { off_processorsc(act_p1); off_memoryc(2); s++; }
							else if (time_p2 >= m && time_mem3 >= m) { off_processorsc(act_p1); off_memoryc(3); s++; }
							if (time_p4 >= m && time_mem2 >= m) { off_processorsc(act_p2); off_memoryc(2); f++; }
							else if (time_p4 >= m && time_mem3 >= m) { off_processorsc(act_p2); off_memoryc(3); f++; }
							break;
						case 4:
							if (time_p2 >= m && time_mem2 >= m) { off_processorsc(act_p1); off_memoryc(2); s++; }
							else if (time_p2 >= m && time_mem4 >= m) { off_processorsc(act_p1); off_memoryc(4); s++; }
							if (time_p4 >= m && time_mem2 >= m) { off_processorsc(act_p2); off_memoryc(2); f++; }
							else if (time_p4 >= m && time_mem4 >= m) { off_processorsc(act_p2); off_memoryc(4); f++; }
							break;
						}
						next_c = comparate_statesc(c1, 0, c3, 0);
						if (next_c == 3) { if (cq.check_empty()) { a++; if (check_call(p3)) set_activec(p3, 3); else cq.push(3, d);
						} else { start_next_queuec(p1, p2, p3, p4); a++; } }
						else { if (cq.check_empty()) { d++; if (check_call(p1)) set_activec(p1,1); else cq.push(1, a);
						} else { start_next_queuec(p1, p2, p3, p4); d++; } }
						break;
					}
					break;
				case 3: // 3 mem
					switch (act_p2) // смотрим по 2 процессору
					{
					case 3:
						if (time_p2 >= m && time_mem3 >= m) { off_processorsc(act_p1); off_memoryc(3); s++; }
						else if (time_p2 >= m && time_mem4 >= m) { off_processorsc(act_p1); off_memoryc(4); s++; }
						if (time_p3 >= m && time_mem3 >= m) { off_processorsc(act_p2); off_memoryc(3); d++; }
						else if (time_p3 >= m && time_mem4 >= m) { off_processorsc(act_p2); off_memoryc(4); d++; }
						next_c = comparate_statesc(c1, 0, 0, c4);
						if (next_c == 1) { if (cq.check_empty()) { f++; if (check_call(p1)) set_activec(p1, 1); else cq.push(1, a);
						} else { start_next_queuec(p1, p2, p3, p4); f++; } }
						else { if (cq.check_empty()) { a++; if (check_call(p4)) set_activec(p4, 4); else cq.push(4, f);
						} else { start_next_queuec(p1, p2, p3, p4); a++; } }
						break;
					case 4:
						if (time_p2 >= m && time_mem3 >= m) { off_processorsc(act_p1); off_memoryc(3); s++; }
						else if (time_p2 >= m && time_mem4 >= m) { off_processorsc(act_p1); off_memoryc(4); s++; }
						if (time_p4 >= m && time_mem3 >= m) { off_processorsc(act_p2); off_memoryc(3); f++; }
						else if (time_p4 >= m && time_mem4 >= m) { off_processorsc(act_p2); off_memoryc(4); f++; }
						next_c = comparate_statesc(c1, 0, c3,0);
						if (next_c == 3) { if (cq.check_empty()) { a++; if (check_call(p3)) set_activec(p3, 3); else cq.push(3, d);
						} else { start_next_queuec(p1, p2, p3, p4); a++; } }
						else { if (cq.check_empty()) { d++; if (check_call(p1)) set_activec(p1, 1); else cq.push(1, a);
						} else { start_next_queuec(p1, p2, p3, p4); d++; } }
						a++; d++;
						break;
					}
					break;
				}
				break;
			case 3: // 3 proc
				act_p2 = 4;
				switch (act_m1) // смотрим по 1 памяти
				{
				case 1: // 1 память
					act_m2 = comparate_statesc(0, m2c, m3c, m4c);
					switch (act_m2) // смотрим по 2 памяти
					{
					case 2:
						if (time_p4 >= m && time_mem1 >= m) { off_processorsc(act_p1); off_memoryc(1); f++; }
						else if (time_p4 >= m && time_mem2 >= m) { off_processorsc(act_p1); off_memoryc(2); f++; }
						if (time_p3 >= m && time_mem1 >= m) { off_processorsc(act_p2); off_memoryc(1); d++; }
						else if (time_p3 >= m && time_mem2 >= m) { off_processorsc(act_p2); off_memoryc(2); d++; }
						break;
					case 3:
						if (time_p4 >= m && time_mem1 >= m) { off_processorsc(act_p1); off_memoryc(1); f++; }
						else if (time_p4 >= m && time_mem3 >= m) { off_processorsc(act_p1); off_memoryc(3); f++; }
						if (time_p3 >= m && time_mem1 >= m) { off_processorsc(act_p2); off_memoryc(1); d++; }
						else if (time_p3 >= m && time_mem3 >= m) { off_processorsc(act_p2); off_memoryc(3); d++; }
						break;
					case 4:
						if (time_p4 >= m && time_mem1 >= m) { off_processorsc(act_p1); off_memoryc(1); f++; }
						else if (time_p4 >= m && time_mem4 >= m) { off_processorsc(act_p1); off_memoryc(4); f++; }
						if (time_p3 >= m && time_mem1 >= m) { off_processorsc(act_p2); off_memoryc(1); d++; }
						else if (time_p3 >= m && time_mem4 >= m) { off_processorsc(act_p2); off_memoryc(4); d++; }
						break;
					}
					next_c = comparate_statesc(c1, c2, 0, 0);
					if (next_c == 1) { if (cq.check_empty()) { s++; if (check_call(p1)) set_activec(p1, 1); else cq.push(1, a);
					} else { start_next_queuec(p1, p2, p3, p4); s++; } }
					else { if (cq.check_empty()) { a++; if (check_call(p2)) set_activec(p2, 2); else cq.push(2, s);
					} else { start_next_queuec(p1, p2, p3, p4); a++; } }
					break;
				case 2: // 2 mem
					act_m2 = comparate_statesc(0, 0, m3c, m4c);
					switch (act_m2) // смотрим по 2 памяти
					{
					case 3:
						if (time_p4 >= m && time_mem2 >= m) { off_processorsc(act_p1); off_memoryc(2); f++; }
						else if (time_p4 >= m && time_mem3 >= m) { off_processorsc(act_p1); off_memoryc(3); f++; }
						if (time_p3 >= m && time_mem2 >= m) { off_processorsc(act_p2); off_memoryc(2); d++; }
						else if (time_p3 >= m && time_mem3 >= m) { off_processorsc(act_p2); off_memoryc(3); d++; }
						break;
					case 4:
						if (time_p4 >= m && time_mem2 >= m) { off_processorsc(act_p1); off_memoryc(2); f++; }
						else if (time_p4 >= m && time_mem4 >= m) { off_processorsc(act_p1); off_memoryc(4); f++; }
						if (time_p3 >= m && time_mem2 >= m) { off_processorsc(act_p2); off_memoryc(2); d++; }
						else if (time_p3 >= m && time_mem4 >= m) { off_processorsc(act_p2); off_memoryc(4); d++; }
						break;
					}
					next_c = comparate_statesc(c1, c2, 0, 0);
					if (next_c == 1) { if (cq.check_empty()) { s++; if (check_call(p1)) set_activec(p1, 1); else cq.push(1, a);
					} else { start_next_queuec(p1, p2, p3, p4); s++; } }
					else { if (cq.check_empty()) { a++; if (check_call(p2)) set_activec(p2, 2); else cq.push(2, s);
					} else { start_next_queuec(p1, p2, p3, p4); a++; } }
					break;
				case 3: // 3 mem
					if (time_p4 >= m && time_mem3 >= m) { off_processorsc(act_p1); off_memoryc(3); f++; }
					else if (time_p4 >= m && time_mem4 >= m) { off_processorsc(act_p1); off_memoryc(4); f++; }
					if (time_p3 >= m && time_mem3 >= m) { off_processorsc(act_p2); off_memoryc(3); d++; }
					else if (time_p3 >= m && time_mem4 >= m) { off_processorsc(act_p2); off_memoryc(4); d++; }
					next_c = comparate_statesc(c1, c2, 0, 0);
					if (next_c == 1) { if (cq.check_empty()) { s++; if (check_call(p1)) set_activec(p1, 1); else cq.push(1, a);
					} else { start_next_queuec(p1, p2, p3, p4); s++; } }
					else { if (cq.check_empty()) { a++; if (check_call(p2)) set_activec(p2, 2); else cq.push(2, s);
					} else { start_next_queuec(p1, p2, p3, p4); a++; } }
					break;
				}
				break;
			}
		}
		else
		{
			if (!cq.check_empty()) 
				time_waitc++;
			int act_m1 = comparate_statesc(m1c, m2c, m3c, m4c), act_m2 = 0; // находим 1 активную
			int act_p1 = comparate_statesc(p1c, p2c, p3c, p4c), act_p2 = 0;
			int next_c = 0;
			switch (act_p1) // смотрим по 1 процессору
			{
			case 1: // 1 проц
				act_p2 = comparate_statesc(0, p2c, p3c, p4c);
				switch (act_m1) // смотрим по 1 памяти
				{
				case 1: // 1 память
					act_m2 = comparate_statesc(0, m2c, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 2:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							time_p1++; time_p2++; time_mem1++; time_mem2++;
							break;
						case 3:
							time_p1++; time_p2++; time_mem1++; time_mem3++;
							break;
						case 4:
							time_p1++; time_p2++; time_mem1++; time_mem4++;
							break;
						}
						next_c = comparate_statesc(0, 0, c3, c4);
						if (next_c == 3) {
							if (cq.check_empty()) {
								if (check_call(p3)) 
									set_activec(p3, 3); 
								else {
									cq.push(3, d);
									b3c = wait;
								} 
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						else {
							if (cq.check_empty()) 
							{ 
								if (check_call(p4)) 
									set_activec(p4, 4); 
								else { 
									cq.push(4, f); 
									b4c = wait;
								} 
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							time_p1++; time_p3++; time_mem1++; time_mem2++;
							break;
						case 3:
							time_p1++; time_p3++; time_mem1++; time_mem3++;
							break;
						case 4:
							time_p1++; time_p3++; time_mem1++; time_mem4++;
							break;
						}
						next_c = comparate_statesc(0, c2, 0, c4);
						if (next_c == 2) {
							if (cq.check_empty()) 
							{ 
								if (check_call(p2)) 
									set_activec(p2, 2);
								else {
									cq.push(2, s); 
									b2c = wait;
								} 
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						else {
							if (cq.check_empty()) 
							{
								if (check_call(p4))
									set_activec(p4, 4);
								else {
									cq.push(4, f);
									b4c = wait; 
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4); 
							s++;
						}
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							time_p1++; time_p4++; time_mem1++; time_mem2++;
							break;
						case 3:
							time_p1++; time_p4++; time_mem1++; time_mem3++;
							break;
						case 4:
							time_p1++; time_p4++; time_mem1++; time_mem4++;
							break;
						}
						next_c = comparate_statesc(0, c2, c3, 0);
						if (next_c == 3) {
							if (cq.check_empty()) { 
								if (check_call(p3))
									set_activec(p3, 3);
								else {
									cq.push(3, d); 
									b3c = wait; 
								} 
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							s++;
						}
						else {
							if (cq.check_empty()) { 
								if (check_call(p2))
									set_activec(p2, 2); 
								else { 
									cq.push(2, s); 
									b2c = wait; 
								} 
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					}
					break;
				case 2: // 2 mem
					act_m2 = comparate_statesc(0, 0, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 2:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							time_p1++; time_p2++; time_mem2++; time_mem3++;
							break;
						case 4:
							time_p1++; time_p2++; time_mem4++; time_mem4++;
							break;
						}
						next_c = comparate_statesc(0, 0, c3, c4);
						if (next_c == 3) {
							if (cq.check_empty()) { 
								if (check_call(p3)) 
									set_activec(p3, 3);
								else { 
									cq.push(3, d);
									b3c = wait; 
								} 
							}
							else
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						else {
							if (cq.check_empty()) { 
								if (check_call(p4)) 
									set_activec(p4, 4); 
								else { 
									cq.push(4, f); 
									b4c = wait;
								} 
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							time_p1++; time_p3++; time_mem2++; time_mem3++;
							break;
						case 4:
							time_p1++; time_p3++; time_mem4++; time_mem4++;
							break;
						}
						next_c = comparate_statesc(0, c2, 0, c4);
						if (next_c == 2) {
							if (cq.check_empty()) { 
								if (check_call(p2)) 
									set_activec(p2,2);
								else {
									cq.push(2, s);
									b2c = wait; 
								} 
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						else {
							if (cq.check_empty()) { 
								if (check_call(p4)) 
									set_activec(p4, 4);
								else { 
									cq.push(4, f); 
									b4c = wait;
								} 
							}
							else 
								start_next_queuec(p1, p2, p3, p4); 
							s++;
						}
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							time_p1++; time_p4++; time_mem2++; time_mem3++;
							break;
						case 4:
							time_p1++; time_p4++; time_mem4++; time_mem4++;
							break;
						}
						next_c = comparate_statesc(0, c2, c3, 0);
						if (next_c == 3) {
							if (cq.check_empty()) {
								if (check_call(p3))
									set_activec(p3, 3);
								else {
									cq.push(3, d); 
									b3c = wait; 
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4); 
							s++;
						}
						else {
							if (cq.check_empty()) { 
								if (check_call(p2))
									set_activec(p2, 2); 
								else { 
									cq.push(2, s); 
									b2c = wait; 
								} 
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					}
					break;
				case 3: // 3 mem
					switch (act_p2) // смотрим по 2 процессору
					{
					case 2:
						time_p1++; time_p2++; time_mem3++; time_mem4++;
						next_c = comparate_statesc(0, 0, c3, c4);
						if (next_c == 3) {
							if (cq.check_empty()) { 
								if (check_call(p3)) 
									set_activec(p3, 3); 
								else {
									cq.push(3, d); 
									b3c = wait; 
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						else {
							if (cq.check_empty()) { 
								if (check_call(p4))
									set_activec(p4, 4); 
								else { 
									cq.push(4, f); 
									b4c = wait; 
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4); 
							d++;
						}
						break;
					case 3:
						time_p1++; time_p3++; time_mem3++; time_mem4++;
						next_c = comparate_statesc(0, c2, 0, c4);
						if (next_c == 2) {
							if (cq.check_empty()) { 
								if (check_call(p2))
									set_activec(p2, 2);
								else { 
									cq.push(2, s);
									b2c = wait;
								}
							}
							else
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						else {
							if (cq.check_empty()) {
								if (check_call(p4)) 
									set_activec(p4, 4);
								else { 
									cq.push(4, f);
									b4c = wait;
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							s++;
						}
						break;
					case 4:
						time_p1++; time_p4++; time_mem3++; time_mem4++;
						next_c = comparate_statesc(0, c2, c3, 0);
						if (next_c == 3) {
							if (cq.check_empty()) { 
								if (check_call(p3)) 
									set_activec(p3, 3);
								else { 
									cq.push(3, d);
									b3c = wait; 
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4); 
							s++;
						}
						else {
							if (cq.check_empty()) { 
								if (check_call(p2))
									set_activec(p2, 2);
								else { 
									cq.push(2, s); 
									b2c = wait; 
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						break;
					}
					break;
				}
				break;
			case 2: // 2 proc
				act_p2 = comparate_statesc(0, 0, p3c, p4c);
				switch (act_m1) // смотрим по 1 памяти
				{
				case 1: // 1 память
					act_m2 = comparate_statesc(0, m2c, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							time_p2++; time_p3++; time_mem1++; time_mem2++;
							break;
						case 3:
							time_p2++; time_p3++; time_mem1++; time_mem3++;
							break;
						case 4:
							time_p2++; time_p3++; time_mem1++; time_mem4++;
							break;
						}
						next_c = comparate_statesc(c1, 0, 0, c4);
						if (next_c == 1) {
							if (cq.check_empty()) {
								if (check_call(p1))
									set_activec(p1, 1);
								else { 
									cq.push(1, a); 
									b1c = wait; 
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						else {
							if (cq.check_empty()) { 
								if (check_call(p4)) 
									set_activec(p4, 4);
								else { 
									cq.push(4, f); 
									b4c = wait; 
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							time_p2++; time_p4++; time_mem1++; time_mem2++;
							break;
						case 3:
							time_p2++; time_p4++; time_mem1++; time_mem3++;
							break;
						case 4:
							time_p2++; time_p4++; time_mem1++; time_mem4++;
							break;
						}
						next_c = comparate_statesc(c1, 0, c3, 0);
						if (next_c == 1) {
							if (cq.check_empty()) { 
								if (check_call(p1)) 
									set_activec(p1, 1);
								else { 
									cq.push(1, a); 
									b1c = wait;
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						else {
							if (cq.check_empty()) { 
								if (check_call(p3)) 
									set_activec(p3, 3);
								else { 
									cq.push(3, d); 
									b3c = wait; 
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					}
					break;
				case 2: // 2 mem
					act_m2 = comparate_statesc(0, 0, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							time_p2++; time_p3++; time_mem2++; time_mem3++;
							break;
						case 4:
							time_p2++; time_p3++; time_mem2++; time_mem4++;
							break;
						}
						next_c = comparate_statesc(c1, 0, 0, c4);
						if (next_c == 1) {
							if (cq.check_empty()) { 
								if (check_call(p1)) 
									set_activec(p1, 1); 
								else {
									cq.push(1, a);
									b1c = wait;
								}
							}
							else
								start_next_queuec(p1, p2, p3, p4);
							f++;
						}
						else {
							if (cq.check_empty()) { 
								if (check_call(p4)) 
									set_activec(p4, 4); 
								else { 
									cq.push(4, f); 
									b4c = wait; 
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							time_p2++; time_p4++; time_mem2++; time_mem3++;
							break;
						case 4:
							time_p2++; time_p4++; time_mem2++; time_mem4++;
							break;
						}
						next_c = comparate_statesc(c1, 0, c3, 0);
						if (next_c == 1) {
							if (cq.check_empty()) { 
								if (check_call(p1)) 
									set_activec(p1, 1); 
								else { 
									cq.push(1, a); 
									b1c = wait; 
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4); 
							d++;
						}
						else {
							if (cq.check_empty()) {
								if (check_call(p3)) 
									set_activec(p3, 3);
								else { 
									cq.push(3, d); 
									b3c = wait; 
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4); 
							a++;
						}
						break;
					}
					break;
				case 3: // 3 mem
					switch (act_p2) // смотрим по 2 процессору
					{
					case 3:
						time_p2++; time_p3++; time_mem3++; time_mem4++;
						next_c = comparate_statesc(c1, 0, 0, c4);
						if (next_c == 1) {
							if (cq.check_empty()) { 
								if (check_call(p1)) 
									set_activec(p1, 1);
								else { 
									cq.push(1, a); 
									b1c = wait; 
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4); 
							f++;
						}
						else {
							if (cq.check_empty()) { 
								if (check_call(p4)) 
									set_activec(p4, 4);
								else { 
									cq.push(4, f);
									b4c = wait; 
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							a++;
						}
						break;
					case 4:
						time_p2++; time_p4++; time_mem3++; time_mem4++;
						next_c = comparate_statesc(c1, 0, c3, 0);
						if (next_c == 1) {
							if (cq.check_empty()) { 
								if (check_call(p1)) 
									set_activec(p1, 1);
								else { 
									cq.push(1, a);
									b1c = wait; 
								}
							}
							else 
								start_next_queuec(p1, p2, p3, p4);
							d++;
						}
						else {
							if (cq.check_empty()) { 
								if (check_call(p3)) 
									set_activec(p3, 3);
								else {
									cq.push(3, d);
									b3c = wait; 
								}
							}
							else start_next_queuec(p1, p2, p3, p4); 
							a++;
						}
						break;
					}
					break;
				}
				break;
			case 3: // 3 proc
				act_p2 = 4;
				switch (act_m1) // смотрим по 1 памяти
				{
				case 1: // 1 память
					act_m2 = comparate_statesc(0, m2c, m3c, m4c);
					switch (act_m2) // смотрим по 2 памяти
					{
					case 2:
						time_p4++; time_p3++; time_mem1++; time_mem2++;
						break;
					case 3:
						time_p4++; time_p3++; time_mem1++; time_mem3++;
						break;
					case 4:
						time_p4++; time_p3++; time_mem1++; time_mem4++;
						break;
					}
					next_c = comparate_statesc(c1, c2, 0, 0);
					if (next_c == 1) {
						if (cq.check_empty()) {
							if (check_call(p1)) 
								set_activec(p1, 1); 
							else {
								cq.push(1, a); 
								b1c = wait;
							}
						}
						else
							start_next_queuec(p1, p2, p3, p4);
						s++;
					}
					else {
						if (cq.check_empty()) { 
							if (check_call(p2)) 
								set_activec(p2, 2); 
							else { 
								cq.push(2, s); 
								b2c = wait; 
							}
						}
						else 
							start_next_queuec(p1, p2, p3, p4);
						a++;
					}
					break;
				case 2: // 2 mem
					act_m2 = comparate_statesc(0, 0, m3c, m4c);
					switch (act_m2) // смотрим по 2 памяти
					{
					case 3:
						time_p4++; time_p3++; time_mem2++; time_mem3++;
						break;
					case 4:
						time_p4++; time_p3++; time_mem4++; time_mem2++;
						break;
					}
					next_c = comparate_statesc(c1, c2, 0, 0);
					if (next_c == 1) {
						if (cq.check_empty()) { 
							if (check_call(p1)) 
								set_activec(p1, 1);
							else { 
								cq.push(1, a); 
								b1c = wait; 
							}
						}
						else 
							start_next_queuec(p1, p2, p3, p4);
						s++;
					}
					else {
						if (cq.check_empty()) {
							if (check_call(p2)) 
								set_activec(p2, 2);
							else {
								cq.push(2, s);
								b2c = wait; 
							}
						}
						else 
							start_next_queuec(p1, p2, p3, p4);
						a++;
					}
					break;
				case 3: // 3 mem
					time_p4++; time_p3++; time_mem3++; time_mem4++;
					next_c = comparate_statesc(c1, c2, 0, 0);
					if (next_c == 1) {
						if (cq.check_empty()) { 
							if (check_call(p1)) 
								set_activec(p1, 1);
							else { 
								cq.push(1, a); 
								b1c = wait; 
							}
						}
						else 
							start_next_queuec(p1, p2, p3, p4);
						s++;
					}
					else {
						if (cq.check_empty()) { 
							if (check_call(p2)) 
								set_activec(p2, 2);
							else {
								cq.push(2, s); 
								b2c = wait;
							}
						}
						else 
							start_next_queuec(p1, p2, p3, p4);
						a++;
					}
					break;
				}
				break;
			}
		}
		break;
	case 4: // есть два новых обращений
		cstatus = 5;
		if ((time_mem1 >= m) || (time_mem2 >= m) || (time_mem3 >= m) || (time_mem4 >= m))
		{
			int act_m1 = comparate_statesc(m1c, m2c, m3c, m4c), act_m2 = 0; // находим 1 активную
			int act_p1 = comparate_statesc(p1c, p2c, p3c, p4c), act_p2 = 0;
			int next_c = 0;
			switch (act_p1) // смотрим по 1 процессору
			{
			case 1: // 1 проц
				act_p2 = comparate_statesc(0, p2c, p3c, p4c);
				switch (act_m1) // смотрим по 1 памяти
				{
				case 1: // 1 память
					act_m2 = comparate_statesc(0, m2c, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 2:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(1);
								a++; }
							else if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(2);
								a++; }
							if (time_p2 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); 
								s++; }
							else if (time_p2 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2); 
								s++; }
							break;
						case 3:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1); 
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(3); 
								a++; }
							if (time_p2 >= m && time_mem1 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(1); 
								s++; }
							else if (time_p2 >= m && time_mem3 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(3); 
								s++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem1 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(1);
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(4); 
								a++; }
							if (time_p2 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(1); 
								s++; }
							else if (time_p2 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(4); 
								s++; }
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p3))
								set_activec(p3, 3);
							else { 
								cq.push(3, d); 
								b3c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f); 
								b4c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1); 
								a++; }
							else if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2); 
								a++; }
							if (time_p3 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1);
								d++; }
							else if (time_p3 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2);
								d++; }
							break;
						case 3:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1); 
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(3);
								a++; }
							if (time_p3 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); 
								d++; }
							else if (time_p3 >= m && time_mem3 >= m) {
								off_processorsc(act_p2);
								off_memoryc(3); 
								d++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem1 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(1);
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(4); 
								a++; }
							if (time_p3 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); 
								d++; }
							else if (time_p3 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(4); 
								d++; }
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2); 
							else { 
								cq.push(2, s); 
								b2c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4); 
							else { 
								cq.push(4, f); 
								b4c = wait; }
						}
						else
						{
							cq.push(2, s);
							b2c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1); 
								a++; }
							else if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2); 
								a++; }
							if (time_p4 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1);
								f++; }
							else if (time_p2 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(2);
								f++; }
							break;
						case 3:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1); 
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(3);
								a++; }
							if (time_p4 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); f++; }
							else if (time_p4 >= m && time_mem3 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(3); f++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(1); 
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(4); 
								a++; }
							if (time_p4 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(1); 
								f++; }
							else if (time_p4 >= m && time_mem4 >= m) {
								off_processorsc(act_p2);
								off_memoryc(4); 
								f++; }
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p3))
								set_activec(p3, 3);
							else { 
								cq.push(3, d); 
								b3c = wait; }
							if (check_call(p2))
								set_activec(p2, 2); 
							else { 
								cq.push(2, s); 
								b2c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(2, s);
							b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					}
					break;
				case 2: // 2 mem
					act_m2 = comparate_statesc(0, 0, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 2:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2);
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) {
								off_processorsc(act_p1);
								off_memoryc(3); 
								a++; }
							if (time_p2 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2);
								s++; }
							else if (time_p2 >= m && time_mem3 >= m) {
								off_processorsc(act_p2);
								off_memoryc(3);
								s++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(2);
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(4); 
								a++; }
							if (time_p2 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(2);
								s++; }
							else if (time_p2 >= m && time_mem4 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(4); 
								s++; }
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3);
							else {
								cq.push(3, d);
								b3c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f);
								b4c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2);
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(3); 
								a++; }
							if (time_p3 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2); 
								d++; }
							else if (time_p3 >= m && time_mem3 >= m) {
								off_processorsc(act_p2);
								off_memoryc(3); 
								d++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2); 
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(4); 
								a++; }
							if (time_p3 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2); 
								d++; }
							else if (time_p3 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(4); 
								d++; }
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2);
							else { 
								cq.push(2, s); 
								b2c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else {
								cq.push(4, f);
								b4c = wait; }
						}
						else
						{
							cq.push(2, s); 
							b2c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							if (time_p1 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2); 
								a++; }
							else if (time_p1 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(3); 
								a++; }
							if (time_p4 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2); 
								f++; }
							else if (time_p4 >= m && time_mem3 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(3); 
								f++; }
							break;
						case 4:
							if (time_p1 >= m && time_mem2 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(2); 
								a++; }
							else if (time_p1 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(4); 
								a++; }
							if (time_p4 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2); 
								f++; }
							else if (time_p4 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(4);
								f++; }
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3); 
							else { 
								cq.push(3, d); 
								b3c = wait; }
							if (check_call(p2))
								set_activec(p2, 2);
							else { 
								cq.push(2, s);
								b2c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(2, s); 
							b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					}
					break;
				case 3: // 3 mem
					switch (act_p2) // смотрим по 2 процессору
					{
					case 2:
						if (time_p1 >= m && time_mem3 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(3);
							a++; }
						else if (time_p1 >= m && time_mem4 >= m) { 
							off_processorsc(act_p1);
							off_memoryc(4); 
							a++; }
						if (time_p2 >= m && time_mem3 >= m) { 
							off_processorsc(act_p2); 
							off_memoryc(3); 
							s++; }
						else if (time_p2 >= m && time_mem4 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(4); 
							s++; }
						if (cq.check_empty())
						{
							if (check_call(p3))
								set_activec(p3, 3);
							else { 
								cq.push(3, d); 
								b3c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f); 
								b4c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 3:
						if (time_p1 >= m && time_mem3 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(3); 
							a++; }
						else if (time_p1 >= m && time_mem4 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(4); 
							a++; }
						if (time_p3 >= m && time_mem3 >= m) {
							off_processorsc(act_p2);
							off_memoryc(3);
							d++; }
						else if (time_p3 >= m && time_mem4 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(4); 
							d++; }
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2); 
							else {
								cq.push(2, s); 
								b2c = wait; }
							if (check_call(p4))
								set_activec(p4, 4);
							else { 
								cq.push(4, f); 
								b4c = wait; }
						}
						else
						{
							cq.push(2, s); 
							b2c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 4:
						if (time_p1 >= m && time_mem3 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(3);
							a++; }
						else if (time_p1 >= m && time_mem4 >= m) {
							off_processorsc(act_p1);
							off_memoryc(4); 
							a++; }
						if (time_p4 >= m && time_mem3 >= m) {
							off_processorsc(act_p2); 
							off_memoryc(3); 
							f++; }
						else if (time_p4 >= m && time_mem4 >= m) {
							off_processorsc(act_p2); 
							off_memoryc(4);
							f++; }
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3);
							else { 
								cq.push(3, d);
								b3c = wait; }
							if (check_call(p2))
								set_activec(p2, 2);
							else { 
								cq.push(2, s); 
								b2c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(2, s);
							b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					}
					break;
				}
				break;
			case 2: // 2 proc
				act_p2 = comparate_statesc(0, 0, p3c, p4c);
				switch (act_m1) // смотрим по 1 памяти
				{
				case 1: // 1 память
					act_m2 = comparate_statesc(0, m2c, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							if (time_p2 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1); 
								s++; }
							else if (time_p2 >= m && time_mem2 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(2); 
								s++; }
							if (time_p3 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); 
								d++; }
							else if (time_p3 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2);
								d++; }
							break;
						case 3:
							if (time_p2 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1);
								s++; }
							else if (time_p2 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(3); 
								s++; }
							if (time_p3 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); 
								d++; }
							else if (time_p3 >= m && time_mem3 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(3); 
								d++; }
							break;
						case 4:
							if (time_p2 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1);
								s++; }
							else if (time_p2 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(4); 
								s++; }
							if (time_p3 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); 
								d++; }
							else if (time_p3 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(4); 
								d++; }
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1);
							else {
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4); 
							else { 
								cq.push(4, f); 
								b4c = wait; }
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							if (time_p2 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(1); 
								s++; }
							else if (time_p2 >= m && time_mem2 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(2); 
								s++; }
							if (time_p4 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1);
								f++; }
							else if (time_p4 >= m && time_mem2 >= m) {
								off_processorsc(act_p2);
								off_memoryc(2);
								f++; }
							break;
						case 3:
							if (time_p2 >= m && time_mem1 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(1);
								s++; }
							else if (time_p2 >= m && time_mem3 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(3);
								s++; }
							if (time_p4 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(1); 
								f++; }
							else if (time_p4 >= m && time_mem3 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(3); 
								f++; }
							break;
						case 4:
							if (time_p2 >= m && time_mem1 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(1); 
								s++; }
							else if (time_p2 >= m && time_mem4 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(4); 
								s++; }
							if (time_p4 >= m && time_mem1 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(1); 
								f++; }
							else if (time_p4 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(4); 
								f++; }
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p3))
								set_activec(p3, 3); 
							else { 
								cq.push(3, d);
								b3c = wait; }
							if (check_call(p1)) 
								set_activec(p1, 1);
							else { 
								cq.push(1, a);
								b1c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(1, a);
							b1c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					}
					break;
				case 2: // 2 mem
					act_m2 = comparate_statesc(0, 0, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							if (time_p2 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(2);
								s++; }
							else if (time_p2 >= m && time_mem3 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(3); 
								s++; }
							if (time_p3 >= m && time_mem2 >= m) {
								off_processorsc(act_p2);
								off_memoryc(2);
								d++; }
							else if (time_p3 >= m && time_mem3 >= m) {
								off_processorsc(act_p2); 
								off_memoryc(3); 
								d++; }
							break;
						case 4:
							if (time_p2 >= m && time_mem2 >= m) {
								off_processorsc(act_p1); 
								off_memoryc(2);
								s++; }
							else if (time_p2 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(4); 
								s++; }
							if (time_p3 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2);
								off_memoryc(2);
								d++; }
							else if (time_p3 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(4); 
								d++; }
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p1))
								set_activec(p1, 1); 
							else {
								cq.push(1, a); 
								b1c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4); 
							else {
								cq.push(4, f);
								b4c = wait; }
						}
						else
						{
							cq.push(1, a); 
							b1c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							if (time_p2 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1);
								off_memoryc(2);
								s++; }
							else if (time_p2 >= m && time_mem3 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(3); 
								s++; }
							if (time_p4 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2); 
								f++; }
							else if (time_p4 >= m && time_mem3 >= m) {
								off_processorsc(act_p2);
								off_memoryc(3);
								f++; }
							break;
						case 4:
							if (time_p2 >= m && time_mem2 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(2);
								s++; }
							else if (time_p2 >= m && time_mem4 >= m) { 
								off_processorsc(act_p1); 
								off_memoryc(4); 
								s++; }
							if (time_p4 >= m && time_mem2 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(2); 
								f++; }
							else if (time_p4 >= m && time_mem4 >= m) { 
								off_processorsc(act_p2); 
								off_memoryc(4); 
								f++; }
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3);
							else { 
								cq.push(3, d);
								b3c = wait; }
							if (check_call(p4)) 
								set_activec(p1, 1);
							else { 
								cq.push(1, a);
								b1c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(1, a);
							b1c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					}
					break;
				case 3: // 3 mem
					switch (act_p2) // смотрим по 2 процессору
					{
					case 3:
						if (time_p2 >= m && time_mem3 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(3);
							s++; }
						else if (time_p2 >= m && time_mem4 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(4); 
							s++; }
						if (time_p3 >= m && time_mem3 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(3); 
							d++; }
						else if (time_p3 >= m && time_mem4 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(4);
							d++; }
						if (cq.check_empty())
						{
							if (check_call(p1))
								set_activec(p1, 1); 
							else { 
								cq.push(1, a);
								b1c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f);
								b4c = wait; }
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 4:
						if (time_p2 >= m && time_mem3 >= m) {
							off_processorsc(act_p1); 
							off_memoryc(3);
							s++; }
						else if (time_p2 >= m && time_mem4 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(4); 
							s++; }
						if (time_p4 >= m && time_mem3 >= m) { 
							off_processorsc(act_p2); 
							off_memoryc(3);
							f++; }
						else if (time_p4 >= m && time_mem4 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(4);
							f++; }
						if (cq.check_empty())
						{
							if (check_call(p3))
								set_activec(p3, 3);
							else {
								cq.push(3, d);
								b3c = wait; }
							if (check_call(p1))
								set_activec(p1, 1);
							else { 
								cq.push(1,a);
								b1c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(1, a);
							b1c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					}
					break;
				}
				break;
			case 3: // 3 proc
				act_p2 = 4;
				switch (act_m1) // смотрим по 1 памяти
				{
				case 1: // 1 память
					act_m2 = comparate_statesc(0, m2c, m3c, m4c);
					switch (act_m2) // смотрим по 2 памяти
					{
					case 2:
						if (time_p4 >= m && time_mem1 >= m) {
							off_processorsc(act_p1);
							off_memoryc(1);
							f++; }
						else if (time_p4 >= m && time_mem2 >= m) {
							off_processorsc(act_p1);
							off_memoryc(2);
							f++; }
						if (time_p3 >= m && time_mem1 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(1);
							d++; }
						else if (time_p3 >= m && time_mem2 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(2); 
							d++; }
						break;
					case 3:
						if (time_p4 >= m && time_mem1 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(1); 
							f++; }
						else if (time_p4 >= m && time_mem3 >= m) {
							off_processorsc(act_p1); 
							off_memoryc(3);
							f++; }
						if (time_p3 >= m && time_mem1 >= m) {
							off_processorsc(act_p2);
							off_memoryc(1); 
							d++; }
						else if (time_p3 >= m && time_mem3 >= m) { 
							off_processorsc(act_p2); 
							off_memoryc(3);
							d++; }
						break;
					case 4:
						if (time_p4 >= m && time_mem1 >= m) {
							off_processorsc(act_p1); 
							off_memoryc(1); 
							f++; }
						else if (time_p4 >= m && time_mem4 >= m) { 
							off_processorsc(act_p1);
							off_memoryc(4); 
							f++; }
						if (time_p3 >= m && time_mem1 >= m) { 
							off_processorsc(act_p2);
							off_memoryc(1); 
							d++; }
						else if (time_p3 >= m && time_mem4 >= m) { 
							off_processorsc(act_p2); 
							off_memoryc(4);
							d++; }
						break;
					}
					if (cq.check_empty())
					{
						if (check_call(p3)) 
							set_activec(p1, 1);
						else { 
							cq.push(1, a);
							b1c = wait; }
						if (check_call(p4)) 
							set_activec(p2, 2); 
						else { 
							cq.push(2, s);
							b2c = wait; }
					}
					else
					{
						cq.push(1, a);
						b1c = wait;
						cq.push(2, s);
						b2c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 2: // 2 mem
					act_m2 = comparate_statesc(0, 0, m3c, m4c);
					switch (act_m2) // смотрим по 2 памяти
					{
					case 3:
						if (time_p4 >= m && time_mem2 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(2); 
							f++; }
						else if (time_p4 >= m && time_mem3 >= m) {
							off_processorsc(act_p1); 
							off_memoryc(3); 
							f++; }
						if (time_p3 >= m && time_mem2 >= m) {
							off_processorsc(act_p2);
							off_memoryc(2);
							d++; }
						else if (time_p3 >= m && time_mem3 >= m) {
							off_processorsc(act_p2);
							off_memoryc(3);
							d++; }
						break;
					case 4:
						if (time_p4 >= m && time_mem2 >= m) { 
							off_processorsc(act_p1); 
							off_memoryc(2); 
							f++; }
						else if (time_p4 >= m && time_mem4 >= m) {
							off_processorsc(act_p1);
							off_memoryc(4); 
							f++; }
						if (time_p3 >= m && time_mem2 >= m) { 
							off_processorsc(act_p2); 
							off_memoryc(2);
							d++; }
						else if (time_p3 >= m && time_mem4 >= m) {
							off_processorsc(act_p2); 
							off_memoryc(4); 
							d++; }
						break;
					}
					if (cq.check_empty())
					{
						if (check_call(p3)) 
							set_activec(p1, 1); 
						else {
							cq.push(1, a);
							b1c = wait; }
						if (check_call(p4)) 
							set_activec(p2, 2); 
						else { 
							cq.push(2, s);
							b2c = wait; }
					}
					else
					{
						cq.push(1, a); 
						b1c = wait;
						cq.push(2, s); 
						b2c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 3: // 3 mem
					if (time_p4 >= m && time_mem3 >= m) { 
						off_processorsc(act_p1); 
						off_memoryc(3);
						f++; }
					else if (time_p4 >= m && time_mem4 >= m) { 
						off_processorsc(act_p1); 
						off_memoryc(4);
						f++; }
					if (time_p3 >= m && time_mem3 >= m) { 
						off_processorsc(act_p2); 
						off_memoryc(3); 
						d++; }
					else if (time_p3 >= m && time_mem4 >= m) {
						off_processorsc(act_p2);
						off_memoryc(4); 
						d++; }
					if (cq.check_empty())
					{
						if (check_call(p3)) 
							set_activec(p1, 1);
						else { 
							cq.push(1, a);
							b1c = wait; }
						if (check_call(p4)) 
							set_activec(p2, 2); 
						else { 
							cq.push(2, s);
							b2c = wait; }
					}
					else
					{
						cq.push(1, a); 
						b1c = wait;
						cq.push(2, s);
						b2c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				}
				break;
			}
		}
		else
		{
			if (!cq.check_empty()) time_waitc++;
			int act_m1 = comparate_statesc(m1c, m2c, m3c, m4c), act_m2 = 0; // находим 1 активную
			int act_p1 = comparate_statesc(p1c, p2c, p3c, p4c), act_p2 = 0;
			int next_c = 0;
			switch (act_p1) // смотрим по 1 процессору
			{
			case 1: // 1 проц
				act_p2 = comparate_statesc(0, p2c, p3c, p4c);
				switch (act_m1) // смотрим по 1 памяти
				{
				case 1: // 1 память
					act_m2 = comparate_statesc(0, m2c, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 2:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							time_p1++; time_p2++; time_mem1++; time_mem2++;
							break;
						case 3:
							time_p1++; time_p2++; time_mem1++; time_mem3++;
							break;
						case 4:
							time_p1++; time_p2++; time_mem1++; time_mem4++;
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3);
							else { 
								cq.push(3, d); 
								b3c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4); 
							else { 
								cq.push(4, f); 
								b4c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							time_p1++; time_p3++; time_mem1++; time_mem2++;
							break;
						case 3:
							time_p1++; time_p3++; time_mem1++; time_mem3++;
							break;
						case 4:
							time_p1++; time_p3++; time_mem1++; time_mem4++;
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2); 
							else {
								cq.push(2, s); 
								b2c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4); 
							else { 
								cq.push(4, f); 
								b4c = wait; }
						}
						else
						{
							cq.push(2, s);
							b2c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							time_p1++; time_p4++; time_mem1++; time_mem2++;
							break;
						case 3:
							time_p1++; time_p4++; time_mem1++; time_mem3++;
							break;
						case 4:
							time_p1++; time_p4++; time_mem1++; time_mem4++;
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3); 
							else {
								cq.push(3, d);
								b3c = wait; }
							if (check_call(p2))
								set_activec(p2, 2);
							else { 
								cq.push(2, s); 
								b2c = wait; }
						}
						else
						{
							cq.push(3, d); 
							b3c = wait;
							cq.push(2, s);
							b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					}
					break;
				case 2: // 2 mem
					act_m2 = comparate_statesc(0, 0, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 2:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							time_p1++; time_p2++; time_mem2++; time_mem3++;
							break;
						case 4:
							time_p1++; time_p2++; time_mem4++; time_mem4++;
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3);
							else { 
								cq.push(3, d); 
								b3c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f); 
								b4c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							time_p1++; time_p3++; time_mem2++; time_mem3++;
							break;
						case 4:
							time_p1++; time_p3++; time_mem4++; time_mem4++;
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p2))
								set_activec(p2, 2);
							else { 
								cq.push(2, s); 
								b2c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f); 
								b4c = wait; }
						}
						else
						{
							cq.push(2, s);
							b2c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							time_p1++; time_p4++; time_mem2++; time_mem3++;
							break;
						case 4:
							time_p1++; time_p4++; time_mem4++; time_mem4++;
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3);
							else { 
								cq.push(3, d); 
								b3c = wait; }
							if (check_call(p2)) 
								set_activec(p2, 2); 
							else { 
								cq.push(2, s); 
								b2c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(2, s); 
							b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					}
					break;
				case 3: // 3 mem
					switch (act_p2) // смотрим по 2 процессору
					{
					case 2:
						time_p1++; time_p2++; time_mem3++; time_mem4++;
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3);
							else {
								cq.push(3, d); 
								b3c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f); 
								b4c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 3:
						time_p1++; time_p3++; time_mem3++; time_mem4++;
						if (cq.check_empty())
						{
							if (check_call(p2)) 
								set_activec(p2, 2);
							else { 
								cq.push(2, s); 
								b2c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f); 
								b4c = wait; }
						}
						else
						{
							cq.push(2, s); 
							b2c = wait;
							cq.push(4, f); 
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 4:
						time_p1++; time_p4++; time_mem3++; time_mem4++;
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3);
							else { 
								cq.push(3, d); 
								b3c = wait; }
							if (check_call(p2)) 
								set_activec(p2, 2); 
							else {
								cq.push(2, s); 
								b2c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(2, s); 
							b2c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					}
					break;
				}
				break;
			case 2: // 2 proc
				act_p2 = comparate_statesc(0, 0, p3c, p4c);
				switch (act_m1) // смотрим по 1 памяти
				{
				case 1: // 1 память
					act_m2 = comparate_statesc(0, m2c, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							time_p2++; time_p3++; time_mem1++; time_mem2++;
							break;
						case 3:
							time_p2++; time_p3++; time_mem1++; time_mem3++;
							break;
						case 4:
							time_p2++; time_p3++; time_mem1++; time_mem4++;
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p1))
								set_activec(p1, 1);
							else { 
								cq.push(1, a);
								b1c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f);
								b4c = wait; }
						}
						else
						{
							cq.push(1, a); 
							b1c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 2:
							time_p2++; time_p4++; time_mem1++; time_mem2++;
							break;
						case 3:
							time_p2++; time_p4++; time_mem1++; time_mem3++;
							break;
						case 4:
							time_p2++; time_p4++; time_mem1++; time_mem4++;
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3);
							else {
								cq.push(3, d);
								b3c = wait; }
							if (check_call(p1)) 
								set_activec(p1, 1); 
							else { 
								cq.push(1, a);
								b1c = wait; }
						}
						else
						{
							cq.push(3, d); 
							b3c = wait;
							cq.push(1, a); 
							b1c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					}
					break;
				case 2: // 2 mem
					act_m2 = comparate_statesc(0, 0, m3c, m4c);
					switch (act_p2) // смотрим по 2 процессору
					{
					case 3:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							time_p2++; time_p3++; time_mem2++; time_mem3++;
							break;
						case 4:
							time_p2++; time_p3++; time_mem2++; time_mem4++;
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1);
							else { 
								cq.push(1, a);
								b1c = wait; }
							if (check_call(p4)) 
								set_activec(p4, 4);
							else { 
								cq.push(4, f); 
								b4c = wait; }
						}
						else
						{
							cq.push(1, a);
							b1c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 4:
						switch (act_m2) // смотрим по 2 памяти
						{
						case 3:
							time_p2++; time_p4++; time_mem2++; time_mem3++;
							break;
						case 4:
							time_p2++; time_p4++; time_mem2++; time_mem4++;
							break;
						}
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3); 
							else { 
								cq.push(3, d); 
								b3c = wait; }
							if (check_call(p1)) 
								set_activec(p1, 1); 
							else { 
								cq.push(1, a);
								b1c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(1, a); 
							b1c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					}
					break;
				case 3: // 3 mem
					switch (act_p2) // смотрим по 2 процессору
					{
					case 3:
						time_p2++; time_p3++; time_mem3++; time_mem4++;
						if (cq.check_empty())
						{
							if (check_call(p1)) 
								set_activec(p1, 1);
							else { 
								cq.push(1, a);
								b1c = wait; }
							if (check_call(p4))
								set_activec(p4, 4); 
							else {
								cq.push(4, f); 
								b4c = wait; }
						}
						else
						{
							cq.push(1, a); 
							b1c = wait;
							cq.push(4, f);
							b4c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					case 4:
						time_p2++; time_p4++; time_mem3++; time_mem4++;
						if (cq.check_empty())
						{
							if (check_call(p3)) 
								set_activec(p3, 3);
							else { 
								cq.push(3, d);
								b3c = wait; }
							if (check_call(p1)) 
								set_activec(p1, 1);
							else { 
								cq.push(1, a);
								b1c = wait; }
						}
						else
						{
							cq.push(3, d);
							b3c = wait;
							cq.push(1, a);
							b1c = wait;
							int size = cq.get_size();
							for (int z = 0; z < size; z++)
								start_next_queuec(p1, p2, p3, p4);
						}
						break;
					}
					break;
				}
				break;
			case 3: // 3 proc
				act_p2 = 4;
				switch (act_m1) // смотрим по 1 памяти
				{
				case 1: // 1 память
					act_m2 = comparate_statesc(0, m2c, m3c, m4c);
					switch (act_m2) // смотрим по 2 памяти
					{
					case 2:
						time_p4++; time_p3++; time_mem1++; time_mem2++;
						break;
					case 3:
						time_p4++; time_p3++; time_mem1++; time_mem3++;
						break;
					case 4:
						time_p4++; time_p3++; time_mem1++; time_mem4++;
						break;
					}
					if (cq.check_empty())
					{
						if (check_call(p1)) 
							set_activec(p1, 1);
						else { 
							cq.push(1, a);
							b1c = wait; }
						if (check_call(p2)) 
							set_activec(p2, 2); 
						else { 
							cq.push(2, s); 
							b2c = wait; }
					}
					else
					{
						cq.push(1, a); 
						b1c = wait;
						cq.push(2, s); 
						b2c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 2: // 2 mem
					act_m2 = comparate_statesc(0, 0, m3c, m4c);
					switch (act_m2) // смотрим по 2 памяти
					{
					case 3:
						time_p4++; time_p3++; time_mem2++; time_mem3++;
						break;
					case 4:
						time_p4++; time_p3++; time_mem4++; time_mem2++;
						break;
					}
					next_c = comparate_statesc(c1, c2, 0, 0);
					if (cq.check_empty())
					{
						if (check_call(p1)) 
							set_activec(p1, 1);
						else {
							cq.push(1, a); 
							b1c = wait; }
						if (check_call(p2)) 
							set_activec(p2, 2); 
						else {
							cq.push(2, s);
							b2c = wait; }
					}
					else
					{
						cq.push(1, a); 
						b1c = wait;
						cq.push(2, s); 
						b2c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				case 3: // 3 mem
					time_p4++; time_p3++; time_mem3++; time_mem4++;
					if (cq.check_empty())
					{
						if (check_call(p1))
							set_activec(p1, 1);
						else { 
							cq.push(1, a); 
							b1c = wait; }
						if (check_call(p2))
							set_activec(p2, 2); 
						else { 
							cq.push(2, s); 
							b2c = wait; }
					}
					else
					{
						cq.push(1, a); 
						b1c = wait;
						cq.push(2, s); 
						b2c = wait;
						int size = cq.get_size();
						for (int z = 0; z < size; z++)
							start_next_queuec(p1, p2, p3, p4);
					}
					break;
				}
				break;
			}
		}
		break;
	}
}

void set_states_3_c(int c1, int c2, int c3, int c4,
	Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m)
{
	switch (c1 + c2 + c3 + c4)
	{
	case 3: // нет обращений
		if ((time_mem1 >= m) || (time_mem2 >= m) || (time_mem3 >= m) || (time_mem4 >= m))
		{
			if (time_p1 >= m) { 
				off_processorsc(1); 
				a++; }  
			if (time_p2 >= m) {
				off_processorsc(2);
				s++; }
			if (time_p3 >= m) { 
				off_processorsc(3); 
				d++; } 
			if (time_p4 >= m) {
				off_processorsc(4);
				f++; }
			if (time_mem1 >= m) 
				off_memoryc(1); 
			if (time_mem2 >= m)
				off_memoryc(2);
			if (time_mem3 >= m) 
				off_memoryc(3); 
			if (time_mem4 >= m) 
				off_memoryc(4);
			int no_c = min_comparate_statesc(p1c, p2c, p3c, p4c); // находим неактивный процессор
			switch (no_c) 
			{ 
			case 1: 
				a++; 
				break; 
			case 2: 
				s++; 
				break; 
			case 3:
				d++; 
				break; 
			case 4: 
				f++; 
				break; 
			}
		}
		else
		{
			if (!cq.check_empty()) 
				time_waitc++;
			int no_c = min_comparate_statesc(p1c, p2c, p3c, p4c); // находим неактивный процессор
			switch (no_c) 
			{ 
			case 1: 
				a++; 
				time_p2++; time_p3++; time_p4++;
				break; 
			case 2: 
				s++; 
				time_p1++; time_p3++; time_p4++;
				break; 
			case 3: 
				d++; 
				time_p2++; time_p1++; time_p4++;
				break; 
			case 4:
				f++; 
				time_p2++; time_p3++; time_p1++;
				break; 
			}
			no_c = min_comparate_statesc(m1c, m2c, m3c, m4c);
			switch (no_c)
			{
			case 1:
				time_mem2++; time_mem3++; time_mem4++;
				break;
			case 2:
				time_mem1++; time_mem3++; time_mem4++;
				break;
			case 3:
				time_mem2++; time_mem1++; time_mem4++;
				break;
			case 4:
				time_mem2++; time_mem3++; time_mem1++;
				break;
			}
		}
		break;
	case 4: // есть новое
		cstatus++;
		if ((time_mem1 >= m) || (time_mem2 >= m) || (time_mem3 >= m) || (time_mem4 >= m))
		{
			if (time_p1 >= m) { 
				off_processorsc(1); 
				a++; }  
			if (time_p2 >= m) { 
				off_processorsc(2);
				s++; }
			if (time_p3 >= m) { 
				off_processorsc(3);
				d++; }
			if (time_p4 >= m) { 
				off_processorsc(4);
				f++; }
			if (time_mem1 >= m)
				off_memoryc(1);
			if (time_mem2 >= m) 
				off_memoryc(2);
			if (time_mem3 >= m) 
				off_memoryc(3);
			if (time_mem4 >= m)
				off_memoryc(4);
			int no_c = min_comparate_statesc(p1c, p2c, p3c, p4c); // находим неактивный процессор
			switch (no_c) 
			{ 
			case 1: 
				if (cq.check_empty()) {
					if (check_call(p1)) 
						set_activec(p1, 1); 
					else {
						cq.push(1, a); 
						b1c = wait; 
					} 
				}
				else 
					start_next_queuec(p1, p2, p3, p4);
				break; 
			case 2: 
				if (cq.check_empty()) { 
					if (check_call(p2))
						set_activec(p2, 2);
					else { 
						cq.push(2, s); 
						b2c = wait; 
					} 
				}
				else 
					start_next_queuec(p1, p2, p3, p4);
				break; 
			case 3: 
				if (cq.check_empty()) { 
					if (check_call(p3))
						set_activec(p3, 3);
					else { 
						cq.push(3, d);
						b3c = wait; 
					} 
				}
				else 
					start_next_queuec(p1, p2, p3, p4);
				break; 
			case 4: 
				if (cq.check_empty()) { 
					if (check_call(p4)) 
						set_activec(p4, 4); 
					else {
						cq.push(4, f); 
						b4c = wait; 
					} 
				}
				else 
					start_next_queuec(p1, p2, p3, p4);
				break; }
		}
		else
		{
			if (!cq.check_empty()) 
				time_waitc++;
			int no_c = min_comparate_statesc(m1c, m2c, m3c, m4c);
			switch (no_c)
			{
			case 1:
				time_mem2++; time_mem3++; time_mem4++;
				break;
			case 2:
				time_mem1++; time_mem3++; time_mem4++;
				break;
			case 3:
				time_mem2++; time_mem1++; time_mem4++;
				break;
			case 4:
				time_mem2++; time_mem3++; time_mem1++;
				break;
			}
			no_c = min_comparate_statesc(p1c, p2c, p3c, p4c); // находим неактивный процессор
			switch (no_c)
			{
			case 1:
				if (cq.check_empty()) { 
					if (check_call(p1)) 
						set_activec(p1, 1); 
					else {
						cq.push(1, a);
						b1c = wait; 
					} 
				}
				else 
					start_next_queuec(p1, p2, p3, p4);
				time_p2++; time_p3++; time_p4++;
				break;
			case 2:
				if (cq.check_empty()) {
					if (check_call(p2)) 
						set_activec(p2, 2);
					else { 
						cq.push(2, s); 
						b2c = wait; 
					} 
				}
				else 
					start_next_queuec(p1, p2, p3, p4);
				time_p1++; time_p3++; time_p4++;
				break;
			case 3:
				if (cq.check_empty()) { 
					if (check_call(p3)) 
						set_activec(p3, 3); 
					else { 
						cq.push(3, d); 
						b3c = wait; 
					} 
				}
				else 
					start_next_queuec(p1, p2, p3, p4);
				time_p2++; time_p1++; time_p4++;
				break;
			case 4:
				if (cq.check_empty()) { 
					if (check_call(p4)) 
						set_activec(p4, 4);
					else { 
						cq.push(4, f); 
						b4c = wait; 
					} 
				}
				else 
					start_next_queuec(p1, p2, p3, p4);
				time_p2++; time_p3++; time_p1++;
				break;
			}
		}
		break;
	}
}

void set_states_4_c(int c1, int c2, int c3, int c4,
	Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m)
{
	if ((time_mem1 >= m) || (time_mem2 >= m) || (time_mem3 >= m) || (time_mem4 >= m))
	{
		if (time_mem1 >= m) { 
			off_memoryc(1); 
			time_mem1 = 0;
		} 
		if (time_p1 >= m) 
		{
			off_processorsc(1);
			time_p1 = 0;
			a++; 
		}
		if (time_mem2 >= m) 
		{ 
			off_memoryc(2); 
			time_mem2 = 0; }
		if (time_p2 >= m) 
		{ 
			off_processorsc(2); 
			time_p2 = 0; 
			s++; 
		}
		if (time_mem3 >= m) {
			off_memoryc(3); 
			time_mem3 = 0; } 
		if (time_p3 >= m) { 
			off_processorsc(3); 
			time_p3 = 0;
			d++; }
		if (time_mem4 >= m) { 
			off_memoryc(4); 
			time_mem4 = 0; } 
		if (time_p4 >= m) { 
			off_processorsc(4); 
			time_p4 = 0; 
			f++; }
	}
	else { 
		time_mem1++; time_mem2++; time_mem3++; 
		time_mem4++; time_p1++; 
		time_p2++; time_p3++; time_p4++; }
}

void switch_work(Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m)
{
	while ((a < p1.size_list) || (s < p2.size_list) || (d < p3.size_list) || (f < p4.size_list))
	{
		print_switch();
		ccp1 = 0; ccp2 = 0; ccp3 = 0; ccp4 = 0;
		if (a < p1.size_list)
			ccp1 = get_commandc(p1, a);
		if (s < p2.size_list)
			ccp2 = get_commandc(p2, s);
		if (d < p3.size_list)
			ccp3 = get_commandc(p3, d);
		if (f < p4.size_list)
			ccp4 = get_commandc(p4, f);
		cstatus = check_switch_status(); // текущее состояние
		switch (cstatus)
		{
		case no_calls:
			set_states_no_c(ccp1, ccp2, ccp3, ccp4, p1, p2, p3, p4);
			clkc++;
			break;
		case one_call:
			set_states_1_c(ccp1, ccp2, ccp3, ccp4, p1, p2, p3, p4, m);
			clkc++;
			break;
		case two_calls:
			set_states_2_c(ccp1, ccp2, ccp3, ccp4, p1, p2, p3, p4, m);
			clkc++;
			break;
		case three_calls:
			set_states_3_c(ccp1, ccp2, ccp3, ccp4, p1, p2, p3, p4, m);
			clkc++;
			break;
		case four_calls:
			set_states_4_c(ccp1, ccp2, ccp3, ccp4, p1, p2, p3, p4, m);
			clkc++;
			break;
		}
	}
}