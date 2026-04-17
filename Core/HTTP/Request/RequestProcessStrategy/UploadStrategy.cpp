/*
   File: UploadStrategy.cpp
   By: Azaria
   Created: 2026/04/08 16:15:17
*/

# include <iostream>
# include <algorithm>

#include "UploadStrategy.hpp"
#include "../../../../Exception/MethodNotAllowed.hpp"
#include "../../../../Exception/BadRequestException.hpp"
#include "../../../../Exception/UnsupportedMediaType.hpp"
#include "../../../../Exception/InternalServerError.hpp"


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


void simulBodyData(BodyType type, Request* request)
{
    switch (type)
    {
        case NONE:
            request->setContentType("");
            request->setBody("");
            break;
      
        case DIRECT:
            request->setContentType("text/plain");
            request->setBody("Ligne 1 du contenu direct\nLigne 2\nLigne 3 avec des données: ABC123!@#");
            // Pour simuler du binaire, on peut mettre des bytes non imprimables
            break;
        
        case MULTIPART:
            request->setContentType("multipart/form-data");
            request->setContentTypeParam("boundary", "boundary123");
            
            request->setBody(
                "--boundary123\r\n"
                "Content-Disposition: form-data; name=\"text_field\"\r\n"
                "\r\n"
                "Valeur simple\r\n"
                "--boundary123\r\n"
                "Content-Disposition: form-data; name=\"file1\"; filename=\"test1.txt\"\r\n"
                "Content-Type: text/plain\r\n"
                "\r\n"
                "Contenu du premier fichier\r\n"
                "--boundary123\r\n"
                "Content-Disposition: form-data; name=\"file2\"; filename=\"test2.sh\"\r\n"
                "Content-Type: application/x-sh\r\n"
                "\r\n"
                "#!/bin/bash\necho \"Hello from script\"\r\n"
                "--boundary123\r\n"
                "Content-Disposition: form-data; name=\"json_data\"\r\n"
                "Content-Type: application/json\r\n"
                "\r\n"
                "{\"key\":\"value\",\"number\":42}\r\n"
                "--boundary123--\r\n"
            );
            break;
        
        case URLENCODED:
            request->setContentType("application/x-www-form-urlencoded");
            
            request->setBody(
                "name=John%20Doe&"
                "email=john%40example.com&"
                "age=30&"
                "interests=coding%2Creading%2Cgaming&"
                "comment=Hello%20world%21&"
                "newsletter=true&"
                "file=data.txt"
            );
            break;
            
        default:
            break;
    }
}

void UploadStrategy::process(Client *client)
{
    Request     *request = client->getRequest();

    if (request->getMethod() != POST 
        && request->getMethod() != GET
        && request->getMethod() != DELETE
    )
    {
        throw MethodNotAllowed();
    }

    {
        simulBodyData(DIRECT, request);
    }

    BodyType bodyType = bodyTypeDetection(request);

    switch (bodyType)
    {
        case DIRECT:
            return handleDirectUpload(client);

        case MULTIPART:
            return handleMultipartUpload(client);

        // case URLENCODED:
        //     return handleUrlEncoded();

        default:
            throw UnsupportedMediaType();
    }
}

BodyType UploadStrategy::bodyTypeDetection(Request* request)
{
    // if (request->getBody().empty() && request->getContentLength() == 0) {
    //     return NONE;
    // }

    // std::string contentType = request->getContentType();
    // std::transform(contentType.begin(), contentType.end(),
    //                contentType.begin(), ::tolower);

    // // Multipart standard (multipart/form-data, multipart/mixed, ...)
    // if (contentType.find("multipart/") == 0) {
    //     std::string boundary = request->getContentTypeParam("boundary");
    //     if (boundary.empty())
    //     {
    //         throw BadRequestException();
    //     }

    //     return MULTIPART;
    // }

    // // URL-encoded : AVANT le bloc application/* générique
    // if (contentType == "application/x-www-form-urlencoded") {
    //     return URLENCODED;
    // }

    // // octet-stream : binaire brut OU multipart binaire selon boundary
    // if (contentType == "application/octet-stream") {
    //     std::string boundary = request->getContentTypeParam("boundary");
    //     return boundary.empty() ? DIRECT : MULTIPART;
    // }

    // // Types connus → envoi direct du corps brut
    // if (contentType.find("text/")        == 0 ||
    //     contentType.find("application/") == 0 ||
    //     contentType.find("image/")       == 0 ||
    //     contentType.find("video/")       == 0 ||
    //     contentType.find("audio/")       == 0) {
    //     return DIRECT;
    // }

    // // Pas de Content-Type mais un corps présent → direct par défaut
    // if (contentType.empty() && !request->getBody().empty()) {
    //     return DIRECT;
    // }

    // return UNKNOWN;
    (void) request;
    return (MULTIPART);
}

