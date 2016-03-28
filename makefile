main: lab4_sol_v1.c
	mpicc lab4_sol_v1.c Lab4_IO.c -o main_v1 -lm
	mpicc lab4_sol_v2.c Lab4_IO.c -o main_v2 -lm
	gcc serialtester.c Lab4_IO.c -o serialtester -lm
