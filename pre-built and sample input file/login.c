#include<stdio.h>

int you_shall_not_pass = 0x7fffffff;

int main()
{
	
	int pass=123;
	int enter=555;
	
	printf("please enter the password:\n");
	scanf("%d",&enter);
	if(enter==pass){
		printf("Secret flag: If winning isn't everything why do they keep score.\n");
	}
	else{
		printf("Wrong password...\n");
	}
	
	if(you_shall_not_pass){
		printf("You shall not pass.\n");
	}
	else{
		printf("Secret flag: Please enjoy the pain which is unable to avoid.\n");	
	}
	
	return 0;
	
}