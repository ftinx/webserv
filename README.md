## Webserv 구현 사항
- Config File 
- Auto Index
- Redirections (301, 302)
- HTTP/1.1 Header
  - Accept-Charsets
  - Accept-Language
  - Allow
  - Authorization
  - Content-Lanugage
  - Content-Length
  - Content-Location
  - Content-Type
  - Date
  - Host
  - Last-Modified
  - Location
  - Referer
  - Retry-After
  - Server
  - Transfer-Encoding
  - User-Agent
  - WWW-Authenticate
- 

## 성능 측정

- **개인 tester**
  - [webserv_tester ](https://github.com/ftinx/webserv_tester)
  - [59개 testcase 통과](https://github.com/ftinx/webserv_tester/blob/main/testcase.md)

- **seige**

![image](https://user-images.githubusercontent.com/22424891/115876975-e97dbc00-a481-11eb-8a30-b6b5e1175ec7.png)

```
{       "transactions":                      2636589,
        "availability":                       100.00,
        "elapsed_time":                       883.76,
        "data_transferred":                   631.13,
        "response_time":                        0.00,
        "transaction_rate":                  2983.38,
        "throughput":                           0.71,
        "concurrency":                          3.89,
        "successful_transactions":           2636589,
        "failed_transactions":                     0,
        "longest_transaction":                107.31,
        "shortest_transaction":                 0.00
}
```

- **tester** 11분 (마지막 testcase 10분)
```
Test GET http://localhost:1234/
Test POST http://localhost:1234/ with a size of 0
Test HEAD http://localhost:1234/
Test GET http://localhost:1234/directory
Test GET http://localhost:1234/directory/youpi.bad_extension
Test GET http://localhost:1234/directory/youpi.bla
Test GET Expected 404 on http://localhost:1234/directory/oulalala
Test GET http://localhost:1234/directory/nop
Test GET http://localhost:1234/directory/nop/
Test GET http://localhost:1234/directory/nop/other.pouic
Test GET Expected 404 on http://localhost:1234/directory/nop/other.pouac
Test GET Expected 404 on http://localhost:1234/directory/Yeah
Test GET http://localhost:1234/directory/Yeah/not_happy.bad_extension
Test Put http://localhost:1234/put_test/file_should_exist_after with a size of 1000
Test Put http://localhost:1234/put_test/file_should_exist_after with a size of 10000000
Test POST http://localhost:1234/directory/youpi.bla with a size of 100000000
Test POST http://localhost:1234/directory/youpla.bla with a size of 100000000
Test POST http://localhost:1234/directory/youpi.bla with a size of 100000 with special headers
Test POST http://localhost:1234/post_body with a size of 0
Test POST http://localhost:1234/post_body with a size of 100
Test POST http://localhost:1234/post_body with a size of 200
Test POST http://localhost:1234/post_body with a size of 101
Test multiple workers(5) doing multiple times(15): GET on /
Test multiple workers(20) doing multiple times(5000): GET on /
Test multiple workers(128) doing multiple times(50): GET on /directory/nop
Test multiple workers(20) doing multiple times(5): Put on /put_test/multiple_same with size 1000000
Test multiple workers(20) doing multiple times(5): Post on /directory/youpi.bla with size 100000000
```


