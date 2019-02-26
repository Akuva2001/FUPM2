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
using namespace std;

class machine;
class machine_code;
class asm_code;

enum h_codes
{
	OK = 0,
	DIVIDE_BY_ZERO = 1,
	MISSED_FUNCTION = 2,
	SINTATIC_MISTAKE = 3,
	UNKNOWN_ERROR = 4,
	UNKNOWN_COMMAND = 5,
};

enum operation_type
{
	RR = 1,			//8 is code, 4 is register to, 20 is memory adress
	RM = 2,			//8 is code, 4 is register to, 4 is register from, 16 is modifier 
	RI = 3,			//8 is code, 4 is register to, 20 is modifier
	Label = -1,
	End = -2,
	ERR = 0
};

map <string, int> Is_REGISTR = {
	{"r0", 0},
	{"r1", 1},
	{"r2", 2},
	{"r3", 3},
	{"r4", 4},
	{"r5", 5},
	{"r6", 6},
	{"r7", 7},
	{"r8", 8},
	{"r9", 9},
	{"r10", 10},
	{"r11", 11},
	{"r12", 12},
	{"r13", 13},
	{"r14", 14},
	{"r15", 15}
};

static int as_register(const string& st) {
	map <string, int> ::iterator it = Is_REGISTR.find(st);
	if (it == Is_REGISTR.end())
		return -1;
	return it->second;
}

map <string, pair<int, int> > Name_Number_Type = {
	{"halt", {0, RI}},
	{"syscall", {1, RI}},
	{"add", {2, RR}},
	{"addi", {3, RI}},
	{"sub", {4, RR}},
	{"subi", {5, RI}},
	{"mul", {6, RR}},
	{"muli", {7, RI}},
	{"div", {8, RR}},
	{"divi", {9, RI}},
	//{"", {10, -1}},
	//{"", {11, -1}},
	{"lc", {12, RI}},
	{"shl", {13, RR}},
	{"shli", {14, RI}},
	{"shr", {15, RR}},
	{"shri", {16, RI}},
	{"and", {17, RR}},
	{"andi", {18, RI}},
	{"or", {19, RR}},
	{"ori", {20, RI}},
	{"xor", {21, RR}},
	{"xori", {22, RI}},
	{"not", {23, RI}},
	{"mov", {24, RR}},
	//{"", {25, -1}},
	//{"", {26, -1}},
	//{"", {27, -1}},
	//{"", {28, -1}},
	//{"", {29, -1}},
	//{"", {30, -1}},
	//{"", {31, -1}},
	{"addd", {32, RR}},
	{"subd", {33, RR}},
	{"muld", {34, RR}},
	{"divd", {35, RR}},
	{"itod", {36, RR}},
	{"dtoi", {37, RR}},
	{"push", {38, RI}},
	{"pop", {39, RI}},
	{"call", {40, RR}},
	{"calli", {41, RM}},
	{"ret", {42, RI}},
	{"cmp", {43, RR}},
	{"cmpi", {44, RI}},
	{"cmpd", {45, RR}},
	{"jmp", {46, RM}},
	{"jne", {47, RM}},
	{"jeq", {48, RM}},
	{"jle", {49, RM}},
	{"jl", {50, RM}},
	{"jge", {51, RM}},
	{"jg", {52, RM}},
	//{"", {53, -1}},
	//{"", {54, -1}},
	//{"", {55, -1}},
	//{"", {56, -1}},
	//{"", {57, -1}},
	//{"", {58, -1}},
	//{"", {59, -1}},
	//{"", {60, -1}},
	//{"", {61, -1}},
	//{"", {62, -1}},
	//{"", {63, -1}},
	{"load", {64, RM}},
	{"store", {65, RM}},
	{"load2", {66, RM}},
	{"store2", {67, RM}},
	{"loadr", {68, RR}},
	{"loadr2", {69, RR}},
	{"storer", {70, RR}},
	{"storer2", {71, RR}},

	{"end", {72, End}},
};
/*
void init_search_tree() {
	Name_Number_Type.insert(pair<string, pair<int, int> >("halt", pair<int, int>(0, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("syscall", pair<int, int>(1, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("add", pair<int, int>(2, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("addi", pair<int, int>(3, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("sub", pair<int, int>(4, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("subi", pair<int, int>(5, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("mul", pair<int, int>(6, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("muli", pair<int, int>(7, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("div", pair<int, int>(8, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("divi", pair<int, int>(9, RI)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(10, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(11, -1)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("lc", pair<int, int>(12, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("shl", pair<int, int>(13, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("shli", pair<int, int>(14, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("shr", pair<int, int>(15, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("shri", pair<int, int>(16, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("and", pair<int, int>(17, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("andi", pair<int, int>(18, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("or", pair<int, int>(19, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("ori", pair<int, int>(20, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("xor", pair<int, int>(21, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("xori", pair<int, int>(22, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("not", pair<int, int>(23, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("mov", pair<int, int>(24, RR)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(25, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(26, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(27, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(28, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(29, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(30, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(31, -1)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("addd", pair<int, int>(32, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("subd", pair<int, int>(33, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("muld", pair<int, int>(34, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("divd", pair<int, int>(35, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("itod", pair<int, int>(36, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("dtoi", pair<int, int>(37, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("push", pair<int, int>(38, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("pop", pair<int, int>(39, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("call", pair<int, int>(40, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("calli", pair<int, int>(41, RM)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("ret", pair<int, int>(42, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("cmp", pair<int, int>(43, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("cmpi", pair<int, int>(44, RI)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("cmpd", pair<int, int>(45, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("jmp", pair<int, int>(46, RM)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("jne", pair<int, int>(47, RM)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("jeq", pair<int, int>(48, RM)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("jle", pair<int, int>(49, RM)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("jl", pair<int, int>(50, RM)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("jge", pair<int, int>(51, RM)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("jg", pair<int, int>(52, RM)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(53, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(54, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(55, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(56, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(57, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(58, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(59, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(60, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(61, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(62, -1)));
	//Name_Number_Type.insert(pair<string, pair<int, int> >("", pair<int, int>(63, -1)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("load", pair<int, int>(64, RM)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("store", pair<int, int>(65, RM)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("load2", pair<int, int>(66, RM)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("store2", pair<int, int>(67, RM)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("loadr", pair<int, int>(68, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("loadr2", pair<int, int>(69, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("storer", pair<int, int>(70, RR)));
	Name_Number_Type.insert(pair<string, pair<int, int> >("storer2", pair<int, int>(71, RR)));

	///////
	Name_Number_Type.insert(pair<string, pair<int, int> >("end", pair<int, int>(72, End)));
}
*/

