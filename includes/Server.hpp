#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#include <unistd.h>
#include <string>

#include <vector>
#include <map>

#include "Response.hpp"
#include "Request.hpp"
#include "Utils.hpp"

#define MAX_SOCK_NUM 1024

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
#include "HttpConfigServer.hpp"

class Server
{
	private:
		HttpConfigServer m_server_block;
		std::string m_server_name;
		int m_port;
		std::string m_err_page_path;
		int m_content_length;
		size_t m_location_size;

		/* Socket */
		struct sockaddr_in m_server_addr;
		struct sockaddr_in m_client_addr;
		// socklen_t addrlen;
		int m_server_socket;
		int m_client_socket;
		int fd_num;
		int sockfd;
		int readn;
		int maxfd;
		fd_set m_main_fds, m_copy_fds;

		/* Request, Response */
		std::vector<Request> m_requests;
		std::vector<Response> m_responses;

	public:
		Server();
		~Server();
		Server(Server const &other);
		Server& operator=(Server const &rhs);

		void init(
			HttpConfigServer server_block,
			std::string server_name,
			int port,
			std::string err_page_path,
			int content_length,
			size_t location_size
		);
		void setServerAddr(int port);
		bool setServerSocket();
		void runServer();
		void closeServer();

		void getRequest();
		void sendResponse(int clientfd);
		Response parseErrorResponse(int clientfd);

		/* SERVER METHOD UTIL */
		// static Response getDirectory();
		Response get(std::string path, Request req, Response (&func)());
		void post(std::string path, void (&func)(Request req, Response res));
		void put(std::string path, void (&func)(Request req, Response res));
		void del(std::string path, void (&func)(Request req, Response res));
		void update(std::string path, void (&func)(Request req, Response res));
		void options(std::string path, void (&func)(Request req, Response res));
		void trace(std::string path, void (&func)(Request req, Response res));

		/* METHOD */
		Response methodHEAD(int clientfd);
		Response methodGET(int clientfd);
		Response methodPOST(int clientfd);
		Response methodPUT(int clientfd);
		Response methodDELETE(int clientfd);
		Response methodOPTIONS(int clientfd);
		Response methodTRACE(int clientfd);

		Response OptionsPathRoot();
		std::map<std::string, std::string> parseQuery(std::string str);
		Response post_200();

		char** makeCgiEnvp(int clientfd);
		Response executeCgi(int clientfd);
		Response postCGI(int clientfd);

		// Response continue_100();
		// Response switchingProtocols_101();

		Response page200();
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

		Response badRequest_400();
		// Response unauthorized_401();
		// Response paymentRequired_402();
		// Response forbidden_403();
		Response page404();
		Response methodNotAllow_405();
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
		Response methodNotImplemented_501();
		// Response badGateWay_502();
		// Response serviceUnavailble_503();
		// Response gatewayTimeout_504();
		// Response HTTPVersionNotSupproted_505();
};

#endif
