#include"Headers.hpp"

TiebaMsgParse::TiebaMsgParse(const TiebaMsgParse& x)
{
	this->tb = x.tb;
}

size_t TiebaMsgParse::GetTiebaNum() const
{
	return tb.size();
}

std::vector<TiebaMsg> TiebaMsgParse::get_tb() const
{
	return tb;
}

bool TiebaMsgParse::search_kw_and_is_sign(std::string& page)
{
	/*
	* (?<="forum_name":")(.*?)(?=")
	* (?<="is_sign":)(\d)
	* 提取forum_name和is_sign的值
	* 但C++regex库不支持零宽度断言？
	* C++中 " 需要转义 \", \ 转义 \\
	*/
	std::regex regex_name("\"forum_name\":\"(.*?)\"");
	std::regex regex_sign("\"is_sign\":(\\d)");
	int num_not_change_tb_name = 0;
	int num_not_change_tb_kw = 0;
	int num_not_sign_tb = 0;
	Transcoding str;

	if (std::regex_search(page, regex_name))
	{
		for (std::smatch x, y; std::regex_search(page, x, regex_name); page = y.suffix())
		{
			TiebaMsg temp;
			temp.name = x[MATCH_CONTENT];
			//中文贴吧名Unicode字符转码
			temp.name = str.Unicode_escape(temp.name);
			if (temp.name == STRING_ERROR)
			{
				num_not_change_tb_name++;
				continue;
			}
			else
			{
				temp.kw = str.GBKToUTF8(temp.name);
				if (temp.kw == STRING_ERROR)
				{
					num_not_change_tb_kw++;
				}
				else
				{
					temp.kw = str.ChineseURLEncode(temp.kw);
					if (temp.kw == STRING_ERROR)
					{
						num_not_change_tb_kw++;
					}
				}
			}
			//关于贴吧首页相同贴吧名会出现两次
			if (tb.size() > 1)
			{
				int flag = false;
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
				std::cout << "不确定" << temp.name << "吧是否已经签到" << std::endl;
				num_not_sign_tb++;
				continue;
			}
			else
			{
				temp.is_sign = (y[MATCH_CONTENT] == "1" ? true : false);
			}
			tb.push_back(temp);
		}
	}

	int size = tb.size();
	if (!size)
	{
		std::cout << "无贴吧信息" << std::endl;
		if (num_not_sign_tb)
		{
			std::cout << "贴吧是否签到查询失败" << num_not_change_tb_name << "个" << std::endl;
			if (num_not_change_tb_name)
			{
				std::cout << "贴吧名转码失败" << num_not_change_tb_name << "个" << std::endl;
				if (num_not_change_tb_kw)
				{
					std::cout << "贴吧名URL编码失败" << num_not_change_tb_kw << "个" << std::endl;
				}
			}
		}
		return false;
	}
	return true;
}

const std::string TiebaMsgParse::search_tbs(const std::string& page) const
{
	std::smatch tbs;
	//(?<='tbs': ")(.*?)(?=")
	//'tbs':\s"(.*?)"
	if (regex_search(page, tbs, std::regex("'tbs':\\s\"(.*?)\"")))//"\"tbs\":\"(.*?)\""
	{
		return tbs[MATCH_CONTENT];
	}
	else
	{
		return STRING_ERROR;
	}
}

void TiebaMsgParse::check_tb_sign() const
{
	if (!tb.size())
	{
		return;
	}

	int count = 0;
	std::cout << "已签到贴吧：" << std::endl;

	printf("\n");
	for (const auto& x : tb)
	{
		if (x.is_sign)
		{
			std::cout << x.name << std::endl;
			count++;
		}
	}
	printf("\n");

	std::cout << "未签到贴吧：" << std::endl;
	for (const auto& x : tb)
	{
		if (!x.is_sign)
		{
			std::cout << x.name << std::endl;
		}
	}
	printf("\n");

	std::cout << "已签到贴吧" << count << "个，未签到贴吧" << tb.size() - count << "个" << std::endl;
}
