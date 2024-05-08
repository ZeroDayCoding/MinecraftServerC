#ifndef __NET_SERVER_HPP__
#define __NET_SERVER_HPP__
#include "common.hpp"
#include "network/network-address.h"
#include <exception>
#include <stdatomic.h>
#include <list>
#include <memory>
#include <thread>

namespace ZDC
{
    namespace NET
    {
        class SocketException {
            // Describes the socket exception.
            virtual const char* what() const noexcept = 0;
        };

        /// @brief A SocketException that will be thrown when the socket is closed.
        class SocketClosedException : public SocketException {
        public:
            virtual const char* what() const noexcept override
            { return "The socket has been closed."; };
        };

        class Client {
        private:
            // The socket handle that the client is connected through.
            int _socket;
            // The internal port number for the client socket handle.
            int _port;
            // The network address for the client.
            NetAddr_t _address;

        public:
            Client(int _socket, int _port, const NetAddr_t& _address)
            : _socket(_socket), _port(_port), _address(_address) {};

        public:
            // The internal port number that the client is connected through.
            inline int port() const { return _port; };
            // The address of the client, may be in either IPV4 or IPV6 format.
            inline const NetAddr_t& address() const { return _address; };

        public:
            /// @brief Reads from the client socket using a timeout.
            /// @param buffer A pointer to the buffer that will be filled with the read in data.
            /// @param size The size, in bytes, that the buffer can handle or, the requested amount of data; whichever is smaller.
            /// @param timeoutMS The amount of time that the read will wait for data to be received.
            /// @return The number of bytes that was read in, and placed into the buffer (negative values indicate error).
            int readWithTimeout(void* buffer, int size, int timeoutMS);
            /// @brief Writes to the client socket using a timeout.
            /// @param buffer A pointer to the buffer that will be written to the client.
            /// @param size The size, in bytes, that the buffer holds or, the amount of data to send; whichever is smaller.
            /// @param timeoutMS The amount of time that the write will wait for data to be written.
            /// @return The number of bytes that was read in, and placed into the buffer (negative values indicate error).
            int writeWithTimeout(const void* buffer, int size, int timeoutMS);

        public:
            /// @brief Reads from the client socket without a timeout.
            /// @param buffer A pointer to the buffer that will be filled with the read in data.
            /// @param size The size, in bytes, that the buffer can handle or, the requested amount of data; whichever is smaller.
            /// @return The number of bytes that was read in, and placed into the buffer (negative values indicate error).
            int read(void* buffer, int size);
            /// @brief Writes to the client socket without a timeout.
            /// @param buffer A pointer to the buffer that will be written to the client.
            /// @param size The size, in bytes, that the buffer holds or, the amount of data to send; whichever is smaller.
            /// @return The number of bytes that was read in, and placed into the buffer (negative values indicate error).
            int write(const void* buffer, int size);

        public:
            /// @brief Closes the socket, throws a SocketClosedException.
            /// @note This function will throw a SocketClosedException, please don't catch it in any handler code.
            void close();
        };

        class ClientHandler {
        public:
            enum class Behavior {
                // The client handler will not spawn a new thread for each client, but will hold the current thread until client returns.
                THREAD_SINGLE = 0,
                // The client handler will spawn a new thread for each of its client handlers.
                THREAD_SPAWN = 1,
            };

        protected:
            // A list of clients that are persisting for further usage.
            std::list<std::unique_ptr<Client>> listClients;
            // The behavior that the client handler will use when connecting or transfering a client.
            const Behavior eBehavior;

        public:
            ClientHandler(Behavior eBehavior) : eBehavior(eBehavior) {};
            ~ClientHandler() = default;

        protected:
            /// @brief Called when a client socket is connected.
            /// @param rClient A reference to a client that will be used.
            virtual void OnClientConnected(std::unique_ptr<Client> rClient) = 0;
            /// @brief Called when a client socket is transfered to this handler.
            /// @param rClient A reference to the unique_ptr for this client.
            virtual void OnClientTransfered(std::unique_ptr<Client> rClient) = 0;

        protected:
            void ClientConnectedWrapper(std::unique_ptr<Client> rClient);
            void ClientTransferedWrapper(std::unique_ptr<Client> rClient);

        public:
            /// @brief Connects the 
            /// @param rClient 
            void connect(std::unique_ptr<Client> rClient);
            void transfer(std::unique_ptr<Client> rClient);
        };

        class Server {
            DISABLE_COPY_CONSTRUCTOR(Server)
            DISABLE_MOVE_CONSTRUCTOR(Server)
        private:
            // The port number that the server is running on.
            int _port = -1;
            // The socket handle that the server is using.
            int _socket = -1;
            // The thread for the seperate server thread.
            std::thread _thread;
            // Set to true if the server should be running.
            _Atomic(bool) bShouldRun = false;
            // Set to true if the server is running.
            _Atomic(bool) bRunning = false;
            // A reference to the client handler, must persist for the duration of Server.
            ClientHandler& rHandler;

        public:
            explicit Server(ClientHandler& handler) noexcept : rHandler(handler) {};
            inline ~Server() noexcept { this->close(); };

        private:
            bool SpawnThread();
            static int ThreadFunction(void*);
        public:
            /// @brief Accepts a new client and passes it to the internal ClientHandler.
            /// @note If the Server started in async mode then calling this will have no effect.
            /// @return True if a client was accepted, otherwise False.
            bool accept();
        public:
            /// @brief Starts up the server in a seperate thread.
            bool startup(int port, bool async);
            /// @brief Restarts the server.
            void restart();
            /// @brief Stops the server, but keeps the socket open and all connected clients open.
            void stop();
            /// @brief Closes down the server and closes the connected clients.
            void close();
        };

    } // namespace NET
} // namespace ZDC
#endif//__NET_SERVER_HPP__ GUARD