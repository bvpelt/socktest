#include "tcpclient.h"
#include <stdio.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[], char *envp[])
{

    TCPClient client;

    int retval = BS_SUCCESS;
    string inputString = "";

    retval = client.startUp();

    bool goOn = true;
    while (goOn)
    {
        cout << "Input: ";
        getline(cin, inputString);

        cout << "Received: " << inputString << endl;

        client.write(inputString, 0);

        if (inputString.length() >= 4)
        {
            goOn = (inputString.substr(0, 4) != "STOP");
        }
    }

    return retval;
}