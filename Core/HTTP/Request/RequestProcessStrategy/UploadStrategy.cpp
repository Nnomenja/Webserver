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
#include "../../../../Core/HTTP/Webserv.hpp"

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

    _uploadStore = client->getRequest()->getLocation().uploads;

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
            {
                throw BadRequestException();
            }

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

bool fileExists(const std::string &path)
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

std::string generateUniqueFilename(const std::string &dir, const std::string &filename)
{
    std::string base;
    std::string ext;

    size_t dot = filename.find_last_of('.');
    if (dot != std::string::npos)
    {
        base = filename.substr(0, dot);
        ext = filename.substr(dot);
    }
    else
    {
        base = filename;
        ext = "";
    }

    int counter = 1;
    std::string finalName = filename;

    while (fileExists(dir + "/" + finalName))
    {
        std::stringstream ss;
        ss << base << "(" << counter << ")" << ext;
        finalName = ss.str();
        counter++;
    }

    return (finalName);
}

std::string sanitizeFilename(const std::string &filename) {
    std::string clean;

    for (size_t i = 0; i < filename.size(); ++i)
    {
        char c = filename[i];

        if (c == '/')
            throw BadRequestException();
        if (std::isalnum(c) || c == '.' || c == '_' || c == '-')
        {
            clean += c;
        }
    }
    if (clean.empty() || clean == "." || clean == "..")
        clean = "file";

    return clean;
}

std::string UploadStrategy::resolveUploadFilename(MultipartHeader& multipartHeader, std::string& uploadStore)
{
    std::string original = multipartHeader.filename;
    std::string safeName = sanitizeFilename(original);
    std::string finalName = generateUniqueFilename(uploadStore, safeName);

    return (finalName);
}

int UploadStrategy::writeContentUntilBoundary(InputReader& inputReader, MultipartHeader& multipartHeader)
{
    const std::string& delim    = multipartHeader.delim;
    const std::string& endDelim = multipartHeader.endDelim;
    std::ofstream outFile;
    bool    hasFile;
    UploadedFile uploadedFile;
    size_t      totalWritten = 0;

    if (!multipartHeader.filename.empty())
    {
        std::string filename = resolveUploadFilename(multipartHeader, _uploadStore);

        outFile.open((_uploadStore + "/" + filename).c_str(), std::ios::binary);
        if (!outFile.is_open())
        {
            throw InternalServerError();  
        }
        hasFile = true;
        uploadedFile.field = multipartHeader.name;
        uploadedFile.original = multipartHeader.filename;
        uploadedFile.saved = filename;
    }
    else
    {
        uploadedFile.field = multipartHeader.name;
        uploadedFile.status = "ok";
        hasFile = false;
    }

    std::string overlap;

    if (inputReader.dataSource == FROM_MEMORY)
    {
        const std::string& buff = inputReader.body->_str_buffer;
        size_t& i = inputReader.index;

        size_t posEnd = std::string::npos;
        size_t posDelim = buff.find(delim, i);

        if (posDelim != std::string::npos)
        {
            size_t start = posDelim + delim.size();

            if (buff.size() >= start + 2 &&
                buff[start] == '-' &&
                buff[start + 1] == '-')
            {
                posEnd = posDelim;
                posDelim = std::string::npos;
            }
        }

        if (posEnd != std::string::npos)
        {
            if (hasFile) outFile.write(buff.c_str() + i, posEnd - i - 2);
            totalWritten += (posEnd - i - 2);
            uploadedFile.size = totalWritten;
            uploadedFile.status = "ok";
            _uploadedFiles.push_back(uploadedFile);
            return (4);
        }
        if (posDelim != std::string::npos)
        {
            if (hasFile) outFile.write(buff.c_str() + i, posDelim - i);
            i = posDelim;
            totalWritten += (posDelim - i);
            uploadedFile.size = totalWritten;
            uploadedFile.status = "ok";
            _uploadedFiles.push_back(uploadedFile);
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
                size_t start = posDelim + delim.size();

                if (window.size() >= start + 2 &&
                    window[start] == '-' &&
                    window[start + 1] == '-')
                {
                    posEnd = posDelim;
                    posDelim = std::string::npos;
                }
            }

            if (posEnd != std::string::npos)
            {
                if (hasFile) outFile.write(window.c_str(), posEnd - 2);
                totalWritten += (posEnd - 2);
                uploadedFile.size = totalWritten;
                uploadedFile.status = "ok";
                _uploadedFiles.push_back(uploadedFile);
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
                totalWritten += posDelim;
                uploadedFile.size = totalWritten;
                uploadedFile.status = "ok";
                _uploadedFiles.push_back(uploadedFile);
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
    uploadedFile.size = totalWritten;
    uploadedFile.status = "fail";
    _uploadedFiles.push_back(uploadedFile);
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
                skipBoundary(inputReader);
                status = 1;
				//fallthrough
            case 1:
                multipartHeader = getMultipartHeader(inputReader);
                multipartHeader.delim = delim;
                multipartHeader.endDelim = endDelim;
                status = 2;
				//fallthrough
            case 2:
                status = writeContentUntilBoundary(inputReader, multipartHeader);
        }
        if (status == 4)
            break ;
    }
   
    std::stringstream body;

    body << "{";
    body << "\"status\":\"success\",";
    body << "\"uploaded\":" << _uploadedFiles.size() << ",";
    body << "\"files\":[";

    for (size_t i = 0; i < _uploadedFiles.size(); i++)
    {
        body << "{";
        body << "\"field\":\"" << _uploadedFiles[i].field << "\",";
        body << "\"original_name\":\"" << _uploadedFiles[i].original << "\",";
        body << "\"saved_as\":\"" << _uploadedFiles[i].saved << "\",";
        body << "\"size\":" << _uploadedFiles[i].size << ",";
        body << "\"status\":\"" << _uploadedFiles[i].status << "\"";
        body << "}";

        if (i + 1 < _uploadedFiles.size())
            body << ",";
    }

    body << "]";
    body << "}";

    response->setStatusCode(201);
    response->setStatusName("Created");

    response->addHeader("Server", "Webserver/1.0");
    response->addHeader("Content-Type", "application/json");
    response->addHeader("Location", request->getLocation().path);

    response->setBody(body.str());
}

