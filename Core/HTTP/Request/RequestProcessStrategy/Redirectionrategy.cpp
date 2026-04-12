#include "RedirectionStrategy.hpp"
#include "../../../../Data/Client.hpp"
#include "../../../../Data/Response.hpp"

void RedirectionStrategy::process(Client *client)
{
    Response *res = client->getResponse();
    Request *req = client->getRequest();
    std::cout << "**************REDIRECTION STRATEGY*******************" << std::endl;
    std::cout << "Location: " << client->getRequest()->getLocation().return_path << std::endl;
    res->setStatus(301);
    res->addHeader("Location", req->getLocation().return_path);
    std::cout << "********************************************" << std::endl;
    (void)client;
}