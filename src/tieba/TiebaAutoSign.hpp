#ifndef _TIEBAAUTOSIGN_HPP
#define _TIEBAAUTOSIGN_HPP

#include"Headers.hpp"

class TiebaAutoSign
{
private:
	std::string Cookie;
	TiebaMsgParse tieba;
	std::vector<TiebaMsg> tb;
public:
	TiebaAutoSign();
	explicit TiebaAutoSign(const std::string& x);

	bool sign_tieba();

	bool GetTiebaMsg();
	void GetTiebaMsg() const;
};

#endif
