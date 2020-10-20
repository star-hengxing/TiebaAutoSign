#include <iostream>
#include <regex>
#include "include/http/HTTP.hpp"
#include "include/http/HTTPS.hpp"
#include "include/Transcoding.hpp"

#include "include/TiebaMsgParse.hpp"

std::vector<TiebaMsg> TiebaMsgParse::search_kw_and_is_sign()
{
	tb.clear();
	HTTPS a("https://tieba.baidu.com/");
	HTTP* b = &a;
	b->get(cookie);
	std::string page = b->execute();
	/*
	* (?<="forum_name":")(.*?)(?=")
	* (?<="is_sign":)(\d)
	* 提取forum_name和is_sign的值
	* 但C++regex库不支持零宽度断言？
	* C++中 " 需要转义 \", \ 转义 \\
	*/
	std::regex regex_name("\"forum_name\":\"(.*?)\"");
	std::regex regex_sign("\"is_sign\":(\\d)");

	if (std::regex_search(page, regex_name))
	{
		for (std::smatch x, y; std::regex_search(page, x, regex_name); page = y.suffix())
		{
			TiebaMsg temp;
			temp.name = x[MATCH_CONTENT];
			try
			{
				//中文贴吧名Unicode字符转码
				temp.name = Unicode_escape(temp.name);
				temp.kw = GBKToUTF8(temp.name);
				temp.kw = ChineseURLEncode(temp.kw);
				//关于贴吧首页相同贴吧名会出现两次
				if (tb.size() > 1)
				{
					bool flag = false;
					for (const auto& z : tb)
					{
						if (temp.name == z.name)
						{
							flag = true;
							break;
						}
					}
					if (flag)
					{
						break;
					}
				}

				if (!std::regex_search(page, y, regex_sign))
				{
					//贴吧签到信息抓取失败
					throw std::runtime_error("贴吧信息有误");
				}
				else
				{
					temp.is_sign = (y[MATCH_CONTENT] == "1" ? true : false);
				}
			}
			catch (const std::exception & e)
			{
				std::cerr << e.what() << '\n';
				continue;
			}
			tb.push_back(temp);
		}
	}

	return tb;
}

const std::string TiebaMsgParse::search_tbs() const
{
	HTTP get_tbs("http://tieba.baidu.com/dc/common/tbs");
	get_tbs.get(cookie);
	std::string page = get_tbs.execute();
	std::smatch tbs;
	//"tbs":"xxxx"
	if (regex_search(page, tbs, std::regex("\"tbs\":\"(.*?)\"")))
	{
		return tbs[MATCH_CONTENT];
	}
	else
	{
		throw std::runtime_error("获取tbs失败");
	}
}

void TiebaMsgParse::check_tb_sign() const noexcept
{
	if (!tb.size())
	{
		return;
	}

	int count = 0;
	for (const auto& x : tb)
	{
		if (x.is_sign)
		{
			count++;
		}
	}

	printf("共有%d个贴吧\n已签到贴吧%d个\n", tb.size(), count);
}

void TiebaMsgParse::one_key_sign_in() const
{
	try
	{
		std::string tbs = this->search_tbs();
		std::string data("ie=utf-8&tbs=" + tbs);

		HTTPS temp("https://tieba.baidu.com/tbmall/onekeySignin1");
		HTTP* x = &temp;
		x->post(cookie, data);
		std::string page = x->execute();

		std::smatch result;
		if (std::regex_search(page, result, std::regex("\"error\":\"(.*?)\"")))
		{
			std::string check = result[MATCH_CONTENT];
			if (check.find("success") != std::string::npos) {}
			else if (check.find("forums is signed") != std::string::npos) {}
			else
			{
				throw std::runtime_error("一键签到失败");
			}
		}
	}
	catch (const char *e)
	{
		std::cerr << e << '\n';
		throw;
	}
}

void TiebaMsgParse::single_tieba_sign_in(const std::string& name, const std::string& kw) const
{
	try
	{
		std::string tbs = search_tbs();
		std::string data = "ie=utf-8&kw=" + kw + "&tbs=" + tbs;

		HTTPS y("https://tieba.baidu.com/sign/add");
		HTTP* x = &y;
		x->post(cookie, data);
		std::string page = x->execute();

		std::smatch result;
		//"error":"(.*?)"
		std::regex_search(page, result, std::regex("\"error\":\"(.*?)\""));
		if (result.size() >= 2)
		{
			page = result[MATCH_CONTENT];
			if (!page.size())
			{
				std::cout << name << "吧签到成功" << std::endl;
				return;
			}
			throw std::runtime_error(Unicode_escape(page));
		}
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what() << '\n';
	}
}

void TiebaMsgParse::all_tieba_sign_in() const
{
	for (const auto& x : tb)
	{
		if (!x.is_sign)
		{
			single_tieba_sign_in(x.name, x.kw);
			Sleep(100);
		}
	}
}