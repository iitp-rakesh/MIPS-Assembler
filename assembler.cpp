/*****************************************************************************
TITLE: Claims																																
AUTHOR: Rakesh kumar
ROLL NO.: 2101AI26
Declaration of Authorship
This cpp file, assembler.cpp, is part of the assignment of CS210 at the 
department of Computer Science and Engg, IIT Patna . 
*****************************************************************************/
#include <bits/stdc++.h>
using namespace std;
//structure for symbols
struct symbol
{
    string name;
    int address;
    bool set;
    int set_value;
};

// structure for literals
struct literal
{
	int literal;
	int address;
};

// Symbol table
vector<symbol> symbolTable;

// Literal table
vector<literal> literalTable;

//Function to remove blank spaces
static inline string &trim(string &str) 
{
    str.erase(find_if(str.rbegin(), str.rend(),not1(ptr_fun<int, int>(isspace))).base(), str.end());
    str.erase(str.begin(), find_if(str.begin(), str.end(),not1(ptr_fun<int, int>(isspace))));
    return str;
}

//Function to check hex number or decimal number
bool str_number(string& str)
{
    int i = 0;
    string temp;
    temp = str;
    //check for decimal number (-,+)
    if (str.front() == '-' or str.front() == '+') 
    {
        temp = str.substr(1, str.length());
    }
    //if hex number then take after 0x
    else if(str.find("0x") == 0) {
        temp = str.substr(2),16;
    }
    std::string::const_iterator it = temp.begin();
    while (it != temp.end() && std::isdigit(*it)) ++it;
    return !temp.empty() && it == temp.end();
}

//function to find symbol in symbol table
bool IsSymbolContain(string name)
{
   auto iter = find_if(symbolTable.begin(), symbolTable.end(), 
               [&](const symbol& ts){return ts.name == name;});
   return iter != symbolTable.end();
}

//function to convert int to hex
string intToHex(int number)
{
  stringstream string;
  string << setfill ('0') << setw(8) 
         << hex << number;
  return string.str();
}

//function to cinvert String to decimal
int stringtoNumber(string str) {
    str = trim(str);
    if(str.find("0x") == 0) {
        return stoul(str, nullptr, 16);
    } else if(str.find("0") == 0) {
        return stoul(str, nullptr, 8);
    } else if(str.find("-") == 0) {
        return -stoul(str.substr(1, str.length()), nullptr, 10);
    } else if(str.find("+") == 0) {
        return stoul(str.substr(1, str.length()), nullptr, 10);
    } else {
        return stoul(str, nullptr, 10);
    }
}
//check correct label name
int IsValidLabelName(string labelName) {
    if( !((labelName[0] >= 'a' && labelName[0] <= 'z') || (labelName[0] >= 'A' && labelName[0] <= 'Z')) ) return false;
    
    for(int i = 0; i < labelName.length(); i++) {
        if( !( isdigit(labelName[i]) || (labelName[0] >= 'a' && labelName[0] <= 'z') || (labelName[0] >= 'A' && labelName[0] <= 'Z') ) ) {
            return false;
        }
    }

    return true;
}

// Mnemonic Opcode table
map<string, string> MnemonicOpcodeTable;
void Mnemonic()
{
	MnemonicOpcodeTable["ldc"] = string("00");
	MnemonicOpcodeTable["adc"] = string("01");
	MnemonicOpcodeTable["ldl"] = string("02");
	MnemonicOpcodeTable["stl"] = string("03");
	MnemonicOpcodeTable["ldnl"] = string("04");
	MnemonicOpcodeTable["stnl"] = string("05");
	MnemonicOpcodeTable["add"] = string("06");
	MnemonicOpcodeTable["sub"] = string("07");
	MnemonicOpcodeTable["shl"] = string("08");
	MnemonicOpcodeTable["shr"] = string("09");
	MnemonicOpcodeTable["adj"] = string("0a");
	MnemonicOpcodeTable["a2sp"] = string("0b");
	MnemonicOpcodeTable["sp2a"] = string("0c");
	MnemonicOpcodeTable["call"] = string("0d");
	MnemonicOpcodeTable["return"] = string("0e");
	MnemonicOpcodeTable["brz"] = string("0f");
	MnemonicOpcodeTable["brlz"] = string("10");
	MnemonicOpcodeTable["br"] = string("11");
	MnemonicOpcodeTable["HALT"] = string("12");
	MnemonicOpcodeTable["data"] = string("13");
	MnemonicOpcodeTable["SET"] = string("14");
}

