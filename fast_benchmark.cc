
#include "functions.h"
#include <pthread.h>
#include <benchmark/benchmark.h>

#define NUM_THREADS 128
char* filename;
long long int file_size = 0;
long long int block_size = 131072;
unsigned int xor_values[NUM_THREADS];

//void read_from_file(long long int id, long long int index)
void read_from_file(long long int id, long long int count)
{
        int fd;
        if((fd = open(filename, 0)) < 0)
        {
                printf("[ERROR]Cannot open file to read!\n");
                return;
        }
        unsigned int* buffer;
        buffer = (unsigned int*)malloc(block_size);
        long long int n;
        long long int index = id*count*block_size;
        //printf("Index: %lld\n", index);

        while(count--)
        {
                if((n = pread(fd, buffer, block_size, index)) > 0)
                {
                        xor_values[id] ^= xorbuf(buffer, n);
                }
                if(n == 0)
                {
                        break;
                }
                index+=n;
        //printf("Next index: %lld\n", index);
        }
        close(fd);
}


void* read_phase(void* arg)
{
        long long int id = (long long int)arg;
        long long int i;
        long long int count = file_size/(NUM_THREADS*block_size)+1;
        //printf("id: %lld Count: %lld\n", id, count);
        //printf("Thread no. %lld\n", id);

        //for(i = id*block_size; i < file_size; i+=(NUM_THREADS*block_size))
        //{
                //printf("ID: %lld index: %lld\n", id, i);
                //read_from_file(id, i);
                read_from_file(id, count);
        //}
        pthread_exit(NULL);
}



int fast()
{

        double start, end;
        long long int i;

        unsigned int *buffer;
        int fd;
        long long int block_count;
        unsigned int xor2 = 0;
        pthread_t *threads;

        struct stat st;

         
        for(i = 0;i < NUM_THREADS;i++)
        {
                xor_values[i] = 0;
        }

        
        filename = "sample.txt";

        start = now();
        stat(filename, &st);
        file_size = st.st_size;
        printf("File size is %lld\n", file_size);

        threads = (pthread_t *) malloc(sizeof(pthread_t)*NUM_THREADS);
        if (!threads)
        {
                printf("Out of memory allocating thread handles!!");
                return 0;
        }
        for (i = 0; i < NUM_THREADS; i++)
        {
                pthread_create(&threads[i], NULL, read_phase, (void *)i);
        }

        for (i = 0; i < NUM_THREADS; i++)
        {
                pthread_join(threads[i], NULL);
        }
        for(i = 0;i < NUM_THREADS;i++)
        {
                xor2 ^= xor_values[i];
        }
        end = now();
        printf("[XOR]%08x\n", xor2);
        printf("Time taken: %f\n", (end-start));
        long long int mib = file_size/(1024*1024);
        printf("Performance: %f \n", mib/(end-start));
return 0;
}



static void fast_benchmark(benchmark::State& state) {
  for (auto _ : state)
    fast();
}
BENCHMARK(fast_benchmark);

BENCHMARK_MAIN();
