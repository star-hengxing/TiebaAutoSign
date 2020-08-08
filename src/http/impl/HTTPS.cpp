#include"Headers.hpp"

HTTPS::HTTPS() {}

HTTPS::HTTPS(const std::string & url) : HTTP::HTTP(url)
{
	SSL_load_error_strings();//加载SSL错误信息 
	SSLeay_add_ssl_algorithms();//添加SSL的加密/HASH算法 
	const SSL_METHOD* meth = SSLv23_client_method();//客户端，服务端选择SSLv23_server_method() 
    ctx = SSL_CTX_new(meth);//建立CTX的SSL会话环境
	if (!ctx)
	{
		ERR_print_errors_fp(stderr);
		std::cout << "SSL_CTX_new error!" << std::endl;
	}
}

HTTPS::~HTTPS()
{
	SSL_CTX_free(ctx);//释放SSL会话环境
}

bool HTTPS::connect_server()
{
	CheckError(connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR, "连接服务器失败");
	ssl = SSL_new(ctx);//创建一个SSL套接字
	CheckError(!ssl, "SSL NEW error");
	SSL_set_fd(ssl, client_socket);//以读写模式绑定流套接字
	CheckError(SSL_connect(ssl)== -1, "SSL connect error");//SSL连接 
	CheckError(SSL_write(ssl, request.c_str(), request.size())==-1,"SSL write error");

	char* buffer = new char[BUFFER * BUFFER];
	response = "";//刷新缓冲
	int index = 0;
	int buffer_size;

	while (1)
	{
		buffer_size = SSL_read(ssl, buffer + index, BUFFER);
		if (buffer_size > 0)
		{
			index += buffer_size;
		}
		else
		{
			response += buffer;
			break;
		}
	}
	delete[] buffer;
	SSL_shutdown(ssl);//关闭SSL套接字 
	SSL_free(ssl);//释放SSL套接字 
	closesocket(client_socket);
	if(response.size())
	{
		return true;
	}
	else
	{
		return false;
	}
}