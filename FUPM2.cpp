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
#include <cmath>
using namespace std;


int main()
{
#ifdef Read_From_Files
	freopen("input.fasm", "r", stdin);
	//freopen("output.txt", "w", stdout);
	//out = stdout;
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
#ifdef Read_From_Files
	//fclose(stdout);
	fclose(stdin);
#endif // Write_To_Files
	return 0;
}
