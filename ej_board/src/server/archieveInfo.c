#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include "hash.h"
#include "server.h"
#include "array.h"
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define UPLOAD_FD "../upload/"

char line[]="\n─────────────────────────────────────────────────────────────────────\n";
char thick_line[]="\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
typedef struct __fileinfo{
	char filename[512];
	//char printfilename[256];
	int filesize;
}FILEINFO,*LPFILEINFO;
int archieveInfo(LPDATA vuser, int sd)
{
    int size,n,nErr;
	int selectMenu;
	int inputcheck;
	int buff_read_size;
	//int filesize;
	char buf[2048];
	char temp[20];
	//char *filename;
	FILE *fp;
	LPFILEINFO fileinfo;
	//char inode_filename[100][1024]={0,};
	LPMANAGER user = (LPMANAGER)vuser;
	LPARRAY filelist;
	nErr = arrayCreate(&filelist);
	if(ERR_ARRAY_OK != nErr){
		printf("filelist init error\n");
		if (NULL != filelist) {
			arrayDestroy(filelist);
		}
		return nErr;
	}
	

	DIR * dp = opendir(UPLOAD_FD);
	if(dp==NULL){
		perror("open dir");
		return 4;
	}
	struct dirent* direntp;

	int i = 0;
	while(1){
		direntp = readdir(dp);
		if(direntp == NULL){
			break;
		}


		if(direntp->d_type == DT_DIR){
			continue;
		}
		fileinfo=(LPFILEINFO)calloc(sizeof(FILEINFO),1);
		strcpy(fileinfo->filename,UPLOAD_FD);
		strcat(fileinfo->filename,direntp->d_name);
		//strcpy(fileinfo->printfilename,direntp->d_name);
		//printf("file 이름: %s\n",fileinfo->filename);
		fp=fopen(fileinfo->filename,"rb");
		if(fp==NULL)
			perror("fileopen");
		fseek(fp,0,SEEK_END);
		fileinfo->filesize = ftell(fp);
		//sprintf(buf," [%d]: inode: %ld, name : %s\n",i+1,direntp->d_ino,direntp->d_name);
		//fstat(direntp->d_ino,&hmm);
		i++;
		nErr = arrayAdd(filelist,(LPDATA)fileinfo);
		if(ERR_ARRAY_OK != nErr)
			perror("arrayAdd");
		fclose(fp);
	}
	
	while(1){
		size = arraySize(filelist);
		usleep(100);
		send(sd,"clear",6,0);
		usleep(100000);
		sprintf(buf,"┌─────────────────────────────────────────┐\n│              자료실 목록                │\n└─────────────────────────────────────────┘"); 
		usleep(100);
		send(sd,buf,strlen(buf),0);
		send(sd,line,strlen(line),0);
		for(int i = 0 ; i < size ; i++){
			arrayGetAt(filelist,i,(LPDATA*)&fileinfo);

			sprintf(buf," [%d]: %s size:%d\n",i+1,&fileinfo->filename[10],fileinfo->filesize);
			usleep(100);
			send(sd,buf,strlen(buf),0);
		}
		usleep(100);
		send(sd,line,strlen(line),0);
		

		sprintf(buf,"Download할 파일의 번호를 선택 하세요( 이전(0))\n 번호를 선택하세요\n"); 
		usleep(100);
		send(sd,buf,strlen(buf),0);

		
		n = recv(sd,buf,sizeof(buf),0);
		buf[n] = '\0';

		if(buf[0] < '0' || buf[0] > '9'){
			selectMenu = 9999;
		}
		else
			selectMenu = atoi(buf);//문자열이면 0으로 리턴한다....

		printf("select menu num: %d \n",selectMenu);

		if(selectMenu >size){
			sprintf(buf,"잘못 입력했습니다.\n아무키나 눌러주세요"); 
			usleep(100);
			send(sd,buf,strlen(buf),0);
			n = recv(sd,buf,sizeof(buf),0);
			buf[n] = '\0';
			selectMenu = 9999;
		}

		if(selectMenu == 0){
			break;
		}else if(selectMenu != 9999){
			nErr = arrayGetAt(filelist,selectMenu-1,(LPDATA*)&fileinfo);
			if(ERR_ARRAY_OK != nErr)
				perror("arrayAdd");
			fp=fopen(fileinfo->filename,"rb");
			if(fp==NULL)
				perror("fileopen");
			char *filebuffer;
			filebuffer=(char*)calloc(512+1,1);

			usleep(100);
			sprintf(buf,"13579filedown2468");
			send(sd,buf,strlen(buf),0);
			usleep(100);
			sprintf(buf,"%s",&fileinfo->filename[10]);
			send(sd,buf,strlen(buf),0);
			usleep(100);
			sprintf(buf,"%d",fileinfo->filesize);
			send(sd,buf,strlen(buf),0);

			n = recv(sd,buf,512,0);
			buf[n] = '\0';
			if(!strcmp(buf,"recvOK"))
			{
				printf("file open\n");
			}
			int send_size;
			int buff_start=0;
			while(send_size=fread(filebuffer,sizeof(char),512,fp)){
				usleep(100);
				send(sd,filebuffer,send_size,0);
				memset(filebuffer,0,512);
				n = recv(sd,buf,512,0);
				buf[n] = '\0';
				if(!strcmp(buf,"recvOK"))
				{
					fflush(stdout);
					printf("#");
				}
			}
			usleep(100);
			sprintf(buf,"13579filesendend2468");
			send(sd,buf,strlen(buf),0);
			//printf("%s",filebuffer);
			printf("\n=========== send end ============\n");
			n = recv(sd,buf,sizeof(buf),0);
			buf[n] = '\0';

			free(filebuffer);
			fclose(fp);
			return 4;
		}
	}


	arrayDestroy(filelist);

	return 4;
}
