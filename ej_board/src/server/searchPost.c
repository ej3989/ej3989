#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include "array.h"
#include "hash.h"
#include "server.h"

char line[]="\n─────────────────────────────────────────────────────────────────────\n";
char thick_line[]="\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";

int searchPost(LPDATA vuser, int sd)
{
    int size,n,nErr;
	int selectMenu;
	char buf[2048];
	char temp[20];
	char buftemp[256];
	char *tempCommand;
	LPPOST tempPost;
	LPUSERDATA userinfo;
	LPMANAGER user = (LPMANAGER)vuser;
	int totalPage;
	int currnetPage;
	int searchcount=0;
    struct tm * local_time;

	LPARRAY searchList;

	arrayCreate(&searchList);

    currnetPage = 0;
	
	int totalSize;
	while(1){
		if(searchcount > 0){
			break;
		}
		size = arraySize(user->post_list);
		totalSize = size;
		//printf("size: %d\n",size);
		totalPage = (size-1) / 5;
		size = size - 5*currnetPage;
		tempCommand="clear";
		send(sd,tempCommand,strlen(tempCommand),0);
		usleep(100);
		sprintf(temp,"%d",sd);
		nErr = hashGetValue(user->curUserListHT,temp,(LPDATA*)&userinfo);

		usleep(100);
		send(sd,line,strlen(line),0);
		sprintf(buf,"current user:%s 현재페이지: %d 총 페이지 : %d 총글 갯수: %d \n",userinfo->id,currnetPage+1,totalPage+1,totalSize);
		usleep(100);
		send(sd,buf,strlen(buf),0);
		usleep(100);
		send(sd,line,strlen(line),0);
		if(size < 5){
			for(int i = 0 ; i < size ; i++){
				nErr = arrayGetAt(user->post_list,i+(currnetPage*5),(LPDATA *)&tempPost);	
				if(ERR_ARRAY_OK != nErr){
					return nErr;
				}
				local_time = localtime(&tempPost->post_time);
				sprintf(buf,"[%d] ID:%-10s ┃ %-30s ┃%d/%d %d:%d:%d\n",i+1,tempPost->publisher,tempPost->post_title
						,local_time->tm_mon,local_time->tm_mday,local_time->tm_hour,local_time->tm_min,local_time->tm_sec);
				usleep(1000);
				send(sd,buf,strlen(buf),0);
			}
		}else{

			for(int i = 0 ; i < 5 ; i++){
				nErr = arrayGetAt(user->post_list,i+(currnetPage*5),(LPDATA *)&tempPost);	
				if(ERR_ARRAY_OK != nErr){
					return nErr;
				}
				local_time = localtime(&tempPost->post_time);
				sprintf(buf,"[%d] ID: %-10s ┃ %-30s ┃%d/%d %d:%d:%d\n",i+1,tempPost->publisher,tempPost->post_title
						,local_time->tm_mon,local_time->tm_mday,local_time->tm_hour,local_time->tm_min,local_time->tm_sec);
				usleep(100);
				send(sd,buf,strlen(buf),0);
			}
		}
		usleep(100);
		send(sd,line,strlen(line),0);

		sprintf(buf,"다음(6) | 이전(7) | 새로고침(8) | 찾기(9) |게시글 보기(1~5) | 이전(0)");
		usleep(100);
		send(sd,buf,strlen(buf),0);
		usleep(100);
		send(sd,line,strlen(line),0);
		n = recv(sd, buf, sizeof(buf), 0);
		buf[n] = '\0';
		if(buf[0] < '0'|| buf[0] > '9')
			selectMenu = 9;
		else
		selectMenu = atoi(buf);

		switch(selectMenu){

			case 6:
				if( (currnetPage+1) <= totalPage){
					currnetPage++;
				}else{
					sprintf(buf,"마지막 페이지 입니다\n");
					usleep(100);
					send(sd,buf,strlen(buf),0);
					n = recv(sd, buf, sizeof(buf), 0);
					buf[n] = '\0';
				}
				break;
			case 7:
				if( (currnetPage-1) >= 0){
					currnetPage--;
				}else{
					sprintf(buf,"처음페이지 입니다\n");
					usleep(100);
					send(sd,buf,strlen(buf),0);
					n = recv(sd, buf, sizeof(buf), 0);
					buf[n] = '\0';
				}
				break;
			case 8:
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				if((selectMenu-1) < size){
					nErr = arrayGetAt(user->post_list,(selectMenu-1)+(currnetPage*5),(LPDATA *)&tempPost);	
					if(ERR_ARRAY_OK != nErr){
						return nErr;
					}
					tempCommand="clear";
					usleep(100);
					send(sd,tempCommand,strlen(tempCommand),0);
					usleep(100000);
					send(sd,line,strlen(line),0);
					usleep(1000);
					sprintf(buf,"제목 : %s \n",tempPost->post_title);
					send(sd,buf,strlen(buf),0);
					usleep(100);
					send(sd,line,strlen(line),0);
					usleep(100);
					sprintf(buf,"내용:");
					send(sd,buf,strlen(buf),0);
					usleep(100);
					sprintf(buf,"%s",line);
					send(sd,buf,strlen(buf),0);
					usleep(100);
					sprintf(buf,"%s",tempPost->post_content);
					send(sd,buf,strlen(buf),0);
					usleep(100);
					send(sd,line,strlen(line),0);
					n = recv(sd, buf, sizeof(buf), 0);
					buf[n] = '\0';
				}else{
					sprintf(buf,"게시글 번호를 확인해 주세요\n");
					usleep(100);
					send(sd,buf,strlen(buf),0);
					n = recv(sd, buf, sizeof(buf), 0);
					buf[n] = '\0';
				}
				break;
			case 0:
				return 3;
				break;
			case 9:
				sprintf(buf,"찾을 제목:");
				usleep(100);
				send(sd,buf,strlen(buf),0);
				n = recv(sd, buf, sizeof(buf), 0);
				buf[n] = '\0';
				for(int i = 0 ; i < totalSize; i++){
					nErr = arrayGetAt(user->post_list,i,(LPDATA *)&tempPost);	
					if(ERR_ARRAY_OK != nErr){
						return nErr;
					}
					//printf("search title %s\n",tempPost->post_title);
					if(strstr(tempPost->post_title,buf)!=NULL){
						arrayAdd(searchList,(LPDATA)tempPost);
						searchcount++;
					}
				}

				if(searchcount > 0){
					sprintf(buftemp,"찾았습니다.\n엔터를 눌러주세요\n");
					usleep(100);
					send(sd,buftemp,strlen(buftemp),0);

				}else{
					sprintf(buftemp,"없습니다. 제목을 확인해 주세요.\n엔터를 눌러주세요\n");
					usleep(100);
					send(sd,buftemp,strlen(buftemp),0);
				}

				n = recv(sd, buf, sizeof(buf), 0);
				buf[n] = '\0';

				break;
			default:
				sprintf(buf,"잘못입력했습니다.\n");
				usleep(100);
				send(sd,buf,strlen(buf),0);
				n = recv(sd, buf, sizeof(buf), 0);
				buf[n] = '\0';
				break;
		}
	}
	while(1){
		size = arraySize(searchList);
		totalSize = size;
		//printf("size: %d\n",size);
		totalPage = (size-1) / 5;
		size = size - 5*currnetPage;
		tempCommand="clear";
		send(sd,tempCommand,strlen(tempCommand),0);
		usleep(100000);
		sprintf(temp,"%d",sd);
		nErr = hashGetValue(user->curUserListHT,temp,(LPDATA*)&userinfo);

		usleep(100);
		send(sd,line,strlen(line),0);
		sprintf(buf,"current user:%s 현재페이지: %d 총 페이지 : %d 총글 갯수: %d \n",userinfo->id,currnetPage+1,totalPage+1,totalSize);
		usleep(100);
		send(sd,buf,strlen(buf),0);
		usleep(100);
		send(sd,line,strlen(line),0);
		if(size < 5){
			for(int i = 0 ; i < size ; i++){
				nErr = arrayGetAt(searchList,i+(currnetPage*5),(LPDATA *)&tempPost);	
				if(ERR_ARRAY_OK != nErr){
					return nErr;
				}
				local_time = localtime(&tempPost->post_time);
				sprintf(buf,"[%d] ID:%-10s ┃ %-30s ┃%d/%d %d:%d:%d\n",i+1,tempPost->publisher,tempPost->post_title
						,local_time->tm_mon,local_time->tm_mday,local_time->tm_hour,local_time->tm_min,local_time->tm_sec);
				usleep(1000);
				send(sd,buf,strlen(buf),0);
			}
		}else{

			for(int i = 0 ; i < 5 ; i++){
				nErr = arrayGetAt(searchList,i+(currnetPage*5),(LPDATA *)&tempPost);	
				if(ERR_ARRAY_OK != nErr){
					return nErr;
				}
				local_time = localtime(&tempPost->post_time);
				sprintf(buf,"[%d] ID: %-10s ┃ %-30s ┃%d/%d %d:%d:%d\n",i+1,tempPost->publisher,tempPost->post_title
						,local_time->tm_mon,local_time->tm_mday,local_time->tm_hour,local_time->tm_min,local_time->tm_sec);
				usleep(100);
				send(sd,buf,strlen(buf),0);
			}
		}
		usleep(100);
		send(sd,line,strlen(line),0);

		sprintf(buf,"다음(6) | 이전(7) | 새로고침(8) |찾은 게시글 보기(1~5) | 이전(0)");
		usleep(100);
		send(sd,buf,strlen(buf),0);
		usleep(100);
		send(sd,line,strlen(line),0);
		n = recv(sd, buf, sizeof(buf), 0);
		buf[n] = '\0';
		if(buf[0] < '0'|| buf[0] > '9')
			selectMenu = 9;
		else
			selectMenu = atoi(buf);

		switch(selectMenu){

			case 6:
				if( (currnetPage+1) <= totalPage){
					currnetPage++;
				}else{
					sprintf(buf,"마지막 페이지 입니다\n");
					usleep(100);
					send(sd,buf,strlen(buf),0);
					n = recv(sd, buf, sizeof(buf), 0);
					buf[n] = '\0';
				}
				break;
			case 7:
				if( (currnetPage-1) >= 0){
					currnetPage--;
				}else{
					sprintf(buf,"처음페이지 입니다\n");
					usleep(100);
					send(sd,buf,strlen(buf),0);
					n = recv(sd, buf, sizeof(buf), 0);
					buf[n] = '\0';
				}
				break;
			case 8:
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				if((selectMenu-1) < size){
					nErr = arrayGetAt(searchList,(selectMenu-1)+(currnetPage*5),(LPDATA *)&tempPost);	
					if(ERR_ARRAY_OK != nErr){
						return nErr;
					}
					tempCommand="clear";
					usleep(100);
					send(sd,tempCommand,strlen(tempCommand),0);
					usleep(100000);
					send(sd,line,strlen(line),0);
					usleep(1000);
					sprintf(buf,"제목 : %s \n",tempPost->post_title);
					send(sd,buf,strlen(buf),0);
					usleep(100);
					send(sd,line,strlen(line),0);
					usleep(100);
					sprintf(buf,"내용:");
					send(sd,buf,strlen(buf),0);
					usleep(100);
					sprintf(buf,"%s",line);
					send(sd,buf,strlen(buf),0);
					usleep(100);
					sprintf(buf,"%s",tempPost->post_content);
					send(sd,buf,strlen(buf),0);
					usleep(100);
					send(sd,line,strlen(line),0);
					//n = recv(sd, buf, sizeof(buf), 0);
					//buf[n] = '\0';
					//==================댓글 표시  ============================
					int comment_size= 0;
					LPCOMMENT temp_comment;
					LPCOMMENT loadcomment;
					temp_comment= (LPCOMMENT)malloc(sizeof(COMMENT));
					comment_size = arraySize(user->comment_list);
					for(int i = 0 ; i < comment_size;i++){
						nErr = arrayGetAt(user->comment_list,i,(LPDATA *)&loadcomment);	
						if(ERR_ARRAY_OK != nErr){
							return nErr;
						}
						if(loadcomment->post_ID == tempPost->post_ID){
							sprintf(buf," └ %s: %s\n",loadcomment->userid,loadcomment->comment);
							usleep(100);
							send(sd,buf,strlen(buf),0);
						}
					}

					usleep(100);
					send(sd,line,sizeof(line),0);
					//====== 댓글 표시 끝 =====================================
					n = recv(sd, buf, sizeof(buf), 0);
					buf[n] = '\0';

				}else{
					sprintf(buf,"게시글 번호를 확인해 주세요\n");
					usleep(100);
					send(sd,buf,strlen(buf),0);
					n = recv(sd, buf, sizeof(buf), 0);
					buf[n] = '\0';
				}
				break;
			case 0:
				arrayDestroy(searchList);
				return 3;
				break;
			default:
				sprintf(buf,"잘못입력했습니다.\n");
				usleep(100);
				send(sd,buf,strlen(buf),0);
				n = recv(sd, buf, sizeof(buf), 0);
				buf[n] = '\0';
				break;
		}
	}
	arrayDestroy(searchList);
	return 3;
}
