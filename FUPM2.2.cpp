#include "pch.h"
//pch.h :
//#define DEBUG
#define _CRT_SECURE_NO_WARNINGS

//#define RW_To_Files

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
#include <cmath>
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
	RR = 1,			//8 is code, 4 is register to, 4 is register from, 16 is modifier 
	RM = 2,			//8 is code, 4 is register to, 20 is memory adress
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
	{"load", {64, RR}},
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
	RR,         //    64      load
	RM,         //    65      store
	RM,         //    66      load2
	RM,         //    67      store2
	RR,         //    68      loadr
	RR,         //    69      loadr2
	RR,         //    70      storer
	RR,         //    71      storer2
};

const string Number_Name[] = {
	"halt",		//	0		RI
	"syscall",	//	1		RI
	"add",		//	2		RR
	"addi",		//	3		RI
	"sub",		//	4		RR
	"subi",		//	5		RI
	"mul",		//	6		RR
	"muli",		//	7		RI
	"div",		//	8		RR
	"divi",		//	9		RI
	"",	//	10		ERR
	"",	//	11		ERR
	"lc",		//	12		RI
	"shl",		//	13		RR
	"shli",		//	14		RI
	"shr",		//	15		RR
	"shri",		//	16		RI
	"and",		//	17		RR
	"andi",		//	18		RI
	"or",		//	19		RR
	"ori",		//	20		RI
	"xor",		//	21		RR
	"xori",		//	22		RI
	"not",		//	23		RI
	"mov",		//	24		RR
	"",	//	25		ERR
	"",	//	26		ERR
	"",	//	27		ERR
	"",	//	28		ERR
	"",	//	29		ERR
	"",	//	30		ERR
	"",	//	31		ERR
	"addd",		//	32		RR
	"subd",		//	33		RR
	"muld",		//	34		RR
	"divd",		//	35		RR
	"itod",		//	36		RR
	"dtoi",		//	37		RR
	"push",		//	38		RI
	"pop",		//	39		RI
	"call",		//	40		RR
	"calli",	//	41		RM
	"ret",		//	42		RI
	"cmp",		//	43		RR
	"cmpi",		//	44		RI
	"cmpd",		//	45		RR
	"jmp",		//	46		RM
	"jne",		//	47		RM
	"jeq",		//	48		RM
	"jle",		//	49		RM
	"jl",		//	50		RM
	"jge",		//	51		RM
	"jg",		//	52		RM
	"",	//	53		ERR
	"",	//	54		ERR
	"",	//	55		ERR
	"",	//	56		ERR
	"",	//	57		ERR
	"",	//	58		ERR
	"",	//	59		ERR
	"",	//	60		ERR
	"",	//	61		ERR
	"",	//	62		ERR
	"",	//	63		ERR
	"load",		//	64		RR
	"store",	//	65		RM
	"load2",	//	66		RM
	"store2",	//	67		RM
	"loadr",	//	68		RR
	"loadr2",	//	69		RR
	"storer",	//	70		RR
	"storer2",	//	71		RR
};

int next_word(const string& st, string & word, string::iterator &it);

int as_int(const string& st) {
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



void pass_spaces(const string &st, string::iterator &it) {
	for (; it < st.end() && (*it == ' ' || *it == '\t'); it++);
	return;
}


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
		for (size_t i = 0; i < size; i++) {
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
		for (size_t i = 0; i < size; i++)
			cout << p.vt[i] << '\n';
		return out;
	}
};

class operation {
public:
	int  type = 0, number = 0, A = 0, B = 0, C = 0;
	unsigned int op = 0;

