//pch.h :

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

enum command_numbers {
	HALT = 0,
	SYSCALL = 1,
	ADD = 2,
	ADDI = 3,
	SUB = 4,
	SUBI = 5,
	MUL = 6,
	MULI = 7,
	DIV = 8,
	DIVI = 9,
	LC = 12,
	SHL = 13,
	SHLI = 14,
	SHR = 15,
	SHRI = 16,
	AND = 17,
	ANDI = 18,
	OR = 19,
	ORI = 20,
	XOR = 21,
	XORI = 22,
	NOT = 23,
	MOV = 24,
	ADDD = 32,
	SUBD = 33,
	MULD = 34,
	DIVD = 35,
	ITOD = 36,
	DTOI = 37,
	PUSH = 38,
	POP = 39,
	CALL = 40,
	CALLI = 41,
	RET = 42,
	CMP = 43,
	CMPI = 44,
	CMPD = 45,
	JMP = 46,
	JNE = 47,
	JEQ = 48,
	JLE = 49,
	JL = 50,
	JGE = 51,
	JG = 52,
	LOAD = 64,
	STORE = 65,
	LOAD2 = 66,
	STORE2 = 67,
	LOADR = 68,
	LOADR2 = 69,
	STORER = 70,
	STORER2 = 71
};

enum syscalls {
	EXIT = 0,
	OPEN = 1,
	READ = 2,
	WRITE = 3,
	CLOSE = 4,
	ALLOC = 5,
	FREE = 6,
	SCANINT = 100,
	SCANDOUBLE = 101,
	PRINTINT = 102,
	PRINTDOUBLE = 103,
	GETCHAR = 104,
	PUTCHAR = 105
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

const int Number_Type[] = {
	RI,         //    0      halt
	RI,         //    1      syscall
	RR,         //    2      add
	RI,         //    3      addi
	RR,         //    4      sub
	RI,         //    5      subi
	RR,         //    6      mul
	RI,         //    7      muli
	RR,         //    8      div
	RI,         //    9      divi
	ERR,         //    10      
	ERR,         //    11      
	RI,         //    12      lc
	RR,         //    13      shl
	RI,         //    14      shli
	RR,         //    15      shr
	RI,         //    16      shri
	RR,         //    17      and
	RI,         //    18      andi
	RR,         //    19      or
	RI,         //    20      ori
	RR,         //    21      xor
	RI,         //    22      xori
	RI,         //    23      not
	RR,         //    24      mov
	ERR,         //    25      
	ERR,         //    26      
	ERR,         //    27      
	ERR,         //    28      
	ERR,         //    29      
	ERR,         //    30      
	ERR,         //    31      
	RR,         //    32      addd
	RR,         //    33      subd
	RR,         //    34      muld
	RR,         //    35      divd
	RR,         //    36      itod
	RR,         //    37      dtoi
	RI,         //    38      push
	RI,         //    39      pop
	RR,         //    40      call
	RM,         //    41      calli
	RI,         //    42      ret
	RR,         //    43      cmp
	RI,         //    44      cmpi
	RR,         //    45      cmpd
	RM,         //    46      jmp
	RM,         //    47      jne
	RM,         //    48      jeq
	RM,         //    49      jle
	RM,         //    50      jl
	RM,         //    51      jge
	RM,         //    52      jg
	ERR,         //    53      
	ERR,         //    54      
	ERR,         //    55      
	ERR,         //    56      
	ERR,         //    57      
	ERR,         //    58      
	ERR,         //    59      
	ERR,         //    60      
	ERR,         //    61      
	ERR,         //    62      
	ERR,         //    63      
	RM,         //    64      load
	RM,         //    65      store
	RM,         //    66      load2
	RM,         //    67      store2
	RR,         //    68      loadr
	RR,         //    69      loadr2
	RR,         //    70      storer
	RR,         //    71      storer2
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

class operation
{
public:
	int  type = 0, number = 0, A = 0, B = 0, C = 0;
	unsigned int op = 0;
	operation(int c = 0) {
		if (c == 0)
			return;
		op = c;
		number = op >> 24;
		type = Number_Type[number];
		A = (op << 8) >> 28;
		switch (type) {
		case RM:
			B = (op << 12) >> 12;
			break;
		case RI:
			B = (op << 12) >> 12;
			if ((B >> 19) > 0)
				B = B & (~(1 << 19)) | (1 << 31);
			break;
		case RR:
			B = (op << 12) >> 24;
			C = (op << 16) >> 16;
			if ((C >> 15) > 0)
				C = C & (~(1 << 15)) | (1 << 31);
			break;
		default:
			break;
		}
		return;
	}


	void operator ()(class machine M);
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


	int init(asm_code & Ac);
};


class machine {
public:
	int memory[1 << 20] = {};
	int r[16] = {};
	int flags = 0;
	bool endf = false;


	int init(class machine_code &Mc);
	int run();
};