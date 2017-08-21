/*
	Creator: Chris Mathew
	School: Rutgers University: New Brunswick
	Class: Computer Architecture CS 211
	Professor: Brian Russell
	Semester: Summer 2017
*/
#include <stdlib.h> 
#include <stdio.h>
#include "mystery.h"

long int num[200];

long int add(long int num1,long int num2){
	return num1 + num2; //.LFB2
}

long int dothething(long int n){
	long int fib, result;

	if (num[n] != -1){ //.LFB3
		result =  num[n];
	}
	else{
	 	fib = -1;
	 	if (n == 0){ //.L4
	 		fib = 0;
	 		
	 	}
	 	else{
	 		if (n == 1){ //.L6
	 			fib = 1;
	 		}
	 		else {
	 			fib = add(dothething(n - 1), dothething(n - 2)); //.L8
	 		}
	 	}
	 	if (num[n] == -1){ //.L7
	 		num[n] = fib;
	 	}
	 	
	 	result = num[n]; //.L9
	 	
	}
	return result; //.L5
	
	
		

}

int main(int argc, char **argv){
	long int n,i;

	n = atoi(argv[1]); //.LFB4

	for (i = 0; i < 200; i++){
		num[i] = -1; //.L12
	}
	
	printf("Value: %ld\n", dothething(n)); //.L11

	return 0;
}

