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
int archieveNew(LPDATA vuser, int sd)
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
		fp=fopen(fileinfo->filename,"rb");
		if(fp==NULL)
			perror("fileopen");
		fseek(fp,0,SEEK_END);
		fileinfo->filesize = ftell(fp);
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

		usleep(100);
		sprintf(buf,"13579uplaodstream2468");
		send(sd,buf,strlen(buf),0);
		while(1){
			n = recv(sd,buf,sizeof(buf),0);
			buf[n] = '\0';
			if(!strncmp(buf,"filesize13579",13)){
				size = atoi(&buf[10]);
				printf("recv input size %d\n",size);
				break;
			}
		}
		n = recv(sd,buf,sizeof(buf),0);
		buf[n] = '\0';
		usleep(100);
		send(sd,buf,strlen(buf),0);//1번호 입력 받고 다시 전달 해 줌 

		n = recv(sd,buf,sizeof(buf),0);//2.번호 처리해서 보맨 
		buf[n] = '\0';

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
			return 4;
			//break;
		}else if(selectMenu != 9999){
			char filename[256]={0,};

			printf("upload ready\n");


			sprintf(buf,"recvOK"); //FILE 만들고 OK를 보낸다.2.5.파일을 만들고 ok 보냄
			usleep(100);
			send(sd,buf,strlen(buf),0);
			
			
			n = recv(sd, buf, 512, 0);//file name recv3. 파일이름 받음 
			buf[n] = '\0';
			strcpy(filename,UPLOAD_FD);
			strcat(filename,buf);

			//printf("file name: %s",filename);
			int filesize;
			char *filebuffer;
			n = recv(sd, buf,512,0); //file size recv 4. 파일크기 받음
			buf[n] = '\0';
			filesize = atoi(buf);

			//printf("filesize %d\n",filesize);

			filebuffer=(char*)calloc(512+10,1);

////////////////////////////////////file open///////////////////////////////////////
			fp=fopen(filename,"wb+");
			if(fp == NULL){
				perror("fileopen");
			}
////////////////////////////////////////////////////////////////////////

			//printf("recv file name size file open ok\n");
			sprintf(buf,"recvOK"); //FILE 만들고 OK를 보낸다.5.파일을 만들고 ok 보냄
			usleep(100);
			send(sd,buf,strlen(buf),0);
			printf("받은파일 이름:%s filesize:%d\n",&filename[10],filesize);
			/////////
			//////
			while(1){
				n = recv(sd,filebuffer,512+10,0);
				if(!strncmp(filebuffer,"13579filesendend2468",20)){
					//printf("readbuff : %d\ndownload end\n",n);
					printf("\ndownload end\n");
					break;
				}
				//printf("readbuff : %d\n",n);
				fwrite(filebuffer,sizeof(char),n,fp);
				fflush(stdout);
				printf("#");
				sprintf(buf,"recvOK");
				usleep(100);
				send(sd,buf,strlen(buf),0);
			}

			n = recv(sd, buf,512,0); 
			buf[n] = '\0';

			free(filebuffer);
			fclose(fp);
			return 4;
		}
	}
	arrayDestroy(filelist);
	return 4;
}
