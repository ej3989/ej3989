#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "server.h"
#include "array.h"

char line[]="\n─────────────────────────────────────────────────────────────────────\n";
char thick_line[]="\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";

int board_init(LPMANAGER board){
	int nErr;
	board->total_post = 0;
	nErr = arrayCreate(&board->post_list);
	if(ERR_ARRAY_OK != nErr){
		if (NULL != board->post_list) {
			arrayDestroy(board->post_list);
		}
		return nErr;
	}
	nErr = arrayCreate(&board->comment_list);
	if(ERR_ARRAY_OK != nErr){
		if (NULL != board->comment_list) {
			arrayDestroy(board->comment_list);
		}
		return nErr;
	}
	return 0;
}
int user_init(LPMANAGER user){
	int nErr;
	user->total_user = 0;
  	nErr = arrayCreate(&user->user_list);
	if(ERR_ARRAY_OK != nErr){
		printf("user init error\n");
		if (NULL != user->user_list) {
			arrayDestroy(user->user_list);
		}
		return nErr;
	}
	return 0;

}

void tempPrintPost(LPMANAGER board){

	int nErr;
	LPARRAY temp_array;
	LPPOST temPost;
	temp_array = board->post_list;
	int size;
	size  = arraySize(temp_array);

	printf("size: %d\n",size);
	for(int i = 0 ; i< size ; i++){
		nErr = arrayGetAt(temp_array,i,(LPDATA*)&temPost);
		printf("%d %s %s %s\n" ,temPost->post_ID,temPost->publisher,temPost->post_title,temPost->post_content);

	}

}
void tempPrintUser(LPMANAGER user){

	int nErr;
	LPARRAY temp_array;
	LPUSERDATA tempUser;
	temp_array = user->user_list;
	int size;
	size  = arraySize(temp_array);

	printf("size: %d\n",size);
	for(int i = 0 ; i< size ; i++){
		nErr = arrayGetAt(temp_array,i,(LPDATA*)&tempUser);
		printf("%s %s\n" ,tempUser->id,tempUser->pass);

	}

}
int addNewPost(LPMANAGER board){
	int nErr;
	LPPOST newPost;
	//printf("addnew %p \n",*temp_array);
	nErr = arrayAdd(board->post_list,(LPDATA)newPost);
	if(ERR_ARRAY_OK != nErr){
		return nErr;
	}

	board->total_post++;

	return 0;
}
int join(LPMANAGER user){
	int nErr;
	LPUSERDATA input_id_passwd;
	nErr = arrayAdd(user->user_list,(LPDATA)input_id_passwd);
	if(ERR_ARRAY_OK != nErr){
		printf("user init error\n");
		return nErr;
	}

	user->total_user ++;
	return 0;
}
//int login(LPMANAGER user){
//	USERDATA input_id_passwd;
//	return 0;
//}
int loadUserData(LPMANAGER user){

	int nErr;
	//USERDATA temp_user_data[3] = {
	//	{"ej3989","1"},
	//	{"ej111","1"},
	//	{"ej222","1"}, 
	//};
	LPUSERDATA tempUser;
	LPUSERDATA temp_user_data[3];
	for(int i = 0 ; i < 3 ;i++){
		temp_user_data[i] = (LPUSERDATA)malloc(sizeof(USERDATA));
	}
	strcpy(temp_user_data[0]->id,"ej3989");
	strcpy(temp_user_data[0]->pass,"1");
	strcpy(temp_user_data[1]->id,"ej222");
	strcpy(temp_user_data[1]->pass,"1");
	strcpy(temp_user_data[2]->id,"ej111");
	strcpy(temp_user_data[2]->pass,"1");

	for(int i = 0 ; i < 3 ;i++){
		//nErr = join(user, temp_user_data[i]);
		nErr = arrayAdd(user->user_list,(LPDATA)temp_user_data[i]);
		//		printf("%s %s %d\n",temp_user_data[i].id,temp_user_data[i].pass,nErr);
		if(nErr != 0){
			printf("error 발생 ");
			return nErr;
		}
		user->total_user ++;
		//nErr = arrayGetAt(user->user_list,i,(LPDATA *)&tempUser);
		//printf("load data:  %s %s \n" ,tempUser->id,tempUser->pass);
	}


	return 0;
}
int loadBoardData(LPMANAGER board){

	time_t temp_time;
	int nErr;

	time(&temp_time);
	//POST itemp_post[3]= {
	//	{1,"ej3989","test 글 입니다.","ej3989 내용은 없습니다.",temp_time},
	//	{2,"ej111","test 글 입니다.","ej111 내용은 없습니다.",temp_time},
	//	{3,"ej222","test 글 입니다.","ej222 내용은 없습니다.",temp_time},
	//};
	LPPOST temp_post[3];
	for(int i = 0 ; i < 3 ;i++){
		temp_post[i] = (LPPOST)malloc(sizeof(POST));
	}
	temp_post[0]->post_ID = 0;
	temp_post[0]->post_time = temp_time;
	strcpy(temp_post[0]->publisher,"ej3989");
	strcpy(temp_post[0]->post_title,"ej3989 test ");
	strcpy(temp_post[0]->post_content,"ej3989 내용은 없습니다. ");
	//printf("%s %s" ,temp_post[0]->publisher,temp_post[0]->post_title);
	temp_post[1]->post_ID = 1;
	temp_post[1]->post_time = temp_time;
	strcpy(temp_post[1]->publisher,"ej111");
	strcpy(temp_post[1]->post_title,"ej111 test ");
	strcpy(temp_post[1]->post_content,"ej111의 글이지만 내용은 없습니다. ");

	temp_post[2]->post_ID = 2;
	temp_post[2]->post_time = temp_time;
	strcpy(temp_post[2]->publisher,"ej222");
	strcpy(temp_post[2]->post_title,"ej222 test ");
	strcpy(temp_post[2]->post_content,"ej222헛  내용은 없습니다. ");

	for(int i = 0 ; i < 3 ;i++){
		nErr = arrayAdd(board->post_list,(LPDATA)temp_post[i]);
		if(nErr != 0)
			printf("error 발생 ");
		board->total_post++;
	}

	return 0;
}
int loadBoardData_file(LPMANAGER board){

	FILE *fp,*fp2;
	int nErr;
	fp = fopen("UserBoard.dat","rb");
	if(fp==NULL){
		perror("file open");
		return -1;
	}
	fp2 = fopen("UserComment.dat","rb");
	if(fp2==NULL){
		perror("file open");
		return -1;
	}

	LPPOST temp_post;
	while(1){
		temp_post = (LPPOST)malloc(sizeof(POST));
		if(temp_post == NULL){
			perror("temp malloc");
			return -1;
		}
		if(fread(temp_post,sizeof(POST),1,fp) == 0){
			board->total_post++;
			break;
		}
		nErr = arrayAdd(board->post_list,(LPDATA)temp_post);
		if(nErr != 0)
			printf("error 발생 ");
		if(temp_post->post_ID > board->total_post){
			board->total_post=temp_post->post_ID;
		}
	}
	LPCOMMENT temp_comment;
	while(1){
		temp_comment = (LPCOMMENT)malloc(sizeof(COMMENT));
		if(temp_comment == NULL){
			perror("temp malloc");
			return -1;
		}
		if(fread(temp_comment,sizeof(COMMENT),1,fp2) == 0){
			break;
		}
		nErr = arrayAdd(board->comment_list,(LPDATA)temp_comment);
		if(nErr != 0)
			printf("error 발생 ");
	}
	fclose(fp);
	fclose(fp2);
	return 0;
}

