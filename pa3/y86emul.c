/* 
	Creator: Chris Mathew
	Project: Y86 Emulator
	School: Rutgers University New Brunswick
	Course: CS 211 Computer Architecture
	Professor: Brian Russell
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "y86emul.h"


//DIRECTIVES

//.text
void textD(char* input, int index) {
    int tokenIndex = 0;
    int currIndex = index;
    int len = strlen(input);

    while (tokenIndex < len){

        bitpair* byte = (bitpair*) memory + currIndex;

        char* high = (char*) malloc(sizeof(char));
        strncpy(high, input + tokenIndex + 1, 1);

        byte->rA = strtol(high, NULL, 16);
        free(high);

        char* low = (char*) malloc(sizeof(char));
        strncpy(low, input + tokenIndex, 1);

        byte->rB = strtol(low, NULL, 16);
        free(low);

        tokenIndex += 2;
        currIndex++;
    }
}

//.byte
void byteD(char* input, int index) {
    int val;
    sscanf(input, "%2x", &val);
    memory[index] = (char) val;
}

//.long
void longD(char* input, int index) {
    int val;
    sscanf(input, "%d", &val);
    memory[index] = val;
}

//.string
void stringD(char* input, int index) {
    int i = 0;
    while (input[i] != '\0') {
        memory[index] = input[i];
        index++;
        i++;
    }
}

//PARSE
void parse(FILE* input) {
    char line[2000];
    int index;
    char* delim = " \t\n\f\v\r";
    int lineIndex = 0;

    while (fgets(line, 2000, input) != NULL) {

    	//gets first token
        char* token = strtok(line, delim);
        
        //Checks if .string
        //If so sets memory

        if(strcmp(token, ".size") == 0){
            token = strtok(NULL, delim);
            sscanf(token, "%x", &index);
            memory = malloc(index);
            
        //Checks if .text
        //If so sets the pc to the first token after conversion 
        //Then take next token, set of instructions, and store in memory
        } else if (strcmp(token, ".text") == 0) {

            token = strtok(NULL, delim);
            sscanf(token, "%x", &index);
            pc = index;

            token = strtok(NULL, delim);
            textD(token, index);

        //Checks if .long
        //If so stores first token into index
        //Then stores second token (value) into token and passes to be stored in memory
        } else if (strcmp(token, ".long") == 0) {

            token = strtok(NULL, delim);
            sscanf(token, "%x", &index);

            token = strtok(NULL, delim);
            longD(token, index);

        //Checks if .byte
        //If so stores first token into index
        //Then stores second token (value) into token and passes to be stored in memory
        } else if (strcmp(token, ".byte") == 0) {

            token = strtok(NULL, delim);
            sscanf(token, "%x", &index);

            token = strtok(NULL, delim);
            byteD(token, index);

        //Checks if .string
        //If so stores first token into index
        //Then stores second token (value) into token and passes to be stored in memory
        } else if (strcmp(token, ".string") == 0) {

            token = strtok(NULL, delim);
            sscanf(token, "%x", &index);
            token = strtok(NULL, delim);
            token++;

            while (token != NULL) {
                char *next = strtok(NULL, delim);

                size_t length;
                if (next != NULL) {
                    length = strlen(token);
                    stringD(token, index);
                    index += length;
                    memory[index] = ' ';
                    index++;
                    token = next;
                } else {
                    length = strlen(token);
                    stringD(token, index);
                    index += length;
                    index++;
                    memory[index] = '\0';
                    break;
                }

            }

        } else {
            fprintf(stderr, "ERROR: Invalid directive.\n");
            exit(1);
        }

        lineIndex++;
        

    }
    

}



//Y86 Instructions 
//No-op : 0x00
void nop() {
    pc += 1;
}

//Sets status to HLT 
void halt(){
    status = HLT;
}

//Puts data in SRC into DST
void rrmovl() {
    bitpair* byte = (bitpair*) (memory + pc + 1);
    int high = byte->rA;
    int low = byte->rB;

    if (high >= 0 && high <= 8 && low >= 0 && low <= 8) {
        registers[high] = registers[low];
        pc += 2;
    } else {
        status = INS;
    }
}

//Puts immediate into register
void irmovl() {
    bitpair* byte = (bitpair*) (memory + pc + 1);
    int index = byte->rA;

    if (byte->rB == 0xf && index >= 0 && index <= 8) {
        int val = *(int *) (memory + pc + 2);
        registers[index] = val;
        pc += 6;
    } else {
        status = INS;
    }
}

//
void rmmovl() {
    bitpair* byte = (bitpair*) (memory + pc + 1);
    int val = *(int *) (memory + pc + 2);
    int high = byte->rA;
    int low = byte->rB;

    if (high >= 0 && high <= 8 && low >= 0 && low <= 8) {
        int highVal = registers[high];
        int lowVal = registers[low];
        int address = highVal + val;
        int *memArray = (int *) (memory + address);
        *memArray = lowVal;
        pc += 6;
    } else {
        status = INS;
    }
}

void mrmovl() {
    bitpair* byte = (bitpair*) (memory + pc + 1);
    int val = *(int *) (memory + pc + 2);
    int high = byte->rA;
    int low = byte->rB;

    if (high >= 0 && high <= 8 && low >= 0 && low <= 8) {
        int valHigh = registers[high];
        int address = valHigh + val;
        registers[low] = *(int *) (memory + address);
        pc += 6;
    } else {
        status = INS;
    }
}

void addl() {
    bitpair* byte = (bitpair*) (memory + pc + 1);
    int high = byte->rA;
    int low = byte->rB;

    if (high >= 0 && high <= 8 && low >= 0 && low <= 8) {
        int highVal = registers[high];
        int lowVal = registers[low];
        int sum = highVal + lowVal;
        registers[high] = sum;

        if (sum == 0) {
            flags[1] = 1; 
        } else {
            flags[1] = 0;
        }

        if ((lowVal > 0 && highVal > 0 && sum < 0) || (lowVal < 0 && highVal < 0 && sum > 0)) {
            flags[0] = 1;
        } else {
            flags[0] = 0;
        }

        if (sum < 0) {
            flags[2] = 1;
        } else {
            flags[2] = 0;
        }

        pc += 2;
    } else {
        status = INS;
    }
}

void subl() {
    bitpair* byte = (bitpair*) (memory + pc + 1);
    int high = byte->rA;
    int low = byte->rB;

    if (high >= 0 && high <= 8 && low >= 0 && low <= 8) {
        int highVal = registers[high];
        int lowVal = registers[low];
        int diff = highVal - lowVal;

        registers[high] = diff;

        if (diff == 0) {
            flags[1] = 1;
        } else {
            flags[1] = 0;
        }

        if ((lowVal < 0 && highVal > 0 && diff < 0) ||
            (lowVal > 0 && highVal < 0 && diff > 0)) {
            flags[0] = 1;
        } else {
            flags[0] = 0;
        }

        if (diff < 0) {
            flags[2] = 1;
        } else {
            flags[2] = 0;
        }

        pc += 2;
    } else {
        status = INS;
    }
}

void andl() {
    bitpair* byte = (bitpair*) (memory + pc + 1);
    int high = byte->rA;
    int low = byte->rB;

    if (high >= 0 && high <= 8 && low >= 0 && low <= 8) {
        int highVal = registers[high];
        int lowVal = registers[low];
        int and = highVal & lowVal;
        registers[high] = and;
        if (and == 0) {
            flags[1] = 1;
        } else {
            flags[1] = 0;
        }
        if (and < 0) {
            flags[2] = 1;
        } else {
            flags[2] = 0;
        }
        flags[0] = 0;
        pc += 2;
    } else {
        status = INS;
    }
}

void xorl() {
    bitpair* byte = (bitpair*) (memory + pc + 1);
    int high = byte->rA;
    int low = byte->rB;

    if (high >= 0 && high <= 8 && low >= 0 && low <= 8) {
        int highVal = registers[high];
        int lowVal = registers[low];
        int xor = highVal ^ lowVal;

        registers[high] = xor;

        if (xor == 0) {
            flags[1] = 1;
        } else {
            flags[1] = 0;
        }

        if (xor < 0) {
            flags[2] = 1;
        } else {
            flags[2] = 0;
        }

        flags[0] = 0;

        pc += 2;
    } else {
        status = INS;
    }
}

void mull() {
    bitpair* byte = (bitpair*) (memory + pc + 1);
    int high = byte->rA;
    int low = byte->rB;

    if (high >= 0 && high <= 8 && low >= 0 && low <= 8) {
        int highVal = registers[high];
        int lowVal = registers[low];
        int product = highVal * lowVal;

        registers[high] = product;

        if (product == 0) {
            flags[1] = 1;
        } else {
            flags[1] = 0;
        }

        if ((lowVal > 0 && highVal > 0 && product < 0) || (lowVal < 0 && highVal < 0 && product < 0) ||
            (lowVal < 0 && highVal > 0 && product > 0) || (lowVal > 0 && highVal < 0 && product > 0)) {
            flags[0] = 1;
        } else {
            flags[0] = 0;
        }

        if (product < 0) {
            flags[2] = 1;
        } else {
            flags[2] = 0;
        }

        pc += 2;
    } else {
        status = INS;
    }
}

void cmpl() {
    pc += 2;
}

void jmp() {
    int val = *(int*) (memory + pc + 1);
    pc = val;
}

void jle() {
    int val = *(int*) (memory + pc + 1);

    if (((flags[2] ^ flags[0]) | flags[1]) == 1) {
        pc = val;
    } else {
        pc += 5;
    }
}

void jl() {
    int val = *(int*) (memory + pc + 1);

    if ((flags[2] ^ flags[0]) == 1) {
        pc = val;
    } else {
        pc += 5;
    }
}

void je() {
    int val = *(int*) (memory + pc + 1);

    if (flags[1] == 1) {
        pc = val;
    } else {
        pc += 5;
    }
}

void jne() {
    int val = *(int*) (memory + pc + 1);

    if (flags[1] == 0) {
        pc = val;
    } else {
        pc += 5;
    }
}

void jge() {
    int val = *(int*) (memory + pc + 1);

    if ((flags[2] ^ flags[0]) == 0) {
        pc = val;
    } else {
        pc += 5;
    }
}

void jg() {
    int val = *(int*) (memory + pc + 1);

    if (((flags[2] ^ flags[0]) & flags[1]) == 0) {
        pc = val;
    } else {
        pc += 5;
    }
}

void call() {
    int val = *(int *) (memory + pc + 1);
    registers[8] = pc + 5;
    push(8);
    pc = val;
}

void ret() {
    pop(8);
    pc = registers[8];
}

void pushl() {
    bitpair* byte = (bitpair*) (memory + pc + 1);

    int registerIndex = byte->rB;
    if (byte->rA == 0xf && registerIndex >= 0 && registerIndex <= 8) {
        push(registerIndex);
    } else {
        status = INS;
    }
}

void push(int edx) {
    int val = registers[edx];
    int esp = registers[4] - 4;
    int *memArray = (int*) (memory + esp);
    *memArray = val;
    registers[4] = esp;
    pc += 2;
}

void popl() {
    bitpair* byte = (bitpair*) (memory + pc + 1);

    int registerIndex = byte->rB;
    if (byte->rA == 0xf && registerIndex >= 0 && registerIndex <= 8) {
        pop(registerIndex);
    } else {
        status = INS;
    }
}

void pop(int edx) {
    int val = registers[4];
    int esp = val + 4;
    int memArray = *(int*) (memory + val);
    registers[4] = esp;
    registers[edx] = memArray;
    pc += 2;
}

void readl() {
    bitpair* byte = (bitpair*) (memory + pc + 1);
    int low = byte->rB;
    int* val = (int *) (memory + pc + 2);
    int input;

    if (byte->rA == 0xf && low >= 0 && low <= 8) {

        flags[0] = 0;
        flags[2] = 0;

        if (scanf("%d", &input) == EOF) {
            flags[1] = 1;
        } else {
            int* longval = (int*) (memory + registers[low] + *val);
            *longval = input;

            flags[1] = 0;
        }

        pc += 6;
    } else {
        status = INS;
    }
}

void readb() {
    bitpair* byte = (bitpair*) (memory + pc + 1);
    int low = byte->rB;
    int* num = (int*) (memory + pc + 2);

    unsigned char input;

    if (byte->rA == 0xf && low >= 0 && low <= 8) {
        flags[0] = 0;
        flags[2] = 0;

        if (scanf("%c", &input) == EOF) {
            flags[1] = 1;
        } else {
            unsigned char* byte = (unsigned char*) (memory + registers[low] + *num);
            *byte = input;
            flags[1] = 0;
        }

        pc += 6;
    } else {
        status = INS;
    }
}

void writeb() {
    bitpair* byte = (bitpair*) (memory + pc + 1);
    int low = byte->rB;
    int val = *(int*) (memory + pc + 2);

    if (byte->rA == 0xf && low >= 0 && low <= 8) {
        unsigned char *byte = (unsigned char *) (memory + registers[low] + val);
        printf("%c", *byte);
        pc += 6;
    } else {
        status = INS;
    }
}

void writel() {
    bitpair* byte = (bitpair*) (memory + pc + 1);
    int low = byte->rB;
    int val = *(int*) (memory + pc + 2);

    if (byte->rA == 0xf && low >= 0 && low <= 8) {
        int *longToWrite = (int *) (memory + registers[low] + val);
        printf("%d", *longToWrite);
        pc += 6;
    } else {
        status = INS;
    }
}
/*void movsbl(){
	bitpair* byte = (bitpair*) (memory + pc + 1);
	int high = byte->rA;
	int low =byte->rB;
	int val = *(int*) (memory + pc + 2);

	if(byte->rA == 0xf & high >=0 && high <= 8){
		registers[low] = (int) (memory[val] == '0x80' ? ((int)memory[val]) + 0xffffff00 : (int)memory[val]);
	}
	pc += 6;
}
*/


