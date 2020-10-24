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

    while (inputString != "STOP")
    {
        cout << "Input: ";
        getline(cin, inputString);

        cout << "Received: " << inputString << endl;

        client.write(inputString, 0);
    }

    return retval;
}