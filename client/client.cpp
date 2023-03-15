#include <iostream>
#include "MyClient.h"

int main()
{
    MyClient* client = new MyClient("127.0.0.1", 10015);
    client->runClient();
}
