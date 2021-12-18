//#include "memoryslot.h"
//
//MemorySlot::MemorySlot(int c1, int c2, int c3, int c4)
//{
//	this->data[0] = c1;
//	this->data[1] = c2;
//	this->data[2] = c3;
//	this->data[3] = c4;
//}
//
//MemorySlot::~MemorySlot()
//{
//	for (int i = 0; i < 4; i++)
//	{
//		this->data[i] = 0;
//	}
//}
//
//void MemorySlot::print_appeals()
//{
//	for (int i = 0; i < 4; i++)
//	{
//		switch (i % 4)
//		{
//		case 0:
//			cout << "Кол-во обращений 1 процессора: " << this->data[i] << endl;
//			break;
//		case 1:
//			cout << "Кол-во обращений 2 процессора: " << this->data[i] << endl;
//			break;
//		case 2:
//			cout << "Кол-во обращений 3 процессора: " << this->data[i] << endl;
//			break;
//		case 3:
//			cout << "Кол-во обращений 4 процессора: " << this->data[i] << endl;
//			break;
//		}
//	}
//}