### 간단한 소켓 서버, 클라이언트 구현 // 2020-02-01(월) // jwon

------

#### :notebook: 학습내용

------

##### 1. 데이터(연결요청)를  받는 소켓의 구현 // (서버)

- **소켓을 생성**하는 함수

  ```c
  #include <sys/socket.h>
  int socket(int domain, int type, int protocol);
  // 성공 fd 값 반환, 실패 -1 반환
  ```

- 소켓에  **IP와  포트번호  할당**하는 함수

  ```c
  #include <sys/socket.h>
  int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
  // 성공 0 반환, 실패 -1 반환
  ```

- 소켓을 **연결요청 가능한 상태**로 만들어주는 함수

  ```c
  #include <sys/socket.h>
  int listen(int sockfd, int backlog);
  // 성공 0 반환, 실패 -1 반환
  ```

- 연결 요청이 오면 **요청을 수락**하는 함수

  ```c
  #include <sys/socket.h>
  int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
  // 성공 fd 값 반환, 실패 -1 반환
  ```

- 코드구현  예시

  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include < string. h>
  #include <unistd.h>
  #include <arpa/inet. h>
  #include <sys/socket.h>

  void error_handling(char、*message);

  int main(int argc, char* argv[])
  {
  	int serv_sock;
      int clnt_sock;

      struct sockaddr_in serv_addr;
      struct sockaddr_in clnt_addr;
      socklen_t clnt_addr_size;

      char massage[] = "Hello World";

      if (argc != 2)
      {
          printf("Usage : %s <port>\n", argv[0]);
          exit(1);
      }

      // 소켓 생성
      serv_sock = socket(PF_INET, SOCK_STREAM, 0);
      if (serv_sock == -1)
          error_handling("socket() error");

      memset(&serv_addr, 0, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_addr.s_addr = hton1(INADDR_ANY);
      serv_addr.sin_port = htons(atoi(argv[1]));

      // IP주소, PORT번호 할당
      if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
          error_handling("bind() error");

      // 소켓을 연결요청을 받아들일 수 있는 상태로 만듬
      if (listen(serv_sock, 5) == -1)
          error_handling("listen() error");

      clnt_addr_size = sizeof(clnt_addr);
      // 연결 요청 수락을 위한 accept 함수  호출,  연결 요청이 있을 때까지 반환하지 않음
      clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
      if (clnt_sock == -1)
          error_handling("accept() error");

      // 아래 write가 실행된다면 연결 요청이 있었다는 의미
      write(clnt_sock, message, sizeof(message));
      close(clnt_sock);
      close(serv_sock);
      return 0;
  }

  void error_handling(char *message)
  {
  	fputs(message, stderr);
  	fputc(’\n', stderr);
  	exit(1);
  }
  ```



##### 2. 데이터(연결요청)를 하는 소켓의 구현 // (클라이언트)

- **연결 요청**하는 함수

  ```c
  #include <sys/socket.h>
  int connect(int sockfd, struct sockaddr *serv_addr, socklen_t addrlen);
  // 성공 0 반환, 실패 -1 반환
  ```

- 코드구현  예시

  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <arpa/inet.h>
  #include <sys/socket.h>

  void error_handling(char、*message);

  int main(int argc, char* argv[])
  {
  	int sock;
  	struct sockaddr_in serv_addr;
  	char message[30];
  	int str_len;

  	if (argc != 3)
  	{
  		printf("Usage : %s <IP> <port>\n", argv[0]);
  		exit(1);
  	}

      //  소켓 생성
  	sock = socket(PF_INET, SOCK_STREAM, 0);
  	if (sock == -1)
  		error_handling("socket() error");

  	memset(&serv_addr, 0, sizeof(serv_addr));
  	serv_addr.sin_family = AF_INET;
  	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  	serv_addr.sin_port = htons(atoi(argv[2]));

      // connect 함수를 호출하여 서버에 연결 요청
  	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
  		error_handling("connect() error!");

  	str_len = read(sock, message, sizeof(message) - 1);
  	if(str_len == -1)
  		error_handling("read () error!");

  	printf("Message from server : %s \n", message);
  	close(sock);
  	return 0;
  }

  void error_handling(char *message)
  {
  	fputs(message, stderr);
  	fputc(’\n', stderr);
  	exit(1);
  }
  ```







------

#### :link: 유용한 정보 링크

------

