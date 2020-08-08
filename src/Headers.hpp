#ifndef _HEADERS_HPP
#define _HEADERS_HPP

#pragma warning (disable:4996)

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#include <openssl/ssl.h>
#include <openssl/err.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")

#include <iostream>
#include <fstream>
#include <Windows.h>

#include <vector>
#include <map>
#include <string>

#include <regex>


constexpr int BUFFER = 1024;

constexpr int MATCH_CONTENT = 1;//regex_search()匹配成功得到的结果放在smatch容器里，容器第一个是正则表达式选定范围的整个字符串，第二个才是匹配()内的字符串(通常来说)

constexpr const char* SAVE_COOKIE_FILE_NAME = "Cookie.txt";
constexpr const char* STRING_ERROR = "npos";
constexpr const wchar_t* WSTRING_ERROR = L"字符串转换失败";

struct HostNameMsg
{
	std::string HostName;
	int AddressType;
	std::vector<std::string> IP;
	int port;
};

struct TiebaMsg
{
	std::string name;//贴吧名
	std::string kw;//贴吧名URL编码
	std::string tbs;//贴吧一次性令牌
	bool is_sign;
	TiebaMsg() : name(""), kw(""), tbs(""), is_sign(false) {}
};

#include "Transcoding.hpp"
#include "HTTP.hpp"
#include "HTTPS.hpp"
#include "TiebaMsgParse.hpp"
#include "TiebaAutoSign.hpp"

#endif //_HEADERS_HPP
