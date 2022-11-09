#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include "hash.h"
#include "server.h"

char line[]="\n─────────────────────────────────────────────────────────────────────\n";
char thick_line[]="\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";

int addNewPost(LPDATA vuser, int sd)
{
    int size,n,nErr;
	int selectMenu;
	char buf[2048];
	char temp[20];
	char content[2000]={0,};
	char *tempCommand;
	LPPOST tempPost;
	LPUSERDATA userinfo;
	LPMANAGER user = (LPMANAGER)vuser;
	time_t temp_time;

	tempCommand="clear";
	usleep(100);
	send(sd,tempCommand,strlen(tempCommand),0);
	
	sprintf(temp,"%d",sd);
	nErr = hashGetValue(user->curUserListHT,temp,(LPDATA*)&userinfo);
	
	tempPost = (LPPOST)calloc(sizeof(POST),1);
	strcpy(tempPost->publisher,userinfo->id);

	usleep(100000);
	send(sd,line ,sizeof(line),0);

	sprintf(buf,"[새로운 글 작성]");
	usleep(1000);
	send(sd,buf, strlen(buf),0);
	usleep(100);
	send(sd,line,sizeof(line),0);
	sprintf(buf,"제목 입력:");
	usleep(100);
	send(sd,buf, strlen(buf),0);

	n = recv(sd,buf,sizeof(buf),0);
	buf[n]='\0';
	strcpy(tempPost->post_title,buf);
	//printf("title:%s\n",buf);

	usleep(1000);
	send(sd,line ,sizeof(line),0);

	sprintf(buf,"내용입력:(종료시 \"exit\"를 입력해 주세요 ");
	usleep(100);
	send(sd,buf ,strlen(buf),0);
	
	usleep(100);
	send(sd,line ,sizeof(line),0);
	while(1){
		n = recv(sd,buf,sizeof(buf),0);
		buf[n]='\0';
		if(!strcmp(buf,"exit")){
			break;
		}
		buf[n-1]='\n';
		strcat(content,buf);

	}
	strcpy(tempPost->post_content,content);
	time(&temp_time);
	tempPost->post_time = temp_time;
	tempPost->post_ID = user->total_post;

	nErr = arrayAdd(user->post_list,(LPDATA)tempPost);
	if(nErr != 0)
		printf("error 발생 ");
	user->total_post++;

	//파일에 내용 저장 추가함
	saveBoardData(user);



	return 3;
}

