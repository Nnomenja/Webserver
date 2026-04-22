#include "StaticStrategy.hpp"
#include "../../MimeTypes.hpp"
#include "../../../../Data/Client.hpp"
#include "../../../../Exception/MethodNotAllowed.hpp"
#include "../../../../Exception/NotFound.hpp"
#include "../../../../Exception/Forbiden.hpp"
#include "../../../../Exception/InternalServerError.hpp"
#include "../../../../utils/PathUtils.hpp"

/**========================================================================
 * todo                             TODO
 *  - Join the pathname with root in order to have the fullpathname
 *  - If it is a directory && if autoindex is on and there is no index file, 
 * change strategy to directory strategy
 *  - Verify that the file exists otherwise throw 404 Not Found
 *   -Verify if file is readable, if not throw 403 Forbidden
 *  - Get the file content and set it in the response body
 *  - Get the file content type and set it in the response header
 *  - Set the response status code to 200
 *   
 *========================================================================**/

void StaticStrategy::process(Client *client, Epoll &epoll, Process &process)
{
    std::cout << "StaticStrategy processing..." << std::endl;
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
    std::cout << "StaticStrategy processing done" << std::endl;
    (void)epoll;
    (void)process;
}