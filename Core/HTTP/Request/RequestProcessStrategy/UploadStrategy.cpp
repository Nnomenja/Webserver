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


void UploadStrategy::process(Client *client, Epoll &epoll, Process &process)
{
    Request     *request = client->getRequest();

    if (request->getMethod() != POST 
        && request->getMethod() != GET
        && request->getMethod() != DELETE
    )
    {
        throw MethodNotAllowed();
    }


    (void)epoll;
    (void)process;

    BodyType bodyType = bodyTypeDetection(request);

    switch (bodyType)
    {
        case MULTIPART:
            return handleMultipartUpload(client);
        default:
            throw UnsupportedMediaType();
    }
}

BodyType UploadStrategy::bodyTypeDetection(Request* request)
{
    if (request->getBody()._str_buffer.empty() && request->getContentLength() == 0) {
        return NONE;
    }

    std::string contentType = request->getHeaders()["content-type"];
    std::transform(contentType.begin(), contentType.end(),
                   contentType.begin(), ::tolower);

    if (contentType.find("multipart/") == 0) {
        std::string boundary = findValue(contentType, "boundary");
        if (boundary.empty())
        {
            throw BadRequestException();
        }
        return MULTIPART;
    }

    if (contentType == "application/x-www-form-urlencoded") {
        return URLENCODED;
    }

    if (contentType == "application/octet-stream") {
        std::string boundary = findValue(contentType, "boundary");
        return boundary.empty() ? DIRECT : MULTIPART;
    }

    if (contentType.find("text/")        == 0 ||
        contentType.find("application/") == 0 ||
        contentType.find("image/")       == 0 ||
        contentType.find("video/")       == 0 ||
        contentType.find("audio/")       == 0) {
        return DIRECT;
    }

    if (contentType.empty() && !request->getBody()._str_buffer.empty()) {
        return DIRECT;
    }

    return (UNKNOWN);
}

