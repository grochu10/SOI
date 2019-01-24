#include <stdio.h>
#include <lib.h>
#include <math.h>


#define SPRIO_OFF	 0	/* Podpriorytet dla nieuzwanego procesu*/
#define SPRIO_NORM	 1	/* Podpriorytet dla normalnego procesu*/
#define SPRIO_BACK       2	/* Podpriorytet dla procesu w tle*/

int test(int);
void delay(void);

/*glowna petla programu*/
int main(int argc, char* argv[])
{
	int n_norm = 0;
	int n_back = 0;

/* Gdy nie podano argumentow */
	if(argc == 1)
	{
		printf("No argument. Give argument\n"); 
		return -1;
	}

	n_norm = atoi(argv[1]);
	n_back = atoi(argv[2]);

/*Tworzenie i wywolywanie procesow normalnych */
	
	for(;n_norm>0; n_norm--)

	if( fork()==0 )
	{
		message mess;
		int res;	
		mess.m1_i2 = SPRIO_NORM;
		/* ( nr serw, nr wyw sys, wskaznik na strukture message) */
		res = _syscall(MM, SETPRIO, &mess); 
		exit(test(SPRIO_NORM));
	}

/*Tworzenie i wywolywanie procesow tla */
	
	for(;n_back>0; n_back--)

	if( fork()==0 )
	{
		message mess;
		int res;	
		mess.m1_i2 = SPRIO_BACK;
		/* ( nr serw, nr wyw sys, wskaznik na strukture message) */
		res = _syscall(MM, SETPRIO, &mess); 
		exit(test(SPRIO_BACK));
	}

	return 0;
}

/*funkcja do obslugi wyswietlania o aktualnym stanie danego procesu*/
int test(int sprio)
{
	int n = 0;
	int i = 0;
	int pri = 0;

	switch(sprio)
	{
	case 1:
		printf("Poczatek procesu normalnego: %d\n", getpid()); 
		break;
	case 2:
		printf("Poczatek procesu tla: %d\n", getpid()); 
		break;
	}
	for(n = 0;n < 30;++n)
	{
		switch(sprio)
		{
			case 1:
				printf("Petla nr %d procesu normalnego numer %d\n",n, getpid());
				delay(); 
				break;
			case 2:
				printf("Petla nr %d procesu tla numer %d\n",n, getpid());
				delay();
				break;
		}
	}
	switch(sprio)
	{
		case 1:
			printf("Koniec procesu normalnego: %d\n", getpid()); 
			break;
		case 2:
			printf("Koniec procesu tla: %d\n", getpid()); 
			break;
	}
	return 0;
}

/*czasochlonna funkcja dla procesow*/
void delay(void)
{
	int n = 0;
	for(n = sin(getpid())+n;n < 100000000;++n);
}
