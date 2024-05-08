#ifndef __NET_WEBSERVER_HPP__
#define __NET_WEBSERVER_HPP__
#include "network/server.hpp"
#include <string>
#include <vector>
#include <map>

namespace ZDC
{
    namespace NET
    {
        namespace Http {
            using Headers = std::map<const std::string, std::string>;

            class Request {
            private:
                std::string strURI;
                Headers oHeaders;
                std::vector<char> vectPayload;

            public:
                Request() = default;
                ~Request() = default;

            public:
                const std::string& uri() const { return strURI; };
                const Headers& headers() const { return oHeaders; };
                const std::vector<char>& payload() const { return vectPayload; };

            public:
                void readPacketFromClient(Client& rClient);
            };
            
            class Response {
            private:
                int nStatusCode;
                std::string strStatusMsg;
                Headers oHeaders;
                std::vector<char> vectPayload;

            public:
                Response() = default;
                ~Response() = default;

            public:
                const int statusCode() const { return nStatusCode; };
                const std::string& statusMessage() const { return strStatusMsg; };
                void status(int code, const std::string& msg)
                { this->nStatusCode = code; this->strStatusMsg = msg; };

            public:
                const std::vector<char>& payload() const { return vectPayload; }
                void setPayload(const std::vector<char>& payload)
                { this->vectPayload = payload; };

            public:
                const Headers& headers() const { return oHeaders; };
                std::string getHeader(const std::string& alias, const std::string& defValue) noexcept
                { 
                    auto found = this->oHeaders.find(alias);
                    if(found == this->oHeaders.end())
                        return defValue;
                    return found->second;
                }
                void setHeader(const std::string& alias, const std::string& value) noexcept
                { this->oHeaders[alias] = value; };
            public:
                void writePacketToClient(Client& rClient);
            };
        }

        class TransactionCompleteException : public SocketException {
        public:
            virtual const char* what() const noexcept override
            { return "Socket transaction completed, request is now invalid."; };
        };

        class WebTransaction {
        public:
            Client& rClient;
            Http::Request oRequest;
            Http::Response oResponse;
        };

        class WebResource {
        public:
            virtual void OnRequested(WebTransaction& rTransaction) = 0;
        };

        class WebServer : public Server, public ClientHandler {
            DISABLE_MOVE_CONSTRUCTOR(WebServer)
            DISABLE_COPY_CONSTRUCTOR(WebServer)
        public:
            WebServer() : Server((ClientHandler&)*this), ClientHandler(ClientHandler::Behavior::THREAD_SINGLE) {};
            ~WebServer() = default;
        protected:
            /// @brief Called when a client socket is connected.
            /// @param rClient A reference to a client that will be used.
            virtual void OnClientConnected(std::unique_ptr<Client> rClient) {};
            /// @brief Called when a client socket is transfered to this handler.
            /// @param rClient A reference to the unique_ptr for this client.
            virtual void OnClientTransfered(std::unique_ptr<Client> rClient) {};
        };

    } // namespace NET
} // namespace ZDC
#endif//__NET_WEBSERVER_HPP__ GUARD