#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

int sub(int sd)
{
    int a,b,n;
	char buf[1024];
    
    sprintf(buf,"%s", "SUB 메뉴입니다\n\n");
	send(sd, buf, strlen(buf), 0);

    sprintf(buf, "%s", "a = ");
	send(sd, buf, strlen(buf), 0);
    n = recv(sd, buf, sizeof(buf), 0);
	buf[n] = '\0';
	a=atoi(buf);

    sprintf(buf, "%s", "b = ");
	send(sd, buf, strlen(buf), 0);
    n = recv(sd, buf, sizeof(buf), 0);
	buf[n] = '\0';
	b=atoi(buf);

    sprintf(buf, "%d - %d = %d\n", a, b, a - b);
	send(sd, buf, strlen(buf), 0);
    
    return 0;
}
