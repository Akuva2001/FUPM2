// FUPM2.cpp :

//#define DEBUG


#include "pch.h"

#include <vector>
#include <string>
#include <map>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <bitset>
#include <climits>
using namespace std;


int main()
{
	/*
	ifstream in;
	in.open("input.txt");
	ofstream out;
	out.open("output.txt");
	asm_code Asm;
	cin >> Asm;
	cout << "\n###########################################\n###########################################\n";
	cout << Asm;
	cout << "\n###########################################\n###########################################\n";
	machine_code Mach;
	cout<<"result "<<Mach.init(Asm)<<'\n';
	out << Mach;
	cout << Mach;
	in.close();
	out.close();
	return 0;
	*/
	for (;;) {
		/*cin>>*((double*)&a[2]);
		cout<<*((double*)&a[2])<<"\n";
		for (int i=0; i<5; i++)
		{
			cout<<bitset<sizeof(int) * CHAR_BIT>(a[i])<<"\n";
			a[i] = 0;
		}
		double x;
		scanf("%Lf", &x);
		//cin>>x;
		cout<<x<<'\n';
		cout<<bitset<sizeof(double) * CHAR_BIT>(x)<<"\n";
		int k, l;
		k = x<<*/

		double p;
		//cin>>p;
		scanf_s("%lf", &p);
		printf("%lf\n", p);
		int *k = (int*)((void *)&p);
		printf("%lf\n", *((double *)k));
		k--;
		cout << bitset<sizeof(int) * CHAR_BIT>(*k) << "\n";
		for (int i = 0; i < 32; i++) {
			cout << (int)(((*k)&(1 << i)) >> i);
		}
		k++;
		cout << '\n';
		cout << '\n';
		cout << bitset<sizeof(int) * CHAR_BIT>(*k) << "\n";
		for (int i = 0; i < 32; i++) {
			cout << (int)(((*k)&(1 << i)) >> i);
		}
		k++;
		cout << '\n';
		cout << '\n';
		cout << bitset<sizeof(int) * CHAR_BIT>(*k) << "\n";
		for (int i = 0; i < 32; i++) {
			cout << (int)(((*k)&(1 << i)) >> i);
		}
		k++;
		cout << '\n';
		cout << '\n';
		cout << bitset<sizeof(int) * CHAR_BIT>(*k) << "\n";
		for (int i = 0; i < 32; i++) {
			cout << (int)(((*k)&(1 << i)) >> i);
		}
		cout << '\n';
		k--;
		printf("%lf\n", *((double *)k));
		k--;
		printf("%lf\n############\n", *((double *)k));
	}
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