/*
	Fetch Decode & Execute
*/

//FETCH
void fetch(FILE* fp){

	if (fp == NULL) {
        fprintf(stderr, "ERROR: The file does not exist.\n");
        exit(1);
    }
    parse(fp);

    fclose(fp);
    status = AOK;

}
//DECODE
void decode(unsigned char *instruction) {

    switch (*instruction) {

        case NOP:
            nop();
            break;

        case HALT:
            halt();
            break;

        case RRMOVL:
            rrmovl();
            break;

        case IRMOVL:
            irmovl();
            break;

        case RMMOVL:
            rmmovl();
            break;

        case MRMOVL:
            mrmovl();
            break;

        case ADDL:
            addl();
            break;

        case SUBL:
            subl();
            break;

        case ANDL:
            andl();
            break;

        case XORL:
            xorl();
            break;

        case MULL:
            mull();
            break;

        case CMPL:
            cmpl();
            break;

        case JMP:
            jmp();
            break;

        case JLE:
            jle();
            break;

        case JL:
            jl();
            break;

        case JE:
            je();
            break;

        case JNE:
            jne();
            break;

        case JGE:
            jge();
            break;

        case JG:
            jg();
            break;

        case CALL:
            call();
            break;

        case RET:
            ret();
            break;

        case PUSHL:
            pushl();
            break;

        case POPL:
            popl();
            break;

        case READB:
            readb();
            break;

        case READL:
            readl();
            break;

        case WRITEB:
            writeb();
            break;

        case WRITEL:
            writel();
            break;

        case MOVSBL:
            mrmovl();
            break;

        default:
            status = INS;
            break;
    }
}

//EXECUTE
int execute(char* memory) {

    while (status == AOK) {
        unsigned char* instruction = (unsigned char*) (memory + pc);
        decode(instruction);
    }
    if(flags[0] == 1 || flags[1] == 1 || flags[2] == 1){
        	return 0;
        }
    switch(status) {
		case HLT:
			fprintf(stderr, "HALT Occured!");
      	  	return 0;
		case ADR:
			fprintf(stderr, "Error: Invalid Address!");
      	  	return 1;
		case INS:
			fprintf(stderr, "ERROR: Invalid Instruction!");
       		return 1;
		default:
			break;
	}
	return 0;
}

//Main Program
int main(int argc, char** argv){
	if(argc != 2){ 	//Case where there is no input
		fprintf(stderr, "Error: You Need Two Arguments!\n");
		return 1;
	}
	if(strcmp(argv[1], "-h") == 0){  //Help case
		printf("Usage: y86emul <y86 input file>\n");
		return 0;
	}
	fp = fopen(argv[1], "r");
	//FETCH
	fetch(fp);
	//EXECUTE
    execute(memory);
    free(memory);
	return 0;
}