// pch.cpp :
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
#include <cmath>
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
			fscanf(stdin, "%d", &M.r[A]);
			break;
		case SCANDOUBLE:
			fscanf(stdin, "%lf", (double*)&M.r[A]);
			break;
		case PRINTINT:
			printf("%d", M.r[A]);
			break;
		case PRINTDOUBLE:
			printf("%lg", *(double *)&M.r[A]);
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
		M.r[A+1] = x % (M.r[B] + C);
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
		M.r[A+1] = M.memory[M.r[B] + C+1];
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
						if (op.B == (1 << 30)) {								//(1<<30) means wrong st
							map<string, int>::iterator l_it = labels.find(A);
							if (l_it != labels.end()) {
								op.B = l_it->second;
							}
							else {
								return MISSED_FUNCTION;
							}
						}
					}
					else {
						op.B = as_int(B);
						if (op.B == (1 << 30)) {								//(1<<30) means wrong st
							map<string, int>::iterator l_it = labels.find(B);
							if (l_it != labels.end()) {
								op.B = l_it->second;
							}
							else {
								return MISSED_FUNCTION;
							}
						}
					}


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
				case WORD:
					op.op = as_int(A);
					(this->vt).push_back(op.op);
#ifdef DEBUG
					cout << "word, A = " << op.A <<"\n";
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
		cout << "r" << 14 << ": " << r[14] - (1<<20) << "\t\t\t" << memory[(1 << 20) - 14] << '\n';
		cout << "r" << 15 << ": " << r[15] << "\t\t\t" << memory[(1 << 20) - 15] << '\n';
		cout << "flags: " << bitset<5>(flags) << "\n######################################\n\n";
#endif // DEBUG
	}
#ifdef DEBUG
	cout << "\nmmmmm\n";
#endif // DEBUG
	return 0;
}