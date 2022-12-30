#include "ServerOP.h"

#include <cstdio>

int main()
{
    ServerOP op("server_config.json");
    op.startServer();

    return 0;
}