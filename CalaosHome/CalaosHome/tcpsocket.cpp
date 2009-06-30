//
//    libsocketcpp - a C++ class/library to allow easy TCP and UDP functionatliy
//    Copyright (C) 2001 Garrett Grice
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation; either
//    version 2 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    see the file "AUTHORS" for contact information.


// #ifdef HAVE_STRING_H
// #include <string.h>
// #endif
// #ifdef HAVE_STRINGS_H
// #include <strings.h>
// #endif

#include "tcpsocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

using namespace CalaosNetwork;

TCPSocket::TCPSocket()
{
        newfd = 0;
        sockfd = 0;
        connected = false;
}

TCPSocket::~TCPSocket()
{
}

bool TCPSocket::Create()
{
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1)
        {
                cout << "socket(AF_INET, SOCK_STREAM): " << strerror(errno) << endl;
                return false;
        }

        return true;
};

void TCPSocket::SetReuse()
{
        int r, reuse = 1;

        r = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (int *) &reuse, sizeof(reuse));
        if (r == -1)
                cout << "setsockopt: SO_REUSEADDR: " << strerror(errno) << endl;
}


bool TCPSocket::Create(char nType)
{
        if (nType == TCP)
                return Create();
        if (nType == UDP)
        {
                sockfd = socket(AF_INET, SOCK_DGRAM, 0);

                if (sockfd == -1)
                {
                        cout << "socket(AF_INET, SOCK_DGRAM): " << strerror(errno) << endl;
                        return false;
                }

                return true;
        }
        return false;
}

bool TCPSocket::Connect(int nPort, char *Hostname)
{
        hostent *Host;
        void *res;

        if ((Host = gethostbyname(Hostname)) == NULL)
        {
                cout << "gethostbyname(" << Hostname << "): " << hstrerror(h_errno) << endl;
                return false;
        }

        INetAddress.sin_family = AF_INET;
        INetAddress.sin_port = htons(nPort);
        INetAddress.sin_addr = *((struct in_addr *) Host->h_addr);
        res = memset(&(INetAddress.sin_zero), '\0', 8);

        if (connect(sockfd, (struct sockaddr *) &INetAddress, sizeof(struct sockaddr)) == -1)
        {
                cout << "TCPSocket::Connect(): " << strerror(errno) << endl;
                return false;
        }

        connected = true;
        return true;
}

int TCPSocket::Broadcast(const void *msg, int len, int bport)
{
        int on = 1;
        struct sockaddr_in sin;
        if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *) &on, sizeof(on)) < 0)
        {
                cout << "TCPSocket::Broadcast(): setsockopt(SO_BROADCAST): " << strerror(errno) << endl;
                return -1;
        }

        bzero(&sin, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_port = htons(bport);
        sin.sin_addr.s_addr = htonl(INADDR_BROADCAST);

        int ret = sendto(sockfd, msg, len, 0, (struct sockaddr *) &sin, sizeof(sin));
        if (ret < 0)
        {
                cout << "TCPSocket::Broadcast(): sendto: " << strerror(errno) << endl;
                return -1;
        }

        return ret;
}

