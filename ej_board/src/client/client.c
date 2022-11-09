#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <termio.h>
#include <netinet/in.h>   
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/unistd.h>
#include "macro.h"
#include "array.h"
#include <dirent.h>
#include <netinet/in.h>
#include <netdb.h>

#define CR '\012'

char line[]="\n─────────────────────────────────────────────────────────────────────\n";
char thick_line[]="\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
#define BUFFER_SIZE 2048
#define DOWNLOAD_FD "../download/"
typedef struct __fileinfo{
	char filename[512];
	//char printfilename[256];
	int filesize;
}FILEINFO,*LPFILEINFO;
void* writer_thread(void *arg);
void* reader_thread(void *arg);
pthread_t tid1, tid2;
LPARRAY print_upload_folder();
int passwdsel;

int main(int argc, char *argv[])
{
	int sockfd, new_fd;
	struct sockaddr_in serv_addr;
	//struct hostent *he;
	int sin_size;
	
	if(argc!=2){
		fprintf(stderr, "Usage : ./client IP_ADDRESS\n");
		return 1;
	}


	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(60000);
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	/*he= gethostbyname(argv[1]);
	if(he==NULL){
	  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	  }else{
	  serv_addr.sin_addr.s_addr = inet_addr(he->h_addr_list[0]);
	  printf("he sodyd: %s",he->h_addr_list[0]);
	  }*/
	memset(&(serv_addr.sin_zero), '\0', 8);

	if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
		perror("CONNECT");
		return 1;
	}

	//system("clear");

	if(pthread_create(&tid1, NULL, writer_thread, &sockfd) != 0){
		perror("pthread_create");
	}
	if(pthread_create(&tid2, NULL, reader_thread, &sockfd) != 0){
		perror("pthread_create");
	}

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	close(sockfd);

	return 0;
}