void UploadStrategy::skipBoundary(InputReader& inputReader)
{
    if (inputReader.dataSource == FROM_MEMORY)
    {
        const std::string& buff = inputReader.body->_str_buffer;
        size_t& i = inputReader.index;

        while (i + 1 < buff.size() && !(buff[i] == '\r' && buff[i + 1] == '\n'))
            i++;

        if (i + 1 >= buff.size())
        {
            inputReader.dataSource = FROM_FD;
        }
        else
        {
            i += 2;
            return ;
        }
    }

    if (inputReader.dataSource == FROM_FD)
    {
        std::fstream& fd = inputReader.body->_file_buffer;
        std::string line;

        std::getline(fd, line);
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
        const std::string& buff = inputReader.body->_str_buffer;
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
        std::fstream& fd = inputReader.body->_file_buffer;
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

// int UploadStrategy::writeContentUntilBoundary(InputReader& inputReader, MultipartHeader& multipartHeader, Request* request)
// {
//     const std::string& delim    = multipartHeader.delim;
//     const std::string& endDelim = multipartHeader.endDelim;
//     std::ofstream outFile;

//     if (!multipartHeader.filename.empty())
//     {
//         std::string filePath;

//         filePath = request->getRootDir() + request->getLocation().path + "/" + multipartHeader.filename + ".";

//         {
//             std::stringstream ss;
//             ss << std::time(NULL);

//             filePath += ss.str();
//         }

//         outFile.open(filePath.c_str(), std::ios::binary);
//         if (!outFile.is_open())
//         {
//             throw InternalServerError();  
//         }
//     }
//     std::string overlap;

//     if (inputReader.dataSource == FROM_MEMORY)
//     {
//         const std::string& buff = inputReader.body->_str_buffer;
//         size_t& i = inputReader.index;

//         // {
//         //     size_t posEnd = buff.find(endDelim, i);
//         //     size_t posDelim = buff.find(delim, i);
//         // }

//         std::string window = buff.substr(i);

//         size_t posEnd = window.find(endDelim);
//         size_t posDelim = window.find(delim);

//         if (posEnd != std::string::npos &&
//             (posDelim == std::string::npos || posEnd <= posDelim))
//         {
//             if (outFile.is_open())
//                 outFile.write(window.c_str(), posEnd);
//             return (4);
//         }

//         if (posDelim != std::string::npos)
//         {
//             if (outFile.is_open())
//                 outFile.write(window.c_str(), posDelim);
//             i += posDelim;
//             return 0;
//         }

//         size_t safeLen = (window.size() > endDelim.size() - 1) ?
//                 (window.size() - (endDelim.size() - 1)) :
//                 0;
//         if (outFile.is_open())
//             outFile.write(window.c_str(), safeLen);
//         overlap = window.substr(safeLen);
//         inputReader.dataSource = FROM_FD;
//     }

//     if (inputReader.dataSource == FROM_FD)
//     {
//         std::fstream& fd = inputReader.body->_file_buffer;

//         const size_t sizeChunk = 1024;
//         char chunk[sizeChunk];

//         while (fd.read(chunk, sizeChunk) || fd.gcount() > 0)
//         {
//             size_t n = fd.gcount();
//             std::string window = overlap + std::string(chunk, n);

//             size_t posEnd   = window.find(endDelim);
//             size_t posDelim = window.find(delim);

//             if (posEnd != std::string::npos)
//             {
//                 if (outFile.is_open())
//                     outFile.write(window.c_str(), posEnd - 2);
//                 return 4;
//             }
//             if (posDelim != std::string::npos)
//             {
//                 if (outFile.is_open())
//                     outFile.write(window.c_str(), posDelim);
//                 std::streamoff back = (std::streamoff)(window.size() - posDelim);
//                 if (back > 0)
//                 {
//                     fd.seekg(-back, std::ios::cur);
//                 }
//                 return 0;
//             }

//             size_t safeLen = (window.size() > endDelim.size() - 1) ?
//                     (window.size() - (endDelim.size() - 1)) :
//                     0;
//             if (outFile.is_open())
//                 outFile.write(window.c_str(), safeLen);
//             overlap = window.substr(safeLen);
//         }

//         if (!overlap.empty())
//         {
//             if (outFile.is_open())
//                 outFile.write(overlap.c_str(), overlap.size());
//         }
//         throw BadRequestException();
//     }

//     return (0);
// }



int UploadStrategy::writeContentUntilBoundary(InputReader& inputReader, MultipartHeader& multipartHeader, Request* request)
{
    const std::string& delim    = multipartHeader.delim;
    const std::string& endDelim = multipartHeader.endDelim;
    std::ofstream outFile;
    bool    hasFile;

    if (!multipartHeader.filename.empty())
    {
        std::string filePath;

        filePath = request->getRootDir() + request->getLocation().path + "/" + multipartHeader.filename + ".";

        {
            std::stringstream ss;
            ss << std::time(NULL);

            filePath += ss.str();
        }

        outFile.open(filePath.c_str(), std::ios::binary);
        if (!outFile.is_open())
        {
            throw InternalServerError();  
        }
        hasFile = true;
    }
    else
        hasFile = false;

    std::string overlap;

    if (inputReader.dataSource == FROM_MEMORY)
    {
        const std::string& buff = inputReader.body->_str_buffer;
        size_t& i = inputReader.index;

        size_t posEnd = std::string::npos;
        size_t posDelim = buff.find(delim, i);

        if (posDelim != std::string::npos)
        {
            if (buff.size() > (posDelim + delim.size() + 2))
            {
                if (buff[posDelim + delim.size() + 1] == '-'
                && buff[posDelim + delim.size() + 2] == '-')
                {
                    posEnd = posDelim;
                    posDelim = std::string::npos;
                }
            }
        }

        if (posEnd != std::string::npos)
        {
            if (hasFile) outFile.write(buff.c_str() + i, posEnd - i - 2);
            return (4);
        }
        if (posDelim != std::string::npos)
        {
            if (hasFile) outFile.write(buff.c_str() + i, posDelim - i);
            i = posDelim;
            return 0;
        }
        size_t safeLen = ((buff.size() - i) > endDelim.size() - 1) ?
                (buff.size() - i - (endDelim.size() - 1)) :
                0;
        if (hasFile) outFile.write(buff.c_str() + i, safeLen);
        overlap = buff.substr(buff.size() - (endDelim.size() - 1));
        inputReader.dataSource = FROM_FD;
    }

    if (inputReader.dataSource == FROM_FD)
    {
        std::fstream& fd = inputReader.body->_file_buffer;

        const size_t sizeChunk = 1024;
        char chunk[sizeChunk];

        std::string window;
        window.reserve(sizeChunk + endDelim.size());

        while (fd.read(chunk, sizeChunk) || fd.gcount() > 0)
        {
            size_t n = fd.gcount();

            window.clear();
            window.append(overlap);
            window.append(chunk, n);

            size_t posEnd = std::string::npos;
            size_t posDelim = window.find(delim);

            if (posDelim != std::string::npos)
            {
                if (window.size() > (posDelim + delim.size() + 2))
                {
                    if (window[posDelim + delim.size() + 1] == '-'
                    && window[posDelim + delim.size() + 2] == '-')
                    {
                        posEnd = posDelim;
                        posDelim = std::string::npos;
                    }
                }
            }

            if (posEnd != std::string::npos)
            {
                if (hasFile) outFile.write(window.c_str(), posEnd - 2);
                return 4;
            }
            if (posDelim != std::string::npos)
            {
                if (hasFile) outFile.write(window.c_str(), posDelim);
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
            if (hasFile) outFile.write(window.c_str(), safeLen);
            overlap = "";
            overlap.append(window, safeLen, window.size() - safeLen);
        }

        if (!overlap.empty() && hasFile)
        {
            outFile.write(overlap.c_str(), overlap.size());
        }
        throw BadRequestException();
    }
    return (0);
}

void UploadStrategy::handleMultipartUpload(Client* client)
{

    Request* request = client->getRequest();
    Response* response = client->getResponse();
    InputReader inputReader;
    std::string delim;
    std::string endDelim;

    inputReader.body = &(request->getBody());
    inputReader.dataSource = FROM_MEMORY;
    inputReader.index = 0;

    {
        std::string boundary = request->getHeaders()["content-type"];
        boundary = findValue(boundary, "boundary");

        delim = "--" + boundary;
        endDelim = delim + "--";
        inputReader.body->_file_buffer.seekg(0, std::ios::beg);
    }
    
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
				//fallthrough
            case 1:
                std::cout << "Get Multipart Header \n";
                multipartHeader = getMultipartHeader(inputReader);
                multipartHeader.delim = delim;
                multipartHeader.endDelim = endDelim;
                status = 2;
				//fallthrough
            case 2:
                std::cout << "write Content Until Boundary\n";
                status = writeContentUntilBoundary(inputReader, multipartHeader, request);
				//fallthrough
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

