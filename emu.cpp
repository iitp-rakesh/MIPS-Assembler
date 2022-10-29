/*****************************************************************************
TITLE: Claims																																
AUTHOR: Rakesh kumar
ROLL NO.: 2101AI26
Declaration of Authorship
This cpp file, emu.cpp, is part of the assignment of CS210 at the 
department of Computer Science and Engg, IIT Patna . 
*****************************************************************************/

#include <bits/stdc++.h>
using namespace std;

//Function to convert int to hexadecimal
string intToHex(int number)
{
  stringstream stream;
  stream << setfill ('0') << setw(8) << hex << number;
  return stream.str();
}

int32_t A, B, PC, SP;

int32_t memory[10000];

vector<int> data_pc;

//function to retrieve operand and opcode
tuple<int32_t, int32_t> instructionToOperandOpcode(int32_t instruction)
{
	int32_t opcode, operand;
	// masking first 6 bits
	opcode = instruction & 0xff;
	// masking last 2 bits
	operand = instruction & 0xffffff00;
	// Arithematic shift by 2
	operand >>= 8;
	return(make_tuple(operand, opcode));
}

// function for usage instructions
void prompt() {
    cout << "Usage: ./emu.exe [option] file.o" << endl;
    cout << "Options:" << endl;
    cout << "\t-trace\tshow instruction trace" << endl;
    cout << "\t-before\tshow memory dump before execution" << endl;
    cout << "\t-after\tshow memory dump after execution" << endl;
    cout << "\t-isa\tdisplay ISA" << endl;
}

// function for memory dump 
void memoryDump(int POC, ofstream& trcfile)
{
	cout << "Dumping from memory";
	trcfile << "Dumping from memory";

	for (int i = 0; i < POC; i++)
	{
		if(!(i % 4))
		{
			cout << endl << endl << intToHex(i) << "\t" << intToHex(memory[i]) << " ";
			trcfile << endl << endl << intToHex(i) << "\t" << intToHex(memory[i]) << " ";
		}
		else
			cout << intToHex(memory[i]) << " ";
			trcfile << intToHex(memory[i]) << " ";
	}
	cout << endl;
}

//Instruction set architecture=ISA
void ISA() {
    cout << "Opcode Mnemonic Operand\n";
    cout << "       data     value\n";
    cout << "0      ldc      value\n";
    cout << "1      adc      value\n";
    cout << "2      ldl      value\n";
    cout << "3      stl      value\n";
    cout << "4      ldnl     value\n";
    cout << "5      stnl     value\n";
    cout << "6      add\n";
    cout << "7      sub\n";
    cout << "8      shl\n";
    cout << "9      shr\n";
    cout << "10     adj      value\n";
    cout << "11     a2sp\n";
    cout << "12     sp2a\n";
    cout << "13     call     offset\n";
    cout << "14     return\n";
    cout << "15     brz      offset\n";
    cout << "16     brlz     offset\n";
    cout << "17     br       offset\n";
    cout << "18     HALT\n";
    cout << "       SET      value\n";
}

// Mnemonic Opcode table
map<int, string> mot;
void mnemonicOpcodeTable()
{
	mot[0] = string("ldc");
	mot[1] = string("adc");
	mot[2] = string("ldl");
	mot[3] = string("stl");
	mot[4] = string("ldnl");
	mot[5] = string("stnl");
	mot[6] = string("add");
	mot[7] = string("sub");
	mot[8] = string("shl");
	mot[9] = string("shr");
	mot[10] = string("adj");
	mot[11] = string("a2sp");
	mot[12] = string("sp2a");
	mot[13] = string("call");
	mot[14] = string("return");
	mot[15] = string("brz");
	mot[16] = string("brlz");
	mot[17] = string("br");
	mot[18] = string("HALT");
	mot[19] = string("data");
	mot[20] = string("SET");
}