void* reader_thread(void *arg){
	int sock = *((int*)arg);

	int n;
	int size;
	int selectMenu;
	char buffer[1024];
	int filesize;
	char filename[512];
	char *filebuffer;
	FILE *fp;
	LPARRAY filelist;
	LPFILEINFO fileinfo;
	while(1){
		n = recv(sock, buffer, 512, 0);
		if(n <= 0){
			printf("\n서버 연결 끊김\n");
			break;
		}
		buffer[n] = '\0';
		if(!strcmp(buffer,"clear")){
			//printf("%s", buffer);
			fflush(stdout);
			system("clear");
		}else if(!strcmp(buffer,"13579filedown2468")){ 

			printf("download ready\n");
			n = recv(sock, buffer, 512, 0);
			buffer[n] = '\0';
			strcpy(filename,DOWNLOAD_FD);
			strcat(filename,buffer);

			n = recv(sock, buffer,512,0);
			buffer[n] = '\0';
			filesize = atoi(buffer);
			filebuffer=(char*)calloc(512+10,1);
			////////////////////////////////////file open///////////////////////////////////////
			fp=fopen(filename,"wb+");
			if(fp == NULL){
				perror("fileopen");
			}
			////////////////////////////////////////////////////////////////////////
			sprintf(buffer,"recvOK"); //FILE 만들고 OK를 보낸다.
			usleep(100);
			send(sock,buffer,strlen(buffer),0);
			printf("받은파일 이름:%s filesize:%d\n",&filename[12],filesize);
			while(1){
				n = recv(sock,filebuffer,512+10,0);
				if(!strncmp(filebuffer,"13579filesendend2468",20)){
					//printf("readbuff : %d\ndownload end\n",n);
					printf("\ndownload end\n");
					break;
				}
				//printf("readbuff : %d\n",n);
				fwrite(filebuffer,sizeof(char),n,fp);
				fflush(stdout);
				printf("#");
				sprintf(buffer,"recvOK");
				usleep(100);
				send(sock,buffer,strlen(buffer),0);

			}
			fclose(fp);
			free(filebuffer);

		}else if(!strcmp(buffer,"13579uplaodstream2468")){
			filelist= print_upload_folder();
			size = arraySize(filelist);
			sprintf(buffer,"filesize13579%d",size);
			usleep(100);
			send(sock,buffer,strlen(buffer),0);

			n = recv(sock,buffer,sizeof(buffer),0); //1내가 보낸 번호 다시 받음 
			buffer[n] = '\0';

			if(buffer[0] < '0' || buffer[0] > '9'){
				selectMenu = 9999;
			}
			else
				selectMenu = atoi(buffer);//문자열이면 0으로 리턴한다....

			if(selectMenu > size){
				selectMenu = 9999;
			}


			sprintf(buffer,"%d",selectMenu);
			usleep(100);
			send(sock,buffer,strlen(buffer),0); //2내가 보낸 번호  받은 걸 처리해 서 보냄 
			//printf("selectMenu send %s\n",buffer);
			////////////////////// file upload///////////////////////////////////////////////////////////////////////////
			if(selectMenu == 0){
				continue;
			}else if(selectMenu != 9999){
				int nErr;
				nErr = arrayGetAt(filelist,selectMenu-1,(LPDATA*)&fileinfo);
				if(ERR_ARRAY_OK != nErr)
					perror("arrayAdd");
				fp=fopen(fileinfo->filename,"rb");
				if(fp==NULL)
					perror("fileopen");
				char *filebuffer;
				filebuffer=(char*)calloc(512+10,1);

				n = recv(sock,buffer,512,0); //2.5.recv ok 받음
				buffer[n] = '\0';
				if(!strcmp(buffer,"recvOK"))
				{	
					printf("file name, size send ready\n");
				}//////error//


				usleep(100000);
				sprintf(buffer,"%s",&fileinfo->filename[12]);
				send(sock,buffer,strlen(buffer),0); //3.파일이름 보
				usleep(100000);
				sprintf(buffer,"%d",fileinfo->filesize);
				send(sock,buffer,strlen(buffer),0);   //4.파일 크기 보냄 
				printf("send size name ok\n");
				//error
				n = recv(sock,buffer,512,0); //5.recv ok 받음을 때 까지 무한 루프
				buffer[n] = '\0';
				if(!strcmp(buffer,"recvOK"))
				{	
					printf("file open\n");
				}//////error//

				int send_size;
				int buff_start=0;
				while(send_size=fread(filebuffer,sizeof(char),512,fp)){
					usleep(100);
					send(sock,filebuffer,send_size,0);
					memset(filebuffer,0,512);
					n = recv(sock,buffer,512,0);
					buffer[n] = '\0';
					if(!strcmp(buffer,"recvOK"))
					{
						fflush(stdout);
						printf("#");
					}
				}
				usleep(100);
				sprintf(buffer,"13579filesendend2468");
				send(sock,buffer,strlen(buffer),0);
				//printf("%s",filebufferfer);
				printf("\n=========== send end ============\n");
				//n = recv(sock,buffer,sizeof(buffer),0);
				//buffer[n] = '\0';
				free(filebuffer);
				fclose(fp);

			}

			arrayDestroy(filelist);
			//////////////////////////file upload//////////////////////////////////////////////////////////////
		}else if(!strcmp(buffer,"13579passwd2468")){
			struct termio tbuf, oldtbuf;
			if(ioctl(0, TCGETA, &tbuf) == -1) { // 현재터미널모드
				perror("ioctl");    
				exit(1);
			}
			oldtbuf=tbuf;
			tbuf.c_lflag &= ~ECHO;
			tbuf.c_lflag&=~ICANON;
			tbuf.c_cc[VMIN] =1;
			tbuf.c_cc[VTIME]=0;


			if(ioctl(0, TCSETAF, &tbuf)==-1) {
				perror("ioctl"); exit(1);
			}
			passwdsel = 1;
		}else if(!strcmp(buffer,"2468passwd13579")){
			passwdsel = 0;
		}else{
			printf("%s", buffer);
			fflush(stdout);

		}
	}
	pthread_cancel(tid1);
	pthread_exit(NULL);
}

