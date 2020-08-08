#include"Headers.hpp"

const std::wstring Transcoding::StringToWstring(const std::string& str) const
{
	std::wstring return_string;
	int size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	if (size == 0)
	{
		return WSTRING_ERROR;
	}
	wchar_t* pstr = new wchar_t[size + 1];
	if (MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), pstr, size) == 0)
	{
		delete[] pstr;
		return WSTRING_ERROR;
	}
	pstr[size] = '\0';
	return_string = pstr;
	delete[] pstr;
	pstr = nullptr;
	return return_string;
}

const std::string Transcoding::WstringToString(const std::wstring& str) const
{
	std::string return_string;
	int size = WideCharToMultiByte(CP_ACP, 0, str.c_str(), str.size(), NULL, 0, NULL, NULL);
	if (size == 0)
	{
		return STRING_ERROR;
	}
	char* pstr = new char[size + 1];
	if (WideCharToMultiByte(CP_ACP, 0, str.c_str(), str.size(), pstr, size, NULL, NULL) == 0)
	{
		delete[] pstr;
		return STRING_ERROR;
	}
	pstr[size] = '\0';
	return_string = pstr;
	delete[] pstr;
	pstr = nullptr;
	return return_string;
}

const std::string Transcoding::ChineseURLEncode(const std::string& str) const
{
	std::string return_string;
	int size = str.size() * 3 + 1;
	char* pstr = new char[size];
	int i = 0;

	for (const auto& y : str)
	{
		if (y >= 0 && y <= 0x80)
		{
			if (y >= 0x41 && y <= 0x5A || y >= 0x61 && y <= 0x7A)
			{
				pstr[i] = y;
				i++;
			}
			else
			{
				if (sprintf(pstr + i, "%%%02X", (unsigned char)y) < 0)
				{
					std::cout << "url encoded error!" << std::endl;
					return STRING_ERROR;
				}
				i += 3;
			}
		}
		else
		{
			if (sprintf(pstr + i, "%%%02X", (unsigned char)y) < 0)
			{
				std::cout << "url encoded error!" << std::endl;
				return STRING_ERROR;
			}
			i += 3;
		}
	}
	pstr[i] = '\0';
	return_string = pstr;
	delete[] pstr;
	pstr = nullptr;
	return return_string;
}

const std::string Transcoding::Unicode_escape(const std::string& str) const
{
	if (str.find("\\u") != std::string::npos)
	{
		int index_head = 0, index_end = 0;
		std::string temp_string, return_string;
		std::wstring wstr;
		while (1)
		{
			index_end = str.find("\\u", index_head);
			if (index_end > index_head)
			{
				return_string += str.substr(index_head, index_end - index_head);
			}
			if (index_end == std::string::npos)
			{
				if (index_head != str.size())
				{
					return_string += str.substr(index_head, str.size() - index_head);
				}
				return return_string;
			}
			else
			{
				index_end += 2;
				//提取16进制Unicode的4个字符
				temp_string = str.substr(index_end, 4);
				//转换为双字节字符对应数字
				wstr = strtol(temp_string.c_str(), NULL, 16);
				if (!wstr.c_str())
				{
					std::cout << "Unicode escape error!" << std::endl;
					return STRING_ERROR;
				}
				else
				{
					index_head = index_end + 4;
					return_string += WstringToString(wstr);
				}
			}
		}
	}
	else
	{
		return str;
	}
}

const std::string Transcoding::GBKToUTF8(const std::string& str) const
{
	std::wstring x = StringToWstring(str);
	std::string return_string;
	int size = WideCharToMultiByte(CP_UTF8, 0, x.c_str(), x.size(), NULL, 0, NULL, NULL);
	if (size == 0)
	{
		return STRING_ERROR;
	}
	char* pstr = new char[size + 1];
	if (WideCharToMultiByte(CP_UTF8, 0, x.c_str(), x.size(), pstr, size, NULL, NULL) == 0)
	{
		delete[] pstr;
		return STRING_ERROR;
	}
	pstr[size] = '\0';
	return_string = pstr;
	delete[] pstr;
	pstr = nullptr;
	return return_string;
}