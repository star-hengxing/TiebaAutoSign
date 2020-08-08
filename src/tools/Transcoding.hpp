#ifndef _TRANSCODING_HPP
#define _TRANSCODING_HPP

#include"Headers.hpp"

class Transcoding
{
public:
    Transcoding() {}

    const std::wstring StringToWstring(const std::string& str) const;
    const std::string WstringToString(const std::wstring& str) const;

    const std::string ChineseURLEncode(const std::string& str) const;
    const std::string Unicode_escape(const std::string& str) const;

    const std::string GBKToUTF8(const std::string& str) const;
};

#endif
