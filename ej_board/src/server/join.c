#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "array.h"
#include "server.h"
#include "hash.h"

int join(LPDATA vuser, int sd)
{
    int size,n,nErr,id_check;
	char buf[1024];
	char temp[20];
	LPUSERDATA tempUserData,checkUserData;
	LPMANAGER user = (LPMANAGER)vuser;

	tempUserData = (LPUSERDATA)calloc(sizeof(USERDATA),1);
	    
	while(1){
		sprintf(buf,"%s", "신규 ID: ");
		send(sd, buf, strlen(buf), 0);
		n = recv(sd, buf, sizeof(buf), 0);
		buf[n] = '\0';
		id_check = 0;
		size = arraySize(user->user_list);
		for(int i = 0 ; i <size ;i++){
			nErr = arrayGetAt(user->user_list , i ,(LPDATA *) &checkUserData);
			if(ERR_ARRAY_OK != nErr){
				return nErr;
			}
			if(!strcmp(checkUserData->id,buf)){
				id_check = 1;
				sprintf(buf,"%s", "동일한 ID가 있습니다. 다시 일벽해 주세요\n");
				send(sd, buf, strlen(buf), 0);

			}
		}
		if(id_check == 0){
			break;
		}
	}
	strcpy(tempUserData->id,buf);

	sprintf(buf,"%s", "passwd: ");
	send(sd, buf, strlen(buf), 0);
	n = recv(sd, buf, sizeof(buf), 0);
	buf[n] = '\0';
	strcpy(tempUserData->pass,buf);


	nErr = arrayAdd(user->user_list,(const LPDATA)tempUserData);
	if(ERR_ARRAY_OK != nErr){
		return nErr;
	}

	// user file save 추가함
	saveUserData(user);
	

	sprintf(temp,"%d",sd);
	nErr = hashSetValue(user->curUserListHT, temp, (LPDATA) tempUserData);
	if (ERR_HASH_OK != nErr) {
		printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
	}
	return 2;
}
