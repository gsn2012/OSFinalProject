#include "functions.h"

int main(int argc, char *argv[])
{
	unsigned int *buffer;
	int fd;
	long long int block_size;
	long long int block_count;
	long long int file_size = 0;
	unsigned int xor = 0;
	int cached = 0;

	struct stat st;
	char* filename;

	if(argc <= 1)
	{
		printf("[ERROR]Not enough arguments!\n");
		return 0;
	}

	filename = argv[1];
	block_size = atoll(argv[2]);
	cached = atoi(argv[3]);

	stat(filename, &st);
	file_size = st.st_size;
	printf("Block Size: %lld\n", block_size);

	//block_count = measure_srch(filename, fd, buffer, block_size, file_size/block_size, cached);	
	block_count = measure_binsrch(filename, fd, buffer, block_size, 1 , file_size/block_size, cached);
	
	printf("Block Count: %lld\n", block_count);
	if(block_count == -1)
	{
		printf("File size is too small to run for reasonable time. Please increase file size.\n");
	}
	return 0;
}
