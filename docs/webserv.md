## About File

- __File Open Mode__

     |  **Mode**   | 의미                                                             |
     | :----------: | :--------------------------------------: |
     | O_CREATE  | 필요한 경우 파일을 생성                                 |
     | O_TRUNC   | 존재하던 데이터를 모두 삭제                          |
     | O_APPEND | 존재하던 데이터를 보존하고 뒤에 이어서 추가  |
     | O_RDONLY | 읽기 전용 모드                                              |
     | O_WRONLY | 쓰기 전용 모드                                             |
     | O_RDWR     | 읽기 쓰기 전용 모드                                      |

## About Socket

> Everything is a File.

Linux/Unix에서는 socket도 하나의 파일(File), 더 정확히는 File Descriptor(FD, 파일 디스크립터)입니다. 이처럼 Low Level File Handling 기반으로 socket 기반의 데이터 송수신이 가능합니다. I/O 작업은 단순히 단일 server 내에서 일어나는 읽기/쓰기 뿐만 아니라 Server-Client 간 네트워크 통신에도 적용되는 개념입니다.

I/O 작업은 user space에서 직접 수행할 수 없기 때문에 user process가 kernel에 I/O 작업을 '요청'하고 '응답'을 받는 구조입니다. 응답을 어떤 순서로 받는지(synchronous/asynchronous), 어떤 타이밍에 받는지(blocking/non-blocking)에 따라 여러 모델로 분류됩니다.

### Synchronous, 동기 

- 모든 I/O 요청-응답 작업이 일련의 순서를 따릅니다. 즉, 작업의 순서가 보장됩니다.
- 작업 완료를 user space에서 판단하고 다음 작업을 언제 요청할지 결정하게 됩니다.
- 일련의 Pipeline을 준수하는 구조에서 효율적입니다.

<img src="https://user-images.githubusercontent.com/22424891/111920168-e6815b80-8ad0-11eb-98f5-6a95bcf5deca.png" height="150px" />

### Asynchronous, 비동기 

- kernel에 I/O 작업을 요청해두고 다른 작업 처리가 가능하나, 작업의 순서는 보장되지 않습니다.
- 작업 완료를 kernel space에서 통보해 줍니다.
- 각 작업들이 독립적이거나, 작업 별 지연이 큰 경우 효율적입니다.

<img src="https://user-images.githubusercontent.com/22424891/111920210-1cbedb00-8ad1-11eb-93e3-63d68621decb.png" height="150px" />

### Blocking, 블로킹 

- 요청한 작업이 모두 완료될 때까지 기다렸다가 완료될 때 응답과 결과를 반환받습니다. [대기 有]
- 요청한 작업 결과를 기다립니다.

### Non-Blocking, 넌-블로킹 

- 작업 요청 이후 결과는 나중에 필요할 때 전달받습니다. [대기 無]
- 요청한 작업 결과를 기다리지 않습니다.
- 중간중간 필요하면 상태 확인은 해볼 수 있습니다. (polling)

OS 환경에 따라서 세부적인 기법 차이가 있지만 Linux 기반의 I/O 모델을 우선으로 살펴보았을때, 아래는 IBM Developer의 I/O 모델 분류표입니다.

<img src="https://user-images.githubusercontent.com/22424891/111920507-be92f780-8ad2-11eb-8654-575fb42fe650.png" height="300px" />

- Synchronous Blocking I/O → Blocking I/O
- Synchronous Non-Blocking I/O → Non-Blocking I/O
- Asynchronous Blocking I/O → I/O Multiplexing
- Asynchronous Non-Blocking I/O → Asynchronous I/O

> I/O Multiplexing이 Asynchronous Blocking 방식인지에 대해 다양한 의견이 있다. 구현 방식에 따라 차이가 있지만 관점 주체에 따라 Blocking/Non-Blocking이 갈리기도 하며, 실제 I/O 동작은 Synchronous 방식으로 동작한다. 심지어 각 기법에 따라 세부적인 로직 및 알림 방식도 달라지기 때문에 Multiplexing을 단순히 Asynchronous Blocking 방식이라고 딱 잘라 정의하기엔 무리가 있다. 따라서 각 기본 개념만 숙지한 채 I/O Mutliplexing은 큰 분류가 아닌 세부 기법을 위주로 이해하자.

### System Call

```c++
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
int bind(int sockfd, struct sockaddr *myaddr, int addrlen);
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, int *addrlen);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```

```c++
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
int close(int fd);
```

<img src="https://user-images.githubusercontent.com/22424891/115947402-59cc2200-a502-11eb-8345-f9f7824e96ca.png" height="550px" />

> 보통 Linux/ MacOS의 backlog queue 제한은 128입니다.

## TCP Connection Status

<img src="https://user-images.githubusercontent.com/22424891/115947430-8d0eb100-a502-11eb-85d6-3b15054d1562.png" height="600px"/>

## I/O Multiplexing (멀티플렉싱, 다중화 Asynchronous Blocking I/O)

