#include "MethodParser.hpp"
#include "../../Settings/Config.hpp"
#include "../../../Exception/BadRequestException.hpp"

MethodParser::MethodParser(Request *target, UnitConf_t	 endpoint):ARequestParserState(METHOD, target, endpoint), _i(0){

};
MethodParser::~MethodParser(){
};

HttpMethod MethodParser::detectMethod(std::string &value) const
{
	if (value == "GET")
		return (GET);
	else if (value == "POST")
		return (POST);
	return (DELETE);
}

void MethodParser::execute()
{
	
	std::cout << "...MethodParser executing..." << std::endl;
	std::vector<std::string>::iterator it;

	for (size_t i = _target->getParserIndex(); i < _target->getBuffer().size(); i++)
	{
		it = _endpoint.method_arr.begin();
		while (it != _endpoint.method_arr.end())
		{
			if ((*it)[_i] == '\0' && _target->getBuffer()[i] == ' ' && _endpoint.method_arr.size() == 1)
			{
				std::cout << "Accepted method-> " << (*it) << std::endl;
				_target->setMethod(detectMethod(*it));
				skipSeparator();
				return;
			}
			if ((*it)[_i] == _target->getBuffer()[i])
				++it;
			else
			{
				it = _endpoint.method_arr.erase(it);
			}
			if (!_endpoint.method_arr.size())
				throw BadRequestException();
		}
		_target->incrementParserIndex();
		_i++;
	}
	_target->resetParserIndex();
	throw EagainParser();
	std::cout << "...................." << std::endl;
}

