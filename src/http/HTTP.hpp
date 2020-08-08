#ifndef _HTTP_HPP
#define _HTTP_HPP

#include"Headers.hpp"

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

    virtual bool connect_server();

    bool parse_response();
    int response_status_code();

    const std::string GetPage() const;

    void get();
    void get(const std::string & headers);
    void post(const std::string & headers,const std::string & data);

    void CheckError(const bool bool_execute, const char* ErrorMessage);
};

#endif
