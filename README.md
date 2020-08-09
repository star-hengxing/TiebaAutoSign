# 概述
从贴吧首页html源码中解析获得所需信息，对每个贴吧发送POST签到  
[贴吧首页](https://tieba.baidu.com/)

# 配置
* **Openssl v1.1.0**以上
* **vs2019** 或者更低版本也行（？~~没测试过~~
* **C++ 11** 标准（~~大概~~？我忘了有没有加C++14/17的内容）

# 编译
* 下载src和openssl库，在vs配置openssl路径，把dll添加在vs项目内，然后bulid

# 贴吧签到解析
手动按下签到，查看网页XHR（前提要打开控制台）  
有两个新的XHR分别是**add**和**loadmonth**  
仔细查看可以看出**add**是我们要post的内容，而**loadmonth**看名字和内容应该是记录时间的

summerpockets吧为例，查看我们浏览器发的**request**里的**data**

> ie=utf-8&kw=summerpockets&tbs=e85da433fd9427d71595558101

浏览器解析为：
|ie|kw|tbs|
|:----:|:----:|:----:|
|utf-8|summerpockets|e85da433fd9427d71595558101|

**kw**是贴吧名，**tbs**是每次点开网页自动生成，相当于一次性令牌，如果发帖回帖都需要tbs

`kw在贴吧首页源码获取，如果是中文要Unicode转码，英文则不用`  
`tbs在单个贴吧首页源码中获取`

# 贴吧签到操作

综上所述，我们首先要在首页源码获取**贴吧名（kw）**，再点击各个贴吧首页源码获取**令牌（tbs）**  
我们模拟POST报文，在请求头填入**cookie**，最后主体附加**data**，按照以下格式  
> ie=utf-8&kw=贴吧名&tbs=令牌  

PS:在浏览器解码前，**中文贴吧名**是一堆十六进制Unicode字符串，%开头，所以自己发包时要编码

# Coding
我选择C++练习爬虫是因为~~懒得配置Python环境以及学习Python的语法和框架~~（虽然用Python真的很方便）  
* 新建一个结构体存取贴吧各种信息
```C++
struct TiebaMsg
{
    std::string name;//贴吧名
    std::string kw;//贴吧名URL编码
    std::string tbs;//贴吧一次性令牌
    bool is_sign;
    TiebaMsg() : name(""), kw(""), tbs(""), is_sign(false) {}
};
```
* 发送请求获得响应解析html，正则匹配kw，tbs数据，并统计关注贴吧数量  
* vector容器存储每个贴吧的信息 
```C++
    TiebaMsg single_tb;
    std::vector<TiebaMsg>tb;
    tb.push_back(single_tb);
    ...
``` 
* 提取信息对每个贴吧进行签到  
    * 将整理的信息填入请求报文中，逐个发包

# 总结
1. 网络编程
    * Windows socket
    * Openssl
        * 下载安装与环境配置
        * Openssl API使用
    * HTTP/HTTPS通信解析
        * 解析域名
        * http报文
            * 请求报文
                * 解析请求报文
                * 伪装请求报文
            * 响应报文
                * 解析响应报文
                    * 查看返回状态码
2. 字符串处理
    * 正则表达式
    * Unicode转码
        * 十六进制Unicode字符串转义
        * 中文URL编码
3. 数据结构
    * vector
    * map（红黑树）

# 参考
1. [利用爬虫自动签到所有贴吧](https://zhuanlan.zhihu.com/p/46137712)  
3. [C/C++实现HTTPS通信（抓取百度页面）](https://blog.csdn.net/ZRXSLYG/article/details/81395640)
4. [Google Chrome抓包分析详解](https://www.jianshu.com/p/f7a94bc5f67c?utm_campaign=maleskine&utm_content=note&utm_medium=seo_notes&utm_source=recommendation)
5. 正则表达式
    * [正则表达式 - 教程](https://www.runoob.com/regexp/regexp-tutorial.html)
    * [LEARN REGEX THE ESAY WAY](https://github.com/ziishaned/learn-regex/blob/master/translations/README-cn.md#27-%E8%BD%AC%E7%A0%81%E7%89%B9%E6%AE%8A%E5%AD%97%E7%AC%A6)
    * [C++11新特性7 - 正则表达式](https://zhuanlan.zhihu.com/p/112219748)
    * [C++ regex: Which group matched?](https://stackoverflow.com/questions/44949784/c-regex-which-group-matched)
6. Unicode转码
    * [WideCharToMultiByte function](https://docs.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-widechartomultibyte)
    * [c++ string、UTF8相互转换方法](https://blog.csdn.net/yinshi_blog/article/details/6731809)
    * [c++ 如何把\\u708e转换成汉字？](https://zhidao.baidu.com/question/2080606348101358108.html)
7. [OpenSSL1.1.1没有libeay.dll和ssleay.dll。](https://blog.csdn.net/iMatt/article/details/93321601)
