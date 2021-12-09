#include "functions.h"

double now()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (double)time.tv_sec + ((double)time.tv_usec/1000000.0);
}

unsigned int xorbuf(unsigned int *buffer, int size) 
{
	unsigned int result = 0;
	for (int i = 0; i < size; i++) 
	{
		result ^= buffer[i];
	}
	return result;
}

unsigned int read_file(char* filename, int fd, unsigned int *buffer, long long int block_size, long long int block_count)
{
	if((fd = open(filename, 0)) < 0)
	{
		printf("[ERROR]Cannot open file to read!\n");
		return 0;
	}
	buffer = (unsigned int*)malloc(block_size*sizeof(int));
	int n;
	long long int size = 0;
	unsigned int xor = 0;

	while(block_count--)
	{
		if((n = read(fd, buffer, block_size)) > 0)
		{
			xor ^= xorbuf(buffer, block_size);
		}
	}
	close(fd);
	return xor;
}

void write_file(char* filename, int fd, unsigned int *buffer, long long int block_size, long long int block_count)
{
	if((fd = open(filename, O_CREAT|O_WRONLY, 0666)) < 0)
	{
		printf("[ERROR]Cannot open file to write!\n");
		return;
	}
	buffer = (int*)malloc(block_size*sizeof(int));
	int n, i;
	long long size = 0;

	for( i = 1; i < block_size; i++)
	{
		buffer[i] = (buffer[i-1]*31) + 17; 
	}

	while(block_count--)
	{
		if((n = write(fd, buffer, block_size)) > 0)
		{
			size += block_size;
		}
	}
	printf("Size: %lld\n", size);
	close(fd);
}

long long int measure_srch(char* filename, int fd, unsigned int* buffer, long long int block_size, long long int max_count, int cached)
{
	double start_time, end_time;
	unsigned int xor = 0;
	long long int count = 1;

	while(count <= max_count)
	{
		if(cached == 0)
		{
			system("sudo sh -c \"/usr/bin/echo 3 > /proc/sys/vm/drop_caches\" >/dev/null 2>&1");
		}
		start_time = now();
		xor = read_file(filename, fd, buffer, block_size, count);
		end_time = now();
		printf("Count: %lld File Size: %lld Time: %f \n", count, block_size*count, (end_time-start_time));
		if((end_time-start_time) >= 5.0)
		{
			printf("[FINAL]Block_count: %lld Filesize: %lld bytes Time: %f seconds \n\n", count, (block_size*count), (end_time-start_time));
			return count;
		}
		count *= 2;
	}

	return -1;
}

long long int measure_binsrch(char* filename, int fd, unsigned int* buffer, long long int block_size, long long int start, long long int end, int cached)
{
	double start_time, end_time;
	unsigned int xor = 0;
	long long int mid = -1;
	
	while(start < end)
	{
		mid = start + (end - start)/2;
		
		if(cached == 0)
		{
			system("sudo sh -c \"/usr/bin/echo 3 > /proc/sys/vm/drop_caches\" >/dev/null 2>&1");
		}
		start_time = now();
		xor = read_file(filename, fd, buffer, block_size, mid);
		end_time = now();
		if((end_time-start_time) < 5.0)
		{
			start = mid + 1;
		}
		else if((end_time-start_time) >= 5.0 && (end_time-start_time) <= 15.0)
		{
			printf("[FINAL]Block_count: %lld Filesize: %lld bytes Time: %f seconds \n\n", mid, (block_size*mid), (end_time-start_time));
			return mid;
		}
		else if((end_time-start_time) > 15.0)
		{
			end = mid - 1;
		}
	}
	return -1;
}

double performance(char* filename, int fd, unsigned int* buffer, long long int block_size, long long int block_count, int cached)
{
	double start_time, end_time;
	unsigned int xor = 0;
	double time = 0;

	for(int i = 0 ; i < 5 ; i++)
	{
		if(cached == 0)
		{
			system("sudo sh -c \"/usr/bin/echo 3 > /proc/sys/vm/drop_caches\" >/dev/null 2>&1");
		}
		start_time = now();
		xor = read_file(filename, fd, buffer, block_size, block_count);
		end_time = now();
		time += (end_time-start_time); 
	}

	return time/5.0;
}
