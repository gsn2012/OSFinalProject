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

unsigned int read_file(int fd, unsigned int *buffer, int block_size, int block_count)
{
	buffer = (unsigned int*)calloc(block_size, sizeof(int));
	int n;
	long long size = 0;
	//int x = 0;
	unsigned int xor = 0;

	while(block_count--)
	{
		if((n = read(fd, buffer, block_size)) > 0)
		{
			size += block_size;	
			xor ^= xorbuf(buffer, block_size);
			//printf("%d %08x\n", x, xor);
			//x++;
		}
	}
	printf("Size: %lld\n", size);
	return xor;
}

void write_file(int fd, unsigned int *buffer, int block_size, int block_count)
{
	buffer = (int*)calloc(block_size, sizeof(int));
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
}

int main(int argc, char *argv[])
{
	
	double start, end;
	int i, mode = -1;

	unsigned int *buffer;
	int fd;
	long long int block_size;
	long long int block_count;
	long long int file_size = 0;
	
	struct stat st;
	char* filename;
	
	if(argc <= 1)
	{
		printf("[ERROR]Not enough arguments!\n");
		return 0;
	}

	filename = argv[1];

	if(argv[2][0] == '-')
	{
		if(argv[2][1] == 'r')
		{
			mode = 0;
		}
		else if(argv[2][1] == 'w')
		{
			mode = 1;
		}
	}	

	block_size = atoi(argv[3]);
	block_count = atoi(argv[4]);
	
	start = now();
	if(mode == -1)
	{
		printf("[ERROR]Enter correct parameters!!\n");
	}		
	else if(mode == 0)
	{
		stat(filename, &st);
		file_size = st.st_size;
		printf("File size is %lld\n", file_size);
		
		if(file_size < (block_size*block_count))
		{
			printf("[ERROR]File too small. Either change file or change parameters!!\n");
			return 0;
		}

		if((fd = open(filename, mode)) < 0)
		{
			printf("[ERROR]Cannot open file to read!\n");
			return 0;
		}

		printf("Read from file!!\n");
		printf("[XOR]%08x\n", read_file(fd, buffer, block_size, block_count));
	}
	else
	{
		if((fd = open(filename, O_CREAT|O_WRONLY, 0666)) < 0)
		{
			printf("[ERROR]Cannot open file to write!\n");
			return 0;
		}
		printf("Write to file!!\n");
		write_file(fd, buffer, block_size, block_count);
	}
	
	close(fd);
	end = now();
	printf("Time taken: %f\n", (end-start));
	return 0;
}
