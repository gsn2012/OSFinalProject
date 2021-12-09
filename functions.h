//Header file for functions.c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>

double now();
unsigned int xorbuf(unsigned int *buffer, int size);
unsigned int read_file(char* filename, int fd, unsigned int *buffer, long long int block_size, long long int block_count);
void write_file(char* filename, int fd, unsigned int *buffer, long long int block_size, long long int block_count);
long long int measure_srch(char* filename, int fd, unsigned int* buffer, long long int block_size, long long int max_count, int cached);
long long int measure_binsrch(char* filename, int fd, unsigned int* buffer, long long int block_size, long long int start, long long int end, int cached);
double performance(char* filename, int fd, unsigned int* buffer, long long int block_size, long long int block_count, int cached);