// to find instructions
int trace(int PC, int POC, ofstream& trcfile)
{
	// Number of executed instructions
	int count = 0;

	// Loop to execute until HALT
	while(true)
	{
		// Book keeping for infinite loop
		int old_pc = PC;

		// Int 32 for operand and opcode
		int32_t operand, operation;

		// Instruction at particular PC
		int32_t instruction = memory[PC];

		// Retrieve operand and opcode
		tie(operand, operation) = instructionToOperandOpcode(instruction);

		// Print statement for tracing
		cout << "PC: " << intToHex(PC) << "\tSP: " << intToHex(SP) << "\tA: " 
		<< intToHex(A) << "\tB: " << intToHex(B) << "\t" << mot[operation] 
		<< " " <<operand << endl << endl;

		trcfile << "PC: " << intToHex(PC) << "\tSP: " << intToHex(SP) << "\tA: " 
		<< intToHex(A) << "\tB: " << intToHex(B) << "\t" << mot[operation] 
		<< " " <<operand << endl << endl;

		if (std::find(data_pc.begin(), data_pc.end(), PC) != data_pc.end())
		{
        	PC++;
        	continue;
		}

		// Switch case for individual instructios
		switch(operation) {
            case 0:
                B = A;
                A = operand;
                break;
            case 1:
                A += operand;
                break;
            case 2:
                B = A;
                A = memory[SP+operand];
                break;
            case 3:
                memory[SP+operand] = A;
                A = B;
                break;
            case 4:
                A = memory[A+operand];
                break;
            case 5:
                memory[A+operand] = B;
                break;
            case 6:
                A += B;
                break;
            case 7:
                A = B - A;
                break;
            case 8:
                A = B << A;
                break;
            case 9:
                A = B >> A;
                break;
            case 10:
                SP += operand;
                break;
            case 11:
                SP = A;
                A = B;
                break;
            case 12:
                B = A;
                A = SP;
                break;
            case 13:
                B = A;
                A = PC;
                PC += operand;
                break;
            case 14:
                PC = A;
                A = B;
                break;
            case 15:
                if(A == 0) 
                	PC += operand;
                break;
            case 16:
                if(A < 0) 
                	PC += operand;
                break;
            case 17:
                PC += operand;
                break;
            case 18:
            	cout << count << " number of instructions executed" << endl << endl;
                return 0;
        }
        // Incrememt PC
        PC++;

        // Check for infinite loop anamoly
		if(old_pc == PC)
		{
			cout << "Infinite loop detected" << endl;
			break;
		}

		// Increment executed instruction count
		count++;
	}
	// Total number of executed instructions
	cout << count << " number of instructions executed" << endl;
}

int main(int argc, char* argv[])
{
	// Correct input command should be given
	if(argc < 3)
	{
		// Print prompt for usage
		prompt();
		return(0);
	}

	// Initialize the machine operand table
	mnemonicOpcodeTable();

	// Argument for input file
	string in_file = argv[2];

	// Argument for output file
	string trace_file = in_file.substr(0, in_file.find(".", 0)) + ".trace";

	// Input object file
	ifstream infile;

	// Output trace file
	ofstream trcfile; 

	// Open file
	infile.open(in_file, ios::out | ios::binary);
	trcfile.open(trace_file);

	int32_t instructionCode;

	int POC = 0;

	// to store mchine code into main memory
	while (infile.read((char*) &instructionCode, sizeof(instructionCode)))
	{
		int32_t operand, operation;
		tie(operand, operation) = instructionToOperandOpcode(instructionCode);
		
		// If instruction is DATA or SET then store the value into memory
		if(operation == 19 || operation == 20)
		{
			memory[POC] = operand;
			data_pc.push_back(POC++);
		}

		// Other instructions stored in memory
		else if(operation >= 0 && operation < 20)
		{
			memory[POC++] = instructionCode;
		}

		// Identify invalid instruction
		else
		{
			cout << "Invalid instruction\n";
			return(0);
		}
	}

	A = 0;
	B = 0;
	PC = 0;
	SP = sizeof(memory)/sizeof(memory[0])-1;
	
	// Memory dump before execution
	if(string(argv[1]) == "-before")
		memoryDump(POC, trcfile);

	// Tracing for each executed instruction
	if(string(argv[1]) == "-trace")
		trace(0, POC, trcfile);

	// Memory dump after execution
	if(string(argv[1]) == "-after")
	{
		trace(0, POC, trcfile);
		memoryDump(POC, trcfile);
	}
	// Close file
	infile.close();
}