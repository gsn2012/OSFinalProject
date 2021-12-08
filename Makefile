measurement:measurement.o
	cc measurement.o -o measurement
measurement.o:measurement.c
	gcc -c measurement.c -o measurement.o
measurement.c:run
	gcc -c run.o -o run
run.o:run.c
	gcc -c run.c -o run.o
run.c:
	echo "run not available"
