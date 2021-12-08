#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>

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

void read_file(char* filename, int fd, unsigned int *buffer, int block_size, int block_count)
{
	if((fd = open(filename, 0)) < 0)
	{
		printf("[ERROR]Cannot open file to read!\n");
		return;
	}
	buffer = (unsigned int*)calloc(block_size, sizeof(int));
	int n;
	long long size = 0;
	unsigned int xor = 0;

	while(block_count--)
	{
		if((n = read(fd, buffer, block_size)) > 0)
		{
			size += block_size;	
			xor ^= xorbuf(buffer, block_size);
		}
	}
	close(fd);
}

int main(int argc, char *argv[])
{

	double start, end;
	unsigned int *buffer;
	int fd;
	long long int block_size;
	long long int block_count;
	long long int start_block_count;
	long long int end_block_count;
	long long int file_size = 0;
	long long int final_size = 0;
	double final_time = 0;
	int iter = 3;

	struct stat st;
	char* filename;

	if(argc <= 1)
	{
		printf("[ERROR]Not enough arguments!\n");
		return 0;
	}

	filename = argv[1];
	block_size = atoi(argv[2]);

	stat(filename, &st);
	file_size = st.st_size;
	printf("File size is %lld\n", file_size);
	printf("Block Size: %lld\n", block_size);
	start_block_count = 1;
	end_block_count = file_size/block_size;

	while(start_block_count < end_block_count)
	{
		long long int mid = start_block_count + (end_block_count - start_block_count)/2;

		//system("sudo sh -c \"/usr/bin/echo 3 > /proc/sys/vm/drop_caches\" >/dev/null 2>&1");
		start = now();
		read_file(filename, fd, buffer, block_size, mid);
		end = now();
		if((end-start) < 5.0)
		{
			start_block_count = mid + 1;
		}
		else if((end-start) >= 5.0 && (end-start) <= 10.0)
		{
			double time = 0;
			int i;
			for( i = 0; i < iter; i++)
			{
				//system("sudo sh -c \"/usr/bin/echo 3 > /proc/sys/vm/drop_caches\" >/dev/null 2>&1");
				start = now();
				read_file(filename, fd, buffer, block_size, mid);
				end = now();
				time += (end-start);
			}
			block_count = mid;
			final_size = (block_size*mid);
			final_time = time/(double)iter;
			printf("Block_count: %lld Filesize: %lld Time: %f\n\n", block_count, final_size, final_time);
			end_block_count = mid - 1;
		}
		else if((end-start) > 10.0)
		{
			end_block_count = mid - 1;
		}
	}

	long long int megabyte_size = final_size/1048576.0;
	printf("[FINAL]Block_count: %lld Filesize: %lld bytes Time: %f seconds Performance: %f MiB/s\n\n", block_count, final_size, final_time, megabyte_size/final_time);
	return 0;
}
