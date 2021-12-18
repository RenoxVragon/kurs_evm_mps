#include "bus.h"
#include "Queue.h"
enum {passive, active, register_work, wait, process, start}; // состояния
enum { all_register = 1, one_process, one_wait, two_wait, three_wait }; // состояния шины
// m - время ожидания доступа
// p1 - p4, m1 - m4 - процессоры и ячейки памяти
int i = 0, j = 0, n = 0, l = 0; // индексы команд процессоров
int p1s = passive, p2s = passive, p3s = passive, p4s = passive; // состояние каждого процессора
int b1s = start, b2s = start, b3s = start, b4s = start; // операция процессора в шине
int m1s = passive, m2s = passive, m3s = passive, m4s = passive; // состояние каждой ячейки памяти
int clk = 0; // такты
int time_wait = 0;  // общее время ожидания доступа 
int time_process = 0; // время выполнения команды
int cp1, cp2, cp3, cp4; // команды процессоров
int status = 0; // состояние шины
Queue q; // очередь команд

void print_bus()
{
	system("cls");
	cout << "P1 " << p1s << " | P2 " << p2s << " | P3 " << p3s << " | P4 " << p4s << endl;
	cout << "-------------------------" << endl;
	cout << "  " << b1s << "  |   " << b2s << "  |   " << b3s << "  |  " << b4s << endl;
	cout << "-------------------------" << endl;
	cout << "M1 " << m1s << " | M2 " << m2s << " | M3 " << m3s << " | M4 " << m4s << endl;
	cout << "Такт: " << clk << endl;
	cout << "Общее время ожидания доступа к шине: " << time_wait << endl;
	cout << "Текущий номер команды 1 процессора: " << i << endl;
	cout << "Текущий номер команды 2 процессора: " << j << endl;
	cout << "Текущий номер команды 3 процессора: " << n << endl;
	cout << "Текущий номер команды 4 процессора: " << l << endl;
	cout << "0 - не активен, 1 - активен, 2 - работа с регистром, 3 - ожидание, 4 - обращение в память, 5 - старт." << endl;
	cout << "Очередь команд по процессорам: ";
	q.print();
	cout << endl;
	Sleep(1000);
}

int get_command(Processor& p, int index)
{
	return p.commands[index];
}

void start_set_states(Processor& p1, Processor& p2, Processor& p3, Processor& p4)
{
	//clk++;
	cp1 = get_command(p1, i);
	cp2 = get_command(p2, j);
	cp3 = get_command(p3, n);
	cp4 = get_command(p4, l);
	switch (cp1 + cp2 + cp3 + cp4)
	{
	case 0: // нет обращений
		b1s = register_work; b2s = register_work; b3s = register_work; b4s = register_work;
		i++; j++; n++; l++;
		break;
	case 1: // есть 1 обращение
		if (cp1 == 1)
		{
			b1s = process;
			b2s = register_work; b3s = register_work; b4s = register_work;
			j++; n++; l++;
			p1s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p1.appeals[r] > max)
				{
					max = p1.appeals[r];
					index_m = r;
				}
			}
			p1.appeals[index_m]--;
			switch(index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		else if (cp2 == 1)
		{
			b2s = process;
			b1s = register_work; b3s = register_work; b4s = register_work;
			i++; n++; l++;
			p2s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p2.appeals[r] > max)
				{
					max = p2.appeals[r];
					index_m = r;
				}
			}
			p2.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		else if (cp3 == 1)
		{
			b3s = process;
			b1s = register_work; b2s = register_work;  b4s = register_work;
			i++; j++; l++;
			p3s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p3.appeals[r] > max)
				{
					max = p3.appeals[r];
					index_m = r;
				}
			}
			p3.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		else 
		{
			b4s = process;
			b1s = register_work; b2s = register_work; b3s = register_work;
			i++; j++; n++;
			p4s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p4.appeals[r] > max)
				{
					max = p4.appeals[r];
					index_m = r;
				}
			}
			p4.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		break;
	case 2: // есть 2 обращения
		if (cp1 == 1)
		{
			b1s = process;
			b2s = register_work; b3s = register_work; b4s = register_work;
			if (cp2 == 1)
			{
				b2s = wait;
				q.push(2, j);
				n++; l++;
			}
			else if (cp3 == 1)
			{
				b3s = wait;
				q.push(3, n);
				j++; l++;
			}
			else 
			{
				b4s = wait;
				q.push(4, l);
				j++; n++;
			}
			p1s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p1.appeals[r] > max)
				{
					max = p1.appeals[r];
					index_m = r;
				}
			}
			p1.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		else if (cp2 == 1)
		{
			b2s = process;
			b1s = register_work; b3s = register_work; b4s = register_work;
			if (cp3 == 1)
			{
				b3s = wait;
				q.push(3, n);
				i++; l++;
			}
			else 
			{
				b4s = wait;
				q.push(4, l);
				i++; n++;
			}
			p2s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p2.appeals[r] > max)
				{
					max = p2.appeals[r];
					index_m = r;
				}
			}
			p2.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		else if (cp3 == 1)
		{
			b3s = process;
			b1s = register_work; b2s = register_work;  b4s = wait;
			q.push(4, l);
			i++; j++;
			p3s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p3.appeals[r] > max)
				{
					max = p3.appeals[r];
					index_m = r;
				}
			}
			p3.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		break;
	case 3: // есть 3 обращения
		if (cp1 == 1)
		{
			b1s = process;
			b2s = register_work; b3s = register_work; b4s = register_work;
			if (cp2 == 1 && cp3 == 1)
			{
				b2s = wait; b3s = wait;
				q.push(2, j);
				q.push(3, n);
				l++;
			}
			if (cp2 == 1 && cp4 == 1)
			{
				b2s = wait; b4s = wait;
				q.push(2, j);
				q.push(4, l);
				n++;
			}
			if (cp3 == 1 && cp4 == 1)
			{
				b3s = wait; b4s = wait;
				q.push(3, n);
				q.push(4, l);
				j++;
			}
			p1s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p1.appeals[r] > max)
				{
					max = p1.appeals[r];
					index_m = r;
				}
			}
			p1.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		else 
		{
			b2s = process;
			b3s = wait; b4s = wait; b1s = register_work;
			q.push(3, n);
			q.push(4, l);
			p2s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p2.appeals[r] > max)
				{
					max = p2.appeals[r];
					index_m = r;
				}
			}
			p2.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		break;
	case 4: // есть 4 обращения
		b1s = process; b2s = wait; b3s = wait; b4s = wait;
		q.push(2, j);
		q.push(3, n);
		q.push(4, l);
		p1s = active;
		int max = 0;
		int index_m = 0;
		for (int r = 0; r < 4; r++)
		{
			if (p1.appeals[r] > max)
			{
				max = p1.appeals[r];
				index_m = r;
			}
		}
		p1.appeals[index_m]--;
		switch (index_m)
		{
		case 0:
			m1s = active;
			break;
		case 1:
			m2s = active;
			break;
		case 2:
			m3s = active;
			break;
		case 3:
			m4s = active;
			break;
		}
		break;
	}
}