	friend ostream& operator << (ostream& out, const operation& op) {
		switch (op.type) {
		case RM:
			out << Number_Name[op.number] << " r" << op.A << ' ' << op.B << '\n';
			break;
		case RI:
			int x;
			if ((op.B >> 19) > 0)
				x = op.B&((1 << 20) - 1) | (1 << 31);
			else
				x = op.B;
			out << Number_Name[op.number] << " r" << op.A << ' ' << x << '\n';
			break;
		case RR:
			if ((op.C >> 15) > 0)
				x = op.C&((1 << 16) - 1) | (1 << 31);
			else
				x = op.C;
			out << Number_Name[op.number] << " r" << op.A << " r" << op.B << ' ' << x << '\n';
			break;
		default:
			break;
		}
		return out;
	}
	friend ofstream& operator << (ofstream& out, operation& op) {
		switch (op.type) {
		case RM:
			out << Number_Name[op.number] << " r" << op.A << ' ' << op.B << '\n';
			break;
		case RI:
			int x;
			if ((op.B >> 19) > 0)
				x = op.B&((1 << 20) - 1) | (1 << 31);
			else
				x = op.B;
			out << Number_Name[op.number] << " r" << op.A << ' ' << x << '\n';
			break;
		case RR:
			if ((op.C >> 15) > 0)
				x = op.C&((1 << 16) - 1) | (1 << 31);
			else
				x = op.C;
			out << Number_Name[op.number] << " r" << op.A << " r" << op.B << ' ' << x << '\n';
			break;
		default:
			break;
		}
		return out;
	}
	operation(int c = 0);
	void gen_code();
	void operator ()(class machine & M);
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
		for (size_t i = 0; i < size; i++)
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
		for (size_t i = 0; i < size; i++)
			out << p.vt[i] << '\n';
		return out;
	}


	void disasm(ostream &out);
	void disasm(ofstream &out);
	int init(asm_code & Ac);
};


class machine {
public:
	//int memory[1 << 20] = {};
	int * memory = NULL;
	int r[16] = {};
	int flags = 0;
	bool endf = false;
	machine() {
		memory = new int[1 << 20];
		memset(memory, 0, (1 << 20) * 4);
	}
	~machine() {
		delete[] memory;
	}


	int init(class machine_code &Mc);
	int run();
	int run(ofstream &out);
};

//#############################################################################################################
//#############################################################################################################


// pch.cpp :
//#define DEBUG

//#include "pch.h"

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



int as_register(const string& st) {
	map <string, int> ::iterator it = Is_REGISTR.find(st);
	if (it == Is_REGISTR.end())
		return -1;
	return it->second;
}

int next_word(const string& st, string & word, string::iterator &it) {
	for (; it < st.end() && (*it == ' ' || *it == '\t'); it++);
	for (; it < st.end() && *it != ' ' && *it != '\t' && *it != ';' && *it != '\n'; it++) {
		word.push_back(*it);
	}
	return 0;
}

operation::operation(int c) {
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
		B = (op << 12) >> 28;
		C = (op << 16) >> 16;
		if ((C >> 15) > 0)
			C = C & (~(1 << 15)) | (1 << 31);
		break;
	default:
		break;
	}
	return;
}

void operation::gen_code() {
	switch (type) {
	case RM:
		op = (number << 24) + (A << 20) + B;
		break;
	case RI:
		if (B < 0)
			B += (1 << 31) + (1 << 19);
		op = (number << 24) + (A << 20) + B;
		break;
	case RR:
		if (C < 0)
			C += (1 << 31) + (1 << 15);
		op = (number << 24) + (A << 20) + (B << 16) + C;
		break;
	default:
		break;
	}
	return;
}

