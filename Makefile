
fast: fast.c functions.c run run2 final_performance
	gcc -O3 -pthread  fast.c functions.c -o fast
final_performance:final_performance.c functions.c
	gcc  final_performance.c functions.c -o final_performance
final_performance.c:
	echo "Your file is missing"
run2:run2.c functions.c
	gcc  run2.c functions.c -o run2
run2.c:
	echo "run2 file absent"
run:run.c functions.c
	gcc  run.c functions.c -o run
run.c: 
	echo "run file absent"

functions.c:
	echo "function files absent"


