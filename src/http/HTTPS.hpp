#ifndef _HTTPS_HPP
#define _HTTPS_HPP

#include"Headers.hpp"

class HTTPS : public HTTP
{
private:
    SSL_CTX* ctx;
    SSL* ssl;
public:
    HTTPS();
    explicit HTTPS(const std::string & url);
    ~HTTPS();

    bool connect_server();
};

#endif
