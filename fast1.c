#include "functions.h"

int main(int argc, char *argv[])
{

	double start, end;
	int i, mode = -1;

	unsigned int *buffer;
	int fd;
	long long int block_size = 1048576;
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

	start = now();
	stat(filename, &st);
	file_size = st.st_size;
	printf("File size is %lld\n", file_size);

	if((fd = open(filename, 0)) < 0)
	{
		printf("[ERROR]Cannot open file to read!\n");
		return 0;
	}

	buffer = (unsigned int*)calloc(block_size, sizeof(char));
	int n;

	while((n = read(fd, buffer, block_size)) > 0)
	{
		xor ^= xorbuf(buffer, n);
	}
	close(fd);

	printf("[XOR]%08x\n", xor);
	end = now();
	printf("Time taken: %f\n", (end-start));
	long long int mib = file_size/(1024*1024);
	printf("Performance: %f\n", mib/(end-start));
	return 0;
}