//Add Instruction to literal and symbol table
string instructionTotable(string instruction, int* location_ptr, int line)
{
    int location = *location_ptr;
    string errors = "";

    //Find label and variables
    if(instruction.find(':') != string::npos)
    {
        int colon = instruction.find(":", 0);

        if(IsSymbolContain(instruction.substr(0, colon)))
        {
        	cout << "ERROR: Duplicate Label at line " << line << endl;
        	errors += "ERROR: Duplicate Label at line " + to_string(line) + "\n";
        }

        if(!IsValidLabelName(instruction.substr(0, colon)))
        {
        	cout << "WARNING: Incorrect label format at line " << line << endl;
        	errors += "WARNING: Incorrect label format at line " + to_string(line) + "\n";
        }

        // if instruction present after the colon
        if(colon != instruction.length() - 1)
        {
        	string subString = instruction.substr(colon + 1, instruction.length());
        	subString = trim(subString);
        	instructionTotable(subString, &location, line);
        	int whiteSpace = subString.find(" ", 0);
        	string subOp = subString.substr(0, whiteSpace);
        	string subVal = subString.substr(whiteSpace + 1, subString.length());
        	subOp = trim(subOp);
        	subVal = trim(subVal);

        	//SET instructions
        	if(subOp == "SET")
        	{
        		symbolTable.push_back({instruction.substr(0, colon), location, 1, stoi(subVal)});
        	}
        	else
        	{
        		symbolTable.push_back({instruction.substr(0, colon), location, 0, -1});
        	}
        }
        else
        {
        	*location_ptr = *location_ptr - 1;
        	symbolTable.push_back({instruction.substr(0, colon), location, 0, -1});
        }
    }

    // check literals and constants
    else
    {
        // Considering that second part of instruction is operand
        int whiteSpace = instruction.find(" ", 0);
        string subString = instruction.substr(whiteSpace + 1, instruction.length());
        subString = trim(subString);

        // Checking for numeral 
        if(str_number(subString))
        {
        	literalTable.push_back({stringtoNumber(subString), -1});
        }
    }
    return(errors);
}
//Analyse function
void analyse(string file, ofstream& logfile)
{
    // Temp str variable for get line
    string line;
    int location = 0;
    int line_count = 1;

    // Reading the input file
    ifstream MyFile(file);

    // Read individual lines
    while (getline (MyFile, line)) 
    {
        // Pre-process the line, trim extra spaces
        string instruction;
        instruction = line.substr(0, line.find(";", 0));
        instruction = trim(instruction);

        // Skip empty lines
        if(instruction == "")
        {
            line_count++;
            continue;
        }

        logfile << instructionTotable(instruction, &location, line_count++);
        location++;
    }

    // LTORG loop for literals
    for(int i=0; i < literalTable.size(); i++)
    {
       	if(literalTable[i].address == -1)
       	{
       		literalTable[i].address = location++;
       	}
    }
    // Close the file after EOF
    MyFile.close();
}

