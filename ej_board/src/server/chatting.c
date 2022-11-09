#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "array.h"
#include "server.h"
#include "hash.h"


int chatting(LPDATA vuser, int sd)
{
	struct epoll_event ev;
	char buf[1024];
	LPMANAGER user=(LPMANAGER)vuser;
	
	char user_sd[64];
	LPUSERDATA userinfo;
	sprintf(user_sd,"%d",sd);
	hashGetValue(user->curUserListHT,user_sd,(LPDATA*)&userinfo);
	//printf("read data:[%s] %s\n",userinfo->id,buf);
	//printf("chatUsercout %d\n",user->chatUserCount);
	for(int j = 0; j< user->chatUserCount; j++){
		if(user->cust[j].sockfd == -1){
			//printf("1 exe\n");
			continue;
		}
		else if(user->cust[j].sockfd == sd){
			//printf("2 exe\n");
			continue;
		}
		else{
			//write(client_list[i].data.fd, buf_in, readn);
			sprintf(buf,"[%s]가입장했습니다.\n",userinfo->id);
			write(user->cust[j].sockfd,buf, strlen(buf));
		}

	}
	usleep(100);
	send(sd,"clear",6,0);
	usleep(100000);
	sprintf(buf,"[채팅방에 입장했습니다]\n종료시\"...\"입력해 주세요\n");
	send(sd,buf,strlen(buf),0);
	sprintf(buf," -현재 유저 목록입니다.\n -접속유저:");
	usleep(100);
	send(sd,buf,strlen(buf),0);
	int currentUserCount = 0;
	for(int j = 0; j< user->chatUserCount; j++){

		if(user->cust[j].sockfd == -1){
			//printf("1 exe\n");
			continue;
		}
		else if(user->cust[j].sockfd == sd){
			//printf("2 exe\n");
			continue;
		}
		else{
			//write(client_list[i].data.fd, buf_in, readn);
			sprintf(user_sd,"%d",user->cust[j].sockfd);
			hashGetValue(user->curUserListHT,user_sd,(LPDATA*)&userinfo);
			sprintf(buf,"[%s] ",userinfo->id);
			usleep(100);
			send(sd,buf,strlen(buf),0);
			currentUserCount++;
			//write(user->cust[j].sockfd,buf, strlen(buf));
		}

	}
	if(currentUserCount == 0){

		sprintf(buf,"현재 접속자가 없습니다.\n");
		usleep(100);
		send(sd,buf,strlen(buf),0);
	}

	sprintf(buf,"\n");
	usleep(100);
	send(sd,buf,strlen(buf),0);



	int current_count;
	ev.events = EPOLLIN;
	ev.data.fd = sd ;
	epoll_ctl(user->efd,EPOLL_CTL_ADD,sd,&ev);
	current_count = user->chatUserCount;
	user->cust[user->chatUserCount++].sockfd  = sd;
	while(1){
		if(user->cust[current_count].sockfd == -1)
			break;

	}


	return 2;

}

