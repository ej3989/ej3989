#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include "hash.h"
#include <time.h>
#include "menu.h"
//#include "member.h"
#include "server.h"

#define BACKLOG 1024
//#define MAXCLIENT 128

MANAGER manager;

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
//	getchar();
	while(select_menu_num){
		//menuCreate(&lpMenu,"input1.txt");
		send(sd,"clear",6,0);
		menuCreate(&lpMenu,inputfile[select_menu_num]);
		select_menu_num = menuRun(lpMenu,(LPDATA)&manager,sd);
		menuDestroy(lpMenu);
	}
	char key[20];
	sprintf(key,"%d",sd);
	hashRemoveKey(manager.curUserListHT,key);

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
	struct epoll_event ev, *client_list;
	//////////////////////////////////////////////////////////////////////////////	
	//BOARD board_main;
	//USER server_user;
	time_t temp_time;
	int nErr;
	
	board_init(&manager);
	user_init(&manager);
	//loadUserData(&manager);
	//loadBoardData(&manager);
	loadUserData_file(&manager);
	loadBoardData_file(&manager);
	//time(&temp_time);/////////////////////////////////////////////////////////////////////
	////////////////////////////hash 초기화 현재user를 SD 기반으로 넣는 다. //////////////
	nErr = hashCreate(&manager.curUserListHT);
	if (ERR_HASH_OK != nErr) {
		printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
		return 0;
	}
	/////////epoll FD 등록 
	if((manager.efd = epoll_create(100))<0){
		perror("epoll_create");
		exit(1);
	}
	client_list = (struct epoll_event *)malloc(sizeof(struct epoll_event *)*MAXCLIENT);
	for(int i = 0 ; i < MAXCLIENT ; i++){
		manager.cust[i].sockfd = -1;

	}
	manager.chatUserCount = 0;
	////////////////////////////////////////////////////////////////////////////////////////
	//	printf("%p array \n",*board_main.post_list);
	printf("star server\n");

	//tempPrintPost(&manager);
	tempPrintUser(&manager);
	//////////////////////////////////////////////////////////////////////////////////
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
	
	ev.events = EPOLLIN;
	ev.data.fd = sockfd;
	epoll_ctl(manager.efd,EPOLL_CTL_ADD,sockfd,&ev);
	////command 창 입력시, 데이터 처리 추가 ..
	//ev.events = EPOLLIN;
	//ev.data.fd = 1;
	//epoll_ctl(manager.efd,EPOLL_CTL_ADD,1,&ev);
	

	///////
	while(1)
	{
		int n;
		int i;
		int readn;
		char buf[1024];
		char tempbuf[1200];
		if((n = epoll_wait(manager.efd,client_list,MAXCLIENT,-1))==-1){
			perror("epoll_wait");
		}
		for(i = 0;i<n;i++){
			if(client_list[i].data.fd == sockfd){
				//
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
				} //
			}/*else if(client_list[i].data.fd == 1){
				printf("뭔가 입력 됐습니다.:%s",stdout);
				
			}*/else{//각 클라이언트 일때 전체 클라이언트에게 메세지를 전달 한다. 

				readn = read(client_list[i].data.fd, buf, 256);
				buf[readn] ='\0';
				if(readn <= 0 ){
					epoll_ctl(manager.efd, EPOLL_CTL_DEL, client_list[i].data.fd, client_list);
					for(int j=0; j<manager.chatUserCount;j++){
						if(manager.cust[j].sockfd == client_list[i].data.fd){
							manager.cust[j].sockfd = -1;
							break;
						}
					}
					//sprintf(tempbuf,"%d",client_list[i].data.fd);
					//hashRemoveKey(manager.curUserListHT,tempbuf);
					//printf("Close fd\n");
					//close(client_list[i].data.fd);
					//다주석처리하면 다같이 강제 종료 됨 
				}else if(!strncmp(buf,"...",3)){
					char user_sd[64];
					LPUSERDATA userinfo;
					sprintf(user_sd,"%d",client_list[i].data.fd);
					hashGetValue(manager.curUserListHT,user_sd,(LPDATA*)&userinfo);
					printf("read data:[%s] %s\n",userinfo->id,buf);
					//printf("chatUsercout %d\n",manager.chatUserCount);
					for(int j = 0; j< manager.chatUserCount; j++){
						if(manager.cust[j].sockfd == -1){
							//printf("1 exe\n");
							continue;
						}
						else if(manager.cust[j].sockfd == client_list[i].data.fd){
							//printf("2 exe\n");
							continue;
						}
						else{
							//write(client_list[i].data.fd, buf_in, readn);
							sprintf(tempbuf,"[%s]퇴장하였습니다.\n",userinfo->id);
							write(manager.cust[j].sockfd,tempbuf, strlen(tempbuf));
						}

					}
					epoll_ctl(manager.efd, EPOLL_CTL_DEL, client_list[i].data.fd, client_list);
					for(int j=0; j<manager.chatUserCount;j++){
						if(manager.cust[j].sockfd == client_list[i].data.fd){
							manager.cust[j].sockfd = -1;
							break;
						}
					}
					printf("Close chatting\n");

				}else {
					char user_sd[64];
					LPUSERDATA userinfo;
					sprintf(user_sd,"%d",client_list[i].data.fd);
					hashGetValue(manager.curUserListHT,user_sd,(LPDATA*)&userinfo);
					printf("read data:[%s] %s\n",userinfo->id,buf);
					//printf("chatUsercout %d\n",manager.chatUserCount);
					for(int j = 0; j< manager.chatUserCount; j++){
						if(manager.cust[j].sockfd == -1){
							//printf("1 exe\n");
							continue;
						}
						else if(manager.cust[j].sockfd == client_list[i].data.fd){
							//printf("2 exe\n");
							continue;
						}
						else{
							//write(client_list[i].data.fd, buf_in, readn);
							sprintf(tempbuf,"[%s]%s\n",userinfo->id,buf);
							write(manager.cust[j].sockfd,tempbuf, strlen(tempbuf));
						}

					}
				}

			}

		}
	}

	//hash free 동적memory free 해야 함 
	return 0;
}
