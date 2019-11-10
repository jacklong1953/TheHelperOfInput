#pragma once

namespace wd {

class Socket
{
public:
    Socket();
    Socket(int &fd);

    void shutdownWrite();
    int fd() const;

    ~Socket();

private:
    int _sfd;

};

}//end of namespace wd
