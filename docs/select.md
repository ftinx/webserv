### select()을 사용한 멀티플렉싱

select()를 이용해 '하나의 프로세스'에서 '다중접속서버'를 구현



### 과정

1. socket()
2. bind()
3. listen()  - 여기까진 멀티프로세싱, 멀티쓰레딩의 과정과 같음
4. fd_set에 listen 소켓 넣음
5. select()로 fd_set에 있는 소켓들의 이벤트를 감지
6. check fd_set으로 어떤 소켓의 이벤트인지 확인
7. listen socket이면, accept()로 connection socket을 만들어 fd_set에 넣어둠
8. connect socket이면, client에 서비스 제공



### 4번부터의 코드 흐름

```c
while(1)
{
  maxfd = getMaxfd(listenSocket) + 1;

  FD_ZERO(&read_fds);
  // 듣기 소켓 세팅
  FD_SET(listenSock, &read_fds);
  // 두번째 루프부터 활용되는 연결 소켓 세팅
  for (i = 0; i < numClient; i++)
    FD_SET(clientSock,&read_fds);
  if (select(maxfd, &read_fds, NULL, NULL, NULL) < 0)
    exit(1);

  // 아래부터 select()가 깨어나서 읽을 수 있는 fd가 들어오면 실행되는 부분
  // 듣기 소켓의 event를 확인하는 if문
  if (FD_ISSET(listenSock, &read_fds))
   {
     connSock = accept(listenSock, ... );
    // 연결 소켓의 주소를 지닌 자료구조인 clientsock에 새로운 연결소켓 등록
     clientsock[numClient++] = connSock;
   }
  // 연결 소켓의 event를 확인하는 for문
  for (i = 0; i < numClient; i++)
  {
    if (FD_ISSET(clientSock[i], &read_fds))
      if((n = read(clientSocket[i], rcvBuffer, sizeof(rcvBuffer)) != 0)
      {
        rcvBuffer[n] = '\0';
        // 읽은 대로 다시 보냄(echo)
        write(clientSock[i], rcvBuffer, n);
      }
  }
}
```



### socket()

Arguments 와 fd_sets macro에 대한 설명은 아래 맨 참조

```c
       #include <sys/select.h>

       int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);

       void FD_CLR(int fd, fd_set *set);
       int  FD_ISSET(int fd, fd_set *set);
       void FD_SET(int fd, fd_set *set);
       void FD_ZERO(fd_set *set);
```

:link: https://man7.org/linux/man-pages/man2/select.2.html
