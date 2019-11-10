#include "SocketIO.h"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>

namespace wd
{

SocketIO::SocketIO(int fd) : _fd(fd) {}

int SocketIO::readn(char *buf, int len)
{
    char *p = buf;
    int total = len;
    while(total > 0)
    {
        int ret = read(_fd, p, total);
        if(ret == -1 && ret == EINTR) 
            continue;
        else if(ret == -1) {
            perror("read");
            return total - ret;
        } else if(ret == 0) {
            return total - ret;
        } else {
            total -= ret;
            p += ret;
        }
    }
    return len - total;
}

int SocketIO::recvPeek(char *buf, int len)
{
    int ret;
    do{
        ret = recv(_fd, buf, len, MSG_PEEK);
    } while(-1 == ret && EINTR == ret);
    return ret;
}

int SocketIO::readLine(char *buf, int maxlen)
{
    int ret;
    char *p = buf;
    int total = maxlen - 1;
    int cnt = 0;
    while(total > 0)
    {
        ret = recvPeek(p, total);
        for(int i = 0; i < ret; ++i)
        {
            if(p[i] == '\n')
            {
                readn(p, i + 1);
                cnt = cnt + (i + 1);
                p += i +1;
                *p = '\0';
                return cnt;
            }
        }
        cnt += ret;
        readn(p, ret);
        p += ret;
        total -= ret;
    }
    return cnt;
}

int SocketIO::getMessage(char *buf, int len)
{
    int ret;
    do{
        ret = recv(_fd, buf, len, 0);
    } while(-1 == ret && EINTR == ret);
    return ret;
}

int SocketIO::writen(const char *buf, int len)
{
    const char *p = buf;
    int total = len;
    while(total > 0)
    {
        int ret = send(_fd, p, total, 0);
        if(-1 == ret && EINTR == ret) {
            continue;
        } else if(-1 == ret) {
            perror("send");
            return len - total;
        } else if( 0 == ret ) {
            return len - total;
        } else {
            total -= ret;
            p += ret;
        }
    }
    return len - total;
}

}//end of namespace wd