int TCPSocket::SendTo(const void *msg, int len, int bport, std::string host)
{
        struct sockaddr_in sin;

        bzero(&sin, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_port = htons(bport);
        if (host == "")
                sin.sin_addr.s_addr = from.sin_addr.s_addr;
        else
        {
                struct hostent *hostInfo;
                hostInfo = gethostbyname(host.c_str());
                if (hostInfo == NULL)
                {
                        cout << "gethostbyname(" << host << "): " << hstrerror(h_errno) << endl;
                        return -1;
                }
                sin.sin_family = hostInfo->h_addrtype;
                memcpy((char *) &sin.sin_addr.s_addr, hostInfo->h_addr_list[0], hostInfo->h_length);
        }

        int ret = sendto(sockfd, msg, len, 0, (struct sockaddr *) &sin, sizeof(sin));
        if (ret < 0)
        {
                cout << "TCPSocket::SendTo(), sendto: " << strerror(errno) << endl;
                return -1;
        }

        return ret;
}

int TCPSocket::SendTo(std::string msg)
{
        int ret = sendto(sockfd, msg.c_str(), msg.length(), 0, (struct sockaddr*)&from, sizeof(from));
        if (ret < 0)
        {
                cout << "TCPSocket::SendTo(), sendto: " << strerror(errno) << endl;
                return -1;
        }

        return ret;
}

int TCPSocket::RecvFrom(char *msg, int msize, int timeout)
{
        //timeout stuff
        fd_set events;
        struct timeval tv;

        if (timeout > 0)
        {
                FD_ZERO(&events);
                FD_SET(sockfd, &events);

                tv.tv_sec = timeout / 1000;
                tv.tv_usec = (timeout - tv.tv_sec * 1000) * 1000;

                if (!select(sockfd + 1, &events, NULL, NULL, &tv))
                {
                        //timeout !
                        cout << "TCPSocket::RecvFrom(): Timeout!" << endl;
                        return -1;
                }
        }

        int addr_in_size = sizeof(struct sockaddr_in);
        int nb = recvfrom(sockfd, msg, msize, 0, (struct sockaddr *) &from, (socklen_t *) & addr_in_size);
        if (nb < 0)
        {
                cout << "TCPSocket::RecvFrom(), recvfrom: " << strerror(errno) << endl;
                return -1;
        }
        return nb;
}

int TCPSocket::Send(const void *Message, int nLength, bool block)
{
        int Bytes_Sent;
        unsigned int flags = 0;

        if (!block)
                flags = MSG_DONTWAIT | 0;
        else
                flags = 0;

        if (newfd == 0)
                Bytes_Sent = send(sockfd, Message, nLength, flags);
        else
                Bytes_Sent = send(newfd, Message, nLength, flags);

        return Bytes_Sent;
}

bool TCPSocket::Send(std::string Message)
{
        if (Send(Message.c_str(), Message.length()) < 0)
                return false;

        return true;
}

int TCPSocket::Recv(void *Buffer, int nLength, bool block)
{
        int Bytes_Recv;
        unsigned int flags = 0;

        if (!block)
                flags = MSG_DONTWAIT;

        if (newfd == 0)
                Bytes_Recv = recv(sockfd, Buffer, nLength, flags);

        else
                Bytes_Recv = recv(newfd, Buffer, nLength, flags);



        return Bytes_Recv;
}

bool TCPSocket::Recv(string & Message, int timeout, int fdpipe)
{
        char buf[4096];
        int ret;

        //timeout stuff
        fd_set events;
        struct timeval tv;
        ssize_t res;

        while (Message.find("\n", 0) == Message.npos &&
               Message.find("\r", 0) == Message.npos)
        {
                if (timeout > 0)
                {
                        int fd = (newfd == 0) ? sockfd : newfd;
                        FD_ZERO(&events);
                        FD_SET(fd, &events);
                        if (fdpipe > 0) FD_SET(fdpipe, &events);

                        tv.tv_sec = timeout / 1000;
                        tv.tv_usec = (timeout - tv.tv_sec * 1000) * 1000;

                        if (!select(fd + 1, &events, NULL, NULL, &tv))
                        {
                                //timeout !
                                cout << "TCPSocket::Recv(): Timeout!" << endl;
                                return false;
                        }

                        if (fdpipe > 0 && FD_ISSET(fdpipe, &events))
                        {
                                //the pipe tell us to stop receiving data
                                char c;
                                res  = read(fdpipe, &c, 1);
                                return false;
                        }
                }

                memset(buf, '\0', sizeof(buf));
                ret = Recv(&buf, sizeof(buf));

                if (ret <= 0)
                {
                        cout << "TCPSocket::Recv(): " << strerror(errno) << endl;
                        return false;
                }

                Message += buf;
        }

        return true;
}

bool TCPSocket::Close()
{
        int res;

        if (sockfd < 1) return false;

        if (newfd == 0)
                res = close(sockfd);
        else
                res = close(newfd);

        if (res == -1)
        {
                cout << "TCPSocket::Close(), close: " << strerror(errno) << endl;
                return false;
        }

        return true;
}

bool TCPSocket::InboundClose()
{
        int res;
        res = close(newfd);

        if (res == -1)
                return false;

        return true;
}

bool TCPSocket::Create(int nPort)
{
        void *res;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1)
                return false;

        SetReuse();

        INetAddress.sin_family = AF_INET;
        INetAddress.sin_port = htons(nPort);
        INetAddress.sin_addr.s_addr = INADDR_ANY;
        res = memset(&(INetAddress.sin_zero), '\0', 8);

        if (bind(sockfd, (struct sockaddr *) &INetAddress, sizeof(struct sockaddr)) == -1)
        {
                cout << "TCPSocket::Create(), bind: " << strerror(errno) << endl;
                return false;
        }

        return true;
}

