#include <iostream>
#include "bssocket.h"
using namespace std;

int main(int argc, char *argv[], char *envp[])
{

    BSSocket bssocket;

    int retval = 0;
    char port[] = "1223";

    retval = bssocket.getAddrInfo(port, AF_INET, AI_PASSIVE); // AI_PASSIVE gives structure for server

    if (0 == retval)
    {
        retval = bssocket.create();
    }

    if (0 == retval)
    {
        retval = bssocket.create(1, 1, 7);

        if (retval <= 0)
        {
            cout << bssocket.getErrorCode() << ": " << bssocket.getErrorMessage() << endl;
        }
    }
}