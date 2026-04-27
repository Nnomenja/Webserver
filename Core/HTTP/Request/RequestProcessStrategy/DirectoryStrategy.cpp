#include "DirectoryStrategy.hpp"
#include "../../../../Data/Client.hpp"
#include "../../../../Data/Response.hpp"
#include "../../../../Exception/Forbiden.hpp"
#include "../../../../utils/PathUtils.hpp"
#include "../../Template.hpp"


void DirectoryStrategy::process(Client *client, Epoll &epoll, Process &process)
{
    DIR*                    dir;
    Request                 *req;
    std::string             root_dir;
    struct dirent*          entry;
    std::vector<t_dirEntry> data;
    t_dirEntry              tmp;
    std::string             element;
    std::string             tmp_path;

    req = client->getRequest();
    dir = opendir(req->getFullPath().c_str());
    root_dir =  req->getRootDir() + req->getLocation().path;
    if (dir == NULL)
        throw Forbiden();


    // back
    tmp.name = "..";
    tmp.isDirectory = true;
    if (req->getPathname() == req->getLocation().path)
        tmp.path = "";
    else
        tmp.path = "http://" + req->getHeaderBykey("host") + req->getPathname() + "/..";
    data.push_back(tmp);

    while (true)
    {
        entry = readdir(dir);
        if (!entry)
            break;
        if (entry->d_name[0] == '.' && entry->d_name[1] == '\0')
            continue;
        element = std::string(entry->d_name); 
        if (element == "..")
            continue;
        else
        {
            tmp_path = req->getFullPath() + "/" + element;
            tmp.name = element;
            tmp.isDirectory =  PathUtils::isDirectory(tmp_path);
            if (tmp.isDirectory && !PathUtils::isTraversableDir(tmp_path))
                tmp.path = "";
            else if (!tmp.isDirectory && !PathUtils::isFileReadable(tmp_path))
                tmp.path = "";
            else
                tmp.path = "http://" + req->getHeaderBykey("host") + req->getPathname() + "/" + element;
        }
        data.push_back(tmp);
    }

    client->getResponse()->setStatus(200);
    client->getResponse()->setBody(Template::DirectoryListing(data));
    client->getResponse()->addHeader("Content-Type", "text/html; charset=UTF-8");
    (void)client;
    (void)epoll;
    (void)process;
}