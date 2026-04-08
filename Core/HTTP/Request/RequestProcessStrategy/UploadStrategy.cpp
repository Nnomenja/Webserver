/*
   File: UploadStrategy.cpp
   By: Azaria
   Created: 2026/04/08 16:15:17
*/

# include <iostream>

#include "UploadStrategy.hpp"
#include "../../../../Data/Client.hpp"

UploadStrategy::UploadStrategy()
{ }

UploadStrategy::~UploadStrategy()
{ }

void simul(Client *client)
{
    std::ostringstream oss;

    std::string body = 
        "<!DOCTYPE html>"
        "<html lang=\"fr\">"
        "<head>"
        "<meta charset=\"UTF-8\">"
        "<title>Webserv - 42 Antananarivo</title>"
        "<style>"
        "body{margin:0;padding:0;background:linear-gradient(135deg,#0f2027,#203a43,#2c5364);"
        "font-family:Arial,sans-serif;color:white;display:flex;justify-content:center;"
        "align-items:center;height:100vh;}"
        ".card{background:rgba(255,255,255,0.1);backdrop-filter:blur(10px);"
        "padding:40px;border-radius:15px;text-align:center;"
        "box-shadow:0 0 30px rgba(0,0,0,0.5);width:500px;}"
        "h1{margin-bottom:10px;font-size:32px;}"
        ".status{color:#22c55e;font-weight:bold;font-size:20px;}"
        ".info{margin-top:20px;font-size:14px;opacity:0.85;}"
        ".footer{margin-top:30px;font-size:12px;opacity:0.6;}"
        "</style>"
        "</head>"
        "<body>"
        "<div class=\"card\">"
        "<h1>Webserv Opérationnel</h1>"
        "<div class=\"status\">HTTP/1.1 200 OK</div>"
        "<div class=\"info\">"
        "<p>Projet: Webserv</p>"
        "<p>Langage: C++98</p>"
        "<p>Architecture: epoll event-driven</p>"
        "<p>Mode: Non-Blocking I/O</p>"
        "</div>"
        "<div class=\"footer\">42 Antananarivo - 2026</div>"
        "</div>"
        "</body>"
        "</html>";

    oss << "HTTP/1.1 200 OK\r\n"
        << "Content-Type: text/html; charset=UTF-8\r\n"
        << "Connection: close\r\n"
        << "Content-Length: " << body.size() << "\r\n"
        << "\r\n"
        << body;

    std::string response = oss.str();
    client->setBuffer(response);
    client->setBufferSize(response.size());
}

void UploadStrategy::process(Client *client)
{
    
    simul(client);
}