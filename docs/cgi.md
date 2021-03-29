# CGI 란?



cgi 는 두개의 서로 다른 프로토콜을 연동하는(소프트웨어 게이트웨이) 표준안이다. CGI 표준을 따라 만들어진 프로그램을 CGI 프로그램 또는 스크립트라고 한다.

cgi도 웹에서 http를 통해 동작하므로 http와 동일한 절차(접속-요청-응답-접속해제)로 동작.

차이는 일반적인 경우 웹상의 요청은 하이퍼링크를 통해 이뤄지지만, cgi의 요청은 주로 form 태그나 isindex 태그를 통해 이뤄진다. 또한 파일시스템에서 문서를 읽어 전송하는 게 아니라, cgi 프로그램이 동적으로 생성하는 문서를 클라이언트에 전송한다.

사용자가 form 태그의 제출(submit) 버튼을 클릭하면 클라이언트는 현재 시간, 날짜와 같은 시스템 정보와 사용자 ID, 도메인명과 같은 사용자 정보를 포함한 요청 메세지(request message)를 웹 서버에 전송한다.

웹서버는 요청된 url 이 어떤 종류의 파일(ssi, cgi, perl)인지 분석하고, **cgi 프로그램일 경우 요청 헤더에 포함된 상태정보를 환경변수(QUERY_STRING, CONTENT_LENGHT 등등)로 설정하고 나서 cgi 프로그램을 실행**시키게 된다.

그러면 cgi 프로그램은 다른 서비스를 이용하기 위해 다른 프로그램을 또다시 호출하거나, **직접 처리를 한 다음 사용자에게 보여줄 문서를 표준 출력으로 보내게 된다**. 이것을 받은 서버는 다시 클라이언트 쪽으로 네트워크를 통해 결과를 전송하게 된다.

<br>

>:heavy_exclamation_mark: cgi는 서브젝트에서 제공합니다. cgi_tester 실행파일이 cgi 스크립트입니다. 따라서 아래의 형식과 에러에 관한 내용은 모두 제공된 cgi 가 처리해줄 것으로 기대하고 간단히만 읽고 넘어갑시다.



Cgi 스크립트는 표준에 따라 `Content-Type:Main-Type/Sub-Type\n\n` 을 먼저 출력하고 헤더와 바디를 구분하기 위해 공백 라인을 하나 추가해야 한다. 이 형식에 맞지 않으면 서버는 '500 Internal Server Error'를 출력한다.

Content-Type은 일반 텍스트 파일인 경우는 `text/html`, html 문서일 경우는 `text/html` , jpeg 이미지일 경우는 `images/jpeg` 등이 가능하며 각각의 파일에 따라서 MIME 유형이 정의돼 있다. 문서는 Content-Type 에 맞게 출력해야 한다.

다음의 주의사항을 지키지 않으면 cgi는 500 internal Server Error를 발행시킨다.

- cgi의 확장자는 `.cgi`로 한다
- 안전하고 효율적인 관리를 위해 cgi-bin/ 디렉토리에서 스크립트 관리
- 퍼미션
  - cgi 프로그램 755
  - 데이터 파일 666
  - 읽기/쓰기용 디렉토리 777
- cgi 출력 형식은 표준을 따른다(위에서 이미 기술)
- web에서 실행



<br>

## 입력

웹서버에서 cgi로, cgi 프로그램에서 웹서버로 서로 통신하는 방법이 마련돼있어야 한다. 우선 cgi 입력은 크게 세가지 방법을 통해 이뤄진다.



#### 환경변수

form 태그에서 method를 get으로 하거나 isindex 태그를 사용하는 경우, cgi 입력은 환경변수로부터 받아들여진다.

> :heavy_exclamation_mark: 웹서버.conf 파일에서 서버 관련 환경변수를, 요청 메세지에서 요청 관련 환경변수를 받아옵니다.



The following are environment variables passed to CGI programs:

Server specific variables:

-  SERVER_SOFTWARE: name/version of HTTP server.
-  SERVER_NAME: host name of the server, may be dot-decimal IP address.
-  GATEWAY_INTERFACE: CGI/version.

Request specific variables:

