#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include "menu.h"
//#include "member.h"

#define BACKLOG 1024


void* start_main(void* arg)
{
	LPMENU lpMenu;
	char *inputfile[20]={
		"\0", "input1.txt", "input2.txt", "input3.txt", "input4.txt",
		"input5.txt", "input6.txt", "input7.txt", "input8.txt","input9.txt" };

	int select_menu_num;
	int sd = *((int*) arg);	
	select_menu_num = 1;

	printf("%s \n",inputfile[1]);
	while(select_menu_num){
		//menuCreate(&lpMenu,"input1.txt");
		menuCreate(&lpMenu,inputfile[select_menu_num]);
		select_menu_num = menuRun(lpMenu, sd);
		menuDestroy(lpMenu);
	}

	close(sd);
	return NULL;
}


char line[]="\n─────────────────────────────────────────────────────────────────────\n";
char thick_line[]="\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";

int main(){

	int sockfd, new_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size;
	int yes=1;
	pthread_t tid;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt");
		exit(1);
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(60000);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(my_addr.sin_zero), '\0', 8);

	if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}

	if(listen(sockfd, BACKLOG) == -1) 
	{
		perror("listen");
		exit(1);
	}

	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);

		if((new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size)) == -1)
		{
			perror("accept");
			continue;
		}
		printf("server : got connection from %s \n", inet_ntoa(their_addr.sin_addr));
		if(pthread_create(&tid, NULL, start_main, &new_fd)!=0) {
			perror("pthread_create");
		} else {
			pthread_detach(tid);
		}
	}

	return 0;
}
