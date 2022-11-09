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
int archieveSearch(LPDATA vuser, int sd)
{
    int size,n,nErr;
	int selectMenu;
	int inputcheck;
	int buff_read_size;
	//int filesize;
	char buf[2048];
	char temp[20];
	char tempbuf[2067];
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


		sprintf(buf,"찾고 싶은 파일명을 입력 하세요 |  이전(0))\n파일명: "); 
		usleep(100);
		send(sd,buf,strlen(buf),0);


		n = recv(sd,buf,sizeof(buf),0);
		buf[n] = '\0';

		if(!strcmp(buf,"0")){
			arrayDestroy(filelist);
			return 4;
		}



		int searchcount=0;
		for(int i = 0; i < size ; i++){
			nErr = arrayGetAt(filelist,i,(LPDATA*)&fileinfo);
			if(ERR_ARRAY_OK != nErr){
				perror("arrayAdd");
			}
			if(strstr(fileinfo->filename,buf)!=NULL){
				sprintf(tempbuf,"찾은 파일: %s\n",&fileinfo->filename[10]);
				usleep(100);
				send(sd,tempbuf,strlen(tempbuf),0);
				searchcount++;
			}

		}
		if(searchcount == 0){
			sprintf(tempbuf,"%s 는 없습니다.\n",buf);
			usleep(100);
			send(sd,tempbuf,strlen(tempbuf),0);
		}


		n = recv(sd,buf,512,0);
		buf[n] = '\0';

		//arrayDestroy(filelist);
		//return 4;
	}


	arrayDestroy(filelist);

	return 4;
}
