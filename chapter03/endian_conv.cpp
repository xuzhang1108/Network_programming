#include <iostream>
#include <arpa/inet.h>
using namespace std;

int main(int argc, char *argv[]){
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    cout<<"Host ordered port: "<<"0x"<<hex<<host_port<<endl;
    cout<<"Network ordered port: "<<"0x"<<hex<<net_port<<endl;
    cout<<"Host ordered address: "<<"0x"<<hex<<host_addr<<endl;
    cout<<"Network ordered address: "<<"0x"<<hex<<net_addr<<endl; 
    
    return 0;
}