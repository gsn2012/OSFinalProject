#include "functions.h"

int main(int argc, char *argv[])
{
	
	double start, end;
	int i, mode = -1;

	unsigned int *buffer;
	int fd;
	long long int block_size;
	long long int block_count;
	long long int file_size = 0;
	unsigned int xor = 0;
	
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

	block_size = atoll(argv[3]);
	block_count = atoll(argv[4]);
	
	start = now();
	if(mode == -1)
	{
		printf("[ERROR]Enter correct parameters!!\n");
		return 0;
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

		printf("Read from file!!\n");
		xor = read_file(filename, fd, buffer, block_size, block_count);
		if(xor < 0)
		{
			return 0;
		}
		printf("[XOR]%08x\n", xor);
	}
	else
	{
		printf("Write to file!!\n");
		write_file(filename, fd, buffer, block_size, block_count);
	}

	end = now();
	printf("Time taken: %f\n", (end-start));
	return 0;
}
