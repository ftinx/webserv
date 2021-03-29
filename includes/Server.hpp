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
#define MAXLINE 2048

class Server
{
	private:
		/* Config */
		std::map<std::string, std::string> m_mime_types;
		HttpConfigServer m_server_block;
		std::string m_server_name;
		int m_port;
		int m_content_length;
		size_t m_location_size;
		std::string m_root;

		/* Parse */
		std::vector<HttpConfigLocation> m_head_location;
		std::vector<HttpConfigLocation> m_get_location;
		std::vector<HttpConfigLocation> m_post_location;
		std::vector<HttpConfigLocation> m_put_location;
		std::vector<HttpConfigLocation> m_delete_location;
		std::vector<HttpConfigLocation> m_options_location;
		std::vector<HttpConfigLocation> m_trace_location;
		std::vector<std::string> m_http_config_file_path_set;
		std::map<std::string, bool> m_get_location_auto_index;

		// /* Socket */
		struct sockaddr_in m_server_addr;
		struct sockaddr_in m_client_addr;
		int m_server_socket;
		int m_client_socket;
		int m_sockfd;

		std::vector< std::pair<FdType, int> > m_fd_table;

		/* Request, Response */
		std::vector<Request> m_requests;
		std::vector<Response> m_responses;

	public:
		Server();
		~Server();
		Server(Server const &other);
		Server& operator=(Server const &rhs);

		/* Getter */
		std::string get_m_server_name();
		int get_m_port();
		std::vector<HttpConfigLocation> get_m_post_location();
		int get_m_server_socket() const;
		int get_m_client_socket() const;

		/* Setter */

		/* Init */
		void noteHttpConfigLocation();
		void init(
			HttpConfigServer server_block,
			std::string server_name,
			int port,
			int content_length,
			size_t location_size,
			std::string root,
			std::map<std::string, std::string> mime_types
		);

		/* Socket */
		void setServerAddr(int port);
		bool setServerSocket();
		void closeServer();

		/* Request */
		void acceptSocket(fd_set *main_fds, int *max_fd);
		void getRequest(fd_set *, fd_set *, fd_set *, fd_set *, int *);
		void readProcess(fd_set *main_fds, fd_set *read_fds, fd_set *write_fds);
		void writeProcess(fd_set *copy_write_fds, fd_set *write_fds);

		/* Server Util */
		std::vector<HttpConfigLocation> getMethodLocation(Method method);
		bool isMatchingLocation(std::string location, std::string path_in, std::string *rest);
		bool checkAcceptedMethod(std::vector<Method> v, Method method);
		std::string checkHttpConfigFilePath(std::string path_in);
		void resetRequest(Request *req);
		void writeLog(std::string type, Response response);

		/* HEAD */
		Response methodHEAD(int clientfd, std::string method="HEAD");

		/* GET */
		// bool checkHttpConfigFilePath(std::string path, std::string method="");
		// bool checkHttpConfigFilePathHead(std::string path);
		std::string getMimeType(std::string extension);
		std::string makeAutoindexPage(std::string root, std::string path);
		Response methodGET(int clientfd, std::string method="GET");

		/* POST */
		std::map<std::string, std::string> makeCgiEnvpMap(Request req, Response res);
		char** makeCgiEnvp(Request req, Response res);
		Response executeCgi(Request req, Response res, std::string method);
		static std::map<std::string, std::string> parseQuery(std::string str);
		static Response postAuth(Request req, Response res);
		Response methodPOST(int clientfd, std::string method="POST");

		/* PUT */
		/* cgi part should be added */
		Response methodPUT(int clientfd, std::string method="PUT");

		/* DELETE */
		Response methodDELETE(int clientfd, std::string method="DELETE");

		/* OPTIONS */
		std::string makeAllowMethod(std::vector<Method> v);
		Response methodOPTIONS(int clientfd, std::string method="OPTIONS");

		/* TRACE */
		Response methodTRACE(int clientfd, std::string method="TRACE");

		/* Response Message */
		static Response makeResponseMessage(
			int status_code, std::string path="./www/index.html", std::string transfer_encoding="",
			std::string method="", std::string content_type="text/html; charset=UTF-8",
			std::string referer="", int date_hour=0, int date_minute=0, int date_second=0,
			std::string allow_method="", std::string content_location="", std::string location="./www/index.html",
			std::string content_language="ko", std::string server="ftnix/1.0 (MacOS)"
		);
		static Response makeResponseBodyMessage(
			int status_code, std::string body="", std::string transfer_encoding="",
			std::string method="", std::string content_type="text/html; charset=UTF-8",
			std::string referer="", int date_hour=0, int date_minute=0, int date_second=0,
			std::string allow_method="", std::string content_location="", std::string location="./www/index.html",
			std::string content_language="ko", std::string server="ftnix/1.0 (MacOS)"
		);
		std::string makeErrorPage(int status);

		/* Server Method Util */
		Response parseErrorResponse(int clientfd);
		Response checkValidRequestHeader(int clientfd);
		Response post(std::string path, Request req, Response res, Response (*func)(Request req, Response res));

		/* Response */
		bool sendResponse(int clientfd);
};

#endif
