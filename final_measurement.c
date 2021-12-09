#include "functions.h"

#define MIB 1048576.0
#define MB 1000000.0

int main(int argc, char *argv[])
{
	unsigned int *buffer;
	int fd;
	FILE* fds;
	long long int block_size_capacity;
	long long int block_size;
	long long int block_count;
	long long int file_size = 0;
	long long int mib_size;
	long long int mb_size;
	double final_time = 0;
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
	block_size_capacity = atoll(argv[3]);
	cached = atoi(argv[4]);

	stat(filename, &st);
	file_size = st.st_size;
	printf("File size is %lld\n", file_size);
	
	while(block_size <= block_size_capacity)
	{
		printf("Block Size: %lld\n", block_size);
		
		block_count = measure_srch(filename, fd, buffer, block_size, file_size/block_size, cached);
		//block_count = measure_binsrch(filename, fd, buffer, block_size, 1 , file_size/block_size, cached);
		
		final_time = performance(filename, fd, buffer, block_size, block_count, cached);
		
		mib_size = (block_size*block_count)/MIB;
		mb_size = (block_size*block_count)/MB;

		fds = fopen("/home/gautam/OSProject/Results_4.csv", "a+");

		if(fds == NULL)
		{
			printf("[ERROR]Cannot not open file fds!!\n");
			return 0;
		}
		else
		{
			fprintf(fds, "%lld,%lld,%lld,%f,%f,%f\n", block_size, block_count, (block_size*block_count), final_time, mib_size/final_time, mb_size/final_time);
		}

		printf("[FINAL]Block_size: %lld Block_count: %lld Filesize: %lld bytes Time: %f seconds Performance: %f MiB/s %f MB/s\n\n", block_size, block_count, (block_size*block_count), final_time, mib_size/final_time, mb_size/final_time);
		block_size*=2;
	}
	fclose(fds);
	return 0;
}
