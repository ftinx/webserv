#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#include <unistd.h>
#include <string>

#include <queue>
#include <vector>
#include <map>

#include "Response.hpp"
#include "Request.hpp"
#include "HttpConfig.hpp"
#include "HttpConfigLocation.hpp"
#include "Utils.hpp"

#define MAX_SOCK_NUM 1024
#define CGI_ENV_NUM 16

/*
**  ** TODO
**
**  1. MAXLINE과 SOCK_SETSIZE size 설정 이유 찾기
**  2. perror() => message 함수로 교체하기
**  3. char *bin2hex 개조
**
**
**  ** Config file 에서 반영해야하는 것들
**
**  1. listen(this->m_server_socket, 5) 의 연결 대기열 개수 => 임의로 10개 지정
**  2. this->m_server_addr.sin_port = htons(PORT);의 PORT => 임의로 #define PORT 3601로 지정
*/

class Server
{
	private:
		/* Config */
		std::map<std::string, std::string> m_mime_types;
		HttpConfigServer m_server_block;
		std::string m_server_name;
		int m_port;
		std::string m_err_page_path;
		int m_content_length;
		size_t m_location_size;
		std::string m_root;

		/* Parse */
		std::vector<HttpConfigLocation> m_headLocation;
		std::vector<HttpConfigLocation> m_getLocation;
		std::vector<HttpConfigLocation> m_postLocation;
		std::vector<HttpConfigLocation> m_putLocation;
		std::vector<HttpConfigLocation> m_deleteLocation;
		std::vector<HttpConfigLocation> m_optionsLocation;
		std::vector<HttpConfigLocation> m_traceLocation;
		std::vector<std::string> m_httpConfigFilePathSet;

		/* Socket */
		struct sockaddr_in m_server_addr;
		struct sockaddr_in m_client_addr;
		int m_server_socket;
		int m_client_socket;
		int fd_num;
		int sockfd;
		int readn;
		int maxfd;
		fd_set m_main_fds, m_copy_fds;
		fd_set m_read_fds, m_write_fds;
		fd_set m_copy_read_fds, m_copy_write_fds;

		/* Request, Response */
		std::vector<Request> m_requests;
		std::vector<Response> m_responses;

	public:
		Server();
		~Server();
		Server(Server const &other);
		Server& operator=(Server const &rhs);

		/* getter */
		std::string get_m_server_name();
		int get_m_port();
		std::vector<HttpConfigLocation> get_m_postLocation();
		fd_set get_m_write_fds();

		/* setter */

		/* util */
		void noteHttpConfigLocation();
		void init(
			HttpConfigServer server_block,
			std::string server_name,
			int port,
			std::string err_page_path,
			int content_length,
			size_t location_size,
			std::string root,
			std::map<std::string, std::string> mime_types
		);
		void setServerAddr(int port);
		bool setServerSocket();
		void runServer();
		void closeServer();

		void getRequest();
		static Response makeResponseMessage(
			int statusCode,
			std::string path = "./www/index.html", std::string method="", std::string contentType="text/html; charset=UTF-8",
			int dateHour=0, int dateMinute=0, int dateSecond=0,
			std::string contentLanguage="ko, en", std::string server="ftnix/1.0 (MacOS)"
		);
		static Response
		makeResponseBodyMessage(
			int statusCode,
			std::string body = "", std::string method="", std::string contentType="text/html; charset=UTF-8",
			int dateHour=0, int dateMinute=0, int dateSecond=0,
			std::string contentLanguage="ko, en", std::string server="ftnix/1.0 (MacOS)"
		);
		void sendResponse(int clientfd);
		Response parseErrorResponse(int clientfd);
		bool checkHttpConfigFilePath(std::string path, std::string method="");
		bool checkHttpConfigFilePathHead(std::string path);

		/* SERVER METHOD UTIL */
		static Response getDirectory(Request req, Response res);
		static Response postAuth(Request req, Response res);
		static std::map<std::string, std::string> parseQuery(std::string str);
		static Response HttpConfigPost(Request req, Response res);

		/* Cgi Utils */
		//static std::string parseCgiPathInfo(Request req);
		std::map<std::string, std::string> makeCgiEnvpMap(Request req, Response res);
		char** makeCgiEnvp(Request req, Response res);
		Response executeCgi(Request req, Response res, fd_set *write_fds);

		Response get(std::string path, Request req, Response res, Response (*func)(Request req, Response res));
		Response post(std::string path, Request req, Response res, fd_set *write_fds, Response (*func)(Request req, Response res, fd_set *write_fds));
		Response put(std::string path, Request req, Response res, Response (*func)(Request req, Response res));
		Response del(std::string path, Request req, Response res, Response (*func)(Request req, Response res));
		Response options(std::string path, Request req, Response res, Response (*func)(Request req, Response res));
		Response trace(std::string path, Request req, Response res, Response (*func)(Request req, Response res));

		/* METHOD */
		std::string getMimeType(std::string extension);
		std::string makeAutoindexPage(std::string path);
		static Response getTest(Request req, Response res);

		Response methodHEAD(int clientfd);
		Response methodGET(int clientfd, std::string method="");
		Response methodPOST(int clientfd);
		Response methodPUT(int clientfd);
		Response methodDELETE(int clientfd);

		Response options_405(std::string allow_method);
		Response options_204(std::string allow_method);
		std::string makeAllowMethod(std::vector<Method> v, bool *options_allowed);
		Response methodOPTIONS(int clientfd);
		Response methodTRACE(int clientfd);

		Response OptionsPathRoot();

		// Response continue_100();
		// Response switchingProtocols_101();

		static Response page200();
		// Response created_201();
		// Response Accepted_202();
		// Response nonAuthoritativeInformation_203();
		// Response noContent_204();
		// Response resetContetn_205();
		// Response partialContent_206();

		// Response multipleChoices_300();
		// Response movedPermanently_301();
		// Response found_302();
		// Response seeOther_303();
		// Response notModified_304();
		// Response useProxy_305();
		// Response temporaryRedirect_307();

		static Response badRequest_400();
		// Response unauthorized_401();
		// Response paymentRequired_402();
		// Response forbidden_403();
		static Response page404(std::string path);
		static Response methodNotAllow_405();
		// Response notAcceptable_406();
		// Response proxyAuthenticationRequired_407();
		// Response requestTimeout_408();
		// Response conflick_409();
		// Response gone_410();
		// Response lengthRequired_411();
		// Response preconditionFailed_412();
		// Response payloadTooLarge_413();
		// Response uriTooLong_414();
		// Response unsupportedMediaType_415();
		// Response rangeNotSatisfiable_416();
		// Response expectationFailed_417();
		// Response upgradeRequired_426();

		// Response internalServerError_500();
		static Response methodNotImplemented_501();
		// Response badGateWay_502();
		// Response serviceUnavailble_503();
		// Response gatewayTimeout_504();
		// Response HTTPVersionNotSupproted_505();
};

#endif
