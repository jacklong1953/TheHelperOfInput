#include "TcpServer.h"
#include "MyTask.h"
#include "Threadpool.h"
#include <iostream>

using namespace std;

wd::Threadpool *pthreadpool = nullptr;
/* wd::Threadpool *pthreadpool = wd::SingleThreadpool::getpThreadpool(); */

void onConnect(const wd::TcpConnectPtr &conn)
{
    cout << conn->toString() << " has connected!" << endl;
    conn->send("welcome log on server!");
}

void onMessage(const wd::TcpConnectPtr &conn)
{
    cout << "onMessage..." << endl;
    string msg = conn->receive();
    cout << "receive message from client: " << msg << endl;
    /* conn->send(msg); */
    wd::Task task(msg, conn);
    pthreadpool->addTask(std::bind(&wd::Task::proccess, task, std::placeholders::_1));
}

void onClose(const wd::TcpConnectPtr &conn)
{
    cout << "onClose..." << endl;
    cout << conn->toString() << " has closed!" << endl;
}

int main()
{
    wd::TcpServer server("./myConfig");

    server.setConnectCallBack(onConnect);
    server.setMessageCallBack(onMessage);
    server.setCloseCallBack(onClose);

    pthreadpool = new wd::Threadpool(4, 10);
    pthreadpool->start();

    server.start();
    pthreadpool->stop();

    return 0;
}