std::string UploadStrategy::creatUploadFileName(Request* request)
{
    ( void ) request;
    return ("teste.txt");
}

std::string getRequestPath( void )
{
    return ("/home/aravelom/Project/current/webserver/www/upload");
}

void UploadStrategy::handleDirectUpload(Client* client)
{
    // Request     *request = client->getRequest();
    Response    *response = client->getResponse();
    // std::string path = getRequestPath();
    // std::ofstream file(path.c_str(), std::ios::binary);
    
    // if (!file.is_open())
    //     throw InternalServerError();

    // // Récupérer le body de la requête
    // std::string strBuff = request->getBodyBuffer();      // partie en RAM
    // std::fstream& fdBuff = request->getBodyFd();         // partie en fichier temporaire

    // if (!strBuff.empty())
    // {
    //     file.write(strBuff.c_str(), strBuff.size());
    //     if (!file.good())
    //         throw InternalServerError();
    // }

    // if (fdBuff.is_open())
    // {
    //     // Rembobiner au début du fichier temporaire
    //     fdBuff.seekg(0, std::ios::beg);

    //     char buffer[8192];
    //     while (fdBuff.read(buffer, sizeof(buffer)))
    //         file.write(buffer, fdBuff.gcount());

    //     // Écrire les derniers octets restants (< 8192)
    //     if (fdBuff.gcount() > 0)
    //         file.write(buffer, fdBuff.gcount());

    //     if (!file.good())
    //         throw InternalServerError();
    // }

    // file.close();

    response->setStatusCode(201);
    response->setStatusName("Created");

    response->addHeader("Location", "path/andfilename");
    // response->addHeader("Content-Type", request->getContentType());
}


void    UploadStrategy::handleMultipartUpload(Client* client)
{

    // simulation de ce ngnix a fais (ram + buffer fd)
        std::string strBuff;
        std::fstream fdBuff(".tmp/tmp_0000",  std::ios::in | std::ios::out | std::ios::trunc);

        if (!fdBuff.is_open())
        {
            std::cout << "Cannot open tmp file in upload hadleMultipartUPload\n";
            return ;
        }

        {
            strBuff = 
                "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
                "Content-Disposition: form-data; name=\"description\"\r\n"
                "\r\n"
                "Mon fichier important\r\n"
                "------WebKitFormBoundary7";
            std::string strTmp = "MA4YWxkTrZu0gW\r\n"
                "Content-Disposition: form-data; name=\"file\"; filename=\"document.pdf\"\r\n"
                "Content-Type: application/pdf\r\n"
                "\r\n"
                "%PDF-1.4\n%âãÏÓ\n1 0 obj\n<</Type/Catalog/Pages 2 0 R>>\nendobj\n2 0 obj\n<</Type/Pages/Kids[3 0 R]/Count 1>>\nendobj\n3 0 obj\n<</Type/Page/MediaBox[0 0 612 792]/Parent 2 0 R/Resources<<>>>>\nendobj\nxref\n0 4\n0000000000 65535 f\n0000000015 00000 n\n0000000065 00000 n\n0000000118 00000 n\ntrailer\n<</Size 4/Root 1 0 R>>\nstartxref\n204\n%%EOF\n"
                "\r\n"
                "------WebKitFormBoundary7MA4YWxkTrZu0gW--\r\n";

                fdBuff << strTmp;
                fdBuff.seekg(0);
        }

    // -------------------------

    // while (1)
    // {
    //     for (size_t i = 0; i < strBuff.size(); )
    //     {
    //         skapBondary();
    //         MultipartHeader multipartHeader = getMultipartHeader(i);
    //         parsingContent(multipartHeader, i);
    //     }
    //     for (endline in buff fd)
    //     {
    //         skapBondary();
    //         MultipartHeader multipartHeader = getMultipartHeader(i);
    //         parsingContent(multipartHeader, i);
    //     }
    // }
    responseData(client->getResponse());
}

