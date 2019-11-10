#pragma once

namespace wd
{

class SocketIO
{
public:
    explicit SocketIO(int fd);

    int readn(char *buf, int len);
    int readLine(char *buf, int len);
    int writen(const char *buf, int len);
    int getMessage(char *, int);

private:
    int recvPeek(char *buf, int len);

private:
    int _fd;

};

}//end of namespace wd
