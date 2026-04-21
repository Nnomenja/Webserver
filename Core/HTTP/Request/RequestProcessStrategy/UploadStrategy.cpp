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

void UploadStrategy::skipBoundary(InputReader& inputReader)
{
    if (inputReader.dataSource == FROM_MEMORY)
    {
        const std::string& buff = *inputReader.buffer;
        size_t& i = inputReader.index;

        while (i + 1 < buff.size() && !(buff[i] == '\r' && buff[i + 1] == '\n'))
            i++;

        if (i + 1 >= buff.size())
        {
            inputReader.dataSource = FROM_FD;
            inputReader.index = 0;
        }
        else
        {
            i += 2;
            return ;
        }
    }

    if (inputReader.dataSource == FROM_FD)
    {
        std::fstream& fd = *inputReader.file;
        std::string line;

        if (!std::getline(fd, line))
            throw InternalServerError();

        // Vérifier format HTTP (\r\n → getline enlève \n)
    }
}

std::string UploadStrategy::trimValue(const std::string& s)
{
    size_t start = 0;
    size_t end = s.size();

    while (start < end && (s[start] == ' ' || s[start] == '\t' || s[start] == '"' || s[start] == '\''))
        start++;
    while (end > start && (s[end-1] == ' ' || s[end-1] == '\t' || s[end-1] == '"' || s[end-1] == '\''))
        end--;
    return s.substr(start, end - start);
}

std::string UploadStrategy::findValue(const std::string& line, const std::string& key)
{
    size_t pos = line.find(key);
    if (pos == std::string::npos)
        return "";

    pos += key.size();

    if (pos < line.size() && (line[pos] == '=' || line[pos] == ':'))
        pos++;

    if (pos >= line.size())
        return "";

    size_t end = line.find(';', pos);
    if (end == std::string::npos)
        end = line.size();

    return trimValue(line.substr(pos, end - pos));
}

MultipartHeader UploadStrategy::parseMultipartHeader(const std::string& header)
{
    MultipartHeader mh;
    size_t pos = 0;

    while (pos < header.size())
    {
        size_t lineEnd = header.find('\n', pos);
        if (lineEnd == std::string::npos)
            lineEnd = header.size();

        std::string line = header.substr(pos, lineEnd - pos);
        pos = lineEnd + 1;

        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);
        if (line.empty())
            continue;

        if (line.find("Content-Disposition:") == 0)
        {
            mh.name     = findValue(line, "name");
            mh.filename = findValue(line, "filename");
        }
        else if (line.find("Content-Type:") == 0)
            mh.contentType = findValue(line, "Content-Type");
        else if (line.find("Content-Transfer-Encoding:") == 0)
            mh.contentTransferEncoding = findValue(line, "Content-Transfer-Encoding");
        else if (line.find("Content-ID:") == 0)
        {
            std::string val = findValue(line, "Content-ID");
            if (!val.empty() && val[0] == '<' && val[val.size()-1] == '>')
                val = val.substr(1, val.size() - 2);
            mh.contentId = val;
        }
        else if (line.find("Content-Location:") == 0)
            mh.contentLocation = findValue(line, "Content-Location");
    }

    return (mh);
}

MultipartHeader UploadStrategy::getMultipartHeader(InputReader& inputReader)
{
    std::string     header;

    if (inputReader.dataSource == FROM_MEMORY)
    {
        const std::string& buff = *inputReader.buffer;
        size_t& i = inputReader.index;

        size_t start = i;

        while (i + 3 < buff.size())
        {
            if (buff[i] == '\r' && buff[i + 1] == '\n' &&
                buff[i + 2] == '\r' && buff[i + 3] == '\n')
            {
                header = buff.substr(start, i - start);
                i += 4;
                return parseMultipartHeader(header);
            }
            i++;
        }

        header = buff.substr(start);
        inputReader.dataSource = FROM_FD;
        inputReader.index = 0;
    }

    if (inputReader.dataSource == FROM_FD)
    {
        std::fstream& fd = *inputReader.file;
        std::string line;

        while (true)
        {
            if (!std::getline(fd, line))
                throw BadRequestException();

            if (!line.empty() && line[line.size() - 1] == '\r')
                line.erase(line.size() - 1);
            else
                throw BadRequestException();

            if (line.empty())
                break;

            header += line + "\r\n";
        }

        return parseMultipartHeader(header);
    }
    return (parseMultipartHeader(header));
}

