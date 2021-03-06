// =====================================================================================
//
//       Filename:  Client.cpp
//
//    Description:  
//
//        Version:  1.0
//        Created:  2015年01月24日 08时38分45秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  wangweihao (wangweihao), 578867817@qq.com
//        Company:  none
//
// =====================================================================================

#include "Client.h"

FTP_admin::FTP_admin(void)
{
    /* 创建套接字
     * IPV4, TCP协议*/
    Socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(Socket_fd == ERROR_SOCKET)
    {
        printf("error:create socket fault!\n");
    }
}

FTP_admin::~FTP_admin(void)
{

}

/* 登录，需要形如192.168.0.1:2234 */
int FTP_admin::Login_server(const std::string &ServerIp)
{
    std::string tmpip;
    std::string tmpport;
    std::size_t pos = ServerIp.find_first_of(":");
    
    /* 提取出ip 和 port */
    if(pos > 0)
    {
        tmpip = ServerIp.substr(0, pos);
        tmpport = ServerIp.substr(pos+1, ServerIp.size());
    }
    else
    {
        tmpip = ServerIp;
        tmpport = FTP_DEFAULT_PORT;
    }
    Server_ip = tmpip;
    Server_port = atoi(tmpport.c_str());
    
    /* 连接服务器 */
    if(Connect(Socket_fd, Server_ip, Server_port) < 1)
    {
        printf("error:connect fault!\n");
        return -1;
    }
    std::cout << "connect success!" << std::endl;

    return 0;
}

int FTP_admin::Quit_server(void)
{
    std::string s = "quit";
    Send(Socket_fd, s);
    return 0;
}

/* 连接服务器 */
int FTP_admin::Connect(int sockfd, const std::string &serverIp, std::size_t port)
{
    struct sockaddr_in address;

    //设置非阻塞
    //int flags = fcntl(sockfd, F_GETFL, 0);
    //fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(Server_port);
    int ret = inet_pton(AF_INET, Server_ip.c_str(), &address.sin_addr);
    if(ret == 0)
    {
        printf("error:inet_pton fault\n");
        return -1;
    }
    //异步IO不阻塞，返回值为-1
    //if(connect(sockfd, (struct sockaddr*)(&address), sizeof(address)) == -1)
    if(connect(sockfd, (struct sockaddr*)(&address), sizeof(address)) < 0)
    {
        printf("connect fault! \n");
        close(sockfd);
        return -1;
    }
    else
    {
        Socket_fd = sockfd;
        //连接选择并发送命令
        //Send(Socket_fd, passWd);
    }
    
    return 0;
    
}

int FTP_admin::Send(int fd, const std::string &passWd)
{
    int ret = send(fd, passWd.c_str(), passWd.size(), 0);
    if(ret < 0)
    {
        return -1;
    }
    //根据命令来执行功能
    Exe_func(passWd);
    return 0;
}

std::string FTP_admin::Select(void)
{
    /* 根据点击的按钮来选择命令 */
    /*  */
}

const std::string FTP_admin::Explain_Order(void)
{
    std::map<std::string, std::string>Order;
    Order.insert(std::make_pair<std::string, std::string>("注册帐号", "31"));
    Order.insert(std::make_pair<std::string, std::string>("登录", "32"));
    Order.insert(std::make_pair<std::string, std::string>("找回密码", "33"));
    Order.insert(std::make_pair<std::string, std::string>("下载", "34"));
    Order.insert(std::make_pair<std::string, std::string>("上传", "35"));
    return Order.find(passWd)->second;

}

/* 本山写 */
void FTP_admin::Exe_func(const std::string &order)
{
    /* 参数passWd
     * 每个功能用单独的线程来执行
     * 执行完后关闭线程*/

    int passwd = atoi(order.c_str());
    switch(passwd)
    {
        case 31:/* 注册帐号 */
            
            break;
        case 32:/* 登录 */
            
            break;
        case 33:/* 找回密码 */

            break;
        case 34:/* 上传 */
            
            break;
        case 35:/* 下载 */
            
            break;
        default:
            break;
    }

}

/* 注册帐号 */
int FTP_admin::InputUsername(const std::string &username)
{
    passWd = "注册帐号:31";
    int ret = 0;
    if((ret = Send(Socket_fd, passWd)) < 0)
    {
        printf("send InputUsername error!\n");
        return -1;
    }
    /* 发送帐号 */
    //Send;
    return 0;
}

/* 登录 */
int FTP_admin::Register(void)
{
    passWd = "登录:32";
    int ret = 0;
    if((ret = Send(Socket_fd, passWd)) < 0)
    {
        printf("send Register error!\n");
        return -1;
    }
    /* 发送帐号密码:检查返回值 */
    //Send;
    return 0;
}

/* 找回密码 */
int FTP_admin::Find_password(void)
{
    passWd = "找回密码:33";
    int ret = 0;

    if((ret = Send(Socket_fd, passWd)) < 0)
    {
        printf("send Find_password error!\n");
        return -1;
    }
    /* 发送密保:检查返回值 */
    //Send
    return 0;
}



/* 上传 */
int FTP_admin::upload(const std::string &SerFile, const std::string &LocFile)
{
    pthread_info info;
    info.SerFile = SerFile;
    info.LocFile = LocFile;

    std::cout << "upload:" << LocFile << std::endl;
    pthread_t pthread_i;
    int ret = pthread_create(&pthread_i, NULL, Put, this);
    if(ret == 0)
    {
        std::cout << "pthread create success!" << std::endl;
    }
    return 0;
}

