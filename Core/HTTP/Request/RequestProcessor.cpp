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

LocationType RequestProcessor::detectStategyType(Client *client)
{
    std::string fullpath;
	Request *req = client->getRequest();
	LocationType type = req->getLocation().type;
	if (type == STATIC || type == DYNAMIC)
	{    
		fullpath = client->getEndpoint().root + client->getRequest()->getPathname();
		req->setFullPath(fullpath);
		if (PathUtils::isDirectory(fullpath))
		{
			if (client->getRequest()->getLocation().index.empty())
			{
				std::cout << "Directory without index file: " << fullpath << std::endl;
				if (!client->getRequest()->getLocation().auto_index)
					throw NotFound();
				else
					return (DIRECTORY);
			}
			else
			{
				fullpath =  "http://" +req->getHeaderBykey("host")  + req->getPathname() + "/" + req->getLocation().index;
				req->setRedirectionPath(fullpath);
				return (REDIRECTION);
			}
		}
		else if (!PathUtils::isPathExist(fullpath))
		{
			std::cout << "Path does not exist: " << fullpath << std::endl;
			throw NotFound();
		}
	}
	return (type);
}

void RequestProcessor::processRequest(Client *client)
{
	LocationType 		type = detectStategyType(client);
	IRequestStrategy	*strategy = createStrategy(type);
	strategy->process(client);
	delete strategy;
	// if (client->getRequest())
	// 	delete  client->getRequest();
	// if (client->getResponse())
	// 	delete  client->getResponse();
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