void operation::operator ()(machine & M) {
	switch (number) {
	case HALT:
		M.endf = true;
		break;
	case SYSCALL:
		switch (B) {
		case EXIT:
			M.endf = true;
			break;
		case OPEN:
		case READ:
		case WRITE:
		case CLOSE:
		case ALLOC:
		case FREE:
			break;
		case SCANINT:
			scanf("%d", &M.r[A]);
			break;
		case SCANDOUBLE:
			scanf("%lf", (double*)&M.r[A]);
			break;
		case PRINTINT:
			printf("%d", M.r[A]);
			break;
		case PRINTDOUBLE:
			printf("%lf", *(double *)&M.r[A]);
			break;
		case GETCHAR:
			M.r[A] = getchar();
			break;
		case PUTCHAR:
			putchar(M.r[A]);
			break;
		default:
			break;
		}
		//cout << "coco\n";
		M.r[15]++;
		//cout << "M.r[15]" << M.r[15] << '\n';
		break;
	case ADD:
		M.r[A] += M.r[B] + C;
		M.r[15]++;
		break;
	case ADDI:
		M.r[A] += B;
		M.r[15]++;
		break;
	case SUB:
		M.r[A] -= M.r[B] + C;
		M.r[15]++;
		break;
	case SUBI:
		M.r[A] -= B;
		M.r[15]++;
		break;
	case MUL:
		*(long long *)(void *)&M.r[A] = (long long)M.r[A] * ((long long)M.r[B] + C);
		M.r[15]++;
		break;
	case MULI:
		*(long long *)(void *)&M.r[A] = (long long)M.r[A] * ((long long)B);
		M.r[15]++;
		break;
	case DIV:
		long long x;
		x = *(long long *)(void *)&M.r[A];
		M.r[A] = x / (M.r[B] + C);
		M.r[A + 1] = x % (M.r[B] + C);
		M.r[15]++;
		break;
	case DIVI:
		x = *(long long *)(void *)&M.r[A];
		M.r[A] = x / (B);
		M.r[A + 1] = x % (B);
		M.r[15]++;
		break;
	case LC:
		M.r[A] = B;
		M.r[15]++;
		break;
	case SHL:
		M.r[A] <<= M.r[B] + C;
		M.r[15]++;
		break;
	case SHLI:
		M.r[A] <<= B + C;
		M.r[15]++;
		break;
	case SHR:
		M.r[A] >>= M.r[B] + C;
		M.r[15]++;
		break;
	case SHRI:
		M.r[A] >>= B + C;
		M.r[15]++;
		break;
	case AND:
		M.r[A] &= M.r[B] + C;
		M.r[15]++;
		break;
	case ANDI:
		M.r[A] &= B + C;
		M.r[15]++;
		break;
	case OR:
		M.r[A] |= M.r[B] + C;
		M.r[15]++;
		break;
	case ORI:
		M.r[A] |= B + C;
		M.r[15]++;
		break;
	case XOR:
		M.r[A] ^= M.r[B] + C;
		M.r[15]++;
		break;
	case XORI:
		M.r[A] ^= B + C;
		M.r[15]++;
		break;
	case NOT:
		M.r[A] = ~M.r[A];
		M.r[15]++;
		break;
	case MOV:
		M.r[A] = M.r[B] + C;
		M.r[15]++;
		break;
	case ADDD:
		*(double *)(void *)&M.r[A] += *(double *)(void *)&M.r[B] + C;
		M.r[15]++;
		break;
	case SUBD:
		*(double *)(void *)&M.r[A] -= *(double *)(void *)&M.r[B] + C;
		M.r[15]++;
		break;
	case MULD:
		*(double *)(void *)&M.r[A] *= *(double *)(void *)&M.r[B] + C;
		M.r[15]++;
		break;
	case DIVD:
		*(double *)(void *)&M.r[A] /= *(double *)(void *)&M.r[B] + C;
		M.r[15]++;
		break;
	case ITOD:
		*(double *)(void *)&M.r[A] = (double)M.r[B] + C;
		M.r[15]++;
		break;
	case DTOI:
		M.r[A] = trunc(*(double *)(void *)&M.r[B]);
		M.r[15]++;
		break;
	case PUSH:
		M.r[14]--;
		M.memory[M.r[14]] = M.r[A] + B;
		M.r[15]++;
		break;
	case POP:
		M.r[A] = M.memory[M.r[14]] + B;
		M.r[14]++;
		M.r[15]++;
		break;
	case CALL:
		M.r[14]--;
		M.memory[M.r[14]] = M.r[15] + 1;
		M.r[15] = M.r[B] + C;
		//M.r[A] = M.r[B] + C;
		break;
	case CALLI:
		M.r[14]--;
		M.memory[M.r[14]] = M.r[15] + 1;
		M.r[15] = B;
		//M.r[A] = B;
		break;
	case RET:
		M.r[15] = M.memory[M.r[14]];
		M.r[14] += B + 1;
		break;
	case CMP:		// < 4, <= 3, == 2, >= 1, > 0 //1 - true, 0 - false
		M.flags = 0;
		if (M.r[A] < M.r[B] + C)
			M.flags |= 1 << 4;
		if (M.r[A] <= M.r[B] + C)
			M.flags |= 1 << 3;
		if (M.r[A] == M.r[B] + C)
			M.flags |= 1 << 2;
		if (M.r[A] >= M.r[B] + C)
			M.flags |= 1 << 1;
		if (M.r[A] > M.r[B] + C)
			M.flags |= 1 << 0;
		M.r[15]++;
		break;
	case CMPI:		// < 4, <= 3, == 2, >= 1, > 0 //1 - true, 0 - false
		M.flags = 0;
		if (M.r[A] < B)
			M.flags |= 1 << 4;
		if (M.r[A] <= B)
			M.flags |= 1 << 3;
		if (M.r[A] == B)
			M.flags |= 1 << 2;
		if (M.r[A] >= B)
			M.flags |= 1 << 1;
		if (M.r[A] > B)
			M.flags |= 1 << 0;
		M.r[15]++;
		break;
	case CMPD:		// < 4, <= 3, == 2, >= 1, > 0 //1 - true, 0 - false
		M.flags = 0;
		if (*(double *)(void *)&M.r[A] < *(double *)(void *)&M.r[B] + C)
			M.flags |= 1 << 4;
		if (*(double *)(void *)&M.r[A] <= *(double *)(void *)&M.r[B] + C)
			M.flags |= 1 << 3;
		if (*(double *)(void *)&M.r[A] == *(double *)(void *)&M.r[B] + C)
			M.flags |= 1 << 2;
		if (*(double *)(void *)&M.r[A] >= *(double *)(void *)&M.r[B] + C)
			M.flags |= 1 << 1;
		if (*(double *)(void *)&M.r[A] > *(double *)(void *)&M.r[B] + C)
			M.flags |= 1 << 0;
		M.r[15]++;
		break;
	case JMP:		// < 4, <= 3, == 2, >= 1, > 0 //1 - true, 0 - false
		M.r[15] = B;
		break;
	case JNE:		// < 4, <= 3, == 2, >= 1, > 0 //1 - true, 0 - false
		if ((M.flags&(1 << 2)) == 0)
			M.r[15] = B;
		else
			M.r[15]++;
		break;
	case JEQ:		// < 4, <= 3, == 2, >= 1, > 0 //1 - true, 0 - false
		if ((M.flags&(1 << 2)) != 0)
			M.r[15] = B;
		else
			M.r[15]++;
		break;
	case JLE:		// < 4, <= 3, == 2, >= 1, > 0 //1 - true, 0 - false
		if ((M.flags&(1 << 3)) != 0)
			M.r[15] = B;
		else
			M.r[15]++;
		break;
	case JL:		// < 4, <= 3, == 2, >= 1, > 0 //1 - true, 0 - false
		if ((M.flags&(1 << 4)) != 0)
			M.r[15] = B;
		else
			M.r[15]++;
		break;
	case JGE:		// < 4, <= 3, == 2, >= 1, > 0 //1 - true, 0 - false
		if ((M.flags&(1 << 1)) != 0)
			M.r[15] = B;
		else
			M.r[15]++;
		break;
	case JG:		// < 4, <= 3, == 2, >= 1, > 0 //1 - true, 0 - false
		if ((M.flags&(1 << 0)) != 0)
			M.r[15] = B;
		else
			M.r[15]++;
		break;
	case LOAD:
		M.r[A] = M.memory[B];
		M.r[15]++;
		break;
	case STORE:
		M.memory[B] = M.r[A];
		M.r[15]++;
		break;
	case LOAD2:
		M.r[A] = M.memory[B];
		M.r[A + 1] = M.memory[B + 1];
		M.r[15]++;
		break;
	case STORE2:
		M.memory[B] = M.r[A];
		M.memory[B + 1] = M.r[A + 1];
		M.r[15]++;
		break;
	case LOADR:
		M.r[A] = M.memory[M.r[B] + C];
		M.r[15]++;
		break;
	case LOADR2:
		M.r[A] = M.memory[M.r[B] + C];
		M.r[A + 1] = M.memory[M.r[B] + C + 1];
		M.r[15]++;
		break;
	case STORER:
		M.memory[M.r[B] + C] = M.r[A];
		M.r[15]++;
		break;
	case STORER2:
		M.memory[M.r[B] + C] = M.r[A];
		M.memory[M.r[B] + C + 1] = M.r[A + 1];
		M.r[15]++;
		break;
	default:
		break;
	}
	//cout << "cococo\n";
	return;
}

