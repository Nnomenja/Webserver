#include "StaticStrategy.hpp"
#include "../../MimeTypes.hpp"
#include "../../../../Data/Client.hpp"
#include "../../../../Exception/MethodNotAllowed.hpp"
#include "../../../../Exception/NotFound.hpp"
#include "../../../../Exception/Forbiden.hpp"
#include "../../../../Exception/InternalServerError.hpp"
#include "../../../../utils/PathUtils.hpp"

void StaticStrategy::process(Client *client, Epoll &epoll, Process &process)
{
    std::string fullpath = client->getRequest()->getFullPath();
    std::string content;
    if (!PathUtils::isFileReadable(fullpath))
        throw Forbiden();
    try
    {
        content = PathUtils::getFileContentbypath(fullpath);
    }
    catch(const std::exception& e)
    {
       throw InternalServerError();
    }
    client->getResponse()->setStatus(200);
    client->getResponse()->setBody(content);
    client->getResponse()->setContentType(MimeTypes::fromFilename(fullpath));
    client->getResponse()->addHeader("Connection", "close");
    (void)epoll;
    (void)process;
}