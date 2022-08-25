#include <iostream>
#include <regex>

#include <http/HTTP.hpp>

HTTP::HTTP()
{
    CheckError(WSAStartup(MAKEWORD(2, 2), &_wsadata) != 0, "初始化套接字库失败");
    CheckError(LOBYTE(_wsadata.wVersion) != 2 || HIBYTE(_wsadata.wHighVersion) != 2, "套接字库版本号不符");
}

HTTP::HTTP(const std::string& url) : HTTP::HTTP()
{
    std::smatch url_msg;

    if (regex_search(url, url_msg, std::regex("(http|https)://([\\w+,\\.,$m]+)(.*)")))
    {
        if (url_msg[MATCH_CONTENT] == "http")
        {
            server_msg.port = 80;
        }
        else
        {
            server_msg.port = 443;
        }
        server_msg.HostName = url_msg[MATCH_CONTENT + 1];
        this->url = url_msg[MATCH_CONTENT + 2];//相对路径
        //this->url = url;//绝对路径
    }
    else
    {
        throw  std::runtime_error("解析URL失败");
    }
    hostent* host = gethostbyname(server_msg.HostName.c_str());
    CheckError(!host, "解析域名失败");
    //server_msg.HostName=host->h_name;
    server_msg.AddressType = ((host->h_addrtype == AF_INET) ? AF_INET : AF_INET6);//IPV4 OR IPV6
    //服务器IP
    for (int i = 0; host->h_addr_list[i]; i++)
    {
        server_msg.IP.push_back(inet_ntoa(*(in_addr*)host->h_addr_list[i]));//转换点分十进制IP
    }
    server_addr.sin_family = server_msg.AddressType;
    server_addr.sin_port = htons(server_msg.port);
    server_addr.sin_addr.S_un.S_addr = inet_addr(server_msg.IP[0].c_str());
    client_socket = socket(server_msg.AddressType, SOCK_STREAM, IPPROTO_TCP);
    CheckError(client_socket == INVALID_SOCKET, "启用套接字失败");
}

HTTP::~HTTP()
{
    closesocket(client_socket);
    WSACleanup();
}

void HTTP::connect_server()
{
    CheckError(connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR, "连接服务器失败");
    CheckError(send(client_socket, request.c_str(), request.size(), 0) == SOCKET_ERROR, "发送请求报文失败");

    char* buffer = new char[BUFFER * BUFFER];
    memset(buffer,0,BUFFER * BUFFER);
    int index = 0;
    int buffer_size;
    response.clear();//刷新缓冲
    while (1)
    {
        buffer_size = recv(client_socket, buffer, BUFFER, 0);
        if (buffer_size > 0)
        {
            index += buffer_size;
        }
        else
        {
            response += buffer;
            break;
        }
    }
    delete[] buffer;
    closesocket(client_socket);
}

void HTTP::parse_response()
{
    if (response.empty())
    {
        return;
    }
    std::string response_head;
    int index_head, i;
    std::smatch response_msg;
    request_head_and_value.clear();
    //(HTTP/1.1)\s(\d+)\s(.*)
    std::regex re("(HTTP/1.1)\\s(\\d+)\\s(.*)");
    for (i = 0, index_head = i;; i++)
    {
        if (i > response.size())
        {
            return;
        }
        if (response[i] == '\r' && response[i + 1] == '\n')
        {
            response_head = response.substr(index_head, i - index_head);
            if (regex_search(response_head, response_msg, re))
            {
                request_head_and_value["Scheme"] = response_msg[MATCH_CONTENT];
                request_head_and_value["Status"] = response_msg[MATCH_CONTENT + 1];
                request_head_and_value["Description"] = response_msg[MATCH_CONTENT + 2];
                break;
            }
            else
            {
                throw std::runtime_error("解析响应行失败");
            }
        }
    }
    //响应头部
    for (i = i + 2, index_head = i;; i++)
    {
        if (response[i] == ':' && response[i + 1] == ' ')
        {
            response_head = response.substr(index_head, i - index_head);
            index_head = i + 2;
        }
        else if (response[i] == '\r' && response[i + 1] == '\n')
        {
            request_head_and_value[response_head] = response.substr(index_head, i - index_head);
            if (response[i + 2] == '\r' && response[i + 3] == '\n')
            {
                break;
            }
            index_head = i + 2;
        }
    }
    if (request_head_and_value.size() < 3)
    {
        throw std::runtime_error("解析响应行失败");
    }
    //解析后把响应主体拷贝给Page
    page = response.substr(i + 4, response.size());
}

int HTTP::response_status_code()
{
    if (request_head_and_value.size() > 0)
    {
        return strtol(request_head_and_value["Status"].c_str(), NULL, 10);
    }
}

std::string HTTP::get_page() const noexcept
{
    return page;
}

const std::string HTTP::execute()
{
    connect_server();
    parse_response();
    return page;
}

//纯get方法
void HTTP::get()
{
    request = "GET " + url + " HTTP/1.1\r\n";
    request += "Host: " + server_msg.HostName + "\r\n";
    request += "Connection: close\r\n";
    request += "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.105 Safari/537.36\r\n";
    request += "Accept-Encoding: identity\r\n";
    request += "Accept-Language: zh-CN,zh;q=0.9\r\n";
    //request += "Upgrade-Insecure-Requests: 1\r\n";
    //request += "referer: "+ server_msg.HostName +"\r\n";
    request += "\r\n";
}

void HTTP::get(const std::string& headers)
{
    request = "GET " + url + " HTTP/1.1\r\n";
    request += "Host: " + server_msg.HostName + "\r\n";
    request += "Connection: close\r\n";
    request += "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.105 Safari/537.36\r\n";
    request += "Accept-Encoding: identity\r\n";
    request += "Accept-Language: zh-CN,zh;q=0.9\r\n";
    //request += "Upgrade-Insecure-Requests: 1\r\n";
    //request += "referer: " + server_msg.HostName + "\r\n";
    request += headers;
    request += "\r\n";
}

void HTTP::post(const std::string& headers, const std::string& data)
{
    request = "POST " + url + " HTTP/1.1\r\n";
    request += "Host: " + server_msg.HostName + "\r\n";
    request += "Connection: close\r\n";
    request += "Accept-Encoding: identity\r\n";
    request += "Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n";
    request += "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.105 Safari/537.36\r\n";
    request += "Content-Length: ";
    request += std::to_string(data.size());
    request += "\r\n";
    request += headers;
    request += "\r\n";
    request += data;
}

//检查异常处理
void HTTP::CheckError(const bool bool_execute, const char* ErrorMessage)
{
    if (bool_execute)
    {
        throw std::runtime_error(ErrorMessage);
    }
}
