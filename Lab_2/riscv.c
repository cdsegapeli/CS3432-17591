#include <stdio.h>
#include <stdbool.h>
#include <string.h> // strtok and fgets
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "memory.h" // built-in functions to read and write to a specific file

int32_t* reg; // Array of 32 32-bit registers

void init_regs();
bool interpret(char* instr);
void write_read_demo();

/**
 * Initialize register array for usage.
 * Malloc space for each 32-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
	int reg_amount = 32;
	reg = malloc(reg_amount * sizeof(int32_t)); // 32 * 4 bytes
	for(int i = 0; i < 32; i++)
		reg[i] = i;
}

//function to convert a decimal string to an int value
int string_to_int(char* str){
	int result = 0;
	char* temp = str;

	while(*temp != '\0'){
		result = result * 10 + (*temp - '0');
		temp++;
	}
	return result;
}

bool string_comp(char* str1, char* str2){
	char* temp1 = str1;
	char* temp2 = str2;

	while(*temp1 != '\0' || *temp2 != '\0'){
		if(*temp1 != *temp2){
			return false;
		}
		temp1++;
		temp2++;
	}
	return true;
}

int find_instruction(char* str){
	char* temp = str;
	if(string_comp(temp, "ADD")){
		return 1;
	}
	else if(string_comp(temp, "ADDI")){
		return 2;
	}
	else if(string_comp(temp, "LW")){
		return 3;
	}
	else if(string_comp(temp, "SW")){
		return 4;
	}
	return 5;
}

/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */
bool interpret(char* instr){
	char* mem_file = "mem.txt";
	char* tokens = strtok(instr, " ");
	int rd;
	int rs1;
	int rs2;
	int32_t imm_val;
	int32_t address;

	// find what instruction was given
int instruction = find_instruction(tokens);

	switch(instruction){
		case 1:
			//printf("ADD instruction\n");
			// get the next token (rd)
			tokens = strtok(NULL, " ");
			tokens++;
			// convert the register to an int value
			rd = string_to_int(tokens);
			//printf("RD: %d\n", rd);

			// get the next token (rs1)
			tokens = strtok(NULL, " ");
			tokens++;
			rs1 = string_to_int(tokens);
			//printf("RS1: %d\n", rs1);

			// get the next token (rs2)
			tokens = strtok(NULL, ";");
			tokens++;
			rs2 = string_to_int(tokens); //+11
			//printf("RS2: %d\n", rs2);
			
			// add rs1 and rs2 and put it in rd
			reg[rd] = reg[rs1] + reg[rs2];
			break;
		
		case 2:
			printf("ADDI instruction\n");
			tokens = strtok(NULL, " ");
			tokens++;
			rd = string_to_int(tokens);

			tokens = strtok(NULL, " ");
			tokens++;
			rs1 = string_to_int(tokens);

			tokens = strtok(NULL, ";");
			imm_val = string_to_int(tokens);

			reg[rd] = reg[rs1] + imm_val;
			break;

		case 3:
			//printf("LW instruction\n");
			tokens = strtok(NULL, " ");
			tokens++;
			rd = string_to_int(tokens);

			tokens = strtok(NULL, "(");
			imm_val = string_to_int(tokens) * 2;

			tokens = strtok(NULL, ")");
			tokens++;
			rs1 = string_to_int(tokens);

			address = reg[rs1] + imm_val;

			reg[rd] = read_address(address, mem_file);
			break;

		case 4:
			//printf("SW instruction\n");
			tokens = strtok(NULL, " ");
			tokens++;
			rs2 = string_to_int(tokens);

			tokens = strtok(NULL, "(");
			imm_val = string_to_int(tokens) * 2;

			tokens = strtok(NULL, ")");
			tokens++;
			rs1 = string_to_int(tokens);

			address = reg[rs1] + imm_val;
			write_address(reg[rs2], address, mem_file);
			break;

		case 5:
			printf("unable to interpret instruction.");
			return false;
	}
	return true;
}

void print_regs()
{
	int col_size = 10;
	for (int i = 0; i < 8; i++)
	{
		printf("X%02i:%.*lld", i, col_size, (long long int)reg[i]);
		printf(" X%02i:%.*lld", i + 8, col_size, (long long int)reg[i + 8]);
		printf(" X%02i:%.*lld", i + 16, col_size, (long long int)reg[i + 16]);
		printf(" X%02i:%.*lld\n", i + 24, col_size, (long long int)reg[i + 24]);
	}
}

/**
 * Simple demo program to show the usage of read_address() and write_address() found in memory.c
 * Before and after running this program, look at mem.txt to see how the values change.
 * Feel free to change "data_to_write" and "address" variables to see how these affect mem.txt
 * Use 0x before an int in C to hardcode it as text, but you may enter base 10 as you see fit.
 */
void write_read_demo(){
	int32_t data_to_write = 0xFFF; // equal to 4095
	int32_t address = 0x98; // equal to 152
	char* mem_file = "mem.txt";

	// Write 4095 (or "0000000 00000FFF") in the 20th address (address 152 == 0x98)
	int32_t write = write_address(data_to_write, address, mem_file);
	if(write == (int32_t) NULL)
		printf("ERROR: Unsucessful write to address %0X\n", 0x40);
	int32_t read = read_address(address, mem_file);

	printf("Read address %lu (0x%lX): %lu (0x%lX)\n", address, address, read, read); // %lu -> format as an long-unsigned
}

/**
 * Your code goes in the main
 *
 */
int main(){
	// Do not write any code between init_regs
	init_regs(); // DO NOT REMOVE THIS LINE

	// Below is a sample program to a write-read. Overwrite this with your own code.
	//write_read_demo();

	print_regs();

	// Below is a sample program to a write-read. Overwrite this with your own code.
	//write_read_demo();

	printf(" RV32 Interpreter.\nType RV32 instructions. Use upper-case letters and space as a delimiter.Use a ';' to signify the end of an instruction\nEnter 'EOF' character to end program\n");

	char *instruction = malloc(1000 * sizeof(char));
	bool is_null = false;
	// fgets() returns null if EOF is reached.
	is_null = fgets(instruction, 1000, stdin) == NULL;
	while (!is_null)
	{
		interpret(instruction);
		printf("\n");
		print_regs();
		printf("\n");

		is_null = fgets(instruction, 1000, stdin) == NULL;
	}

	printf("Good bye!\n");

	return 0;
}
