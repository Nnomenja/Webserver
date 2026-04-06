#include "./RequestProcessor.hpp"
#include "./RequestProcessStrategy/StaticStrategy.hpp"
#include "./RequestProcessStrategy/DynamicStrategy.hpp"
#include "./RequestProcessStrategy/RedirectionStrategy.hpp"
#include "./RequestProcessStrategy/DirectoryStrategy.hpp"

#include "../../Data/Client.hpp"

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

void RequestProcessor::processRequest(Client *client)
{
	IRequestStrategy	*strategy = createStrategy(client->getRequest()->getLocation().type);
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
		default:
			return (new  RedirectionStrategy());
			break;
	}
}

