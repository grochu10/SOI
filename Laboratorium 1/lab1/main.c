#include <stdio.h>
#include <lib.h>

int getprocnr( int );

int main (int argc, char* argv[])
{
	int res;
	int n = 10;
	int a;
	int i;

	/* Gdy nie podano argumentu */
	if(argc == 1)
	{
		printf("No argument. Give argument\n"); 
		return -1;
	}

	a = atoi(argv[1]);
	for(i = 0;i < 10;i++)
	{
		res = getprocnr(a+i-n);
		printf("PID: %d --> index: ", a+i-n);
		if(res !=-1) 
			printf("%d\n",res);
		else 
			printf("NOT FOUND!, ERROR: %d\n",errno);
	}

	return 0;
} 

int getprocnr( int pr_id )
{
	message msg;	
	msg.m1_i1 = pr_id;

	return _syscall( MM, GETPROCNR, &msg ); 
}
