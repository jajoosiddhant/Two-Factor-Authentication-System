#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

char str[50] = "/usr/bin/python sms-send.py satya ";    
int main()

{
	srand(time(NULL));
	int r_no[4];
	for(int i = 0 ; i < 4; i++)
	{
		r_no[i] = rand() % 10;
		printf("Random number %d\n", r_no[i]);
	}
	uint16_t otp;
	otp = (r_no[0] & 0x0F) << 12;
	printf("otp %x\n", otp);
	otp = otp | (r_no[1] & 0x0F) << 8;
	printf("otp %x\n", otp);
	otp = otp | (r_no[2] & 0x0F) << 4;
	printf("otp %x\n", otp);
	otp = otp | (r_no[3] & 0x0F);
	printf("otp %x\n", otp);
	
	char p[4];	
	sprintf(p,"%x", otp);
	//printf("Otp %d\n", otp); 
	strcat(str, p);
	printf("String %s\n", str);
	char* paramsList[] = { "/bin/bash", "-c", str, NULL};
    	//execv("/bin/bash",paramsList); 
	system(str);
	while(1);
    	return 0;

}
 
