#include "functions.h"

int main(int argc, char *argv[])
{
	unsigned int *buffer;
	int fd;
	long long int block_count;
	int ans;
	int ans2;
	double start,end,start2,end2;
	char* filename;
	struct stat st;

	if(argc <= 1)
	{
		printf("[ERROR]Not enough arguments!\n");
		return 0;
	}

	filename = argv[1];
	block_count = atoll(argv[2]); // number of iterations


	start = now();
	ans = measure_uid(block_count);	
	end = now();
	start2 = now();
	ans2 = measure_lseek(filename,block_count);	
	end2 = now();

	printf("GETUID : Number of calls per sec is  %f, Time %f \n", ans/(end-start),(end-start));
	printf("LSEEK: Number of calls per sec is  %f, Time %f \n",  ans2/(end2-start2),(end2-start2));

	return 0;
}

