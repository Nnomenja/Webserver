#include "RequestProcessor.hpp"
#include "StaticStrategy.hpp"
#include "DynamicStrategy.hpp"
#include "RedirectionStrategy.hpp"
#include "ErrorStrategy.hpp"
#include "DirectoryStrategy.hpp"

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

void RequestProcessor::processRequest(Client &client)
{
	IRequestStrategy	*strategy = createStrategy(client.getEndpoint().type);
	strategy->process(client);
	delete strategy;
}

IRequestStrategy *RequestProcessor::createStrategy(EndpointType type)
{
	switch (type)
	{
		case STATIC:
			return (new StaticStrategy());
			break;
		case DYNAMIC:
			return (new DynamicStrategy());
			break;
		case ERROR:
			return (new ErrorStrategy());
			break;
		case DIRECTORY:
			return (new DirectoryStrategy());
			break;
		default:
			return (new  RedirectionStrategy());
			break;
	}
}

