### ftinx config file 구성, 문법

1. **구성**

   - http block

     | 구성    | 성격    | 기본값  | 필수 | 유효성      | 예시                            |
     | ------- | ------- | ------- | ---- | ----------- | ------------------------------- |
     | open    | bracket |         | O    |             | `http {`                        |
     | name    | element | `ftinx` | O    |             | `software_name ftinx`           |
     | version | element | `0.1`   | O    |             | `software_version 0.1`          |
     | include | element |         | O    | `*.types`   | `include mime.types`            |
     | root    | element |         | O    | `dir exist` | `root /Users/ftinx/Webserv/www` |
     | server  | block   |         | O    |             | `server {`                      |
     | close   | bracket |         | O    |             | `}`                             |
     |         |         |         |      |             |                                 |
     
   - server block

     | 구성               | 성격    | 기본값    | 필수 | 유효성                          | 예시                                    |
     | ------------------ | ------- | --------- | ---- | ------------------------------- | --------------------------------------- |
     | open               | bracket |           | O    |                                 | `server {`                              |
     | server name        | element | `default` | O    |                                 | `server_name first_server`              |
     | listen(port)       | element | `8080`    | O    | `1024 ~ 49151`                  | `listen 8080`                           |
     | defalut error page | element |           |      | `file exist`                    | `default_error_page default_error.html` |
     | content length     | element |           |      |                                 | `content_length 2048`                   |
     | return             | element |           |      | `method(301 302 307), location` | `return 301 http://localhost:8081/`     |
     | location           | block   |           | O    |                                 | `location /test {`                      |
     | close              | bracket |           | O    |                                 | `}`                                     |
     |                    |         |           |      |                                 |                                         |

   - location block

     | 구성                 | 성격             | 기본값    | 필수 | 유효성                          | 예시                                        |
     | -------------------- | ---------------- | --------- | ---- | ------------------------------- | ------------------------------------------- |
     | open, path           | bracket, element |           | O    |                                 | `location /test {`                          |
     | limit expect         | element(multi)   |           | O    |                                 | `limit_except GET POST`                     |
     | root                 | element          |           |      | `dir exist`                     | `root /Users/ftinx/Webserv/Sample2`         |
     | index                | element(multi)   |           |      | `file exist`                    | `index index.html index.htm index.php`      |
     | cgi                  | element(multi)   |           |      |                                 | `cgi .bin .cgi .php`                        |
     | cgi path             | element          |           |      | `dir exist`                     | `root /Users/ftinx/Webserv/Sample2/cgi-bin` |
     | limit body size      | element          | `INT_MAX` |      | `0 ~ INT_MAX`                   | `limit_body_size 100`                       |
     | autoindex            | element          | `off`     |      | `on / off`                      | `autoindex on`                              |
     | auth basic           | element(multi)   |           |      |                                 | `auth_basic jwon yechoi holee`              |
     | auth basic user file | element          |           |      | `file exist`                    | `auth_basic_user_file .htpasswd`            |
     | return               | element          |           |      | `method(301 302 307), location` | `return 302 /`                              |
     | close                | bracket          |           | O    |                                 | `}`                                         |
     |                      |                  |           |      |                                 |                                             |

2. **문법**

   1. 공통

      1. http block은 1개만 유효하다

      2. element는 `[KEY][SPACE(s)][VALUE]`의 형식을 따른다

      3. `KEY`가 중복되는 경우

         1. `VALUE`가 1개만 올 수 있는 element 인 경우 값을 덮어쓴다

         ```
         server {
         	server_name first
         	server_name second
         ```

         ```
         string server_name = second
         ```

         2. `VALUE`가 1개 이상 올 수 있는 multi 성격의 element 인 경우 값을 추가한다

         ```
         server {
         	cgi .bin
         	cgi .cgi .php
         ```

         ```
         vector<string> cgi;
            cgi[0] = .bin
            cgi[1] = .cgi
            cgi[2] = .php
         ```

      4. 정의되지 않은 `KEY`는 무시한다

      5. 중괄호(`{`, `}`)

         1. 왼쪽 중괄호와 오른쪽 중괄호 개수가 같아야 한다
         2. 한개의 라인에 여러개의 중괄호가 올 수 없다
         3. `VALUE`에는 중괄호가 포함될 수 없다

      6. 주석

         1. 주석은 `#comment`의 형식을 따른다
         2. 라인의 첫 글자가 `#` 으로 시작한다면 해당 라인은 주석으로 간주하며 파싱하지 않는다

         ```
         #http block 시작
         http {
             software_name ftinx
             software_version 0.1
             include mime.types
             root /Users/ftinx/Webserv/
         #server block 시작
         		server {
         ```

         3. 라인의 마지막에도 `[SPACE(s)]` 이후 주석을 사용할 수 있다

         ```
         		server {
         			server_name first #서버네임
         			listen 8080 #포트
         ```

   2. http block

      1. server block은 최소 1개 이상이어야 한다

   3. server block

      1. location block은 최소 1개 이상이어야 한다
      2. location block과 return은 같이 존재할 수 없다

   4. location block

3. **유효성 검사**

   - [x] config file path 인자 값이 비어있는 경우 `default.conf` 파싱
   - [x] `KEY` 중복 체크
   - [x] 중괄호 체크
     - [x] 왼쪽 중괄호와 오른쪽 중괄호 개수 체크
     - [x] 한개의 라인에 여러개의 중괄호 여부 체크
   - [x] 주석으로만 구성되었거나 비어있는 라인 체크
   - [x] http block 존재 여부 체크
   - [x] server block 존재 여부 체크
     - [x] server block과 return요소 중복 체크
   - [x] location block 존재 여부 체크

5. **논의 필요**
   
   1. content-length 관련 [#35 (comment)](https://github.com/ftinx/webserv/issues/35#issuecomment-793454981)
   3. 기본값, 필수요소, 유효성체크 필요한 부분 있으면 코멘트 달아주세요
   
5. **샘플**

   1. config/default.conf

   ```yaml
   http {
       software_name ftinx
       software_version 0.1
       include mime.types # 'mime.types' file must be included
       # ㄴ https://developer.mozilla.org/ko/docs/Web/HTTP/Basics_of_HTTP/MIME_types
       # ㄴ https://www.nginx.com/resources/wiki/start/topics/examples/full/#mime-types
       default_type application/octet-stream
       # ㄴ https://developer.mozilla.org/en-US/docs/Learn/Server-side/Configuring_server_MIME_types
       # root /Users/ftinx/www # Please enter an absolute root path
       server {
           server_name default_server
           listen 1234 # port
           default_error_page default_error.html # Change the default error page
           # content_length 2048
           location / {
               limit_except GET # Allowed methods GET HEAD PUT POST OPTIONS DELETE TRACE
               # cgi .bin .cgi .bla
               # cgi_path /Users/ftinx/cgi-bin/cgi_tester
               index index.html index.htm index.php
               # limit_body_size 100 # http://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size
               # root /Users/ftinx/www
               auth_basic ftinx
               auth_basic_user_file ../.auth/.htpasswd_default # default id/pwd : ftinx/1234
               autoindex on
           }
           location /redirect {  # second location block for test'302 Found' redirection
               limit_except GET
               return 302 /
           }
       }
       server { # second server block for test '301 Moved Permanently' redirection
           server_name redirect_server
           listen 9999
           return 301 https://42seoul.kr/
       }
   }
   ```

   



