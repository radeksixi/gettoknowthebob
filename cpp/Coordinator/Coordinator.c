#include <iostream>
#include <list>
#include <wiringPi.h>
#include <wiringSerial.h>
using namespace std;
void sendCommand(int connectionId, char *command, int waitTime, bool debug);

int main()
{
    std::list<char *> commandList;
    
    commandList.push_back("AT+RST\r\n");
    commandList.push_back("AT+CWMODE=1\r\n");
    commandList.push_back("AT+CWJAP=\"NetworkSSID\",\"Password\"\r\n");
    commandList.push_back("AT+CIFSR\r\n");

    wiringPiSetup();
    int connectionId = serialOpen("/dev/ttyAMA0", 115200);

    // Send all commands from stack
    while(commandList.size() != 0)
    {
        //const char * c = commandStack[i].c_str();
        sendCommand(connectionId, commandList.front(), 3000, true);
        commandList.pop_front();
    }

    string wait;
    cin >> wait;

    serialClose(connectionId);

    return 0;
}

// Time to wait for response (in ms). If debug is set to true command string will be printed
void sendCommand(int connectionId, char *command, int waitTime, bool debug)
{
    // let's flush remaining data
    serialFlush(connectionId);

    if(debug)
    {
        cout << "----------------------------------" << endl;
        cout << "Command:" << command << endl << "Data: ";
    }
    // send data
    serialPuts(connectionId, command);

    int timer = millis()+waitTime+1;

    while(timer > millis())
    {
        if(serialDataAvail(connectionId)> 0)
        {
            while(serialDataAvail(connectionId)> 0)
            {
                cout << (char)serialGetchar(connectionId);
            }
        }
    }

    cout << endl;
}

