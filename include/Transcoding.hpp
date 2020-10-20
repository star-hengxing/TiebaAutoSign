#ifndef _TRANSCODING_HPP
#define _TRANSCODING_HPP

#include <string>

constexpr const char* const STRING_ERROR("字符串转换失败");
constexpr const char* const URL_ENCODE_ERROR("url encoded error");
constexpr const char* const URL_ESCAPE_ERROR("Unicode escape error");


const std::wstring StringToWstring(const std::string& str);
const std::string WstringToString(const std::wstring& str);

const std::string ChineseURLEncode(const std::string& str);
const std::string Unicode_escape(const std::string& str);

const std::string GBKToUTF8(const std::string& str);


#endif