int machine_code::init(asm_code & Ac) {
	map<string, int> labels;
	size_t size = Ac.vt.size();

	//The first pass
	int k = 0;
	for (size_t i = 0; i < size; i++) {
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
	for (size_t i = 0; i < size; i++) {
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
				switch ((p_it->second).second) {
				case RM:
					int reg1;
					reg1 = as_register(A);
					if (reg1 == -1) {
						map<string, int>::iterator l_it = labels.find(A);
						if (l_it != labels.end()) {
							op.A = 0;
							op.B = l_it->second;
						}
						else {
							return MISSED_FUNCTION;
						}
					}
					else {
						op.A = reg1;
						op.B = as_int(B);
					}
					op.op = (op.number << 24) + (op.A << 20) + op.B;
					(this->vt).push_back(op.op);
#ifdef DEBUG
					cout << "name = " << op.number << ", A = " << op.A << ", B = " << op.B << ", C = " << op.C << "\n";
					cout << "name = " << bitset<8>(op.number) << ", A = " << bitset<4>(op.A) << ", B = " << bitset<20>(op.B) << "\n";
					cout << bitset<32>(op.op) << "\n   ^   ^   ^   ^   ^   ^   ^   ^\n";
					cout << op << "\n\n";
#endif // DEBUG


					k++;
					break;
				case RI:
					op.A = as_register(A);
					if (op.A == -1) {
						op.A = 0;
						op.B = as_int(A);
					}
					else
						op.B = as_int(B);

					if (op.B < 0)
						op.B += (1 << 31) + (1 << 19);
					op.op = (op.number << 24) + (op.A << 20) + op.B;
					(this->vt).push_back(op.op);
#ifdef DEBUG
					cout << "name = " << op.number << ", A = " << op.A << ", B = " << op.B << ", C = " << op.C << "\n";
					cout << "name = " << bitset<8>(op.number) << ", A = " << bitset<4>(op.A) << ", B = " << bitset<20>(op.B) << "\n";
					cout << bitset<32>(op.op) << "\n   ^   ^   ^   ^   ^   ^   ^   ^\n";
					cout << op << "\n\n";
#endif // DEBUG
					k++;
					break;
				case RR:
					op.A = as_register(A);
					op.B = as_register(B);
					op.C = as_int(C);
					if (op.C < 0)
						op.C += (1 << 31) + (1 << 15);
					op.op = (op.number << 24) + (op.A << 20) + (op.B << 16) + op.C;
					(this->vt).push_back(op.op);
#ifdef DEBUG
					cout << "name = " << op.number << ", A = " << op.A << ", B = " << op.B << ", C = " << op.C << "\n";
					cout << "name = " << bitset<8>(op.number) << ", A = " << bitset<4>(op.A) << ", B = " << bitset<4>(op.B) << ", C = " << bitset<16>(op.C) << "\n";
					cout << bitset<32>(op.op) << "\n   ^   ^   ^   ^   ^   ^   ^   ^\n";
					cout << op << "\n\n";
#endif // DEBUG
					k++;
					break;
				case End:
					if (A[0] <= '9' && A[0] >= '0') {
						op.A = as_int(A);
					}
					else {
						map<string, int>::iterator l_it = labels.find(A);
						if (l_it != labels.end()) {
							op.A = l_it->second;
						}
						else {
							return MISSED_FUNCTION;
						}
					}
					this->start_adress = op.A;
					this->code = k;
					return OK;
					break;
				default:
					break;
				}
			}
		}
	}
	return UNKNOWN_ERROR;
}

