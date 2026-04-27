#include "RedirectionStrategy.hpp"
#include "../../../../Data/Client.hpp"
#include "../../../../Data/Response.hpp"

void RedirectionStrategy::process(Client *client, Epoll &epoll, Process &process)
{
    Response *res = client->getResponse();
    Request *req = client->getRequest();
    res->setStatus(client->getRequest()->getLocation().ret.code);
    res->addHeader("Location", req->getLocation().ret.target);
    (void)client;
    (void)epoll;
    (void)process;
}