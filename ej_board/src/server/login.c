#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "server.h"
#include "hash.h"


int login(LPDATA vuser, int sd)
{
    int size,n,nErr,id_check;
	char buf[1024];
	char tempbuf[1024];
	char temp[20];
	char userid[20];
	char userpasswd[20];
	LPUSERDATA tempUserData,addHash;
	LPMANAGER user = (LPMANAGER)vuser;
    
    sprintf(buf,"%s", "ID: ");
	send(sd, buf, strlen(buf), 0);
    n = recv(sd, buf, sizeof(buf), 0);
	buf[n] = '\0';
    
	addHash = (LPUSERDATA)calloc(sizeof(USERDATA),1);
	size = arraySize(user->user_list);
	id_check = 0;
	for(int i = 0 ; i <size ;i++){
		nErr = arrayGetAt(user->user_list , i ,(LPDATA *) &tempUserData);
		if(ERR_ARRAY_OK != nErr){
			return nErr;
		}
		if(!strcmp(tempUserData->id,buf)){
			id_check = 1;
			strcpy(userid,buf);
			strcpy(userpasswd,tempUserData->pass);
			break;
		}
	}
///

///
	if(id_check == 0){
		sprintf(buf,"%s", "해당 사용자가 없습니다.\n");
		send(sd, buf, strlen(buf), 0);
		n = recv(sd, buf, sizeof(buf), 0);
		buf[n] = '\0';
		return 1;
	}
	///////////////////////////////////////////////////////
	//printf("input id check: %s",buf);
	strcpy(addHash->id,buf);

	sprintf(buf,"%s", "13579passwd2468");
	usleep(100);
	send(sd, buf, strlen(buf), 0);

	sprintf(buf,"%s", "passwd: ");
	usleep(100);
	send(sd, buf, strlen(buf), 0);


	n = recv(sd, buf, sizeof(buf), 0);
	buf[n] = '\0';

	//printf("server passwd:%s",buf);

	sprintf(tempbuf,"%s", "2468passwd13579");
	usleep(1000);
	send(sd, tempbuf, strlen(tempbuf), 0);

	id_check = 0;
	//printf("user passwd: [%s] [%s] \n",userpasswd,buf);
	if(!strcmp(userpasswd,buf)){
		id_check = 1;
	}
	if(id_check == 0){
		sprintf(buf,"%s", "\n비밀번호가 맞지 않습니다.\n");
		usleep(100);
		send(sd, buf, strlen(buf), 0);
		n = recv(sd, buf, sizeof(buf), 0);
		buf[n] = '\0';
		return 1;
	}

	strcpy(addHash->pass,buf);


	///////로그인 성공시 sd를 이용해서 현재 아이디 저장 /////////////////////////
	//로그인 전에 해당 아디로 접속이 되어 있는지 확인 후 있으면 다시 로그인 처리 
	POSITION pos;
	char *key;
	LPUSERDATA HTuser;
	nErr = hashGetFirstPostion(user->curUserListHT, &pos);
	if (ERR_HASH_OK != nErr) {
		printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
	}
	//다음 위치로 이동하여 
	while (NULL != pos) {
		nErr = hashGetNextPostion(user->curUserListHT, &pos, &key, (LPDATA*) &HTuser);
		if (ERR_HASH_OK != nErr) {
			printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
			break;
		}
		//같은 아이디가 있으면 접속 중으로 표시하고 다시 로그인 한다 
		printf("hash tabel search: %s \n",HTuser->id);
		if(!strcmp(HTuser->id,addHash->id)){
			sprintf(buf,"현재 %s 가 접속 중입니다. \nID를 확인 해 주세요 \n",addHash->id);
			usleep(100);
			send(sd,buf,strlen(buf),0);

			n = recv(sd, buf, sizeof(buf), 0);
			buf[n] = '\0';
			return 1;
		}
	}


	/////////////////////////////////////////////////////////////////////////////	
	sprintf(temp,"%d",sd);
	nErr = hashSetValue(user->curUserListHT, temp, (LPDATA)addHash );
	if (ERR_HASH_OK != nErr) {
		printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
	}
	//printf("current user:%s %s \n",addHash.id,tempUserData->pass);
	return 2;
}