'다중화'란 무엇일까요. 간단하게 '하나'를 '여러 개'처럼 보이게 동작하게 한다는 뜻입니다. 이를 I/O 관점에서 해석하면 '한 프로세스가 여러 파일(file)을 관리'하는 기법이라 볼 수 있습니다. 파일은 프로세스가 kernel에 진입할 수 있도록 다리 역할을 하는 인터페이스입니다. server-client 환경이라면 하나의 server에서 여러 socket 즉, 파일을 관리하여 여러 client가 접속할 수 있게 구성됩니다. socket 또한 IP/Port를 가진 파일일 뿐입니다.

프로세스에서 특정 파일에 접근할 때는 파일 디스크립터(이하 FD, File Descriptor)라는 추상적인 값을 사용하게 되는데, 이 FD들을 어떻게 감시하냐는게 I/O Multiplexing의 주요 맹점이라 할 수 있겠습니다. 여기서 어떤 상태로 대기하냐에 따라 select, poll, epoll(linux), kqueue(bsd), iocp(windows) 등 다양한 기법들이 등장합니다.

kernel은 I/O 요청을 받으며 처리를 시작함과 동시에 user process에게 미완료 상태를 반환하고 user process는 데이터가 준비됐다는 알람이 올 때까지 대기하는 모습을 보여주고 있습니다. 여기서 blocking 인지 non-blocking 인지부터 헷갈리는데 명확히 구분하자면 user process에서의 read, write 같은 I/O 작업 자체가 block 되는 것이 아니라 select, poll 같은 mutliplexing 관련 system call에 대한 kernel의 응답이 block 된다고 봐야 합니다. I/O Multiplexing을 마냥 Asynchronous Blocking 기법으로 분류하기엔 혼동 여지가 있는 이유 중 하나입니다. 여기선 첫 read 요청에 대해 즉각 미완료 상태를 반환받는 Non-blocking socket의 동작을 보여주는데, select의 결과에 따라 read/write system call을 수행하게끔 구현하면 위 그림에서 Non-Blocking 요소를 없앨 순 있습니다. 다만, 데이터가 checksum 실패로 폐기되는 등의 일부 상황에선 select()가 어떤 FD에 데이터 있으니 읽으라고 알려와서 읽었다가 socket이 Block 되는 상황이 발생할 수 있습니다. 이런 상황에선 데이터를 받는 socket을 Non-Blocking으로 구성하여 EWOULDBLOCK error만 return 하고 넘어가게끔 설계하여 안전성을 높일 수 있습니다.

이어서, kernel의 응답을 기다리다 보면 kernel에서 결과 값이 준비되었다는 callback 신호가 오고 user process는 자신의 buffer로 데이터를 복사해오는 모습을 보여주는데, 사실 select 방식의 실제 구현으로 들어가면 select 호출 결과가 유의미한 값으로 나올 때까지 user process에서 loop를 돌리며 대기하는 방식입니다. 결국 select를 요청한 user process가 return 받은 값을 보고 후속 작업 유무를 판단하는 것입니다. 예측할 수 없게 인입되는 여러 I/O 요청을 한 번에 관리할 수 있어 Asynchronous 하다고 보는 경우도 있지만 결국 실제 I/O 동작은 Synchronous 한 동작을 보입니다.

이 부분 또한 혼동 여지가 있기에 I/O Multiplexing 모델에 대한 개념적 구분은 여기서 줄이도록 합니다. 해당 그림은 I/O Multiplexing을 적용할 수 있는 하나의 예시이며, select 같은 기법으로 여러 I/O 작업을 독립적으로 관리할 수 있다고만 이해하고 넘어가도록 합니다.

- select (Level-Triggered)
- pselect
- poll
- ppoll
- epoll (Edge-Triggered)

## Webserv

## HTTP 란?

The Hypertext Transfer Protocol (HTTP) is a stateless application-level protocol for distributed, collaborative, hypertext information systems.

HTTP(Hypertext Transfer Protocol)는 분산, 협업, 하이퍼 텍스트 정보 시스템을 위한 비상태 애플리케이션 레벨 프로토콜이다.

[RFC 7230](https://hochan049.gitbook.io/cs-interview/web/rfc-7320)에 명시되어 있다.

## HTTP Request, Response 

- Chrome
<img src="https://user-images.githubusercontent.com/22424891/115949417-fea12c00-a50f-11eb-8cd7-f8f57e87f8d2.png" height="600px"/>

- Raw Message
```
---------------------------------------------------------------------------
GET / HTTP/1.1
Accept: */*
User-Agent: rawtester
Host: localhost:8080
Host: rawtester
Authorization: Basic andvbjowNTE4


###########################################################################
HTTP/1.1 400 Bad Request
content-language: ko
content-length: 0
content-type: text/html
date: Sat, 24 Apr 2021 14:01:28 KST
last-modified: Sat, 24 Apr 2021 14:01:28 KST
server: ftinx/0.1: first_server
status: 400
```
