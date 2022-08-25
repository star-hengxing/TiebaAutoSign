#pragma once

#include <string>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "HTTP.hpp"

class HTTPS : public HTTP
{
private:
    SSL_CTX* ctx = nullptr;
    SSL* ssl = nullptr;

public:
    HTTPS();
    explicit HTTPS(const std::string& url);
    ~HTTPS();

    void connect_server() override;
};