static void next_word(const string& st, string & word, string::iterator &it) {
	for (; it < st.end() && (*it == ' ' || *it == '\t'); it++);
	for (; it < st.end() && *it != ' ' && *it != '\t' && *it != ';' && *it != '\n'; it++) {
		word.push_back(*it);
	}
	return;
}

static int as_int(const string& st) {
	int x = 0;
	size_t i = 0;
	size_t l = st.length();
	if (l == 0)
		return 0;
	if (st[0] == '-')
		i++;
	for (; i < l; i++)
		x = x * 10 + st[i] - '0';
	if (st[0] == '-')
		x *= -1;
	return x;
}

class machine {
public:
	int memory[1 << 20];
	int reg[16];
	int init(machine_code &) {

	}
	int flop() {

	}
	int end() {

	}
};

class asm_code {
public:
	vector<string> vt;

	friend istream& operator >> (istream& in, asm_code& p) {
		for (; in;) {
			string cpy;
			getline(in, cpy);
			p.vt.push_back(cpy);
			string::iterator it = cpy.begin();
			string A;
			next_word(cpy, A, it);
			if (A == "end")
				return in;
		}
		return in;
	}
	friend ifstream& operator >> (ifstream& in, asm_code& p) {
		for (; in;) {
			string cpy;
			getline(in, cpy);
			p.vt.push_back(cpy);
			string::iterator it = cpy.begin();
			string A;
			next_word(cpy, A, it);
			if (A == "end")
				return in;
		}
		return in;
	}
	friend ostream& operator << (ostream& out, asm_code& p) {
		const size_t size = p.vt.size();
		for (int i = 0; i < size; i++) {
#ifdef DEBUG
			cout << p.vt[i] << "####\n";
#else
			cout << p.vt[i] << '\n';
#endif // DEBUG

		}
		return out;
	}
	friend ofstream& operator << (ofstream& out, asm_code& p) {
		const size_t size = p.vt.size();
		for (int i = 0; i < size; i++)
			cout << p.vt[i] << '\n';
		return out;
	}
};

/*
static void pass_spaces(const string &st, string::iterator &it) {
	for (; it < st.end() && (*it == ' ' || *it == '\t'); it++);
	return;
}
*/

class operation
{
public:
	int  type = 0, number = 0, A = 0, B = 0, C = 0;
	bool correct_flag = false;
	unsigned int op = 0;
};

class machine_code {
public:
	vector<int> vt;
	char name[16] = "ThisIsFUPM2Exec";
	int code = 0;
	int constants = 0;
	int data = 0;
	int start_adress = 0;
	int stack_adress = (1 << 20) - 1;