int comparate_states(int s1, int s2, int s3, int s4)
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

void off_processors(int state)
{
	switch (state)
	{
	case 1:
		p1s = passive;
		break;
	case 2:
		p2s = passive;
		break;
	case 3:
		p3s = passive;
		break;
	case 4:
		p4s = passive;
		break;
	}
}

void off_memory(int state)
{
	switch (state)
	{
	case 1:
		m1s = passive;
		break;
	case 2:
		m2s = passive;
		break;
	case 3:
		m3s = passive;
		break;
	case 4:
		m4s = passive;
		break;
	}
}

void set_active(Processor& p, int next_p)
{
	switch (next_p)
	{
	case 1:
		p1s = active;
		b1s = process;
		break;
	case 2:
		p2s = active;
		b2s = process;
		break;
	case 3:
		p3s = active;
		b3s = process;
		break;
	case 4:
		p4s = active;
		b4s = process;
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
		m1s = active;
		break;
	case 1:
		m2s = active;
		break;
	case 2:
		m3s = active;
		break;
	case 3:
		m4s = active;
		break;
	}
}

void start_next_queue(Processor& p1, Processor& p2, Processor& p3, Processor& p4)
{
	int data = q.pop();
	switch (data)
	{
	case 1:
		set_active(p1, 1);
		break;
	case 2:
		set_active(p2, 2);
		break;
	case 3:
		set_active(p3, 3);
		break;
	case 4:
		set_active(p4, 4);
		break;
	}
}

int check_bus_status()
{
	switch (b1s + b2s + b3s + b4s)
	{
	case 8:
		return all_register;
		break;
	case 10:
		return one_process;
		break;
	case 11:
		return one_wait;
		break;
	case 12:
		return two_wait;
		break;
	case 13:
		return three_wait;
		break;
	case 20:
		return all_register;
		break;
	}
}

void set_states_all_reg(int c1, int c2, int c3, int c4, 
	Processor& p1, Processor& p2, Processor& p3, Processor& p4)
{
	switch (c1 + c2 + c3 + c4)
	{
	case 0: // нет обращений
		b1s = register_work; b2s = register_work; b3s = register_work; b4s = register_work;
		i++; j++; n++; l++;
		break;
	case 1: // есть 1 обращение
		if (cp1 == 1)
		{
			b1s = process;
			b2s = register_work; b3s = register_work; b4s = register_work;
			j++; n++; l++;
			p1s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p1.appeals[r] > max)
				{
					max = p1.appeals[r];
					index_m = r;
				}
			}
			p1.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		else if (cp2 == 1)
		{
			b2s = process;
			b1s = register_work; b3s = register_work; b4s = register_work;
			i++; n++; l++;
			p2s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p2.appeals[r] > max)
				{
					max = p2.appeals[r];
					index_m = r;
				}
			}
			p2.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		else if (cp3 == 1)
		{
			b3s = process;
			b1s = register_work; b2s = register_work;  b4s = register_work;
			i++; j++; l++;
			p3s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p3.appeals[r] > max)
				{
					max = p3.appeals[r];
					index_m = r;
				}
			}
			p3.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		else
		{
			b4s = process;
			b1s = register_work; b2s = register_work; b3s = register_work;
			i++; j++; n++;
			p4s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p4.appeals[r] > max)
				{
					max = p4.appeals[r];
					index_m = r;
				}
			}
			p4.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		break;
	case 2: // есть 2 обращения
		if (cp1 == 1)
		{
			b1s = process;
			b2s = register_work; b3s = register_work; b4s = register_work;
			if (cp2 == 1)
			{
				b2s = wait;
				q.push(2, j);
				n++; l++;
			}
			else if (cp3 == 1)
			{
				b3s = wait;
				q.push(3, n);
				j++; l++;
			}
			else
			{
				b4s = wait;
				q.push(4, l);
				j++; n++;
			}
			p1s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p1.appeals[r] > max)
				{
					max = p1.appeals[r];
					index_m = r;
				}
			}
			p1.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		else if (cp2 == 1)
		{
			b2s = process;
			b1s = register_work; b3s = register_work; b4s = register_work;
			if (cp3 == 1)
			{
				b3s = wait;
				q.push(3, n);
				i++; l++;
			}
			else
			{
				b4s = wait;
				q.push(4, l);
				i++; n++;
			}
			p2s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p2.appeals[r] > max)
				{
					max = p2.appeals[r];
					index_m = r;
				}
			}
			p2.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		else if (cp3 == 1)
		{
			b3s = process;
			b1s = register_work; b2s = register_work;  b4s = wait;
			q.push(4, l);
			i++; j++;
			p3s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p3.appeals[r] > max)
				{
					max = p3.appeals[r];
					index_m = r;
				}
			}
			p3.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		break;
	case 3: // есть 3 обращения
		if (cp1 == 1)
		{
			b1s = process;
			b2s = register_work; b3s = register_work; b4s = register_work;
			if (cp2 == 1 && cp3 == 1)
			{
				b2s = wait; b3s = wait;
				q.push(2, j);
				q.push(3, n);
				l++;
			}
			if (cp2 == 1 && cp4 == 1)
			{
				b2s = wait; b4s = wait;
				q.push(2, j);
				q.push(4, l);
				n++;
			}
			if (cp3 == 1 && cp4 == 1)
			{
				b3s = wait; b4s = wait;
				q.push(3, n);
				q.push(4, l);
				j++;
			}
			p1s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p1.appeals[r] > max)
				{
					max = p1.appeals[r];
					index_m = r;
				}
			}
			p1.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		else
		{
			b2s = process;
			b3s = wait; b4s = wait; b1s = register_work;
			q.push(3, n);
			q.push(4, l);
			p2s = active;
			int max = 0;
			int index_m = 0;
			for (int r = 0; r < 4; r++)
			{
				if (p2.appeals[r] > max)
				{
					max = p2.appeals[r];
					index_m = r;
				}
			}
			p2.appeals[index_m]--;
			switch (index_m)
			{
			case 0:
				m1s = active;
				break;
			case 1:
				m2s = active;
				break;
			case 2:
				m3s = active;
				break;
			case 3:
				m4s = active;
				break;
			}
		}
		break;
	case 4: // есть 4 обращения
		b1s = process; b2s = wait; b3s = wait; b4s = wait;
		q.push(2, j);
		q.push(3, n);
		q.push(4, l);
		p1s = active;
		int max = 0;
		int index_m = 0;
		for (int r = 0; r < 4; r++)
		{
			if (p1.appeals[r] > max)
			{
				max = p1.appeals[r];
				index_m = r;
			}
		}
		p1.appeals[index_m]--;
		switch (index_m)
		{
		case 0:
			m1s = active;
			break;
		case 1:
			m2s = active;
			break;
		case 2:
			m3s = active;
			break;
		case 3:
			m4s = active;
			break;
		}
		break;
	}
}

