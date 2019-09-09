#include "ServerClass.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/tcp.h>


ServerClass::ServerClass()
{
    //ctor
}

ServerClass::~ServerClass()
{
    //dtor
}

void ServerClass::Init(int port)
{
    int status;
    addrinfo hints;

    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC; // either ipv4 or ipv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;    // fill in my ip!

    printf("getaddrinfo\n");

    char port_string[128];
    sprintf(port_string,"%d",port);
    if ((status = getaddrinfo(NULL,port_string,&hints,&m_ServerAddrInfo)) != 0)
    {
        fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(status));
        freeaddrinfo(m_ServerAddrInfo);
        m_ServerAddrInfo = NULL;
        return;
    }

    // Create our listening socket for clients to connect to
    printf("creating listening socket\n");
    m_ListenSocket = socket(m_ServerAddrInfo->ai_family,m_ServerAddrInfo->ai_socktype,m_ServerAddrInfo->ai_protocol);
    if (m_ListenSocket == -1)
    {
        fprintf(stderr,"Failed to create socket. Error: %d\n",errno);
        return;
    }

    // Set the listen socket to allow multiple connections
    int opt = 1; //TRUE;
    status = setsockopt(m_ListenSocket,SOL_SOCKET,SO_REUSEADDR, (char*)&opt, sizeof(opt));
    if (status == -1)
    {
        fprintf(stderr,"setsockopt failed");
    }

    // disable the NAGLE algorithm on the socket
    /*int flag = 1;
    status = setsockopt(m_ListenSocket,  // socket affected
                        IPPROTO_TCP,     // set option at TCP level
                        TCP_NODELAY,     // name of option
                        (char *) &flag,  // the cast is historical cruft
                        sizeof(int));    // length of option value
    if (status == -1)
    {
        fprintf(stderr,"disable nagle failed");
    }*/

    // Bind the listening socket to a port
    printf("binding port\n");
    status = bind(m_ListenSocket,m_ServerAddrInfo->ai_addr,m_ServerAddrInfo->ai_addrlen);
    if (status == -1)
    {
        fprintf(stderr,"Failed to bind socket to port. Error: %d\n",errno);
        return;
    }

    // Now we start listening for connections
    printf("listening..\n");
    const int MAX_QUEUED_CONNECTIONS = 5;
    status = listen(m_ListenSocket,MAX_QUEUED_CONNECTIONS);
    if (status == -1)
    {
        fprintf(stderr,"listen failed.  Error: %d\n",errno);
        return;
    }

    // Create the initial set of read sockets
    FD_ZERO(&m_AllSocketSet);
    FD_SET(m_ListenSocket,&m_AllSocketSet);
    m_MaxSocket = m_ListenSocket;

    // Show the host name and ipaddress
    char hname[1024];
    gethostname(hname,sizeof(hname));
    printf("HostName: %s\n",hname);

    // host entries
    hostent * host_entry = gethostbyname(hname);
    for (int i=0; i<host_entry->h_length; ++i)
    {
        //printf("addr %d: %s\n",i,host_entry->h_addr_list[i]);
    }
}

void ServerClass::Process()
{
    int status = 0;
    static int counter = 0;

    // Copy the set of read sockets so we can pass it to 'select'.  select modifies the
    // set we pass it, leaving only the sockets with something to read.
    fd_set readset;
    memcpy(&readset,&m_AllSocketSet,sizeof(m_AllSocketSet));

    // Now use 'select' to see if any of the sockets have anything to read
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    counter++;
    //if ((counter & 0x7ff) == 0) printf("select %d\n",counter);
    status = select(m_MaxSocket+1,&readset,NULL,NULL,&timeout);


    if (status == -1)
    {
        fprintf(stderr,"select error: %d\n",errno);
        return;
    }

    // see what select found for us to read
    if (FD_ISSET(m_ListenSocket,&readset))
    {
        // Accept a connection
        sockaddr client_addr;

        socklen_t  client_addr_len = sizeof(client_addr);
        int client_socket = accept(m_ListenSocket,&client_addr,&client_addr_len);
        if (client_socket == -1)
        {
            fprintf(stderr,"accept failed.  Error: %d\n",errno);
            return;
        }
        printf("client accepted!\n");

        sockaddr_in * test = (sockaddr_in*)(&client_addr);
        printf("socket: %d   ip: %s   port: %d\n", client_socket, inet_ntoa(test->sin_addr),ntohs(test->sin_port));

        // Add the connection to the set
        FD_SET(client_socket,&m_AllSocketSet);
        if (client_socket > m_MaxSocket)
        {
            m_MaxSocket = client_socket;
        }
        m_ClientSockets.insert(client_socket);

        Handle_New_Client_Connected(client_socket);
    }

    // Handle all the rest of the clients that have something for us to read
    for (int i=0; i<m_MaxSocket+1; ++i)
    {
        if (FD_ISSET(i,&readset) && (i!=m_ListenSocket))
        {
            LOG((" client socket %i has something to read\n",i));
            char msg_buf[2048];
            int bytes_recv = recv(i,msg_buf,sizeof(msg_buf),0);
            if (bytes_recv == 0)
            {
                m_ClientSockets.erase(i);
                Handle_Client_Disconnected(i);

                // client disconnected!
                FD_CLR(i,&m_AllSocketSet);
                shutdown(i,2);
                close(i);
                printf("client %d disconnected.\n",i);
            }
            else
            {
                msg_buf[bytes_recv] = 0;
                LOG(("recv: %s\n",msg_buf));
                Handle_Incoming_Message(i,msg_buf);
            }
        }
    }
}

void ServerClass::Send_String(int client_socket,const char * msg)
{
    int status = send(client_socket,msg,strlen(msg),MSG_NOSIGNAL);
    if (status == -1)
    {
        fprintf(stderr,"send failed, error: %d\n",errno);
    }
    LOG(("send: %s\n",msg));
}

void ServerClass::Send_Binary_Data(int client_socket,unsigned char * data,int data_size)
{
    int status = send(client_socket,data,data_size,MSG_NOSIGNAL);
    if (status == -1)
    {
        fprintf(stderr,"send failed, error: %d\n",errno);
    }
    LOG(("send binary data, %d bytes\r\n\r\n",data_size));
}

void ServerClass::Get_IP_Of_Host(const char * hostname)
{
    addrinfo hints,*res;
    int status;

    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((status = getaddrinfo(hostname,NULL,&hints,&res))!= 0)
    {
        fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(status));
        return;
    }

    addrinfo * cur;
    for (cur = res; cur != NULL; cur = cur->ai_next)
    {
        void * addr;
        const char * ipver;

        // handle either ipv4 or ipv6
        if (cur->ai_family == AF_INET)
        {
            // ipv4
            sockaddr_in * ipv4 = (sockaddr_in*)cur->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else
        {
            // ipv6
            sockaddr_in6 * ipv6 = (sockaddr_in6*)cur->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        char ipstr[INET6_ADDRSTRLEN];
        inet_ntop(cur->ai_family,addr,ipstr,sizeof(ipstr));

        printf("  %s : %s\n",ipver,ipstr);
    }
}