int loadUserData_file(LPMANAGER user){

	FILE *fp;
	int nErr;
	fp = fopen("UserInfo.dat","rb");
	if(fp==NULL){
		perror("file open");
		return -1;
	}

	LPUSERDATA temp_user_data;

	while(1){
		temp_user_data = (LPUSERDATA)malloc(sizeof(USERDATA));
		if(temp_user_data == NULL){

			perror("temp user data");
			return -1;
		}

		if(fread(temp_user_data,sizeof(USERDATA),1,fp)==0){
			break;
		}
		nErr = arrayAdd(user->user_list,(LPDATA)temp_user_data);
		if(nErr != 0){
			printf("error 발생 ");
			return nErr;
		}
		user->total_user ++;


	}

	fclose(fp);

	return 0;
}
int saveUserData(LPMANAGER user){

	FILE *fp;
	int nErr;
	int size;
	fp = fopen("UserInfo.dat","w+b");
	if(fp==NULL){
		perror("file write");
		return -1;
	}

	LPUSERDATA temp_user;
	size = arraySize(user->user_list);
	for(int i = 0; i < size ; i++){
		nErr = arrayGetAt(user->user_list,i,(LPDATA *)&temp_user);
		if(nErr != 0){
			printf("error 발생 ");
			return nErr;
		}

		fwrite(temp_user,sizeof(USERDATA),1,fp);
	}

	fclose(fp);
	return 0;
}
int saveCommentData(LPMANAGER board){

	FILE *fp;
	int nErr;
	int size;

	fp = fopen("UserComment.dat","w+b");
	if(fp==NULL){
		perror("file write");
		return -1;
	}

	LPCOMMENT temp_comment;
	size = arraySize(board->comment_list);
	for(int i = 0; i < size ; i++){
		nErr = arrayGetAt(board->comment_list,i,(LPDATA *)&temp_comment);
		if(nErr != 0){
			printf("error 발생 ");
			return nErr;
		}

		fwrite(temp_comment,sizeof(COMMENT),1,fp);
	}

	fclose(fp);
	return 0;
}

