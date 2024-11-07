# Vole Machine Simulator

## Authors
- Mahmoud Ibraheem Mohamed Mohamed
- Omar Fawzy Abdulhamid Mahmoud
- Marwan Mohamed Hassen

## IDs
- 20230367
- 20230631
- 20230387

## Section: S17
## TA: Hussien Tarek
## Date: 2/11/2024

## Overview
A C++ implementation of a Vole Machine simulation. The Vole Machine is a simple virtual machine designed to execute a set of predefined instructions. The Vole Machine simulation includes classes and functions for registers, memory, and the virtual machine itself, along with a user interface for interaction.

## Table of Contents
- [UML](https://github.com/user-attachments/assets/0004c9bd-668e-4b77-9841-8ae41ef5e889)
- [Header File: vole_machine.h](#header-file-vole_machineh)
- [Source File: vole_machine.cpp](#source-file-vole_machinecpp)
- [Main File: A1-Task4-S17-20230367_20230631_20230387.cpp](#main-file-A1-Task4-S17-20230367_20230631_20230387cpp)

## UML
![UML image](https://github.com/user-attachments/assets/0004c9bd-668e-4b77-9841-8ae41ef5e889)

## Header File: vole_machine.h
This header file defines the classes and functions used in the Vole Machine simulation.

```cpp
// File: A1-Task4-S17-20230367_20230631_20230387.cpp
// Purpose: Simulate Vole Machine with its all commands in cpp
// Authors: Mahmoud Ibraheem Mohamed Mohamed
//          Omar Fawzy Abdulhamid Mahmoud
//          Marwan Mohamed Hassen
// ID: 20230367
//     20230631
//     20230387
// Section: S17
// TA: Hussien Tarek
// Date: 2/11/2024

#ifndef VOLEMACHINE_VOLE_MACHINE_H
#define VOLEMACHINE_VOLE_MACHINE_H

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;

class Register {
private:
    unsigned char value;

public:
    // Assignments
    Register(); // initial value 0
    Register(unsigned char v);

    unsigned char get_value() const;

    void set_value(unsigned char &ch);

    Register &operator=(unsigned char ch);

    bool operator==(unsigned char ch) const;

    bool operator==(int ch) const;

    bool operator==(const Register &r) const;
};

class Memory {
public:
    Register part[256];

    // Access modifiers
    Register &operator[](string &XY);
    Register &operator[](unsigned char pos);
};

class VM {
private:
    // R, S, T are the position of registers
    // XY is the position of memory
    Register registers[16];
    Memory memory;
    Memory file;
    unsigned char file_PC = 0x10;
    unsigned char PC = 0x10;
    string hex_output;   // Contains a list with all inserted values in 00 pos in memory in hex
    string ascii_output; // Contains a list with all inserted values in 00 pos in memory in ASCII
    bool steps = false;
    bool file_is_loaded = false;

    void load1(unsigned char R, unsigned char XY); // Load from Memory XY
    void load2(unsigned char R, unsigned char XY); // Load XY
    void store(unsigned char R, unsigned char XY); // store register in XY
    void move(unsigned char R, unsigned char S);   // copy value from the first register to the second register
    // add two registers as float or integer in 2's complement
    unsigned char add(unsigned char S, unsigned char T, bool is_float);

    // if the given register is equal to register[0] hump to memory XY
    void jump(unsigned char R, string XY);

    unsigned char OR(unsigned char S, unsigned char T);     // bitwise OR between two registers
    unsigned char AND(unsigned char S, unsigned char T);    // bitwise AND between two registers
    unsigned char XOR(unsigned char S, unsigned char T);    // bitwise XOP between two registers
    unsigned char rotate(unsigned char R, unsigned char X); // rotates register's bits to right by the given number
    void jump2(unsigned char R, string XY); // if the given register is greater than register[0] hump to memory XY
    static bool read_and_validate_yes_no(); // [Y-N] input for steps
public:
    friend ostream &
    operator<<(ostream &o, const VM &vm); // operator overload to print Memory and registers
    void wipe();                          // The variables used in operate() are reset
    void delete_file();                   // Delete file
    bool is_loaded() const { return file_is_loaded; }

    void read_file();          // store instructions in memory if they're valid
    void operate(bool _steps); // operate by steps or by whole

    // Utility functions

    // a toupper overload for string
    // Capitalize characters in the given string
    static string &toupper(string &s);

    // Check if the string is in the form of Hexadecimal
    static bool is_hex(string &s);

    // Convert hexadecimal to decimal number (Max is 255 for Vole Machine)
    static unsigned char hex_to_dec(string hex);

    // Convert decimal number to hexadecimal
    static string decimal_to_hex(int decimal);

    // Converts to Non-implicit
    static unsigned char float_fix(unsigned char f);
};

class VM_UI {
private:
    bool isRunning = true;
    VM vole_machine;
    enum class enMainMenuOption; // Contains options in a human-readable format
    // Function to perform the selected main menu option
    void PerformMainMenuOption(const enMainMenuOption &MainMenuOption);

public:
    static short ReadUserChoice(const short &From, const short &To);// Function to read user choice within a given range
    static void PauseScreen(); // Function to pause the screen
    static void ClearScreen(); // Function to clear the screen
    void ShowMainMenu();       // Function to display the main menu
};

#endif // VOLEMACHINE_VOLE_MACHINE_H
```

## Source File: vole_machine.cpp
This file contains the implementation of the classes and functions defined in the header file.

```cpp
// File: A1-Task4-S17-20230367_20230631_20230387.cpp
// Purpose: Simulate Vole Machine with its all commands in cpp
// Authors: Mahmoud Ibraheem Mohamed Mohamed
//          Omar Fawzy Abdulhamid Mahmoud
//          Marwan Mohamed Hassen
// ID: 20230367
//     20230631
//     20230387
// Section: S17
// TA: Hussien Tarek
// Date: 2/11/2024

#include "vole_machine.h"

// a toupper overload for string
// Capitalize characters in the given string
string &VM::toupper(string &s) {
    for (char &i: s) {
        i = (char) std::toupper(i);
    }
    return s;
}

// Check if the string is in the form of Hexadecimal
bool VM::is_hex(string &s) {
    // first capitalize
    s = toupper(s);
    // loop for every character
    for (char &i: s) {
        // check if the character is within the specified range, if not return false
        if (!(('0' <= i && i <= '9') || ('A' <= i && i <= 'F')))
            return false;
    }
    return true;
}

// Convert hexadecimal to decimal number (Max is 255 for Vole Machine)
unsigned char VM::hex_to_dec(string hex) {
    // initialize variable
    unsigned char dec = 0;

    // summation of [weight * (base ^ index)]
    for (int i = 0; i < hex.size(); ++i) {
        if (hex[i] <= '9')
            dec += (hex[i] - '0') * pow(16, hex.size() - i - 1);
        else
            dec += (hex[i] - 'A' + 10) * pow(16, hex.size() - i - 1);
    }

    return dec;
}

// Convert decimal number to hexadecimal
string VM::decimal_to_hex(int decimal) {
    // to return "00" instead of an empty string
    if (decimal == 0)
        return "00";

    // initialize variables
    string hex;
    const char hex_digits[] = "0123456789ABCDEF";

    // Logic: divide the number by 16 and take the remainder as index in hex_digits
    while (decimal > 0) {
        // remainder already defined in <cmath>
        int Remainder = decimal % 16;
        hex = hex_digits[Remainder] + hex;
        decimal /= 16;
    }

    // to return a constant size of 2
    if (hex.size() == 1) {
        hex = '0' + hex;
    }

    return hex;
}

// Converts to Non-implicit
unsigned char VM::float_fix(unsigned char f) {
    // if Mantissa is empty, reset Exponent
    if ((f & 0b0'000'1111) == 0) {
        f &= 0b1'000'0000;
    }

    // if mantissa doesn't have 1 in the first bit AND Exponent has at least one bit
    while (((f & 0b0'000'1111) < 8) && ((f & 0b0'111'0000) > 0)) {
        // shift Mantissa bits to the left
        unsigned char temp = (f & 0b0'000'1111) << 1;

        // Apply the new Mantissa
        f &= 0b1'111'0000;
        f |= temp;

        // subtract one from Exponent
        f -= 0b0'001'0000;
    }

    return f;
}

Register::Register() : value(0) {} // initial value 0
Register::Register(unsigned char v) : value(v) {}

unsigned char Register::get_value() const { return value; }

void Register::set_value(unsigned char &ch) { value = ch; }

Register &Register::operator=(unsigned char ch) {
    value = ch;
    return *this;
}

bool Register::operator==(unsigned char ch) const { return value == ch; }

bool Register::operator==(int ch) const { return value == ch; }

bool Register::operator==(const Register &r) const { return value == r.value; }

// Access to memory part
Register &Memory::operator[](string &XY) {
    unsigned char pos = VM::hex_to_dec(XY);
    return part[pos];
}

Register &Memory::operator[](unsigned char pos) { return part[pos]; }

// sets memory, registers and output to 0
void VM::wipe() {
    memory = {};
    for (auto &i: registers) {
        i = 0;
    }
    hex_output = "";
    ascii_output = "";
}

// Remove File
void VM::delete_file() {
    file = {};
    file_is_loaded = false;
}

// LOAD the register R with the bit pattern found in the memory cell whose address is XY
void VM::load1(unsigned char R, unsigned char XY) { registers[R] = memory[XY]; }

// LOAD the register R with the bit pattern XY
void VM::load2(unsigned char R, unsigned char XY) {
    registers[R] = Register(XY);
}

// STORE the bit pattern found in register R in the memory cell whose address is XY
// If you store in memory cell 00 print to screen
void VM::store(unsigned char R, unsigned char XY) {
    memory[XY] = registers[R];
    if (XY == 0) {
        string hex_value = decimal_to_hex(registers[R].get_value());
        hex_output.append(hex_value);
        hex_output.push_back(' ');
        ascii_output.push_back((char) registers[R].get_value());
    }
}

// MOVE (Copy) the bit pattern found in register R to register S
void VM::move(unsigned char R, unsigned char S) { registers[S] = registers[R]; }

// ADD the bit patterns in registers S and T as though they were two’s complement representations
// or in float representations (1.3.4)
unsigned char VM::add(unsigned char S, unsigned char T, bool is_float) {
    if (is_float) {
        // fix exponent by increasing the lower one (0 -> mantissa, mantissa.pop).
        // take the sign of bigger mantissa and check if different (subtraction)
        // take the exponent of higher

        /* For Floats:
         * binary
         * | sign bit
         * | |  Exponent
         * | |  |   Mantissa
         * 0b0'000'0000
         */

        // take registers and lower their exponent as possible
        unsigned char first_number = float_fix(registers[S].get_value()),
                second_number = float_fix(registers[T].get_value());

        // make them have the same Exponent (higher one)
        if ((first_number & 0b0'111'0000) > (second_number & 0b0'111'0000)) {
            // shift Mantissa to right by the difference of Exponents
            unsigned char temp_man = second_number & 0b0'000'1111;
            temp_man >>= ((first_number & 0b0'111'0000) - (first_number & 0b0'111'0000)) >> 4;

            // assemble new value
            second_number &= 0b1'000'0000;
            second_number |= first_number & 0b0'111'0000;
            second_number |= temp_man;
        } else {
            // shift Mantissa to right by the difference of Exponents
            unsigned char temp_man = first_number & 0b0'000'1111;
            temp_man >>= ((first_number & 0b0'111'0000) - (first_number & 0b0'111'0000)) >> 4;

            // assemble new value
            first_number &= 0b1'000'0000;
            first_number |= second_number & 0b0'111'0000;
            first_number |= temp_man;
        }

        // result variable
        unsigned char R = 0;

        // ADD if same sign bit
        if ((first_number & 0b1'000'0000) == (second_number & 0b1'000'0000)) {
            // add two Mantissas
            unsigned short temp = (first_number & 0b0'000'1111) + (second_number & 0b0'000'1111);
            // overflow handle
            if (temp & 0b0'001'0000) {
                temp >>= 1;
                R += 0b0'001'0000;
            }

            // combine Sign and Exponent with new Mantissa
            R += (first_number & 0b1'111'0000) | temp;
        } else {
            // Subtract
            // subtract small Mantissa from larger Mantissa
            if ((first_number & 0b0'000'1111) > (second_number & 0b0'000'1111)) {
                R = first_number - (second_number & 0b0'000'1111);
            } else {
                R = second_number - (first_number & 0b0'000'1111);
            }
        }

        R = float_fix(R);

        return R;
    }
    // 2's complement addition
    return registers[S].get_value() + registers[T].get_value();
}

// If it satisfies the condition, change PC
void VM::jump(unsigned char R, string XY) {
    if (registers[R] == registers[0]) {
        PC = hex_to_dec(XY) - 1;
    }
}

// Uses bitwise operator or
unsigned char VM::OR(unsigned char S, unsigned char T) {
    return registers[S].get_value() | registers[T].get_value();
}

// Uses bitwise operator and
unsigned char VM::AND(unsigned char S, unsigned char T) {
    return registers[S].get_value() & registers[T].get_value();
}

// Uses bitwise operator xor
unsigned char VM::XOR(unsigned char S, unsigned char T) {
    return registers[S].get_value() ^ registers[T].get_value();
}

// rotate register R by X bits (0-15)
unsigned char VM::rotate(unsigned char R, unsigned char X) {
    unsigned char temp = registers[R].get_value() << (16 - X);
    temp |= registers[R].get_value() >> X;
    return temp;
}

// If it satisfies the condition, change PC
void VM::jump2(unsigned char R, string XY) {
    if ((char) registers[R].get_value() > (char) registers[0].get_value()) {
        PC = hex_to_dec(XY) - 1;
    }
}

// Main Vole Machine's program
void VM::operate(bool _steps) {
    // If there is no loaded file
    if (!is_loaded()) {
        cout << "No file loaded";
        return;
    }

    // remove old memory
    wipe();

    // load memory from the file
    memory = file;
    PC = file_PC;

    // set mode
    steps = _steps;

    bool run = true;
    while (run) {
        // indicates that is the final iteration
        if (PC == 254) {
            run = false;
        }

        // store instruction and the register right to it
        unsigned char IR = memory[PC].get_value() >> 4;
        unsigned char reg = memory[PC].get_value() & 0b00001111;

        // display next instruction then apply
        if (steps) {
            cout << "IR: " << decimal_to_hex(memory[PC].get_value())
                 << decimal_to_hex(memory[PC + 1].get_value())
                 << "   PC:" << decimal_to_hex(PC) << endl;
            // ask the user if he wants to exalt or continue
            if (!read_and_validate_yes_no()) {
                cout << *this << "Simulation Ended!" << endl;
                return;
            }
        }

        // Preform the operation
        PC++;
        switch (IR) {
            case 0x1:
                load1(reg, memory[PC].get_value());
                break;
            case 0x2:
                load2(reg, memory[PC].get_value());
                break;
            case 0x3:
                store(reg, memory[PC].get_value());
                break;
            case 0x4:
                move(memory[PC].get_value() >> 4, memory[PC].get_value() & 0b00001111);
                break;
            case 0x5:
                registers[reg] = add(memory[PC].get_value() >> 4, memory[PC].get_value() & 0b00001111, false);
                break;
            case 0x6:
                registers[reg] = add(memory[PC].get_value() >> 4, memory[PC].get_value() & 0b00001111, true);
                break;
            case 0x7:
                registers[reg] = OR(memory[PC].get_value() >> 4, memory[PC].get_value() & 0b00001111);
                break;
            case 0x8:
                registers[reg] = AND(memory[PC].get_value() >> 4, memory[PC].get_value() & 0b00001111);
                break;
            case 0x9:
                registers[reg] = XOR(memory[PC].get_value() >> 4, memory[PC].get_value() & 0b00001111);
                break;
            case 0xA:
                registers[reg] = rotate(reg, memory[PC].get_value() & 0b00001111);
                break;
            case 0xB:
                jump(reg, decimal_to_hex(memory[PC].get_value()));
                break;
            case 0xC:
                run = false;
                break;
            case 0xD:
                jump2(reg, decimal_to_hex(memory[PC].get_value()));
                break;
            default:
                break;
        }
        PC++;

        // print after operation
        if (steps) {
            cout << *this;
            if (!hex_output.empty()) {
                cout << "Current Output: " << hex_output << endl;
                cout << "ASCII Screen  : " << ascii_output << endl;
            }
            cout << endl;
        }
    }
    // for run Program as a whole
    if (!steps)
        cout << *this;

    // print all registered Hexadecimals in memory 00
    if (!hex_output.empty()) {
        cout << "Output: " << hex_output << endl;
        cout << "ASCII : " << ascii_output << endl;
    }

    cout << "Simulation Ended!" << endl;
}

ostream &operator<<(ostream &o, const VM &vm) {
    o << "Memory:\n";
    o << "    00";
    for (unsigned char i = 1; i < 16; ++i) {
        o << ' ' << VM::decimal_to_hex(i);
    }
    for (int i = 0; i < 256; ++i) {
        if (i % 16 == 0) {
            o << "\n" << VM::decimal_to_hex(i) << ": ";
        }
        o << VM::decimal_to_hex(vm.memory.part[i].get_value()) << " ";
    }
    o << "\n\n";
    o << "Rs:";
    for (auto reg: vm.registers) {
        o << ' ' << VM::decimal_to_hex(reg.get_value());
    }
    o << endl;
    return o;
}

void VM::read_file() {
    // ask user where to start the program
    while (true) {
        cout << "Please insert where does program start in Hexadecimal (leave empty for default 0x10):" << endl;
        string input;
        getline(cin, input);
        if (input.empty()) {
            file_PC = 0x10;
            break;
        }
            // check if inout is in the right format
        else if (input.size() <= 2 && is_hex(input)) {
            unsigned char decimal_input = hex_to_dec(input);
            // must be even
            if (decimal_input % 2) {
                cout << "Start position must be even!\n";
            } else {
                file_PC = hex_to_dec(input);
                break;
            }
        } else {
            cout << "Wrong input!\n";
        }
    }

    ifstream instructions_file;

    while (!instructions_file.is_open()) {
        cout << "Please insert file contains instructions:" << endl;
        string file_loc;
        getline(cin, file_loc);

        // file must contain ".txt"
        if (file_loc.size() < 4 || file_loc.substr(file_loc.size() - 4) != ".txt") {
            cout << "Something wrong!" << endl;
            continue;
        }

        // if the file doesn't exist or empty, retry
        instructions_file.open(file_loc);
        if (!instructions_file.is_open() || instructions_file.peek() == EOF) {
            cout << "Something wrong!" << endl;
        }
    }

    delete_file();

    // Note: instructions are taken by words, not lines
    // store a file in memory
    unsigned short skipped = 0;
    for (short i = 0; !instructions_file.eof() && ((PC + 2 * i) < 255); ++i) {
        // take an instruction
        string input;
        instructions_file >> input;

        // Check if instruction is in shape
        if (input.size() != 4 || !is_hex(input) || input[0] > 'D' || input[0] == '0') {
            if (!input.empty()) {
                cout << input << " is skipped" << endl;
                skipped++;
            }
            i--;
            continue;
        }

        // load instruction in memory
        file[file_PC + 2 * i] = hex_to_dec(input.substr(0, 2));
        file[file_PC + 1 + 2 * i] = hex_to_dec(input.substr(2, 2));
    }

    // notify user if there are skipped lines
    if (skipped == 1)
        cout << "1 line has been skipped" << endl;
    else if (skipped)
        cout << skipped << " lines have been skipped" << endl;

    // that means the instruction file didn't load fully
    if (!instructions_file.eof())
        cout << "Your file has been trimmed due to reaching memory limit!" << endl;

    file_is_loaded = true;
}

// Function to read and validate yes/no input and return a bool
bool VM::read_and_validate_yes_no() {
    string Input;
    do {
        cout << "Do you want to execute? (y/n): ";
        getline(cin, Input);
        if (Input.length() == 1 && (Input[0] == 'y' || Input[0] == 'Y')) {
            return true;
        } else if (Input.length() == 1 && (Input[0] == 'n' || Input[0] == 'N')) {
            return false;
        } else {
            cout << "Invalid input!" << endl;
        }
    } while (true);
}

enum class VM_UI::enMainMenuOption {
    eLoadFile = 1,
    eExecuteAsWhole,
    eExecuteStepByStep,
    Exit
};

/**
 * Prompts the user to choose an option within a specified range.
 *
 * @param From The lower bound of the range
 * @param To The upper bound of the range
 * @return The valid user choice
 */
short VM_UI::ReadUserChoice(const short &From, const short &To) {
    short Choice = 0; // Variable to store the user's choice
    string Input;     // Variable to store user input
    do {
        cout << "Choose what do you want to do [" << From << "-" << To << "] : ";
        getline(cin, Input); // Read user input

        // Check for empty input
        if (Input.empty()) {
            cout << "Empty input!" << endl;
        }
            // Validate that input consists only of digits
        else if (all_of(Input.begin(), Input.end(), [](char c) { return isdigit(c); })) {
            Choice = short(stoi(Input)); // Convert input to short
        } else {
            cout << "Invalid input, Enter a valid choice from [" << From << "-" << To
                 << "]" << endl; // Notify user of invalid input
        }

    } while (Choice <= (From - 1) || Choice > To); // Repeat until a valid choice is made
    return Choice; // Return the valid choice
}

/**
 * Pauses the screen until the user presses a key.
 */
void VM_UI::PauseScreen() {
    cout.flush(); // Flush the output buffer
#if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
    system("read -n 1 -s"); // Unix-based systems
#else
    system("PAUSE >nul"); // Windows systems
#endif
}

/**
 * Clears the console screen based on the operating system.
 */
void VM_UI::ClearScreen() {
#if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
    system("clear"); // Clear screen for Unix-based systems
#else
    system("CLS"); // Clear screen for Windows systems
#endif
}

/**
 * Executes the action associated with the selected menu option.
 *
 * @param MainMenuOption The selected menu option
 */
void VM_UI::PerformMainMenuOption(const enMainMenuOption &MainMenuOption) {
    switch (MainMenuOption) {
        case enMainMenuOption::eLoadFile:
            ClearScreen();
            vole_machine.read_file();
            break;

        case enMainMenuOption::eExecuteAsWhole:
            ClearScreen();
            vole_machine.operate(false);
            break;

        case enMainMenuOption::eExecuteStepByStep:
            ClearScreen();
            vole_machine.operate(true);
            break;

        case enMainMenuOption::Exit:
            isRunning = false;
            ClearScreen();
            cout << "\nProgram Ends :-)" << endl;
            break;

        default:
            cout << "\nInvalid operation selected" << endl;
            break;
    }
    if (isRunning) {
        cout << "\n\nPress any key to go back to main menu... ";
        PauseScreen(); // Pause the screen after each option's execution
        ClearScreen(); // Clear the screen after pausing
    }
}

/**
 * Displays the main menu with options for the user to select from.
 */
void VM_UI::ShowMainMenu() {
    ClearScreen();
    isRunning = true;
    vole_machine.delete_file();
    while (isRunning) {
        cout << "______________________________________________\n";
        cout << "\t\t" << "Main Menu\n";
        cout << "______________________________________________\n";
        cout << "\t" << "[1] Load file.\n";
        cout << "\t" << "[2] Execute program as a whole.\n";
        cout << "\t" << "[3] Execute program step by step.\n";
        cout << "\t" << "[4] Exit.\n";
        cout << "______________________________________________\n";

        enMainMenuOption choice = static_cast<enMainMenuOption>(ReadUserChoice(1, 4));
        PerformMainMenuOption(choice);
    }
}
```

## Main File: A1-Task4-S17-20230367_20230631_20230387.cpp
This file contains the main function that initializes the user interface and starts the program.

```cpp
// File: A1-Task4-S17-20230367_20230631_20230387.cpp
// Purpose: Simulate Vole Machine with its all commands in cpp
// Authors: Mahmoud Ibraheem Mohamed Mohamed
//          Omar Fawzy Abdulhamid Mahmoud
//          Marwan Mohamed Hassen
// ID: 20230367
//     20230631
//     20230387
// Section: S17
// TA: Hussien Tarek
// Date: 2/11/2024

#include "lib/vole_machine.h"

int main() {
    VM_UI ui;
    ui.ShowMainMenu();
    return 0;
}
```
