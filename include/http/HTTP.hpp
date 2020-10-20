#ifndef _HTTP_HPP
#define _HTTP_HPP

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <string>
#include <vector>
#include <map>

constexpr int BUFFER(1024);

constexpr int MATCH_CONTENT(1);//regex_search()匹配成功得到的结果放在smatch容器里，容器第一个是正则表达式选定范围的整个字符串，第二个才是匹配()内的字符串(通常来说)

struct HostNameMsg
{
	std::string HostName;
	int AddressType;
	std::vector<std::string> IP;
	int port;
};

class HTTP
{
protected:
    SOCKET client_socket;
    SOCKADDR_IN server_addr;
    WSADATA _wsadata;
    //static WSADATA _wsadata;

    HostNameMsg server_msg;
    std::map<std::string,std::string> request_head_and_value;

    std::string url;
    std::string request;
    std::string response;
    std::string page;//response body
public:
    HTTP();
    explicit HTTP(const std::string & URL);
    virtual ~HTTP();

    virtual void connect_server();

    void parse_response();
    int response_status_code();

    virtual const std::string execute();

    inline const std::string GetPage() const noexcept;

    void get();
    void get(const std::string & headers);
    void post(const std::string & headers,const std::string & data);

    void CheckError(const bool bool_execute, const char* ErrorMessage);
};

#endif