int saveBoardData(LPMANAGER board){

	FILE *fp;
	int nErr;
	int size;

	fp = fopen("UserBoard.dat","w+b");
	if(fp==NULL){
		perror("file write");
		return -1;
	}

	LPPOST temp_post;
	size = arraySize(board->post_list);
	for(int i = 0; i < size ; i++){
		nErr = arrayGetAt(board->post_list,i,(LPDATA *)&temp_post);
		if(nErr != 0){
			printf("error 발생 ");
			return nErr;
		}

		fwrite(temp_post,sizeof(POST),1,fp);
	}

	fclose(fp);
	return 0;
}
int printPage(LPMANAGER vuser,int sd,int currnetPage){
	int size,n,nErr;
	int selectMenu;
	char buf[2048];
	char temp[20];
	char *tempCommand;
	LPPOST tempPost;
	LPUSERDATA userinfo;
	LPMANAGER user = (LPMANAGER)vuser;
	int totalPage;
	//int currnetPage;
	struct tm * local_time;

	//currnetPage = 0;

	int totalSize;
	size = arraySize(user->post_list);
	totalSize = size;
	printf("size: %d\n",size);
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
			sprintf(buf,"[%d] 작성자  %-10s ┃ %-30s ┃ %d:%d:%d\n",i+1,tempPost->publisher,tempPost->post_title,local_time->tm_hour,local_time->tm_min,local_time->tm_sec);
			usleep(100);
			send(sd,buf,strlen(buf),0);
		}
	}else{

		for(int i = 0 ; i < 5 ; i++){
			nErr = arrayGetAt(user->post_list,i+(currnetPage*5),(LPDATA *)&tempPost);	
			if(ERR_ARRAY_OK != nErr){
				return nErr;
			}
			local_time = localtime(&tempPost->post_time);
			sprintf(buf,"[%d] ID: %-10s ┃ %-30s ┃ %d:%d:%d\n",i+1,tempPost->publisher,tempPost->post_title,local_time->tm_hour,local_time->tm_min,local_time->tm_sec);
			usleep(100);
			send(sd,buf,strlen(buf),0);
		}
	}
	usleep(100);
	send(sd,line,strlen(line),0);

	sprintf(buf,"다음페이지(6)|이전페이지(7)|새로고침(8)|게시글 보기(1~5) |이전 (0)");
	usleep(100);
	send(sd,buf,strlen(buf),0);
	usleep(100);
	send(sd,line,strlen(line),0);



	return currnetPage ;
}
