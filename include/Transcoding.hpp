#pragma once

#include <string>

constexpr auto STRING_ERROR = "字符串转换失败";
constexpr auto URL_ENCODE_ERROR = "url encoded error";
constexpr auto URL_ESCAPE_ERROR = "Unicode escape error";

std::wstring StringToWstring(const std::string& str);
std::string WstringToString(const std::wstring& str);
std::string ChineseURLEncode(const std::string& str);
std::string Unicode_escape(const std::string& str);
std::string GBKToUTF8(const std::string& str);
