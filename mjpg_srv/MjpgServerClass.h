#ifndef MJPGSERVERCLASS_H
#define MJPGSERVERCLASS_H

#include "ServerClass.h"


class MjpgServerClass : public ServerClass
{
    public:
        MjpgServerClass();
        virtual ~MjpgServerClass();

        virtual void Process();
        void Send_New_Image(unsigned char * image_data, unsigned int image_data_size);

    protected:

        void Send_Mjpg_Http_Header(int client_socket);
        virtual void Handle_New_Client_Connected(int client_socket);
        virtual void Handle_Client_Disconnected(int client_socket);
        virtual void Handle_Incoming_Message(int client_socket,char * msg);

    private:

        void Deliver_Next_Image_To_Clients(unsigned char * image_data, unsigned int image_data_size);

        bool m_NewImageRecieved;
        int m_ImageDataSize;
        unsigned char m_ImageData[1920*1080*4];     // grossly oversized image buffer!
        std::set<int> m_ClientsReadyForImages;
};

#endif // MJPGSERVERCLASS_H
