// pch.cpp :

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

void operation::operator ()(machine M) {
	switch (number) {
	case HALT:
		M.endf = true;
		break;
	case SYSCALL:
		switch (B) {
		case EXIT:
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
	case ADD:
	case ADDI:
	case SUB:
	case SUBI:
	case MUL:
	case MULI:
	case DIV:
	case DIVI:
	case LC:
	case SHL:
	case SHLI:
	case SHR:
	case SHRI:
	case AND:
	case ANDI:
	case OR:
	case ORI:
	case XOR:
	case XORI:
	case NOT:
	case MOV:
	case ADDD:
	case SUBD:
	case MULD:
	case DIVD:
	case ITOD:
	case DTOI:
	case PUSH:
	case POP:
	case CALL:
	case CALLI:
	case RET:
	case CMP:
	case CMPI:
	case CMPD:
	case JMP:
	case JNE:
	case JEQ:
	case JLE:
	case JL:
	case JGE:
	case JG:
	case LOAD:
	case STORE:
	case LOAD2:
	case STORE2:
	case LOADR:
	case LOADR2:
	case STORER:
	case STORER2:
		break;
	default:
		break;
	}
	return;
}


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


/*
static void pass_spaces(const string &st, string::iterator &it) {
	for (; it < st.end() && (*it == ' ' || *it == '\t'); it++);
	return;
}
*/

static int as_register(const string& st) {
	map <string, int> ::iterator it = Is_REGISTR.find(st);
	if (it == Is_REGISTR.end())
		return -1;
	return it->second;
}

int machine_code::init(asm_code & Ac) {
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
				if ((p_it->second).second == RM) {
					int reg1 = as_register(A);
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


int machine::init(machine_code &Mc) {
	this->r[14] = Mc.stack_adress;
	this->r[15] = Mc.start_adress;
	//memcpy(this->memory, (void*)(Mc.vt), min((int)(Mc.vt.size()), (1 << 20)) * sizeof(int));
	for (int i = 0; i < Mc.vt.size() && i < (1 << 20); i++) {
		this->memory[i] = Mc.vt[i];
	}
}
int machine::run() {
	while (!endf) {

	}
}