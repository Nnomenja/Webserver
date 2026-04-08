#include "StaticStrategy.hpp"
#include "../../MimeTypes.hpp"
#include "../../../Data/Client.hpp"
#include "../../../Exception/MethodNotAllowed.hpp"
#include "../../../Exception/NotFound.hpp"
#include "../../../Exception/Forbiden.hpp"
#include "../../../Exception/InternalServerError.hpp"
#include "../../../utils/PathUtils.hpp"

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

void StaticStrategy::process(Client *client)
{
    std::string fullpath;
    std::string content;
    
    if (client->getRequest()->getMethod() != GET)
        throw MethodNotAllowed();
    fullpath = client->getEndpoint().root + client->getRequest()->getPathname();
    if (!PathUtils::isPathExist(fullpath))
        throw NotFound();
    if (PathUtils::isDirectory(fullpath))
    {

        if (client->getRequest()->getLocation().index.empty())
        {
            if (!client->getRequest()->getLocation().auto_index)
                throw NotFound();
            else
            {
                std::cout << "Autoindex is on, but there is no index file, directory listing will be implemented in the future" << std::endl;
                return ;
            }
        }
        else
            fullpath += "/" + client->getRequest()->getLocation().index;
    }
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
    client->getResponse()->setStatusCode(200);
    client->getResponse()->setBody(content);
    client->getResponse()->addHeader("Content-Type", MimeTypes::fromFilename(fullpath));
    client->getResponse()->addHeader("Content-Length", std::to_string(content.size()));
    client->getResponse()->addHeader("Connection", "close");
}