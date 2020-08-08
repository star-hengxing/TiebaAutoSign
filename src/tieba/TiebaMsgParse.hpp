#ifndef _TIEBAMSGPARSE_HPP
#define _TIEBAMSGPARSE_HPP

#include"Headers.hpp"

class TiebaMsgParse
{
private:
    std::vector<TiebaMsg>tb;

public:
    TiebaMsgParse() {}
    explicit TiebaMsgParse(const TiebaMsgParse & x);

    bool search_kw_and_is_sign(std::string&  page);
    const std::string  search_tbs(const std::string& page) const;

    size_t GetTiebaNum() const;
    void check_tb_sign() const;
    std::vector<TiebaMsg> get_tb() const;
};

#endif