void set_states_one_p(int c1, int c2, int c3, int c4,
	Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m)
{
	switch (c1 + c2 + c3 + c4)
	{
	case 1: // 0 доп обращений
		if (time_process == m)
		{
			time_process = 0;
			int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
			int state_p = comparate_states(p1s, p2s, p3s, p4s);
			int state_m = comparate_states(m1s, m2s, m3s, m4s);
			switch (state)
			{
			case 1:
				b1s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				i++; j++; n++; l++;
				break;
			case 2:
				b2s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				i++; j++; n++; l++;
				break;
			case 3:
				b3s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				i++; j++; n++; l++;
				break;
			case 4:
				b4s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				i++; j++; n++; l++;
				break;
			}
		}
		else
		{
			//time_wait++;
			time_process++;
			int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
			switch (state)
			{
			case 1:
				j++; n++; l++;
				break;
			case 2:
				i++; n++; l++;
				break;
			case 3:
				i++; j++; l++;
				break;
			case 4:
				i++; j++; n++;
				break;
			}
		}
		break;
	case 2: // 1 доп обращение, становится process или в очередь
		if (time_process == m)
		{
			time_process = 0;
			int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
			int state_p = comparate_states(p1s, p2s, p3s, p4s);
			int state_m = comparate_states(m1s, m2s, m3s, m4s);
			int next_p = 0;
			switch (state) // обновление шины, следующий процесс запускается
			{
			case 1:
				b1s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				next_p = comparate_states(0, c2, c3, c4);
				switch (next_p)
				{
				case 2:
					set_active(p2, next_p);
					i++; n++; l++;
					break;
				case 3:
					set_active(p3, next_p);
					i++; j++; l++;
					break;
				case 4:
					set_active(p4, next_p);
					i++; j++; n++;
					break;
				}
				break;
			case 2:
				b2s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				next_p = comparate_states(c1, 0, c3, c4);
				switch (next_p)
				{
				case 1:
					set_active(p1, next_p);
					j++; n++; l++;
					break;
				case 3:
					set_active(p3, next_p);
					j++; i++; l++;
					break;
				case 4:
					set_active(p4, next_p);
					j++; i++; n++;
					break;
				}
				break;
			case 3:
				b3s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				next_p = comparate_states(c1, c2, 0, c4);
				switch (next_p)
				{
				case 1:
					set_active(p1, next_p);
					n++; j++; l++;
					break;
				case 2:
					set_active(p2, next_p);
					n++; i++; l++;
					break;
				case 4:
					set_active(p4, next_p);
					n++; j++; i++;
					break;
				}
				break;
			case 4:
				b4s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				next_p = comparate_states(c1, c2, c3, 0);
				switch (next_p)
				{
				case 1:
					set_active(p1, next_p);
					l++; n++; j++;
					break;
				case 2:
					set_active(p2, next_p);
					l++; n++; i++;
					break;
				case 3:
					set_active(p3, next_p);
					l++; j++; i++;
					break;
				}
				break;
			}
		}
		else
		{
			time_wait++;
			time_process++;
			int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
			int next_p = 0;
			switch (state)
			{
			case 1:
				next_p = comparate_states(0, c2, c3, c4);
				switch (next_p)
				{
				case 2:
					q.push(2, j);
					b2s = wait;
					n++; l++;
					break;
				case 3:
					q.push(3, n);
					b3s = wait;
					j++; l++;
					break;
				case 4:
					q.push(4, l);
					b4s = wait;
					n++; j++;
					break;
				}
				break;
			case 2:
				next_p = comparate_states(c1, 0, c3, c4);
				switch (next_p)
				{
				case 1:
					q.push(1, i);
					b1s = wait;
					n++; l++;
					break;
				case 3:
					q.push(3, n);
					b3s = wait;
					i++; l++;
					break;
				case 4:
					q.push(4, l);
					b4s = wait;
					n++; i++;
					break;
				}
				break;
			case 3:
				next_p = comparate_states(c1, c2, 0, c4);
				switch (next_p)
				{
				case 1:
					q.push(1, i);
					b1s = wait;
					j++; l++;
					break;
				case 2:
					q.push(2, j);
					b2s = wait;
					i++; l++;
					break;
				case 4:
					q.push(4, l);
					b4s = wait;
					i++; j++;
					break;
				}
				break;
			case 4:
				next_p = comparate_states(c1, c2, c3, 0);
				switch (next_p)
				{
				case 1:
					q.push(1, i);
					b1s = wait;
					j++; n++;
					break;
				case 2:
					q.push(2, j);
					b2s = wait;
					i++; n++;
					break;
				case 3:
					q.push(3, n);
					b3s = wait;
					i++; j++;
					break;
				}
				break;
			}
		}
		break;
	case 3: // 2 доп обращения, 1 становится process или оба в очередь
		if (time_process == m)
		{
			time_process = 0;
			int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
			int state_p = comparate_states(p1s, p2s, p3s, p4s);
			int state_m = comparate_states(m1s, m2s, m3s, m4s);
			int next_p = 0;
			switch (state) // обновление шины, следующий процесс запускается
			{
			case 1:
				b1s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				next_p = comparate_states(0, c2, c3, c4);
				switch (next_p)
				{
				case 2:
					set_active(p2, next_p);
					if (c3 == 1)
					{
						q.push(3, n);
						b3s = wait;
						i++; l++;
					}
					else
					{
						q.push(4, l);
						b4s = wait;
						i++; n++;
					}
					break;
				case 3:
					set_active(p3, next_p);
					q.push(4, l);
					b4s = wait;
					i++; j++;
					break;
				}
				break;
			case 2:
				b2s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				next_p = comparate_states(c1, 0, c3, c4);
				switch (next_p)
				{
				case 1:
					set_active(p1, next_p);
					if (c3 == 1)
					{
						q.push(3, n);
						b3s = wait;
						j++; l++;
					}
					else
					{
						q.push(4, l);
						b4s = wait;
						j++; n++;
					}
					break;
				case 3:
					set_active(p3, next_p);
					q.push(4, l);
					b4s = wait;
					j++; i++;
					break;
				}
				break;
			case 3:
				b3s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				next_p = comparate_states(c1, c2, 0, c4);
				switch (next_p)
				{
				case 1:
					set_active(p1, next_p);
					if (c2 == 1)
					{
						q.push(2, j);
						b2s = wait;
						n++; l++;
					}
					else
					{
						q.push(4, l);
						b4s = wait;
						n++; j++;
					}
					break;
				case 2:
					set_active(p2, next_p);
					q.push(4, l);
					b4s = wait;
					n++; i++;
					break;
				}
				break;
			case 4:
				b4s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				next_p = comparate_states(c1, c2, c3, 0);
				switch (next_p)
				{
				case 1:
					set_active(p1, next_p);
					if (c2 == 1)
					{
						q.push(2, j);
						b2s = wait;
						l++; n++;
					}
					else
					{
						q.push(3, n);
						b3s = wait;
						l++; j++;
					}
					l++; n++; j++;
					break;
				case 2:
					set_active(p2, next_p);
					q.push(3, n);
					b3s = wait;
					l++; i++;
					break;
				}
				break;
			}
		}
		else
		{
			time_wait++;
			time_process++;
			int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
			int next_p = 0;
			switch (state)
			{
			case 1:
				if (c2 == 1 && c3 == 1)
				{
					q.push(2, j);
					q.push(3, n);
					b2s = wait;
					b3s = wait;
					l++;
				}
				if (c2 == 1 && c4 == 1)
				{
					q.push(2, j);
					q.push(4, l);
					b2s = wait;
					b4s = wait;
					n++;
				}
				if (c3 == 1 && c4 == 1)
				{
					q.push(3, n);
					q.push(4, l);
					b3s = wait;
					b4s = wait;
					j++;
				}
				break;
			case 2:
				if (c1 == 1 && c3 == 1)
				{
					q.push(1, i);
					q.push(3, n);
					b1s = wait;
					b3s = wait;
					l++;
				}
				if (c1 == 1 && c4 == 1)
				{
					q.push(1, i);
					q.push(4, l);
					b1s = wait;
					b4s = wait;
					n++;
				}
				if (c3 == 1 && c4 == 1)
				{
					q.push(3, n);
					q.push(4, l);
					b3s = wait;
					b4s = wait;
					i++;
				}
				break;
			case 3:
				if (c2 == 1 && c1 == 1)
				{
					q.push(2, j);
					q.push(1, i);
					b2s = wait;
					b1s = wait;
					l++;
				}
				if (c2 == 1 && c4 == 1)
				{
					q.push(2, j);
					q.push(4, l);
					b2s = wait;
					b4s = wait;
					i++;
				}
				if (c1 == 1 && c4 == 1)
				{
					q.push(1, i);
					q.push(4, l);
					b1s = wait;
					b4s = wait;
					j++;
				}
				break;
			case 4:
				if (c2 == 1 && c3 == 1)
				{
					q.push(2, j);
					q.push(3, n);
					b2s = wait;
					b3s = wait;
					i++;
				}
				if (c1 == 1 && c3 == 1)
				{
					q.push(1, i);
					q.push(3, n);
					b1s = wait;
					b3s = wait;
					j++;
				}
				if (c1 == 1 && c2 == 1)
				{
					q.push(1, i);
					q.push(2, j);
					b1s = wait;
					b2s = wait;
					n++;
				}
				break;
			}
		}
		break;
	case 4: // 3 доп обращения, 1 становится process или все в очередь
		if (time_process == m)
		{
			time_process = 0;
			int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
			int state_p = comparate_states(p1s, p2s, p3s, p4s);
			int state_m = comparate_states(m1s, m2s, m3s, m4s);
			switch (state)
			{
			case 1:
				b1s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				set_active(p2, 2);
				q.push(3, n);
				q.push(4, l);
				b3s = wait;
				b4s = wait;
				i++;
				break;
			case 2:
				b2s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				set_active(p1, 1);
				q.push(3, n);
				q.push(4, l);
				b3s = wait;
				b4s = wait;
				j++;
				break;
			case 3:
				b3s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				set_active(p1, 1);
				q.push(2, j);
				q.push(4, l);
				b2s = wait;
				b4s = wait;
				n++;
				break;
			case 4:
				b4s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				set_active(p1, 1);
				q.push(2, j);
				q.push(3, n);
				b2s = wait;
				b3s = wait;
				l++;
				break;
			}
		}
		else
		{
			time_wait++;
			time_process++;
			int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
			switch (state)
			{
			case 1:
				q.push(2, j);
				q.push(3, n);
				q.push(4, l);
				b2s = wait;
				b3s = wait;
				b4s = wait;
				break;
			case 2:
				q.push(1, i);
				q.push(3, n);
				q.push(4, l);
				b1s = wait;
				b3s = wait;
				b4s = wait;
				break;
			case 3:
				q.push(1, i);
				q.push(2, j);
				q.push(4, l);
				b1s = wait;
				b2s = wait;
				b4s = wait;
				break;
			case 4:
				q.push(1, i);
				q.push(2, j);
				q.push(3, n);
				b1s = wait;
				b2s = wait;
				b3s = wait;
				break;
			}
		}
		break;
	}
}

