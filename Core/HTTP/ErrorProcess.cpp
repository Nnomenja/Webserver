#include "ErrorProcess.hpp"
#include "../../Data/Client.hpp"
#include "../../utils/PathUtils.hpp"
#include <string>
#include <sstream>
#include <ctime>
#include "Template.hpp"


void    ErrorProcess::processError(const ServerException &e, Client *client)
{
    std::string root = client->getEndpoint().root;
    if (client->getRequest()->getLocation().root.size())
        root = client->getRequest()->getLocation().root;
    std::string ErrorPagePath =  client->getDefaultErrorPagePath(e.getCode());
    
    Response *res = client->getResponse();
    std::stringstream ss;

    res->setStatus(e.getCode());
    try
    {
        if (!ErrorPagePath.size())
            throw std::exception();
        res->setBody(PathUtils::getFileContentbypath(root + ErrorPagePath));

    }
    catch(const std::exception& e)
    {
         res->setBody(Template::Error(res->getStatusCode(), res->getStatusMessage()));
    }
    res->addHeader("Content-Type", "text/html; charset=UTF-8");
    ss << res->getBody().size();
    res->addHeader("Content-Length", ss.str());
    ss.str("");
    res->addHeader("Connection", "close");
}
