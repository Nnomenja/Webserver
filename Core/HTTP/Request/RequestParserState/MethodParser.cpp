#include "MethodParser.hpp"
#include "../../../Settings/Config.hpp"
#include "../../../../Exception/BadRequestException.hpp"
#include "../../../../Exception/NotImplement.hpp"

MethodParser::MethodParser(Request *target, UnitConf_t	 endpoint):ARequestParserState(METHOD, target, endpoint), _i(0), _finished(false){
	_methods.push_back("GET");
	_methods.push_back("POST");
	_methods.push_back("PATH");
	_methods.push_back("PUT");
	_methods.push_back("DELETE");
	_methods.push_back("UPDATE");
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
	
	std::vector<std::string>::iterator it;
	HttpMethod method;

	if (_finished)
	{
		skipSeparator();
		return ;
	}
	for (size_t i = _target->getParserIndex(); i < _target->getBuffer().size(); i++)
	{
		it = _methods.begin();
		while (it != _methods.end())
		{
			if ((*it)[_i] == '\0' && _target->getBuffer()[i] == ' ' && _methods.size() == 1)
			{
				method = detectMethod(*it);
				if ((*it) != "GET" && (*it) != "POST" && (*it) != "DELETE")
					throw NotImplement();
				_target->setMethod(method);
				_finished = true;
				skipSeparator();
				return;
			}
			if ((*it)[_i] == _target->getBuffer()[i])
				++it;
			else
			{
				it = _methods.erase(it);
			}
			if (!_methods.size())
				throw BadRequestException();
		}
		_target->incrementParserIndex();
		_i++;
	}
	_target->resetParserIndex();
	throw EagainParser();
}