bool TCPSocket::Create(int nPort, char nType)
{
        void *res;

        if (nType == TCP)
        {
                sockfd = socket(AF_INET, SOCK_STREAM, 0);
                if (sockfd == -1)
                {
                        cout << "socket(AF_INET, SOCK_STREAM): " << strerror(errno) << endl;
                        return false;
                }
        }
        if (nType == UDP)
        {
                sockfd = socket(AF_INET, SOCK_DGRAM, 0);
                if (sockfd == -1)
                {
                        cout << "socket(AF_INET, SOCK_DGRAM): " << strerror(errno) << endl;
                        return false;
                }
        }

        SetReuse();

        INetAddress.sin_family = AF_INET;
        INetAddress.sin_port = htons(nPort);
        INetAddress.sin_addr.s_addr = INADDR_ANY;
        res = memset(&(INetAddress.sin_zero), '\0', 8);

        if (bind(sockfd, (struct sockaddr *) &INetAddress, sizeof(struct sockaddr)) == -1)
        {
                cout << "TCPSocket::Create(), bind: " << strerror(errno) << endl;
                return false;
        }


        return true;

}

bool TCPSocket::Listen()
{

        if (listen(sockfd, 5) == -1)
        {
                cout << "TCPSocket::Listen(), listen: " << strerror(errno) << endl;
                return false;
        }

        return true;
}

bool TCPSocket::Accept(int fdpipe)
{
        size_t sin_size = sizeof(struct sockaddr_in);

        fd_set events;
        FD_ZERO(&events);
        FD_SET(sockfd, &events);
        if (fdpipe > 0) FD_SET(fdpipe, &events);

        if (!select(sockfd + 1, &events, NULL, NULL, NULL))
        {
                cout << "TCPSocket::Accept(): Terminating." << endl;
                return false;
        }

        if (fdpipe > 0 && FD_ISSET(fdpipe, &events))
        {
                //the pipe tell us to stop
                return false;
        }

        if ((newfd = accept(sockfd, (struct sockaddr *) &RemoteAddress, (socklen_t *)&sin_size)) == -1)
        {
                cout << "TCPSocket::Accept(), accept: " << strerror(errno) << endl;
                return false;
        }
        return true;
}

char *TCPSocket::GetRemoteIP()
{
        return inet_ntoa(RemoteAddress.sin_addr);
}

char *TCPSocket::GetUDPRemoteIP()
{
        return inet_ntoa(from.sin_addr);
}

bool TCPSocket::Shutdown()
{
        int res;

        if (sockfd < 1) return false;

        if (newfd == 0)
                res = shutdown(sockfd, SHUT_RDWR);
        else
                return false;

        if (res == -1)
        {
                cout << "TCPSocket::Shutdown(), shutdown: " << strerror(errno) << endl;
                return false;
        }

        return true;
}
/*
std::string TCPSocket::GetLocalIP(std::string intf)
{
        std::string ip;

        struct ifreq ifr;

        int skfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (skfd < 0)
                cout << "TCPSocket::GetLocalIP(): can't open socket! " << strerror(errno) << endl;

        strncpy(ifr.ifr_name, intf.c_str(), IFNAMSIZ);

        if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0)
                return "";

        if (ioctl(skfd, SIOCGIFADDR, &ifr) < 0)
                return "";

        ip = inet_ntoa(*(struct in_addr *) &(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr));

        close(skfd);

        cout << "TCPSocket::GetLocalIP(): " << ip << endl;

        return ip;
}
*/