	int init(asm_code & Ac) {
		map<string, int> labels;
		size_t size = Ac.vt.size();

		//The first pass
		int k = 0;
		for (int i = 0; i < size; i++) {
			size_t t = Ac.vt[i].find(';');
			if (t != string::npos) {
				Ac.vt[i].erase(Ac.vt[i].begin() + t, Ac.vt[i].end());
			}
			string word;
			string::iterator it = Ac.vt[i].begin();
			t = Ac.vt[i].find(':');
			if (t != string::npos) {
				for (; it < Ac.vt[i].end() && it < Ac.vt[i].begin() + t && (*it == ' ' || *it == '\t'); it++);
				for (; it < Ac.vt[i].end() && it < Ac.vt[i].begin() + t && *it != ' ' && *it != '\t' && *it != ':' && *it != '\n'; it++) {
					word.push_back(*it);
				}
				labels.insert(pair<string, int>(word, k));
				Ac.vt[i].erase(Ac.vt[i].begin(), Ac.vt[i].begin() + t + 1);
			}
			it = Ac.vt[i].begin();
			word.clear();
			next_word(Ac.vt[i], word, it);
			if (word.size() != 0) {
				k++;
			}
		}
		//################DEBUG
#ifdef DEBUG
		cout << "\n###########################################\nafter first pass\n###########################################\n";
		cout << Ac;
		cout << "\n###########################################\n###########################################\n";
		cout << "LABELS:\n";
		for (auto it = labels.begin(); it != labels.end(); it++) {
			cout << "###" << it->first << "###" << it->second << "\n";
		}
		cout << "\n###########################################\nsecond pass\n###########################################\n";
#endif // DEBUG
		//################DEBUG
		//The second pass
		k = 0;
		for (int i = 0; i < size; i++) {
			string cpy = Ac.vt[i];
			string name, A, B, C;
			string::iterator it = cpy.begin();
			next_word(cpy, name, it);
			if (name.size() != 0) {
				
				next_word(cpy, A, it);
				next_word(cpy, B, it);
				next_word(cpy, C, it);
				//###############DEBUG
#ifdef DEBUG
				cout << "name = #" << name << "#, A = #" << A << "#, B = #" << B << "#, C = #" << C << "#\n";
#endif // DEBUG
				//###############DEBUG
				map <string, pair<int, int> > ::iterator p_it = Name_Number_Type.find(name);
				if (p_it == Name_Number_Type.end()) {
					return UNKNOWN_COMMAND;
				}
				else {
					operation op;
					op.type = (p_it->second).second;
					op.number = (p_it->second).first;
					op.correct_flag = true;
					if ((p_it->second).second == RM) {
						int reg1 = as_register(A);
						if (reg1 == -1) {
							map<string, int>::iterator l_it = labels.find(A);
							if (l_it != labels.end()) {
								op.A = 0;
								op.B = l_it->second;
							}
							else {
								op.correct_flag = false;
								return MISSED_FUNCTION;
							}
						}
						else {
							op.A = reg1;
							op.B = as_int(B);
						}
						op.op = (op.number << 24) + (op.A << 20) + op.B;
						(this->vt).push_back(op.op);
						k++;
					}
					else if ((p_it->second).second == RI) {
						op.A = as_register(A);
						op.B = as_int(B);
						if (op.B < 0)
							op.B += (1 << 31) + (1 << 19);
						op.op = (op.number << 24) + (op.A << 20) + op.B;
						(this->vt).push_back(op.op);
						k++;
					}
					else if ((p_it->second).second == RR) {
						op.A = as_register(A);
						op.B = as_register(B);
						op.C = as_int(C);
						if (op.C < 0)
							op.C += (1 << 31) + (1 << 15);
						op.op = (op.number << 24) + (op.A << 20) + (op.B << 16) + op.C;
						(this->vt).push_back(op.op);
						k++;
					}
					else if ((p_it->second).second == End) {
						if (A[0] <= '9' && A[0] >= '0') {
							op.A = as_int(A);
						}
						else {
							map<string, int>::iterator l_it = labels.find(A);
							if (l_it != labels.end()) {
								op.A = l_it->second;
							}
							else {
								op.correct_flag = false;
								return MISSED_FUNCTION;
							}
						}
						this->start_adress = op.A;
						this->code = k;
						return OK;
					}
				}
			}
		}
		return UNKNOWN_ERROR;
	}
	friend ostream& operator << (ostream& out, machine_code& p) {
		out << "name " << p.name << '\n';
		out << "code " << p.code << '\n';
		out << "constants " << p.constants << '\n';
		out << "data " << p.data << '\n';
		out << "start_adress " << p.start_adress << '\n';
		out << "stack_adress " << p.stack_adress << '\n';
		const size_t size = p.vt.size();
		for (int i = 0; i < size; i++)
			out << p.vt[i] << '\n';
		return out;
	}
	friend ofstream& operator << (ofstream& out, machine_code& p) {
		out << "name " << p.name << '\n';
		out << "code " << p.code << '\n';
		out << "constants " << p.constants << '\n';
		out << "data " << p.data << '\n';
		out << "start_adress " << p.start_adress << '\n';
		out << "stack_adress " << p.stack_adress << '\n';
		const size_t size = p.vt.size();
		for (int i = 0; i < size; i++)
			out << p.vt[i] << '\n';
		return out;
	}
};


int main()
{
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