void machine_code::disasm(ostream &out) {
	for (auto it = vt.begin(); it != vt.end(); it++) {
		operation op = operation(*it);
		out << op;
	}
	out << "end " << start_adress;
	return;
}

void machine_code::disasm(ofstream &out) {
	for (auto it = vt.begin(); it != vt.end(); it++) {
		operation op = operation(*it);
		out << op;
	}
	out << "end " << start_adress;
	return;
}

int machine::init(machine_code &Mc) {
	this->r[14] = Mc.stack_adress;
	this->r[15] = Mc.start_adress;
	//memcpy(this->memory, (void*)(Mc.vt), min((int)(Mc.vt.size()), (1 << 20)) * sizeof(int));
	for (size_t i = 0; i < Mc.vt.size() && i < (1 << 20); i++) {
		this->memory[i] = Mc.vt[i];
	}
	return 0;
}

int machine::run() {
	while (endf == false) {
#ifdef DEBUG
		cout << "r15 " << r[15];
		cout << ", memory[r[15]] " << bitset<32>(memory[r[15]]) << "\n";
#endif // DEBUG
		operation op = operation(memory[r[15]]);
#ifdef DEBUG
		cout << op;
		cout << "number = " << op.number << ", A = " << op.A << ", B = " << op.B << ", C = " << op.C << "\n";
		cout << bitset<32>(op.op) << "\n   ^   ^   ^   ^   ^   ^   ^   ^\n";
#endif // DEBUG
		op(*this);
#ifdef DEBUG
		cout << "###\n";
		for (int i = 0; i < 14; i++) {
			cout << "r" << i << ": " << r[i] << "\t\t\t" << memory[(1 << 20) - i] << '\n';
		}
		cout << "r" << 14 << ": " << r[14] - (1 << 20) << "\t\t\t" << memory[(1 << 20) - 14] << '\n';
		cout << "r" << 15 << ": " << r[15] << "\t\t\t" << memory[(1 << 20) - 15] << '\n';
		cout << "flags: " << bitset<5>(flags) << "\n######################################\n\n";
#endif // DEBUG
	}
#ifdef DEBUG
	cout << "\nmmmmm\n";
#endif // DEBUG
	return 0;
}

