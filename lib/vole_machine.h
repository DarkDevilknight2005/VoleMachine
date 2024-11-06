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

// a toupper overload for string
// Capitalize characters in the given string
string &toupper(string &s);

// Check if the string is in the form of Hexadecimal
bool is_hex(string &s);

// Convert hexadecimal to decimal number (Max is 255 for Vole Machine)
unsigned char hex_to_dec(string hex);

// Convert decimal number to hexadecimal
string decimal_to_hex(int decimal);

// Converts to Non-implicit
unsigned char float_fix(unsigned char f);

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
  string hex_output;   // Contains a list with all inserted values in 00 pos in
                       // memory in hex
  string ascii_output; // Contains a list with all inserted values in 00 pos in
                       // memory in ASCII
  bool steps = false;
  bool file_is_loaded = false;

  void load1(unsigned char R, unsigned char XY); // Load from Memory XY
  void load2(unsigned char R, unsigned char XY); // Load XY
  void store(unsigned char R, unsigned char XY); // store register in XY
  void move(unsigned char R,
            unsigned char
                S); // copy value from the first register to the second register
  // add two registers as float or integer in 2's complement
  unsigned char add(unsigned char S, unsigned char T, bool is_float);
  // if the given register is equal to register[0] hump to memory XY
  void jump(unsigned char R, string XY);
  unsigned char OR(unsigned char S,
                   unsigned char T); // bitwise OR between two registers
  unsigned char AND(unsigned char S,
                    unsigned char T); // bitwise AND between two registers
  unsigned char XOR(unsigned char S,
                    unsigned char T); // bitwise XOP between two registers
  unsigned char rotate(
      unsigned char R,
      unsigned char X); // rotates register's bits to right by the given number
  void jump2(unsigned char R, string XY); // if the given register is greater
                                          // than register[0] hump to memory XY
  static bool read_and_validate_yes_no(); // [Y-N] input for steps
public:
  friend ostream &
  operator<<(ostream &o,
             const VM &vm); // operator overload to print Memory and registers
  void wipe();              // The variables used in operate() are reset
  void delete_file();       // Delete file
  bool is_loaded() const { return file_is_loaded; }

  void read_file();          // store instructions in memory if they're valid
  void operate(bool _steps); // operate by steps or by whole
};

class VM_UI {
private:
  bool isRunning = true;
  VM vole_machine;
  enum class enMainMenuOption; // Contains options in a human-readable format
  // Function to perform the selected main menu option
  void PerformMainMenuOption(const enMainMenuOption &MainMenuOption);

public:
  static short ReadUserChoice(
      const short &From,
      const short &To); // Function to read user choice within a given range
  static void PauseScreen(); // Function to pause the screen
  static void ClearScreen(); // Function to clear the screen
  void ShowMainMenu();       // Function to display the main menu
};

#endif // VOLEMACHINE_VOLE_MACHINE_H
