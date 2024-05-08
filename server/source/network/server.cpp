#include "network/server.hpp"
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")
class WinSockInit {
private:
    WSADATA wsaData;

public:
    WinSockInit() {
        int iResult;
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            fprintf(stderr, "WSAStartup failed: %d\n", iResult);
            exit(ENETDOWN);
        }
    }
    ~WinSockInit() { WSACleanup(); }
};
static WinSockInit init;
#else 
#include <sys/socket.h>
#include <netinet/in.h>
#endif//_WIN32 WinSock

namespace ZDC
{
    namespace NET
    {

        void ClientHandler::connect(std::unique_ptr<Client> rClient) {
            if(!rClient) return;
            if(this->eBehavior == Behavior::THREAD_SINGLE)
                this->ClientConnectedWrapper(std::move(rClient));
            
        }

        void ClientHandler::transfer(std::unique_ptr<Client> rClient) {
            if(!rClient) return;
            
        }

        bool Server::SpawnThread() {
            this->stop();
            this->_thread = std::thread(this->ThreadFunction, this);
            return (this->_thread.get_id() == std::thread::id());
        }

        inline std::unique_ptr<Client> accept(int _server) {
            struct sockaddr_in sockaddr;
            memset(&sockaddr, 0, sizeof(sockaddr));
            socklen_t sockaddr_len = sizeof(sockaddr);
            int _client = ::accept(_server, (struct sockaddr*)&sockaddr, &sockaddr_len);
            if(_client < 0)
                return {};
            NetAddr_t addr;
            NetAddrType eType = ((sockaddr.sin_family == AF_INET6) ? NetAddrType::IPV6 : NetAddrType::IPV4);
            addr.type = (int)eType;
            if(eType == NetAddrType::IPV4)
                memcpy(&addr.ipv4.AsNumber, sockaddr.sin_zero, sizeof(NetAddrIPV4::AsNumber));
            else if(eType == NetAddrType::IPV6)
                memcpy(&addr.ipv6.AsNumber, sockaddr.sin_zero, sizeof(NetAddrIPV6::AsNumber));
            return std::make_unique<Client>(_client, sockaddr.sin_port, addr);
        }
        
        int Server::ThreadFunction(void* _server) {
            Server* server = (Server*)_server;
            if(!server) return 0;
            server->bShouldRun = true;
            server->bRunning = true;
            std::unique_ptr<Client> client;
            while(server->bShouldRun) {
                client = NET::accept(server->_socket);
                if(!client) continue;
                server->rHandler.connect(std::move(client));
            }
            server->bRunning = false;
        }

        bool Server::accept() {
            // Prevent any other thread from calling this function.
            std::thread::id tid = this->_thread.get_id();
            if(tid != std::thread::id())
                return false;
            // Try to accept a client and give it to our handler.
            std::unique_ptr<Client> client;
            client = NET::accept(this->_socket);
            if(!client) return false;
            this->rHandler.connect(std::move(client));
            return true;
        }

        bool Server::startup(int port, bool async) {
            const int enable = 1;
            this->close();
            int _socket = socket(AF_INET, SOCK_STREAM, 0);
            if(_socket < 0)
                return false;

        #ifdef SO_REUSEADDR
            if(setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0){
                ::close(_socket);
                return false;
            }
        #endif
        #ifdef SO_REUSEPORT
            if(setsockopt(_socket, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(enable)) < 0){
                ::close(_socket);
                return false;
            }
        #endif

            struct sockaddr_in sockaddr;
            memset(&sockaddr, 0, sizeof(sockaddr));
            sockaddr.sin_family = AF_INET;
            sockaddr.sin_port = htons(port);
            sockaddr.sin_addr.s_addr = INADDR_ANY;
            if(bind(_socket, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
                ::close(_socket);
                return false;
            }

            if(listen(_socket, 5) < 0) {
                ::close(_socket);
                return false;
            }

            this->_socket = _socket;
            this->_port = port;

            if(async && !this->SpawnThread()) {
                this->close();
                return false;
            }
            return true;
        }

        void Server::close() {
            if(this->_socket >= 0) {
                ::close(this->_socket);
                this->_socket = -1;
                this->_port = -1;
            }
        }

    } // namespace NET
} // namespace ZDC