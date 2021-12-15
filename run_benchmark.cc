#include "functions.h"
#include <benchmark/benchmark.h>

int run_measure(long long int block_size, 	long long int block_count)
{
	
	double start, end;
	int i;

	unsigned int *buffer;
	int fd;

	long long int file_size = 0;
	unsigned int xor2 = 0;
	
	struct stat st;
	char* filename;
	

    filename = "largefile.txt";

	
	start = now();
		
		stat(filename, &st);
		file_size = st.st_size;
		// printf("File size is %lld\n", file_size);
		
		if(file_size < (block_size*block_count))
		{
			printf("[ERROR]File too small. Either change file or change parameters!!\n");
			return 0;
		}

		// printf("Read from file!!\n");
		xor2 = read_file(filename, fd, buffer, block_size, block_count);
		if(xor2 < 0)
		{
			return 0;
		}
		// printf("[xor2]%08x\n", xor2);
	
	end = now();
	// printf("Time taken: %f\n", (end-start));
	return 0;
}


// Define another benchmark
static void aMiB(benchmark::State& state) {
  for (auto _ : state)
    run_measure(134217728,10);
}
BENCHMARK(aMiB);

static void bMiB(benchmark::State& state) {

  for (auto _ : state)
    run_measure(67108864,1);
}
BENCHMARK(bMiB);

static void cMiB(benchmark::State& state) {

  for (auto _ : state)
   run_measure(33554432,1);

}
BENCHMARK(cMiB);
static void dMiB(benchmark::State& state) {

  for (auto _ : state)
    run_measure(16777216,1);

}
BENCHMARK(dMiB);
static void eMiB(benchmark::State& state) {

  for (auto _ : state)
    run_measure(8388608,1);

}
BENCHMARK(eMiB);

BENCHMARK_MAIN();

