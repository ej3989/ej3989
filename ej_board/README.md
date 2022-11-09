1. /home/mobis/ 에서 ej3989.tar.gz 압축 해제 

2. 공유 라이브러리 폴더 추가 
export LD_LIBRARY_PATH=/home/mobis/myLib:/home/mobis/ej3989/ej_board/lib:$LD_LIBRARY_PATH

3. 전체 파일 make 진행 
cd /home/mobis/ej3989/ej_board/src 
make clean
make

4. 서버 동작 
cd /home/mobis/ej3989/ej_board/bin
./server_main
 서버 동작시 현재 저장되어 있는 아이디와 비번을 표시 함 

 make 와 서버 동작을 하번에 할때 에는아래 스크립트bin 폴더에서 실행
 . runmake.sh

5. client 동작
cd /home/mobis/ej3989/ej_board/bin
./client 127.0.0.1
주소 입력이 귀찮을 시,
아래 스크립트 실행 
. runclient.sh 

5. 로그인 
기본 아이디 
ID:ej111 	PW:1
ID:ej3989 	PW:1
ID:ej401 	PW:1
ID:ej222	PW:1

가입을 통해 아이디 추가 가능

이후 동작은 client 메뉴를 통해 진행 하면 됩니다.
모든 메뉴의 종료는 프로그램 종료 를 함 
이전 으로 가기위해 서는 이전 메뉴를 선택해야 함 

새글 작성 시 , 완료는 exit 를 입력
채팅시 종료는 , ... 을 입력



