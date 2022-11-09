#ifndef __SERVER_H__
#define __SERVER_H__
#include <time.h>
#include <sys/epoll.h>
#include "array.h"
#include "hash.h"
#define MAXCLIENT 128

typedef struct _custOnChatting {
	int sockfd;

}custOnChat,*LPcustOnchar;
typedef struct { 
	int post_ID; //key값응로 할려고 했음 
	char publisher[16];
	char post_title[100];
	char post_content[2000];
	time_t post_time;//이걸로 아이디 대체 할 수 있을 것 같음 
	// 이후 댓글 추가 
}POST,*LPPOST;
typedef struct  {
	char id[16];
	char pass[16];
}USERDATA,*LPUSERDATA;
typedef struct {
	int post_ID;
	char userid[16];
	char comment[100];

}COMMENT,*LPCOMMENT;

typedef struct  {

	LPARRAY post_list;// 동적배열 key post ID post를 동적으로 연결 한다. 
	int total_post ;

}BOARD,*LPBOARD;

typedef struct {

	LPARRAY post_list;// 동적배열 key post ID post를 동적으로 연결 한다. 
	LPARRAY user_list;//userinfo 동적으로 연결 한다.
	LPHASH curUserListHT;
	custOnChat cust[MAXCLIENT];
	int chatUserCount; // chatting user count
	int efd;//epoll evenvt 등록을 위해서 함수에 전달
	int total_user;
	int total_post ;
	LPARRAY comment_list;
}MANAGER,*LPMANAGER;

typedef struct {
	LPARRAY user_list;//userinfo 동적으로 연결 한다.
	int total_user;

}USER,*LPUSER; //user 전체 정보

typedef struct page {
	int post_id[5];// --> 5개의 글을 보낸다.5개 안되면 빈것으로 보낸 다...
	char post_title[5][100];
	time_t post_time[5];

}PAGE,*LPPAGE;

//========================구조체 초기화 함수 
int board_init(LPMANAGER board);
int user_init(LPMANAGER user);
int loadBoardData(LPMANAGER board);
int loadUserData(LPMANAGER user);
int loadBoardData_file(LPMANAGER board);
int loadUserData_file(LPMANAGER user);

//==========================input1.txt 로그인 메뉴 =========================================
//int login(LPMANAGER user,int sd);
//int join(LPMANAGER user, int sd);

//========================input2.txt 게시판 자료실 채팅 선택 메뉴 ==========================================


//=======================input3.txt Board 메뉴 ========================================
//int addNewPost(LPMANAGER board);//void로 해야 하는 건 지, 불러서 안에서 해야 함 ...newpost 삭제


//=========================input4.txt 자료실 메뉴 =======================================


//=========================server 종료 시 저장 함수==========================================
int saveUserData(LPMANAGER user);
int saveBoardData(LPMANAGER board);
int saveCommentData(LPMANAGER board);

//===============================임시 출력 함수 ==========================================
void tempPrintPost(LPMANAGER board);
void tempPrintUser(LPMANAGER user);
//===============================page 출력 함수 =================================================
int printPage(LPMANAGER board,int sd,int currentpage);
//===============================================================================================
#endif
