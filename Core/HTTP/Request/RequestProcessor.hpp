#ifndef REQUESTPROCESSOR_HPP
#define REQUESTPROCESSOR_HPP

#include "./RequestProcessStrategy/IRequestStrategy.hpp"
#include "../../../Enum/LocationType.hpp"
#include <iostream>

class Client;
class RequestProcessor {
	private:
		IRequestStrategy*	createStrategy(LocationType type);
		LocationType		detectStategyType(Client *client);
	public:
		RequestProcessor();
		RequestProcessor(const RequestProcessor& other);
		RequestProcessor& operator=(const RequestProcessor& other);
		~RequestProcessor();

		/**
		 * @brief Process request and store the accoring data in client
		 */
		void processRequest(Client* client);
};

#endif
