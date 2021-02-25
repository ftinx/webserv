## Team Coding Convention

### 들여쓰기
- Tab Size: 4

### 변수명
- 클래스의 멤버 변수 
	- prefix: ```m_```
	- 소문자
	- 스네이크 표기 ('_')

### 함수
- 소문자

- 카멜 표기 (중간에 대문자 ex, ```void webServer(void);```)

- getter 함수
	- prefix: ```get_m_```
	- 스네이크 표기 ```get_m_variable```
	- 한 줄로 쓸 수 있는 경우 한줄로 표현
	
- setter 함수
	- prefix: ```set_m_```
	- 스네이크 표기 ```set_m_variable```
	- 한 줄로 쓸 수 있는 경우 한줄로 표현
	
- return type과 괄호는 함수명과 분리
	-	>```
		>char *
		>strdup(const char *s)
		>{
		>}
		>```
	
- hpp 파일에서 \*와 &는 반환값의 경우 자료형 뒤에 붙이고, 매개변수일 경우 변수명 앞에 붙인다.

  >  `char* strdup(const char *s)`

  

### 클라스
- Canonical form 준수

### 헤더 파일 내 순서 우선 순위

- 아래와 같은 우선순위를 갖으며 위에서 부터 차례대로 작성한다.  
	- enum
	- static 변수
	- 멤버 변수
	- private 멤버 메소드
	- 캐노니컬 폼
	- getter
	- setter
	- public 멤버 메소드
	- 중첩 예외 클래스

- 성격이 같은 변수/메소드들은 붙여서 기술하고, 성격이 다르다면 한 줄 개행하고 가급적 주석으로 표시한다.

### 소스파일 내 나열 순서 우선 순위

- 헤더파일과 동일한 순서대로 작성한다.
- 성격이 다른 메소드들은 주석으로 영역을 명확하게 구분한다.
```
/* ************************************************************************** */
/* ------------------------------ CONSTRUCTOR ------------------------------- */
/* ************************************************************************** */ 
```

- 헤더파일 내 명시된 멤버 메소드를 구현하기 위해 필요한 유틸 함수의 경우 프로젝트 전체에 쓰일만한 범용성이 있다면 libft, 해당 소스 파일에서 여러 번 쓰일 가능성이 있다면 소스 파일 상단 util 섹션에 구현한다. 
- 오직 하나의 멤버 메소드만을 위한 유틸 함수들은 해당 메소드의 바로 위에 noname namespace를 만들어서 묶는다.
```
namespace {
	int	setEnv(char **env, int idx, std::string key, std::string val) {...}
	char **dupBaseEnvWithExtraSpace(Config *config, const Request& request) {...}
	std::string getCGIEnvValue(const Request& request, std::string token, Server *server = NULL, Config config = Config()) {...}
}

char**
Server::createCGIEnv(const Request& request)
{
	char **env = dupBaseEnvWithExtraSpace(m_config, request);
	int idx = ft::lenDoubleStr(m_config->get_m_base_env());
	setEnv(env, idx++, "AUTH_TYPE", "");
	setEnv(env, idx++, "CONTENT_LENGTH", getCGIEnvValue(request, "CONTENT_LENGTH"));
	...
}
```