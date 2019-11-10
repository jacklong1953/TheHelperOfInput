#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <netinet/in.h>
#include <iostream>

using namespace std;
void readJson(const string &, const string &);

int main()
{
    int sfd;
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0)
    {
        perror("socket");
        return -1;
    }
    int ret;
    struct sockaddr_in ser;
    const char *ip = "192.168.3.213";
    unsigned short port = 8888;
    bzero(&ser, sizeof(ser));
    ser.sin_addr.s_addr = inet_addr(ip);
    ser.sin_port = htons(port);
    ser.sin_family = AF_INET;
    ret = connect(sfd, (struct sockaddr *)&ser, sizeof(ser));
    if(ret < 0)
    {
        perror("connect");
        return -1;
    }
    /* printf("connection is established!\n"); */
    char buf[1024] = { 0 };
    ret = recv(sfd, buf, sizeof(buf), 0);
    cout << buf << endl;
    char sendbuf[1024] = { 0 };
    char recvbuf[1024] = { 0 };
    while(1)
    {
        /* string str; */
        /* cin >> str; */
        /* ret = send(sfd, str.c_str(), str.size(), 0); */
        fgets(sendbuf, sizeof(sendbuf), stdin);
        ret = send(sfd, sendbuf, sizeof(sendbuf), 0);
        if(ret == 0)
        {
            cout << "server is interrupted!" << ret << endl;
            break;
        }
        ret = recv(sfd, recvbuf, sizeof(recvbuf), 0);
        /* printf("ret = %d\n", ret); */
        cout << "receive message: " << endl;
        readJson(recvbuf, sendbuf);
        bzero(sendbuf, sizeof(sendbuf));
        bzero(recvbuf, sizeof(recvbuf));
    }
    close(sfd);
    return 0;
}

void readJson(const string &recvStr, const string &queryWord)
{
    Json::Reader reader;
    Json::Value root;
    if(reader.parse(recvStr, root))
    {
        int sz = root[queryWord].size();
        for(int i = 0; i < sz; ++i)
        {
            cout << "---------------";
            cout << root[queryWord][i];
        }
    }
}
