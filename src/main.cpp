#include <iostream>
#include <fstream>

#include <TiebaMsgParse.hpp>

static constexpr auto FILENAME = "Cookie.txt";

std::string get_file(const char* path);

int main()
{
    // Windows下运行避免乱码
    system("chcp 65001");
    system("cls");
    try
    {
        std::string temp = get_file(FILENAME);
        std::string cookie("Cookie: " + temp + "\r\n");
        TiebaMsgParse x(cookie);
        x.one_key_sign_in();
        x.search_kw_and_is_sign();
        x.check_tb_sign();
        x.all_tieba_sign_in();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        system("pause");
    }
}

std::string get_file(const char* path)
{
    std::ifstream in(path, std::ios::in | std::ios::ate);
    if (in.fail())
    {
        throw std::runtime_error("Cookie.txt 不存在");
    }

    const auto size = in.tellg();
    in.seekg(0, std::ios::beg);

    std::string str;
    str.resize(size);
    in.read((char*)str.data(), size);

    in.close();
    return str;
}