void set_states_one_w(int c1, int c2, int c3, int c4,
	Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m)
{
	switch (c1 + c2 + c3 + c4)
	{
	case 2: // нет новых
		if (time_process == m)
		{
			time_process = 0;
			int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
			int state_p = comparate_states(p1s, p2s, p3s, p4s);
			int state_m = comparate_states(m1s, m2s, m3s, m4s);
			int next_p = 0;
			switch (state)
			{
			case 1:
				b1s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(0, p2s, p3s, p4s);
				switch (next_p)
				{
				case 2:
					i++; n++; l++;
					break;
				case 3:
					i++; j++; l++;
					break;
				case 4:
					i++; j++; n++;
					break;
				}
				break;
			case 2:
				b2s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(p1s, 0, p3s, p4s);
				switch (next_p)
				{
				case 1:
					j++; n++; l++;
					break;
				case 3:
					j++; i++; l++;
					break;
				case 4:
					j++; i++; n++;
					break;
				}
				break;
			case 3:
				b3s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(p1s, p2s, 0, p4s);
				switch (next_p)
				{
				case 1:
					n++; j++; l++;
					break;
				case 2:
					n++; i++; l++;
					break;
				case 4:
					n++; j++; i++;
					break;
				}
				break;
			case 4:
				b4s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(p1s, p2s, p3s, 0);
				switch (next_p)
				{
				case 1:
					l++; n++; j++;
					break;
				case 2:
					l++; i++; n++;
					break;
				case 3:
					l++; j++; i++;
					break;
				}
				break;
			}
		}
		else
		{
			time_wait++;
			time_process++;
			int next_c = 0;
			int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
			switch (state)
			{
			case 1:
				next_c = comparate_states(0, b2s, b3s, b4s);
				switch (next_c)
				{
				case 2:
					n++; l++;
					break;
				case 3:
					j++; l++;
					break;
				case 4:
					j++; n++;
					break;
				}
				break;
			case 2:
				next_c = comparate_states(b1s, 0, b3s, b4s);
				switch (next_c)
				{
				case 1:
					n++; l++;
					break;
				case 3:
					i++; l++;
					break;
				case 4:
					i++; n++;
					break;
				}
				break;
			case 3:
				next_c = comparate_states(b1s, b2s, 0, b4s);
				switch (next_c)
				{
				case 1:
					j++; l++;
					break;
				case 2:
					i++; l++;
					break;
				case 4:
					j++; i++;
					break;
				}
				break;
			case 4:
				next_c = comparate_states(b1s, b2s, b3s, 0);
				switch (next_c)
				{
				case 1:
					n++; j++;
					break;
				case 2:
					i++; n++;
					break;
				case 3:
					j++; i++;
					break;
				}
				break;
			}
		}
		break; 
	case 3: // есть 1 новое 
		if (time_process == m)
		{
			time_process = 0;
			int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
			int state_p = comparate_states(p1s, p2s, p3s, p4s);
			int state_m = comparate_states(m1s, m2s, m3s, m4s);
			int next_p = 0;
			int next_c = 0;
			switch (state)
			{
			case 1:
				b1s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(0, p2s, p3s, p4s);
				switch (next_p)
				{
				case 2:
					next_c = comparate_states(0, 0, c3, c4);
					if (next_c == 3)
					{
						q.push(3, n);
						b3s = wait;
						i++; l++;
					}
					else
					{
						q.push(4, l);
						b4s = wait;
						i++; n++;
					}
					break;
				case 3:
					next_c = comparate_states(0, c2, 0, c4);
					if (next_c == 2)
					{
						q.push(2, j);
						b2s = wait;
						i++; l++;
					}
					else
					{
						q.push(4, l);
						b4s = wait;
						i++; j++;
					}
					break;
				case 4:
					next_c = comparate_states(0, c2, c3, 0);
					if (next_c == 3)
					{
						q.push(3, n);
						b3s = wait;
						i++; j++;
					}
					else
					{
						q.push(2, j);
						b2s = wait;
						i++; n++;
					}
					break;
				}
				break;
			case 2:
				b2s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(p1s, 0, p3s, p4s);
				switch (next_p)
				{
				case 1:
					next_c = comparate_states(0, 0, c3, c4);
					if (next_c == 3)
					{
						q.push(3, n);
						b3s = wait;
						j++; l++;
					}
					else
					{
						q.push(4, l);
						b4s = wait;
						j++; n++;
					}
					break;
				case 3:
					next_c = comparate_states(c1, 0, 0, c4);
					if (next_c == 1)
					{
						q.push(1, i);
						b1s = wait;
						j++; l++;
					}
					else
					{
						q.push(4, l);
						b4s = wait;
						j++; i++;
					}
					break;
				case 4:
					next_c = comparate_states(c1, 0, c3, 0);
					if (next_c == 3)
					{
						q.push(3, n);
						b3s = wait;
						i++; j++;
					}
					else
					{
						q.push(1, i);
						b1s = wait;
						j++; n++;
					}
					break;
				}
				break;
			case 3:
				b3s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(p1s, p2s, 0, p4s);
				switch (next_p)
				{
				case 2:
					next_c = comparate_states(c1, 0, 0, c4);
					if (next_c == 1)
					{
						q.push(1, i);
						b1s = wait;
						n++; l++;
					}
					else
					{
						q.push(4, l);
						b4s = wait;
						n++; i++;
					}
					break;
				case 1:
					next_c = comparate_states(0, c2, 0, c4);
					if (next_c == 2)
					{
						q.push(2, j);
						b2s = wait;
						n++; l++;
					}
					else
					{
						q.push(4, l);
						b4s = wait;
						n++; j++;
					}
					break;
				case 4:
					next_c = comparate_states(c1, c2, 0, 0);
					if (next_c == 1)
					{
						q.push(1, i);
						b1s = wait;
						n++; j++;
					}
					else
					{
						q.push(2, j);
						b2s = wait;
						n++; i++;
					}
					break;
				}
				break;
			case 4:
				b4s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(p1s, p2s, p3s, 0);
				switch (next_p)
				{
				case 1:
					next_c = comparate_states(0, c2, c3, 0);
					if (next_c == 3)
					{
						q.push(3, n);
						b3s = wait;
						l++; j++;
					}
					else
					{
						q.push(2, j);
						b2s = wait;
						l++; n++;
					}
					break;
				case 2:
					next_c = comparate_states(c1, 0, c3, 0);
					if (next_c == 1)
					{
						q.push(1, i);
						b1s = wait;
						l++; n++;
					}
					else
					{
						q.push(3, n);
						b3s = wait;
						l++; i++;
					}
					break;
				case 3:
					next_c = comparate_states(c1, c2, 0, 0);
					if (next_c == 1)
					{
						q.push(1, i);
						b1s = wait;
						l++; j++;
					}
					else
					{
						q.push(2, j);
						b2s = wait;
						l++; i++;
					}
					break;
				}
				break;
			}
		}
		else
		{
		time_wait++;
		time_process++;
		int next_c = 0;
		int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
		switch (state)
		{
		case 1:
			next_c = comparate_states(0, b2s, b3s, b4s);
			switch (next_c)
			{
			case 2:
				if (c3 == 1)
				{
					q.push(3, n);
					b3s = wait;
					l++;
				}
				else
				{
					q.push(4, l);
					b4s = wait;
					n++;
				}
				break;
			case 3:
				if (c2 == 1)
				{
					q.push(2, j);
					b2s = wait;
					l++;
				}
				else
				{
					q.push(4, l);
					b4s = wait;
					j++;
				}
				break;
			case 4:
				if (c3 == 1)
				{
					q.push(3, n);
					b3s = wait;
					j++;
				}
				else
				{
					q.push(2, j);
					b2s = wait;
					n++;
				}
				break;
			}
			break;
		case 2:
			next_c = comparate_states(b1s, 0, b3s, b4s);
			switch (next_c)
			{
			case 1:
				if (c3 == 1)
				{
					q.push(3, n);
					b3s = wait;
					l++;
				}
				else
				{
					q.push(4, l);
					b4s = wait;
					n++;
				}
				break;
			case 3:
				if (c1 == 1)
				{
					q.push(1, i);
					b1s = wait;
					l++;
				}
				else
				{
					q.push(4, l);
					b4s = wait;
					i++;
				}
				break;
			case 4:
				if (c3 == 1)
				{
					q.push(3, n);
					b3s = wait;
					i++;
				}
				else
				{
					q.push(1, i);
					b1s = wait;
					l++;
				}
				break;
			}
			break;
		case 3:
			next_c = comparate_states(b1s, b2s, 0, b4s);
			switch (next_c)
			{
			case 1:
				if (c2 == 1)
				{
					q.push(2, j);
					b2s = wait;
					l++;
				}
				else
				{
					q.push(4, l);
					b4s = wait;
					j++;
				}
				break;
			case 2:
				if (c1 == 1)
				{
					q.push(1, i);
					b1s = wait;
					l++;
				}
				else
				{
					q.push(4, l);
					b4s = wait;
					i++;
				}
				break;
			case 4:
				if (c1 == 1)
				{
					q.push(1, i);
					b1s = wait;
					j++;
				}
				else
				{
					q.push(2, j);
					b2s = wait;
					i++;
				}
				break;
			}
			break;
		case 4:
			next_c = comparate_states(b1s, b2s, b3s, 0);
			switch (next_c)
			{
			case 1:
				if (c3 == 1)
				{
					q.push(3, n);
					b3s = wait;
					j++;
				}
				else
				{
					q.push(2, j);
					b2s = wait;
					n++;
				}
				break;
			case 2:
				if (c3 == 1)
				{
					q.push(3, n);
					b3s = wait;
					i++;
				}
				else
				{
					q.push(1, i);
					b1s = wait;
					n++;
				}
				break;
			case 3:
				if (c2 == 1)
				{
					q.push(2, j);
					b2s = wait;
					i++;
				}
				else
				{
					q.push(1, i);
					b1s = wait;
					j++;
				}
				break;
			}
			break;
		}
		}
		break;
	case 4: // eсть 2 новых
		if (time_process == m)
		{
			time_process = 0;
			int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
			int state_p = comparate_states(p1s, p2s, p3s, p4s);
			int state_m = comparate_states(m1s, m2s, m3s, m4s);
			int next_p = 0;
			switch (state)
			{
			case 1:
				b1s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(0, p2s, p3s, p4s);
				switch (next_p)
				{
				case 2:
					q.push(3, n);
					q.push(4, l);
					b3s = wait;
					b4s = wait;
					i++;
					break;
				case 3:
					q.push(2, j);
					q.push(4, l);
					b2s = wait;
					b4s = wait;
					i++;
					break;
				case 4:
					q.push(2, j);
					q.push(3, n);
					b2s = wait;
					b3s = wait;
					i++;
					break;
				}
				break;
			case 2:
				b2s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(p1s, 0, p3s, p4s);
				switch (next_p)
				{
				case 1:
					q.push(3, n);
					q.push(4, l);
					b3s = wait;
					b4s = wait;
					j++;
					break;
				case 3:
					q.push(1, i);
					q.push(4, l);
					b1s = wait;
					b4s = wait;
					j++;
					break;
				case 4:
					q.push(1, i);
					q.push(3, n);
					b1s = wait;
					b3s = wait;
					j++;
					break;
				}
				break;
			case 3:
				b3s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(p1s, p2s, 0, p4s);
				switch (next_p)
				{
				case 2:
					q.push(1, i);
					q.push(4, l);
					b1s = wait;
					b4s = wait;
					n++;
					break;
				case 1:
					q.push(2, j);
					q.push(4, l);
					b2s = wait;
					b4s = wait;
					n++;
					break;
				case 4:
					q.push(1, i);
					q.push(2, j);
					b1s = wait;
					b2s = wait;
					n++;
					break;
				}
				break;
			case 4:
				b4s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(p1s, p2s, p3s, 0);
				switch (next_p)
				{
				case 1:
					q.push(2, j);
					q.push(3, n);
					b2s = wait;
					b3s = wait;
					l++;
					break;
				case 2:
					q.push(1, i);
					q.push(3, n);
					b1s = wait;
					b3s = wait;
					l++;
					break;
				case 3:
					q.push(1, i);
					q.push(2, j);
					b1s = wait;
					b2s = wait;
					l++;
					break;
				}
				break;
			}
		}
		else
		{
		time_wait++;
		time_process++;
		int next_c = 0;
		int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
		switch (state)
		{
		case 1:
			next_c = comparate_states(0, b2s, b3s, b4s);
			switch (next_c)
			{
			case 2:
				q.push(3, n);
				q.push(4, l);
				b3s = wait;
				b4s = wait;
				break;
			case 3:
				q.push(2, j);
				q.push(4, l);
				b2s = wait;
				b4s = wait;
				break;
			case 4:
				q.push(2, j);
				q.push(3, n);
				b2s = wait;
				b3s = wait;
				break;
			}
			break;
		case 2:
			next_c = comparate_states(b1s, 0, b3s, b4s);
			switch (next_c)
			{
			case 1:
				q.push(3, n);
				q.push(4, l);
				b3s = wait;
				b4s = wait;
				break;
			case 3:
				q.push(1, i);
				q.push(4, l);
				b1s = wait;
				b4s = wait;
				break;
			case 4:
				q.push(1, i);
				q.push(3, n);
				b1s = wait;
				b3s = wait;
				break;
			}
			break;
		case 3:
			next_c = comparate_states(b1s, b2s, 0, b4s);
			switch (next_c)
			{
			case 1:
				q.push(2, j);
				q.push(4, l);
				b2s = wait;
				b4s = wait;
				break;
			case 2:
				q.push(1, i);
				q.push(4, l);
				b1s = wait;
				b4s = wait;
				break;
			case 4:
				q.push(1, i);
				q.push(2, j);
				b1s = wait;
				b2s = wait;
				break;
			}
			break;
		case 4:
			next_c = comparate_states(b1s, b2s, b3s, 0);
			switch (next_c)
			{
			case 1:		
				q.push(2, j);
				q.push(3, n);
				b2s = wait;
				b3s = wait;
				break;
			case 2:
				q.push(1, i);
				q.push(3, n);
				b1s = wait;
				b3s = wait;
				break;
			case 3:
				q.push(1, i);
				q.push(2, j);
				b1s = wait;
				b2s = wait;
				break;
			}
			break;
		}
		}
		break;
	}
}