-  SERVER_PROTOCOL: HTTP/version.
-  SERVER_PORT: TCP port (decimal).
-  REQUEST_METHOD: name of HTTP method (see above).
-  PATH_INFO: path suffix, if appended to URL after program name and a slash.
-  PATH_TRANSLATED: corresponding full path as supposed by server, if PATH_INFO is present.
-  SCRIPT_NAME: relative path to the program, like /cgi-bin/script.cgi.
-  QUERY_STRING: the part of URL after ? character. The query string may be composed of *name=value pairs separated with ampersands (such as var1=val1&var2=val2...) when used to submit form data transferred via GET method as defined by HTML application/x-www-form-urlencoded.
-  REMOTE_HOST: host name of the client, unset if server did not perform such lookup.
-  REMOTE_ADDR: IP address of the client (dot-decimal).
-  AUTH_TYPE: identification type, if applicable.
-  REMOTE_USER used for certain AUTH_TYPEs.
-  REMOTE_IDENT: see ident, only if server performed such lookup.
-  CONTENT_TYPE: Internet media type of input data if PUT or POST method are used, as provided via HTTP header.
-  CONTENT_LENGTH: similarly, size of input data (decimal, in octets) if provided via HTTP header.
-  Variables passed by user agent (HTTP_ACCEPT, HTTP_ACCEPT_LANGUAGE, HTTP_USER_AGENT, HTTP_COOKIE and possibly others) contain values of corresponding HTTP headers and therefore have the same sense.



#### 표준입력

입력 폼에서 post 를 사용했을 경우에는 표준입력으로 데이터가 전달된다. 서버가 데이터 끝에 eof 심볼을 삽입하지 않을 수도 있기 때문에, 환경변수 CONTENT_LENGTH를 읽어들여 그 값만큼 데이터를 읽어들여야 한다.

> :heavy_exclamation_mark: pipe와 dup을 통해 cgi 프로세스의 STDIN과 STDOUT 을 조작합니다.
>
> :link: [먼저 진행한 팀의 아름다운 정리에 감사를...!](https://github.com/get-Pork-Belly/Webserv/wiki/CGI%EC%99%80-%ED%8C%8C%EC%9D%B4%ED%94%84-%EA%B7%B8%EB%A6%AC%EA%B3%A0-dup)



#### 명령행

isindex를 사용한 경우 명령행의 매개변수로 인자를 넘겨받을 수 있다. 명령행이 사용된 것을 알아내기 위해서는 클라이언트로부터 전달된 데이터에 인코딩이 되지 않은 '='를 찾으면 된다.

> :heavy_exclamation_mark: 고려하지 않아도 될듯 합니다.



<br>

## 출력

cgi 프로그램의 출력은

- 동적으로 생성해낸 새로운 문서(content-type)일 수도 있고
- 서버가 특정한 문서를 되돌려 주도록 하는 지시정보(Locations) 일 때도 있으며
- 단지 명령의 수행 결과를 리턴하는 상태라인(status)일 수도 있다

cgi 프로그램의 출력은 반드시 content-type, location, status-line 중 하나를 가장 먼저 출력해야 한다.

이러한 헤더에 이어 빈줄이 하나 삽입돼야 하며, 그렇지 않으면 500에러를 발생한다.



#### Content-Type: 리턴될 문서의 MIME 타입

MIME 타입 중 하나를 Content-type 헤더를 이용해 출력한다.

- `Content-Type:` Content-Type 과 콜론 사이에 공백이 존재해선 안된다



#### Location: 리턴될 문서의 URL

웹 서버는 이 헤더를 만나면, 헤더 값이 절대경로면 문서를 직접 돌려준다.

완전한 URL일 경우에는 클라이언트에 리다이렉트를 보내준다. 리다이렉트를 하는 경우 3xx번대 상태코드를 전송한다.



#### Status Line

일반적으로는 cgi 프로그램을 수행할 때, 서버는 cgi 프로그램의 출력에 추가 헤더와 상태 정보를 덧붙여 클라이언트에 전송한다.

> :heavy_exclamation_mark: cgi 프로그램이 상태정보를 직접 출력하는 경우는 서버를 경유하지 않는 경우이므로, 우리의 웹서브는 고려하지 않아도 될듯합니다. 웹서버가 직접 더하는 걸 가정합니다.
