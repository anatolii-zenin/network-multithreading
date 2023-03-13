#include <iostream>
#include "MyServer.h"

int main()
{
    MyServer* svr = new MyServer();
    svr->runServer("127.0.0.1", 10015);
}
