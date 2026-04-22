#include "RedirectionStrategy.hpp"
#include "../../../../Data/Client.hpp"
#include "../../../../Data/Response.hpp"

void RedirectionStrategy::process(Client *client, Epoll &epoll, Process &process)
{
    Response *res = client->getResponse();
    Request *req = client->getRequest();
    std::cout << "**************REDIRECTION STRATEGY*******************" << std::endl;
    std::cout << "Location: " << client->getRequest()->getLocation().ret.target << std::endl;
    res->setStatus(client->getRequest()->getLocation().ret.code);
    res->addHeader("Location", req->getLocation().ret.target);
    std::cout << "********************************************" << std::endl;
    (void)client;
    (void)epoll;
    (void)process;
}