/* 上传调用函数 */
void* Put(void *p)
{
    std::string LocFile;
    std::string SerFile;
    FILE *file = NULL;
    int ret_yes = 0;
    int ret_no = -1;

    int data_fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(data_fd != -1);
    if(((FTP_admin*)p)->Create_Link(data_fd) < 0)
    {
        printf("data link error!\n");
        //return -1;
    }
    file = fopen(LocFile.c_str(), "rb");
    assert(file != NULL);

    /* 发送密码 */
    ((FTP_admin*)p)->passWd = "上传:34";
    int ret;
    if((ret = ((FTP_admin*)p)->Send(((FTP_admin*)p)->Socket_fd, ((FTP_admin*)p)->passWd)) < 0)
    {
        printf("send passWd error!\n");
        close(data_fd);
        //return -1;
    }

    int len = 0;
    char *databuf = new char[FTP_DEFAULT_BUFFER];
    unsigned long Size = ((FTP_admin*)p)->Get_Filelength(LocFile);
    if(Size < FTP_DEFAULT_BUFFER)
    {
        len = Size;
    }
    else
    {
        len = FTP_DEFAULT_BUFFER;
    }
    
    fseek(file, Size, SEEK_SET);
    while(!feof(file))
    {
        len = fread(databuf, 1, len, file);
        if(len < 0)
        {
            break;
        }

        if(((FTP_admin*)p)->Send(data_fd, databuf) < 0)
        {
            close(data_fd);
            //return -1;
        }
    }
    close(data_fd);
    fclose(file);

    //结束执行的线程
    pthread_exit(0);
    //return 0;

}


/* 下载 */
int FTP_admin::download(const std::string &SerFile, const std::string &LocFile)
{
    pthread_info info;
    info.SerFile = SerFile;
    info.LocFile = LocFile;

    std::cout << "download:" << SerFile << std::endl;
    pthread_t pthread_i;
    int ret = pthread_create(&pthread_i, NULL, Get, this);
    if(ret == 0)
    {
        std::cout << "pthread create success!" << std::endl;
    }
}

/* 下载调用函数 */
void* Get(void *p)
{
    std::string LocFile;
    std::string SerFile;
    FILE *file = NULL;
    unsigned long DataLen = FTP_DEFAULT_BUFFER;
    int buffer = FTP_DEFAULT_BUFFER;

    //新建连接
    int data_fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(data_fd != -1);
    if(((FTP_admin*)p)->Create_Link(data_fd) < 0)
    {
        printf("data link error!\n");
        //return -1;
    }
    
    //本地创建文件
    file = ((FTP_admin*)p)->Create_Localfile(std::string(FTP_DEFAULT_PATH + LocFile));
    assert(file != NULL);
    
    /* 发送密码 */
    ((FTP_admin*)p)->passWd = "下载:35";

    int ret = 0;
    if((ret = ((FTP_admin*)p)->Send(((FTP_admin*)p)->Socket_fd, ((FTP_admin*)p)->passWd)) < 0)
    {
        printf("send passWd error!\n");
        close(data_fd);
        //return -1;
    }
    
    //获得文件长度
    unsigned long Length = ((FTP_admin*)p)->Get_Filelength(SerFile);
    
    char *databuf = new char[FTP_DEFAULT_BUFFER];
    unsigned int len = 0;
    unsigned int File_len = 0;
    if(Length < FTP_DEFAULT_BUFFER)
    {
        len = Length;
    }
    else
    {
        len = FTP_DEFAULT_BUFFER;
    }
    while((len = ((FTP_admin*)p)->getData(data_fd, databuf, len)))
    {
        File_len += len;

        int num = fwrite(databuf, 1, len, file);
        memset(databuf, 0, buffer);

        if(File_len == Length && Length != 0)
        {
            break;
        }

        if((File_len + FTP_DEFAULT_BUFFER) > Length && Length != 0)
        {
            delete []databuf;
            buffer = Length - File_len;
            databuf = new char[buffer];
        }
    }
    close(data_fd);
    fclose(file);
    delete []databuf;

    //结束线程
    pthread_exit(0);

    //return 0;
}

/* 创建本地文件 */
FILE* FTP_admin::Create_Localfile(const std::string &LocFile)
{
    return fopen(LocFile.c_str(), "w+b");
}

/* 创建连接 */
int FTP_admin::Create_Link(int data_fd)
{
    std::string Ip = Server_ip;
    std::size_t Port = Server_port;
    if(Ip.size() != 0 && Port != 0)
    {
        if(Connect(data_fd, Ip, Port) < 0)
        {
            printf("error:connect fault!\n");
            return -1;
        }
    }
}

/* 获取文件长度 */
unsigned long FTP_admin::Get_Filelength(const std::string &SerFile)
{
    /* 服务器发送主界面显示的信息，里面包含每个文件的长度 */
}

/* 获取数据 */
int FTP_admin::getData(int fd, char *buf, unsigned int len)
{
    if(buf == NULL)
    {
        printf("error:buf is null!\n");
        return -1;
    }
    if(fd == ERROR_SOCKET)
    {
        printf("error:socket is invaild!\n");
    }
    int nlen;
    memset(buf, 0, len);
    if((nlen = recv(fd, buf, len, 0)) > 0)
    {
        return nlen;
    }
    else
    {
        return -1;
    }
}
