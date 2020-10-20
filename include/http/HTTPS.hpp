#ifndef _HTTPS_HPP
#define _HTTPS_HPP

#include <openssl/ssl.h>
#include <openssl/err.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
#include <string>

#include "HTTP.hpp"

class HTTPS : public HTTP
{
private:
    SSL_CTX* ctx=nullptr;
    SSL* ssl=nullptr;
public:
    HTTPS();
    explicit HTTPS(const std::string & url);
    ~HTTPS();

    void connect_server() override;
};

#endif
