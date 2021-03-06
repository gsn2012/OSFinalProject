# Introduction
This is the final project for the Intro to OS class. 

AIM : The project is centered around performance.
We will try to get disk I/O as fast as possible and evaluate the effects of caches and the cost of system calls. In the end we will try to optimize our program so that it can efficiently read a file using multithreading and other optimization techniques.


## Compilation 

Use build to compile all the files.

```
./build
```
After compiling all the files we can now test them. We have the following files which can be tested:

1. run  
2. run2
3. final_performance
4. fast


### Run
```
./run <filename> [-r|-w] <block_size> <block_count>
```

### Measurement
<mark>NOTE</mark> : 
1. To measure the cached readings set the cached_flag to 1
2. To measure the non-cached readings set the cached_flag to 0
```
./run2 <filename> <block_size> <cached_flag>
```
Returns block_count

### Performance
```
./final_performance <filename> <start_block_size> <end_block_size> <cached_flag>
```

Makes a csv file in which all the reading are captured for block size starting from 'start_block_size' to 'end_block_size'

### System call

```
./syscall <filename> <no_of_itertions>
```

### Final Raw performance
```
./fast <filename>
```

Print performance speed in MiB/s 
