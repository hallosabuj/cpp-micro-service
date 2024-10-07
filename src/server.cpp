#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <vector>
#include "server.h"

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
using namespace std;

int hashMethod(boost::beast::string_view method){
    if(method == "GET"){
        return 1;
    }else if(method == "POST"){
        return 2;
    }
    return 0;
}
int hashURI(boost::beast::string_view uri){
    if(uri == "/api/hello-world"){
        return 1;
    }
    return 0;
}
void handleRequest(http::request<http::string_body>& request, tcp::socket& socket) {
    cout << request.method_string() << endl;
    http::response<http::string_body> response;
    switch (hashMethod(request.method_string())){
        // GET Methods
        case 1:{
            switch (hashURI(request.target()))
            {
                case 1:
                    // /api/hello-world
                    // Prepare the response message
                    response.version(request.version());
                    response.result(http::status::ok);
                    response.set(http::field::server, "My HTTP Server");
                    response.set(http::field::content_type, "text/plain");
                    response.body() = "Hello, World!";
                    response.prepare_payload();
                    break;
                
                default:{
                    response.result(http::status::not_found);
                    response.set(http::field::content_type, "text/plain");
                    response.body() = "File not found\r\n";
                    break;
                }
            }
            break;
        }
        // POST Methods
        case 2:{
            /* code */
            break;
        }
        default:{
            cout << "unknown method " <<  request.method_string() << endl;
            break;
        }
    }

    // Send the response to the client
    boost::beast::http::write(socket, response);
}

void runServer() {
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, {tcp::v4(), 8080});

    while (true) {
        tcp::socket socket(io_context);
        acceptor.accept(socket);

        // Read the HTTP request
        boost::beast::flat_buffer buffer;
        http::request<http::string_body> request;
        boost::beast::http::read(socket, buffer, request);

        // Handle the request
        handleRequest(request, socket);

        // Close the socket
        socket.shutdown(tcp::socket::shutdown_send);
    }
}

int init() {
    try {
        runServer();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}

class Server{
    private:
        /* data */
        string server_ip;
        vector<string> endpointList;
        vector<string> methodList;
    public:
        void SetServerIP(string);
        string GetServerIP();

        void AddEndpoint(string);
        int HashEndpoint(string);
        void AddMethod(string);
        int HashMethod(string);

        void HandleRequest(http::request<http::string_body>& request, tcp::socket& socket){
            cout << request.method_string() << endl;
            http::response<http::string_body> response;
            switch (this->HashMethod(request.method_string().to_string())){
                // GET Methods
                case 1:{
                    switch (this->HashEndpoint(request.target().to_string()))
                    {
                        case 1:
                            // /api/hello-world
                            // Prepare the response message
                            response.version(request.version());
                            response.result(http::status::ok);
                            response.set(http::field::server, "My HTTP Server");
                            response.set(http::field::content_type, "text/plain");
                            response.body() = "Hello, World!";
                            response.prepare_payload();
                            break;
                        
                        default:{
                            response.result(http::status::not_found);
                            response.set(http::field::content_type, "text/plain");
                            response.body() = "File not found\r\n";
                            break;
                        }
                    }
                    break;
                }
                // POST Methods
                case 2:{
                    /* code */
                    break;
                }
                default:{
                    cout << "unknown method " <<  request.method_string() << endl;
                    break;
                }
            }

            // Send the response to the client
            boost::beast::http::write(socket, response);
        }

        Server(/* args */);
        ~Server();
};

Server::Server(/* args */){
    cout << "Constructor called" << endl;
}

Server::~Server(){
    cout << "Destructor called" << endl;
}

void Server::SetServerIP(string ip){
    this->server_ip = ip;
}

string Server::GetServerIP(){
    return this->server_ip;
}

void Server::AddEndpoint(string endpoint){
    this->endpointList.push_back(endpoint);
}
int Server::HashEndpoint(string endpoint){
    for(int i=0; i<this->endpointList.size(); i++){
        if(this->endpointList[i] == endpoint){
            return 1;
        }
    }
    return 0;
}

void Server::AddMethod(string method){
    this->methodList.push_back(method);
}
int Server::HashMethod(string method){
    for(int i=0; i<this->methodList.size(); i++){
        if(this->methodList[i] == method){
            return 1;
        }
    }
    return 0;
}