#include <iostream>
#include <fstream>
#include "include/TiebaMsgParse.hpp"

constexpr static const char* const FILENAME("Cookie.txt");

using namespace std;

const string get_file(const string& name);

int main()
{
	system("chcp 65001");// Windows下运行避免乱码
	system("cls");
	try
	{
		string temp = get_file(FILENAME);
		string cookie("Cookie: " + temp + "\r\n");
		TiebaMsgParse x(cookie);
		x.one_key_sign_in();
		x.search_kw_and_is_sign();
		x.check_tb_sign();
		x.all_tieba_sign_in();
	}
	catch (const exception & e)
	{
		cerr << e.what() << '\n';
		system("pause");
	}

	return 0;
}

const string get_file(const string& name)
{
	fstream load;
	string str;
	load.open(name, ios::in);
	if (load)
	{
		while (load.peek() != EOF)
		{
			getline(load, str);
		}
		load.close();
	}
	if (str.empty())
	{
		throw runtime_error("文件不存在");
	}
	return str;
}