//#############################################################################################################
//#############################################################################################################


// FUPM2.cpp :

//#define DEBUG


//#include "pch.h"

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
#ifdef RW_To_Files
	freopen("input.fasm", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif // Write_To_Files
	asm_code Asm;
	cin >> Asm;
#ifdef DEBUG
	cout << "\n###########################################\n###########################################\n";
	cout << Asm;
	cout << "\n###########################################\n###########################################\n";
#endif // DEBUG
	machine_code Mach;
	int code = Mach.init(Asm);
#ifdef DEBUG
	cout << "result " << code << '\n';
	out << Mach;
	cout << Mach;
	cout << "\n###########################################\ndisasm\n###########################################\n";
	Mach.disasm(cout);
	cout << "\n###########################################\ninit\n###########################################\n";
#endif // DEBUG
	machine M;
	M.init(Mach);
#ifdef DEBUG
	for (int i = 0; i < 16; i++) {
		cout << "r" << i << ": " << M.r[i] << '\n';
	}
	cout << "flags: " << M.flags << '\n';
	cout << "MEMORY:\n";
	for (int i = 0; i < Mach.vt.size(); i++)
		cout << i << " " << bitset<sizeof(int) * CHAR_BIT>(M.memory[i]) << "\n";
	cout << "\n###########################################\nrun\n###########################################\n";
#endif // DEBUG
	M.run();
#ifdef RW_To_Files
	fclose(stdout);
	fclose(stdin);
#endif // Write_To_Files
	return 0;
}
