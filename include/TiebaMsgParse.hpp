#pragma once

#include <vector>
#include <string>

struct TiebaMsg
{
    std::string name; // 贴吧名
    std::string kw; // 贴吧名URL编码
    bool is_sign;
    TiebaMsg() : is_sign(false) {}
};

class TiebaMsgParse
{
private:
    std::vector<TiebaMsg> tb;
    std::string cookie;
public:
    explicit TiebaMsgParse(const std::string & c) : cookie(c) {};

    std::vector<TiebaMsg> search_kw_and_is_sign();

    const std::string search_tbs() const;

    void check_tb_sign() const noexcept;

    void one_key_sign_in() const;

    void single_tieba_sign_in(const std::string& name,const std::string& kw) const;

    void all_tieba_sign_in() const;
};
