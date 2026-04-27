# include "./RequestProcessor.hpp"
# include "./RequestProcessStrategy/StaticStrategy.hpp"
# include "./RequestProcessStrategy/DynamicStrategy.hpp"
# include "./RequestProcessStrategy/RedirectionStrategy.hpp"
# include "./RequestProcessStrategy/DirectoryStrategy.hpp"
# include "./RequestProcessStrategy/UploadStrategy.hpp"

#include "../../../Data/Client.hpp"
#include "../../../Exception/NotFound.hpp"
#include "../../../Exception/Forbiden.hpp"
#include "../../../Exception/InternalServerError.hpp"
#include "../../../utils/PathUtils.hpp"
#include "../ErrorProcess.hpp"
RequestProcessor::RequestProcessor(){};

RequestProcessor::RequestProcessor(const RequestProcessor& other){
	*this = other;
  
};

RequestProcessor& RequestProcessor::operator=(const RequestProcessor& other){
	if (this == &other)
		return (*this);  
  
  return *this;
};

RequestProcessor::~RequestProcessor(){};


std::string RequestProcessor::findDefaultIndex(std::string &fullpath, std::vector<std::string> arr)
{
	std::string path;
	for (std::vector<std::string>::const_iterator it = arr.begin(); it != arr.end(); ++it)
	{
		path = fullpath + (*it);
		if (PathUtils::isPathExist(path))
		{
			fullpath = path;
			return (*it);
		}
	}
	return ("");
}

LocationType RequestProcessor::detectStategyType(Client *client)
{
    std::string fullpath = "";
	Request *req = client->getRequest();
	LocationType type = req->getLocation().type;
	std::string root = client->getRequest()->getLocation().root;
	if (root.empty())
		root = client->getEndpoint().root;
	if (type == STATIC || type == DYNAMIC)
	{    
		fullpath = root + client->getRequest()->getPathname();
		req->setFullPath(fullpath);
		if (PathUtils::isDirectory(fullpath))
		{
			std::string index = findDefaultIndex(fullpath, client->getRequest()->getLocation().index_vect);
			if (index.empty())
			{
				if (!client->getRequest()->getLocation().auto_index)
					throw NotFound();
				else
					return (DIRECTORY);
			}
			else
			{
				fullpath =  "http://" + req->getHeaderBykey("host")  + req->getPathname() + index;
				req->setRedirectionPath(fullpath, 302);
				return (REDIRECTION);
			}
		}
		else if (!PathUtils::isPathExist(fullpath))
		{
			std::cout << "Path does not exist: " << fullpath << std::endl;
			throw NotFound();
		}

		if (type == DYNAMIC)
		{
			std::string ext = req->getPathExtension();
			std::string cgi_bin = client->getCGIbinByExtension(ext);
			if (cgi_bin.empty())
				return (STATIC);
			client->setCGIbin(cgi_bin);
		}
	}
	return (type);
}

void RequestProcessor::processRequest(Client *client, Epoll &epoll, Process &process)
{
	LocationType 		type = detectStategyType(client);
	IRequestStrategy	*strategy = createStrategy(type);
	try
	{
		strategy->process(client, epoll, process);		
	}
	catch(const ServerException& e)
	{

		ErrorProcess::processError(e, client);
	}
	delete strategy;
}

IRequestStrategy *RequestProcessor::createStrategy(LocationType  type)
{
	switch (type)
	{
		case STATIC:
			return (new StaticStrategy());
			break;
		case DYNAMIC:
			return (new DynamicStrategy());
			break;
		case DIRECTORY:
			return (new DirectoryStrategy());
			break;
		case UPLOAD:
			return (new UploadStrategy());
		default:
			return (new  RedirectionStrategy());
			break;
	}
}

