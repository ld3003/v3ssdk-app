#ifndef SERVERCLASS_H
#define SERVERCLASS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <set>


#if 1
#define LOG(x)  printf x
#else
#define LOG(x)
#endif



class ServerClass
{
public:
    ServerClass();
    virtual ~ServerClass();
    void Init(int port);
    virtual void Process();
    static void Get_IP_Of_Host(const char * hostname);

protected:

    virtual void Handle_New_Client_Connected(int client_socket) {}
    virtual void Handle_Client_Disconnected(int client_socket) {}
    virtual void Handle_Incoming_Message(int client_socket,char * msg) {}

    void Send_String(int client_socket,const char * msg);
    void Send_Binary_Data(int client_socket,unsigned char * data,int data_size);

    addrinfo * m_ServerAddrInfo;
    int m_ListenSocket;
    std::set<int> m_ClientSockets;

    fd_set m_AllSocketSet;
    int m_MaxSocket;

private:
};

#endif // SERVERCLASS_H
