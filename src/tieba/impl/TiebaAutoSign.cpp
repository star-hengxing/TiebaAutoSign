#include"Headers.hpp"

TiebaAutoSign::TiebaAutoSign()
{
	std::string tb_cookie;
	std::fstream load;
	load.open(SAVE_COOKIE_FILE_NAME, std::ios::in);
	if (!load)
	{
		std::cout << "本地无保存的Cookie" << std::endl;

		std::cout << "请输入Cookie：";
		getline(std::cin, tb_cookie);

		system("cls");
		TiebaAutoSign::TiebaAutoSign(tb_cookie);
	}
	else
	{
		while (load.peek() != EOF)
		{
			getline(load, tb_cookie);
		}
	}
	Cookie = "Cookie: " + tb_cookie + "\r\n";
	load.close();
}

TiebaAutoSign::TiebaAutoSign(const std::string& x)
{
	Cookie = "Cookie: " + x + "\r\n";
	std::fstream save;
	save.open(SAVE_COOKIE_FILE_NAME, std::ios::trunc | std::ios::out);
	if (!save)
	{
		std::cout << "保存Cookie失败" << std::endl;
	}
	else
	{
		save << x;
	}
	save.close();
}

bool TiebaAutoSign::GetTiebaMsg()
{
	HTTP* x = nullptr;
	std::string tieba_url = "https://tieba.baidu.com/";
	std::string page;
	HTTPS your_tieba(tieba_url);
	x = &your_tieba;

	x->get(Cookie);
	if (x->connect_server())
	{
		x->parse_response();
		if (x->response_status_code() == 200)
		{
			TiebaMsgParse y;
			page = x->GetPage();
			if (y.search_kw_and_is_sign(page))
			{
				y.check_tb_sign();
				tb = y.get_tb();
			}
			else
			{
				return false;
			}
		}
		else
		{
			std::cout << x->response_status_code() << std::endl;
		}
	}
	else
	{
		std::cout << "贴吧网页获取失败" << std::endl;
	}
	return true;
}

bool TiebaAutoSign::sign_tieba()
{
	Transcoding str;
	TiebaMsgParse tieba;
	HTTP* x = nullptr;
	std::string url;
	std::string page;
	std::string data;
	int count = 0;
	for (auto& y : tb)
	{
		if (!y.is_sign)
		{
			Sleep(500);
			count++;
			// url = "http://tieba.baidu.com/dc/common/tbs";
			// HTTP z(url);
			url = "https://tieba.baidu.com/f?kw=" + y.kw + "&fr=index";
			HTTPS z(url);
			x = &z;
			x->get(Cookie);
			if (x->connect_server())
			{
				x->parse_response();
				if (x->response_status_code() == 200)
				{
					page = x->GetPage();
					y.tbs = tieba.search_tbs(page);

					data = "ie=utf-8&kw=" + y.kw + "&tbs=" + y.tbs;
					url = "https://tieba.baidu.com/sign/add";
					HTTPS u(url);
					x = &u;
					x->post(Cookie, data);
					if (x->connect_server())
					{
						x->parse_response();
						if (x->response_status_code() == 200)
						{
							page = x->GetPage();
							std::smatch result;
							//"error":"(.*?)"
							std::regex re("\"error\":\"(.*?)\"");
							std::regex_search(page, result, re);
							if (result.size() >= 2)
							{
								page = result[MATCH_CONTENT];
								if (!page.size())
								{
									std::cout << y.name << "吧签到成功" << std::endl;
									continue;
								}
								std::cout << str.Unicode_escape(page) << std::endl;
								return false;
							}
						}
						else
						{
							std::cout << "错误代码：" << x->response_status_code() << std::endl;
							return false;
						}
					}
				}
				else
				{
					std::cout << "错误代码：" << x->response_status_code() << std::endl;
					return false;
				}
			}
			else
			{
				std::cout << "获取单个贴吧首页失败" << std::endl;
				return false;
			}
		}
	}
	if (!count)
	{
		std::cout << "没有贴吧需要签到" << std::endl;
	}
	return true;
}