void set_states_two_w(int c1, int c2, int c3, int c4,
	Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m)
{
	switch (c1 + c2 + c3 + c4)
	{
	case 3: // нет новых
		if (time_process == m)
		{
			time_process = 0;
			int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
			int state_p = comparate_states(p1s, p2s, p3s, p4s);
			int state_m = comparate_states(m1s, m2s, m3s, m4s);
			int next_p = 0;
			switch (state)
			{
			case 1:
				b1s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(0, p2s, p3s, p4s);
				switch (next_p)
				{
				case 2:
					if (b3s == wait)
					{
						i++; l++;
					}
					else
					{
						i++; n++;
					}
					break;
				case 3:
					if (b2s == wait)
					{
						i++; l++;
					}
					else
					{
						i++; j++;
					}
					break;
				case 4:
					if (b3s == wait)
					{
						i++; j++;
					}
					else
					{
						i++; n++;
					}
					break;
				}
				break;
			case 2:
				b2s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(p1s, 0, p3s, p4s);
				switch (next_p)
				{
				case 1:
					if (b3s == wait)
					{
						j++; l++;
					}
					else
					{
						j++; n++;
					}
					break;
				case 3:
					if (b1s == wait)
					{
						j++; l++;
					}
					else
					{
						j++; i++;
					}
					break;
				case 4:
					if (b3s == wait)
					{
						j++; i++;
					}
					else
					{
						j++; n++;
					}
					break;
				}
				break;
			case 3:
				b3s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(p1s, p2s, 0, p4s);
				switch (next_p)
				{
				case 1:
					if (b2s == wait)
					{
						n++; l++;
					}
					else
					{
						n++; j++;
					}
					break;
				case 2:
					if (b1s == wait)
					{
						n++; l++;
					}
					else
					{
						n++; i++;
					}
					break;
				case 4:
					if (b1s == wait)
					{
						n++; j++;
					}
					else
					{
						n++; i++;
					}
					break;
				}
				break;
			case 4:
				b4s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(p1s, p2s, p3s, 0);
				switch (next_p)
				{
				case 1:
					if (b3s == wait)
					{
						l++; j++;
					}
					else
					{
						l++; n++;
					}
					break;
				case 2:
					if (b3s == wait)
					{
						l++; i++;
					}
					else
					{
						l++; n++;
					}
					break;
				case 3:
					if (b1s == wait)
					{
						l++; j++;
					}
					else
					{
						l++; i++;
					}
					break;
				}
				break;
			}
		}
		else
		{
		time_wait++;
		time_process++;
		int next_c = 0;
		int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
		switch (state)
		{
		case 1:
			next_c = comparate_states(0, b2s, b3s, b4s);
			switch (next_c)
			{
			case 2:
				if (b3s == wait)
					l++;
				else
					n++;
				break;
			case 3:
				if (b2s == wait)
					l++;
				else
					j++;
				break;
			case 4:
				if (b3s == wait)
					j++;
				else
					n++;
				break;
			}
			break;
		case 2:
			next_c = comparate_states(b1s, 0, b3s, b4s);
			switch (next_c)
			{
			case 1:
				if (b3s == wait)
					l++;
				else
					n++;
				break;
			case 3:
				if (b1s == wait)
					l++;
				else
					i++;
				break;
			case 4:
				if (b3s == wait)
					i++;
				else
					n++;
				break;
			}
			break;
		case 3:
			next_c = comparate_states(b1s, b2s, 0, b4s);
			switch (next_c)
			{
			case 1:
				if (b2s == wait)
					l++;
				else
					j++;
				break;
			case 2:
				if (b1s == wait)
					l++;
				else
					i++;
				break;
			case 4:
				if (b1s == wait)
					j++;
				else
					i++;
				break;
			}
			break;
		case 4:
			next_c = comparate_states(b1s, b2s, b3s, 0);
			switch (next_c)
			{
			case 1:
				if (b3s == wait)
					j++;
				else
					n++;
				break;
			case 2:
				if (b3s == wait)
					i++;
				else
					n++;
				break;
			case 3:
				if (b1s == wait)
					j++;
				else
					i++;
				break;
			}
			break;
		}
		}
		break;
	case 4: // есть новый
		if (time_process == m)
		{
			time_process = 0;
			int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
			int state_p = comparate_states(p1s, p2s, p3s, p4s);
			int state_m = comparate_states(m1s, m2s, m3s, m4s);
			int next_p = 0;
			switch (state)
			{
			case 1:
				b1s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(0, p2s, p3s, p4s);
				switch (next_p)
				{
				case 2:
					if (b3s == wait)
					{
						q.push(4, l);
						b4s = wait;
					}
					else
					{
						q.push(3, n);
						b3s = wait;
					}
					break;
				case 3:
					if (b2s == wait)
					{
						q.push(4, l);
						b4s = wait;
					}
					else
					{
						q.push(2, j);
						b2s = wait;
					}
					break;
				case 4:
					if (b3s == wait)
					{
						q.push(2, j);
						b2s = wait;
					}
					else
					{
						q.push(3, n);
						b3s = wait;
					}
					break;
				}
				break;
			case 2:
				b2s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(p1s, 0, p3s, p4s);
				switch (next_p)
				{
				case 1:
					if (b3s == wait)
					{
						q.push(4, l);
						b4s = wait;
					}
					else
					{
						q.push(3, n);
						b3s = wait;
					}
					break;
				case 3:
					if (b1s == wait)
					{
						q.push(4, l);
						b4s = wait;
					}
					else
					{
						q.push(1, i);
						b1s = wait;
					}
					break;
				case 4:
					if (b3s == wait)
					{
						q.push(1, i);
						b1s = wait;
					}
					else
					{
						q.push(3, n);
						b3s = wait;
					}
					break;
				}
				break;
			case 3:
				b3s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(p1s, p2s, 0, p4s);
				switch (next_p)
				{
				case 1:
					if (b2s == wait)
					{
						q.push(4, l);
						b4s = wait;
					}
					else
					{
						q.push(2, j);
						b2s = wait;
					}
					break;
				case 2:
					if (b1s == wait)
					{
						q.push(4, l);
						b4s = wait;
					}
					else
					{
						q.push(1, i);
						b1s = wait;
					}
					break;
				case 4:
					if (b1s == wait)
					{
						q.push(2, j);
						b2s = wait;
					}
					else
					{
						q.push(1, i);
						b1s = wait;
					}
					break;
				}
				break;
			case 4:
				b4s = register_work;
				off_processors(state_p);
				off_memory(state_m);
				start_next_queue(p1, p2, p3, p4);
				next_p = comparate_states(p1s, p2s, p3s, 0);
				switch (next_p)
				{
				case 1:
					if (b3s == wait)
					{
						q.push(2, j);
						b2s = wait;
					}
					else
					{
						q.push(3, n);
						b3s = wait;
					}
					break;
				case 2:
					if (b3s == wait)
					{
						q.push(1, i);
						b1s = wait;
					}
					else
					{
						q.push(3, n);
						b3s = wait;
					}
					break;
				case 3:
					if (b1s == wait)
					{
						q.push(2, j);
						b2s = wait;
					}
					else
					{
						q.push(1, i);
						b1s = wait;
					}
					break;
				}
				break;
			}
		}
		else
		{
		time_wait++;
		time_process++;
		int next_c = 0;
		int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
		switch (state)
		{
		case 1:
			next_c = comparate_states(0, b2s, b3s, b4s);
			switch (next_c)
			{
			case 2:
				if (b3s == wait)
				{
					q.push(4, l);
					b4s = wait;
				}
				else
				{
					q.push(3, n);
					b3s = wait;
				}
				break;
			case 3:
				if (b2s == wait)
				{
					q.push(4, l);
					b4s = wait;
				}
				else
				{
					q.push(2, j);
					b2s = wait;
				}
				break;
			case 4:
				if (b3s == wait)
				{
					q.push(2, j);
					b2s = wait;
				}
				else
				{
					q.push(3, n);
					b3s = wait;
				}
				break;
			}
			break;
		case 2:
			next_c = comparate_states(b1s, 0, b3s, b4s);
			switch (next_c)
			{
			case 1:
				if (b3s == wait)
				{
					q.push(4, l);
					b4s = wait;
				}
				else
				{
					q.push(3, n);
					b3s = wait;
				}
				break;
			case 3:
				if (b1s == wait)
				{
					q.push(4, l);
					b4s = wait;
				}
				else
				{
					q.push(1, i);
					b1s = wait;
				}
				break;
			case 4:
				if (b3s == wait)
				{
					q.push(1, i);
					b1s = wait;
				}
				else
				{
					q.push(3, n);
					b3s = wait;
				}
				break;
			}
			break;
		case 3:
			next_c = comparate_states(b1s, b2s, 0, b4s);
			switch (next_c)
			{
			case 1:
				if (b2s == wait)
				{
					q.push(4, l);
					b4s = wait;
				}
				else
				{
					q.push(2, j);
					b2s = wait;
				}
				break;
			case 2:
				if (b1s == wait)
				{
					q.push(4, l);
					b4s = wait;
				}
				else
				{
					q.push(1, i);
					b1s = wait;
				}
				break;
			case 4:
				if (b1s == wait)
				{
					q.push(2, j);
					b2s = wait;
				}
				else
				{
					q.push(1, i);
					b1s = wait;
				}
				break;
			}
			break;
		case 4:
			next_c = comparate_states(b1s, b2s, b3s, 0);
			switch (next_c)
			{
			case 1:
				if (b3s == wait)
				{
					q.push(2, j);
					b2s = wait;
				}
				else
				{
					q.push(3, n);
					b3s = wait;
				}
				break;
			case 2:
				if (b3s == wait)
				{
					q.push(1, i);
					b1s = wait;
				}
				else
				{
					q.push(3, n);
					b3s = wait;
				}
				break;
			case 3:
				if (b1s == wait)
				{
					q.push(2, j);
					b2s = wait;
				}
				else
				{
					q.push(1, i);
					b1s = wait;
				}
				break;
			}
		}
		}
		break;
	}
}

