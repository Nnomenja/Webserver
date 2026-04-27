/*
   File: UploadStrategy.hpp
   By: Azaria
   Created: 2026/04/08 16:13:50
*/

#ifndef UPLOADSTRATEGY_HPP
#define UPLOADSTRATEGY_HPP

# include "IRequestStrategy.hpp"
# include "../../../../Data/Client.hpp"
# include "../../../../Data/Request.hpp"


typedef struct SMultipartHeader
{
    std::string name;
    std::string filename;
    std::string contentType;
    std::string contentTransferEncoding;
    std::string contentId;
    std::string contentLocation;
    std::map<std::string, std::string> customParams;
    std::string delim;
    std::string endDelim;
    
} MultipartHeader;

typedef struct SUploadedFile {
    std::string field;
    std::string original;
    std::string saved;
    size_t      size;
    std::string status;
} UploadedFile;

enum DataSource
{
    FROM_MEMORY,
    FROM_FD
};

enum BodyType
{
    NONE,
    DIRECT,
    MULTIPART,
    URLENCODED,
    UNKNOWN
};

typedef struct SInputReader
{
    DataSource          dataSource;
    t_body              *body;
    size_t              index;
}           InputReader;


class UploadStrategy : public IRequestStrategy
{
    public :
        UploadStrategy();
        ~UploadStrategy();

        void            process(Client* client, Epoll &epoll, Process &process);

    private :

        std::string     trimValue(const std::string& s);
        std::string     findValue(const std::string& line, const std::string& key);
        MultipartHeader parseMultipartHeader(const std::string& header);

        BodyType        bodyTypeDetection(Request* request);
        void            handleMultipartUpload(Client* client);
        void            skipBoundary(InputReader& inputReader);
        MultipartHeader getMultipartHeader(InputReader& inputReader);
        int             writeContentUntilBoundary(InputReader& inputReader,MultipartHeader& multipartHeader);
        std::string     resolveUploadFilename(MultipartHeader& multipartHeader, std::string& uploadStore);

        std::vector \
        <UploadedFile>  _uploadedFiles;
        std::string     _uploadStore;
};

#endif /* UPLOADSTRATEGY_HPP */