tuple<string, string, string> inst_to_code(string instruction, int* location_ptr, int line)
{
	// Program Counter
	int location = *location_ptr;

	// Hex Code of Program Counter
	string encoding = intToHex(location) + " ";

	// Warnings and errors
    string encodingWarning = "";
	string errors = "";
	string machineCode = "";

    // Identify label and variables
    if(instruction.find(':') != string::npos)
    {
        int colon = instruction.find(":", 0);

        // Instruction could be present after the colon
        if(colon != instruction.length() - 1)
        {
        	string subString = instruction.substr(colon + 1, instruction.length());
        	subString = trim(subString);
        	tie(encodingWarning, errors, machineCode) = inst_to_code(subString, &location, line);
        	string temp = encodingWarning;
        	temp = temp.substr(9, 9);
        	encoding += temp;
        }
        // If no instruction after colon, PC shouldn't change
        else
        {
        	encoding += "         ";
        	*location_ptr = *location_ptr - 1;
        }
        encoding += instruction + "\n";
    }

    // Identify literals and constants
    else
    {
        // Considering that second part of instruction is operand
        int whiteSpace = instruction.find(" ", 0);

        // Temporary variable    for operand encoding
        string tempHexString;

        // String containing operation
        string subStringOperation = instruction.substr(0, whiteSpace);

        // String containing operand
        string sub_operand = instruction.substr(whiteSpace + 1, instruction.length());

        sub_operand = trim(sub_operand);
        subStringOperation = trim(subStringOperation);

        // Checking for invalid mnemonics
        if(MnemonicOpcodeTable[subStringOperation] == "")
        {
        	errors += "ERROR: Mnemonic not chk at line " + to_string(line) + "\n";
        	cout << "ERROR: Mnemonic not chk at line " << line << endl;
        }

        // Checking for No operand instructions
        else if(subStringOperation == "add" || subStringOperation == "sub"
        		|| subStringOperation == "shl"|| subStringOperation == "shr"
        		|| subStringOperation == "a2sp"|| subStringOperation == "sp2a"
        		|| subStringOperation == "return"|| subStringOperation == "HALT")
        {
        	encoding += "000000" + MnemonicOpcodeTable[subStringOperation] + " "; 
        	machineCode += "000000" + MnemonicOpcodeTable[subStringOperation] + " "; 
        	if(subStringOperation.length() != instruction.length())
			{
				errors += "ERROR: Operand not expected at line " + to_string(line) + "\n";
				cout << "ERROR: Operand not expected at line " << line << endl;
			}
        }
        // Checking for numeral operand to encode directly
        else if(str_number(sub_operand))
        {
        	tempHexString = intToHex(stringtoNumber(sub_operand));
            // if(subStringOperation == "SET" || subStringOperation == "data")
            //     encoding += tempHexString + " ";
            // else
            //     encoding += tempHexString.substr(tempHexString.length() - 6, tempHexString.length()) + MnemonicOpcodeTable[subStringOperation] + " "; 
        	encoding += tempHexString.substr(tempHexString.length() - 6, tempHexString.length()) + MnemonicOpcodeTable[subStringOperation] + " "; 
        	machineCode += tempHexString.substr(tempHexString.length() - 6, tempHexString.length()) + MnemonicOpcodeTable[subStringOperation] + " "; 
        }
        // Checking for variable operand to encode address
        else
        {
        	int chk = 0;
        	for(int i=0; i < symbolTable.size(); i++)
        	{
        		if(symbolTable[i].name.compare(sub_operand) == 0)
        		{
        			chk = 1;
        			// SET variables considered numeral
        			if(symbolTable[i].set)
        			{
        				tempHexString = intToHex(symbolTable[i].set_value);
        			}
        			// Operands which need offset from PC
        			else if(subStringOperation == "call" || subStringOperation == "brz"
        				|| subStringOperation == "brlz"|| subStringOperation == "br")
        			{
        				tempHexString = intToHex(symbolTable[i].address - location - 1);
        			}
        			// Normal case, encode address
        			else
        			{
        				tempHexString = intToHex(symbolTable[i].address);
        			}
                    encoding += tempHexString.substr(tempHexString.length() - 6, tempHexString.length()) + MnemonicOpcodeTable[subStringOperation] + " "; 
        			machineCode += tempHexString.substr(tempHexString.length() - 6, tempHexString.length()) + MnemonicOpcodeTable[subStringOperation] + " "; 
        			break;
        		}
        	}
            //if subStringOperation length == instruction length
        	if(subStringOperation.length() == instruction.length())
        	{
        		errors += "ERROR: Operand expected at line " + to_string(line) + "\n";
        		cout << "ERROR: Operand expected at line " << line << endl;
        	}
        	else if(!chk)
        	{
        		errors += "ERROR: Unknown Symbol as operand at line " + to_string(line) + "\n";
        		cout << "ERROR: Unknown Symbol as operand at line " << line << endl;
        	}
        }
        encoding += instruction + "\n";
    }
    return(make_tuple(encoding, errors, machineCode));
}
//Function to synthesize the file
void synthesize(string file, ofstream& OutputFile, ofstream& logfile, ofstream& ObjectFile)
{
    // Temp str variable for get line
    string line;

    // Program Counter
    int location = 0;

    // Line counter
    int line_count = 1;

    // Reading the input file
    ifstream MyFile(file);

    // Read individual lines
    while (getline (MyFile, line)) 
    {
        // Pre-process the line, trim extra spaces
        string instruction;
        instruction = line.substr(0, line.find(";", 0));
        instruction = trim(instruction);

        // Jump empty lines
        if(instruction == "")
        {
            line_count++;
            continue;
        }

        // Write Encoded instruction into listing file
        string encoding, errors, machineCode;
        tie(encoding, errors, machineCode) = inst_to_code(instruction, &location, line_count++);
        OutputFile << encoding;
 
        if(machineCode != "")
        {
        	uint32_t temp = stoul("0x" + trim(machineCode), nullptr, 16);
        	ObjectFile.write((char *)&temp, sizeof(temp));
        }	
        logfile << errors;
        location++;
    }
}

int main(int argc, char* argv[]) 
{
	//Mnemonic Opcode table
	Mnemonic();

	//input file
	string Input_file = argv[1];

	// output file
	string Output_file = Input_file.substr(0, Input_file.find(".", 1)) + ".l";
    
	// object file
	string Object_file = Input_file.substr(0, Input_file.find(".", 1)) + ".o";

	// log file
	string log_file = Input_file.substr(0, Input_file.find(".", 1)) + ".log";

	// Output listing, log file
    ofstream OutputFile(Output_file);
    ofstream ObjectFile(Object_file,ios::out | ios::binary);
    ofstream logfile(log_file);

    //analysis part
    analyse(Input_file, logfile);

    //synthesis part
    synthesize(Input_file, OutputFile, logfile, ObjectFile);

    // Close files
    OutputFile.close();
    logfile.close();
    ObjectFile.close();
}