void set_states_three_w(int c1, int c2, int c3, int c4,
	Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m)
{
	if (time_process == m)
	{
		time_process = 0;
		int state = comparate_states(b1s, b2s, b3s, b4s); // находим process
		int state_p = comparate_states(p1s, p2s, p3s, p4s);
		int state_m = comparate_states(m1s, m2s, m3s, m4s);
		switch (state)
		{
		case 1:
			b1s = register_work;
			off_processors(state_p);
			off_memory(state_m);
			i++;
			start_next_queue(p1, p2, p3, p4);
			break;
		case 2:
			b2s = register_work;
			off_processors(state_p);
			off_memory(state_m);
			j++;
			start_next_queue(p1, p2, p3, p4);
			break;
		case 3:
			b3s = register_work;
			off_processors(state_p);
			off_memory(state_m);
			n++;
			start_next_queue(p1, p2, p3, p4);
			break;
		case 4:
			b4s = register_work;
			off_processors(state_p);
			off_memory(state_m);
			l++;
			start_next_queue(p1, p2, p3, p4);
			break;
		}
	}
	else
	{
		time_wait++;
		time_process++;
	}
}

void bus_work(Processor& p1, Processor& p2, Processor& p3, Processor& p4, int m)
{
	while ((i < p1.size_list) || (j < p2.size_list) || (n < p3.size_list) || (l < p4.size_list))
	{
		print_bus();
		cp1 = 0; cp2 = 0; cp3 = 0; cp4 = 0;
		if(i < p1.size_list)
			cp1 = get_command(p1, i);
		if (j < p2.size_list)
			cp2 = get_command(p2, j);
		if (n < p3.size_list)
			cp3 = get_command(p3, n);
		if (l < p4.size_list)
			cp4 = get_command(p4, l);
		status = check_bus_status(); // текущее состояние
		switch (status)
		{
		case all_register:
			set_states_all_reg(cp1, cp2, cp3, cp4, p1, p2, p3, p4);
			clk++;
			break;
		case one_process:
			set_states_one_p(cp1, cp2, cp3, cp4, p1, p2, p3, p4, m);
			clk++;
			break;
		case one_wait:
			set_states_one_w(cp1, cp2, cp3, cp4, p1, p2, p3, p4, m);
			clk++;
			break;
		case two_wait:
			set_states_two_w(cp1, cp2, cp3, cp4, p1, p2, p4, p4, m);
			clk++;
			break;
		case three_wait:
			set_states_three_w(cp1, cp2, cp3, cp4, p1, p2, p3, p4, m);
			clk++;
			break;
		}
	}
}