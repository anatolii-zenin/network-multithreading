#include <iostream>
#include "MyClient.h"

int main()
{
    MyClient* client = new MyClient();
    client->runClient("127.0.0.1", 10015);
}