int UploadStrategy::writeContentUntilBoundary(InputReader& inputReader, MultipartHeader& multipartHeader, Request* request)
{
    const std::string& delim    = multipartHeader.delim;
    const std::string& endDelim = multipartHeader.endDelim;

    if (multipartHeader.filename.empty())
        throw BadRequestException();

    std::stringstream filePath;
    filePath << request->getRootDir() 
                << request->getLocation().path + "/"
                << multipartHeader.filename
                << "."
                << std::time(NULL);
                


    std::ofstream outFile(filePath.str().c_str(), std::ios::binary);
    if (!outFile.is_open())
    {
        throw InternalServerError();  
    }
    std::string overlap;

    if (inputReader.dataSource == FROM_MEMORY)
    {
        const std::string& buff = *inputReader.buffer;
        size_t& i = inputReader.index;

        std::string window = buff.substr(i);

        size_t posEnd = window.find(endDelim);
        size_t posDelim = window.find(delim);

        if (posEnd != std::string::npos &&
            (posDelim == std::string::npos || posEnd <= posDelim))
        {

            outFile.write(window.c_str(), posEnd);
            return (4);
        }

        if (posDelim != std::string::npos)
        {
            outFile.write(window.c_str(), posDelim);
            i += posDelim;
            return 0;
        }

        size_t safeLen = (window.size() > endDelim.size() - 1) ?
                  (window.size() - (endDelim.size() - 1)) :
                  0;
        
        outFile.write(window.c_str(), safeLen);
        overlap = window.substr(safeLen);
        inputReader.dataSource = FROM_FD;
    }

    if (inputReader.dataSource == FROM_FD)
    {
        std::fstream& fd = *inputReader.file;
        const size_t sizeChunk = 1024;
        char chunk[sizeChunk];

        while (fd.read(chunk, sizeChunk) || fd.gcount() > 0)
        {
            size_t n = fd.gcount();
            std::string window = overlap + std::string(chunk, n);

            size_t posEnd   = window.find(endDelim);
            size_t posDelim = window.find(delim);

            if (posEnd != std::string::npos)
            {
                outFile.write(window.c_str(), posEnd);

                return 4;
            }
            if (posDelim != std::string::npos)
            {
                outFile.write(window.c_str(), posDelim);
                std::streamoff back = (std::streamoff)(window.size() - posDelim);
                if (back > 0)
                {
                    fd.seekg(-back, std::ios::cur);
                }
                return 0;
            }

            size_t safeLen = (window.size() > endDelim.size() - 1) ?
                    (window.size() - (endDelim.size() - 1)) :
                    0;

            outFile.write(window.c_str(), safeLen);
            overlap = window.substr(safeLen);
        }

        if (!overlap.empty())
            outFile.write(overlap.c_str(), overlap.size());
        throw BadRequestException();
    }

    return (0);
}


void UploadStrategy::handleMultipartUpload(Client* client)
{

    Request* request = client->getRequest();
    Response* response = client->getResponse();

    // simulation de ce ngnix a fais (ram + buffer fd)
        std::string strBuff;
        std::fstream fdBuff(".tmp/tmp_0000",  std::ios::in | std::ios::out | std::ios::trunc);

        if (!fdBuff.is_open())
        {
            std::cout << "Cannot open tmp file in upload hadleMultipartUPload\n";
            return ;
        }

        {
        // std::string body = 
        //     "------WebKitFormBoundarysyCQvabA6mzrJ2y5\r\n"
        //     "Content-Disposition: form-data; name=\"file\"; filename=\"test.sh\"\r\n"
        //     "Content-Type: application/x-shellscript\r\n"
        //     "\r\n"
        //     "echo \"hello aravelom!\"\r\n"
        //     "\r\n"
        //     "------WebKitFormBoundarysyCQvabA6mzrJ2y5--\r\n";

        //     request->setBody(body);

            size_t pos = request->getBody()._str_buffer.size() / 2;

            strBuff = request->getBody()._str_buffer.substr(0, pos);

            std::cout << "💓💓💓💓" << std::endl;
            std::cout << strBuff << std::endl;
            std::cout << "💓💓💓💓" << std::endl;


            std::string strTmp = request->getBody()._str_buffer.substr(pos + 1);

                fdBuff << strTmp;
                fdBuff.seekg(0);
        }

    InputReader inputReader;

    inputReader.buffer = &(strBuff);
    inputReader.file = &(fdBuff);
    inputReader.dataSource = FROM_MEMORY;
    inputReader.index = 0;

    std::string boundary = request->getHeaders()["content-type"];
    boundary = findValue(boundary, "boundary");


    {
        boundary = "----WebKitFormBoundarysyCQvabA6mzrJ2y5";
    }

    std::string delim = "--" + boundary;
    std::string endDelim = delim + "--";

    while (1)
    {
        int status = 0;
        MultipartHeader multipartHeader;

        switch (status)
        {
            case 0:
                std::cout << "Skip Bondary \n";
                skipBoundary(inputReader);
                status = 1;

            case 1:
                std::cout << "Get Multipart Header \n";
                multipartHeader = getMultipartHeader(inputReader);
                multipartHeader.delim = delim;
                multipartHeader.endDelim = endDelim;
                status = 2;
            case 2:
                std::cout << "write Content Until Boundary\n";
                status = writeContentUntilBoundary(inputReader, multipartHeader, request);
        }
        if (status == 4)
            break ;
    }
   
    response->setStatusCode(201);
    response->setStatusName("Created");

    response->addHeader("Server", "Webserver/1.0");
    response->addHeader("Location", request->getLocation().path);

    response->setBody(" hello 201 hahahahahahaha ");
}