void* writer_thread(void *arg){
	int sock = *((int*)arg);

	int n;
	char buffer[1024];
	char ch;
	struct termio tbuf, oldtbuf;
	if(ioctl(0, TCGETA, &tbuf) == -1) { // 현재터미널모드
		perror("ioctl");    
		exit(1);
	}
	oldtbuf=tbuf;
	tbuf.c_lflag |=ECHO;
	tbuf.c_lflag|=ICANON;
	tbuf.c_cc[VMIN] =1;
	tbuf.c_cc[VTIME]=0;

	while(1){

		int count=0;
		while(1){
			ch=getchar();
			if(ch == CR || ch =='\n'){ 
				buffer[count] = '\0';
				if(passwdsel == 1){
					if(ioctl(0, TCSETAF, &tbuf)==-1) {
						perror("ioctl"); exit(1);
					}
					passwdsel = 0;
				}
				if(count == 0){
					buffer[0] = '\n';
					buffer[1] = '\0';

				}
				//printf("buffer %s",buffer);
				break;
			}

			if(passwdsel==1){
				if(ch == 127 || ch == 8){
					printf("\b \b");
					buffer[count--] = '\0';

					if(count <0)
						count = 0;
					continue;
				}else{
					printf("*");
				}
			}

			buffer[count++]=ch;
		}
		send(sock,buffer,strlen(buffer),0);

		//if(passwdsel == 0){
		//	fgets(buffer, 1024, stdin);
		//	n = strlen(buffer);
		//	buffer[n-1] = '\0';
		//	if(!strcmp(buffer, "/q")){
		//		break;
		//	}
		//	send(sock, buffer, n, 0);
		//}else if(passwdsel == 1){
		//	if(ioctl(0, TCSETAF, &tbuf)==-1) {
		//		perror("ioctl"); exit(1);
		//	}
		//	int count=0;
		//	while(1){
		//		ch=getchar();
		//		if(ch == CR){ 
		//			buffer[count] = '\0';
		//			passwdsel = 0;
		//			break;
		//		}else if(ch == 127 || ch == 8){
		//			printf("\b \b");
		//			count--;
		//		}else{
		//			printf("*");
		//			buffer[count++]=ch;
		//		}
		//	}
		//	send(sock,buffer,strlen(buffer),0);
		//	//printf("client buffer passwd:%s",buffer);
		//}

	}
	pthread_cancel(tid2);
	pthread_exit(NULL);	
}


LPARRAY print_upload_folder(){
	int nErr;
	FILE *fp;
	LPFILEINFO fileinfo;
	LPARRAY filelist;

	nErr = arrayCreate(&filelist);
	if(ERR_ARRAY_OK != nErr){
		printf("filelist init error\n");
		if (NULL != filelist) {
			arrayDestroy(filelist);
		}
		//return nErr;
	}

	DIR * dp = opendir(DOWNLOAD_FD);
	if(dp==NULL){
		perror("open dir");
		//return 4;
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
		strcpy(fileinfo->filename,DOWNLOAD_FD);
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
	int size;

	//printf("----------upload 폴더 리스트 ------------------\n");
	printf("┌─────────────────────────────────────────┐\n│         내 upload 폴더 리스트           │\n└─────────────────────────────────────────┘"); 
	printf("%s",line);
	size = arraySize(filelist);
	for(int i = 0 ; i < size ; i++){
		arrayGetAt(filelist,i,(LPDATA*)&fileinfo);

		printf(" [%d]: %s size:%d\n",i+1,&fileinfo->filename[12],fileinfo->filesize);
	}

	printf("%s",line);
	printf("upload 할 파일의 번호를 선택 하세요( 이전(0))\n 번호를 선택하세요\n"); 

	return filelist;
}
