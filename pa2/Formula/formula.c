/*
	Creator: Chris Mathew
	School: Rutgers University: New Brunswick
	Class: Computer Architecture CS 211
	Professor: Brian Russell
	Semester: Summer 2017
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "nCr.h"

int main(int argc, char** argv){
	struct timeval tv_start, tv_end; 
	unsigned long int n, i, elapsedTime;
	
	gettimeofday(&tv_start,NULL);

	
	if(strcmp(argv[1], "-h") == 0){
		printf("Usage: formula<power>\n");
		return 0;
	}

	
	n = atoi(argv[1]);
	if(n < 0||n > 65){
		printf("Error: Bads Input. Power should be greater than or equal to zero, but less than 66 to prevent overflows\n");
		return 0;
	}

	
	if(argc != 2){
    	printf("ERROR: Too many arguements.  Provide one integer arguement.\n");
    	return 0;
  	}
	
	
	printf("(1 + x)^%lu = 1", n);
	for(i = 1; i <= n; i++){
		printf(" + %lu*x^%lu", nCr(n,i),i);
	}
	printf("\n");

	gettimeofday(&tv_end,NULL);

	elapsedTime = ((tv_end.tv_sec - tv_start.tv_sec) * 1000000) + (tv_end.tv_usec - tv_start.tv_usec);
	printf("Time Required = %lu microsecond\n",elapsedTime);

	return 0;
}
