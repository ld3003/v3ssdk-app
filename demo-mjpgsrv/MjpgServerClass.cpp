#include "MjpgServerClass.h"
#include "stdio.h"
#include "string.h"


// Good info here: http://answers.opencv.org/question/6976/display-iplimage-in-webbrowsers/

MjpgServerClass::MjpgServerClass() :
    m_NewImageRecieved(false),
    m_ImageDataSize(0)
{
    memset(m_ImageData,0,sizeof(m_ImageData));
}

MjpgServerClass::~MjpgServerClass()
{
    //dtor
}

void MjpgServerClass::Send_Mjpg_Http_Header(int client_socket)
{
    // Send out the mjpeg header
    char buffer[2048];
    sprintf(buffer,"%s",
        "HTTP/1.0 200 OK\r\n" "Cache-Control: no-cache\r\n"
        "Pragma: no-cache\r\nExpires: Thu, 01 Dec 1994 16:00:00 GMT\r\n"
        "Connection: close\r\nContent-Type: multipart/x-mixed-replace; "
        "boundary=--mjpegstream\r\n\r\n");

    Send_String(client_socket,buffer);
}
void MjpgServerClass::Handle_New_Client_Connected(int client_socket)
{
    LOG(("MjpgServerClass::Handle_New_Client_Connected %d\r\n",client_socket));
}

void MjpgServerClass::Handle_Client_Disconnected(int client_socket)
{
    m_ClientsReadyForImages.erase(client_socket);

}

void MjpgServerClass::Handle_Incoming_Message(int client_socket,char * msg)
{
    // print out the message
    LOG(("Incoming message: %s\r\n",msg));

    // if its an HTTP GET, send the header and start sending images
    if (strncmp(msg,"GET",3) == 0)
    {
        LOG(("Recognized HTTP GET from client: %d\r\n",client_socket));
        Send_Mjpg_Http_Header(client_socket);
        m_ClientsReadyForImages.insert(client_socket);


    }

}

void MjpgServerClass::Process()
{
    ServerClass::Process();

    // if we have new image data, pass it on to the clients
    if (m_NewImageRecieved)
    {
        Deliver_Next_Image_To_Clients(m_ImageData,m_ImageDataSize);
        m_NewImageRecieved = false;
    }
}

void MjpgServerClass::Send_New_Image(unsigned char * image_data, unsigned int image_data_size)
{
    if (image_data_size < sizeof(m_ImageData))
    {
        memcpy(m_ImageData,image_data,image_data_size);
        m_ImageDataSize = image_data_size;
    }
    m_NewImageRecieved = true;
}

int MjpgServerClass::get_client_count()
{
    return m_ClientsReadyForImages.size();
}


void MjpgServerClass::Deliver_Next_Image_To_Clients(unsigned char * image_data, unsigned int image_data_size)
{
    std::set<int>::iterator it;
    for (it = m_ClientsReadyForImages.begin(); it != m_ClientsReadyForImages.end(); ++it)
    {
        int client_sock = *it;
        LOG(("Sending image to socket: %d size: %d\r\n",client_sock,image_data_size));

        char buffer[2048];
        sprintf(buffer,"--mjpegstream\r\nContent-Type: image/jpeg\r\nContent-Length: %d\r\n\r\n",image_data_size);
        Send_String(client_sock,buffer);

        Send_Binary_Data(client_sock, image_data, image_data_size);
    }
}
