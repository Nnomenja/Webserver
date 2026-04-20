/*
   File: UploadStrategy.cpp
   By: Azaria
   Created: 2026/04/08 16:15:17
*/

# include <iostream>

#include "UploadStrategy.hpp"
#include "../../../../Data/Client.hpp"
#include "../../../../Exception/MethodNotAllowed.hpp"


UploadStrategy::UploadStrategy()
{ }

UploadStrategy::~UploadStrategy()
{ }

void responseData(Response *response)
{
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

        std::ostringstream oss;
        oss << body.size();
        
        response->setStatusCode(200);
        response->setStatusName("OK");

        response->addHeader("Content-Type", "text/html; charset=UTF-8");
        response->addHeader("Connection", "close");
        response->addHeader("Content-Length", oss.str());
        response->setBody(body);
}

/*
    Requête →
    Vérif méthode →
    Vérif config →
    Lire body →
    Vérif taille →
    Parser fichier →
    Sécuriser nom →
    Construire path →
    Écrire fichier →
    Réponse HTTP
*/

void UploadStrategy::process(Client *client, Epoll &epoll, Process &process)
{
    Request     *request = client->getRequest();
    Response    *response = client->getResponse();

    if (request->getMethod() != POST 
        && request->getMethod() != GET
        && request->getMethod() != DELETE
    )
    {
        throw MethodNotAllowed();
    }

    responseData(response);
    (void)epoll;
    (void)process;
}