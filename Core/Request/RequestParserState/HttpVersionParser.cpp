#include "HttpVersionParser.hpp"
#include "../../../Exception/BadRequestException.hpp"

HttpVersionParser::HttpVersionParser(Request *target, UnitConf_t endpoint):ARequestParserState(VERSION, target, endpoint), _protocol_name("HTTP/"), protocol_readed(false), _j(0), _finished(false){
    _versions.push_back("1.0");
	_versions.push_back("1.1");
	_versions.push_back("2");
	_versions.push_back("3");
};
HttpVersionParser::~HttpVersionParser(){
};

void HttpVersionParser::execute()
{
	std::cout << "HttpVersionParser executing..." << std::endl;

    char c;
    std::vector<std::string>::iterator it;


    if (_finished)
    {
        skipCRLF();
        return;
    }
    for (size_t i = _target->getParserIndex(); i < _target->getBufferSize(); i++)
    {
        c = _target->getBuffer()[i];
        if (!protocol_readed)
        {
			if (_protocol_name[_j] == '\0'){
				protocol_readed = true;
				_j = 0;
				i--;
			}
			else
			{
				_j++;
				_target->incrementParserIndex();
			}
        }
        else
        {
            it = _versions.begin();
            while(it != _versions.end())
            {
                if ((*it)[_j] == c && (*it)[_j + 1] == '\0' && _versions.size() == 1)
                {
                    std::cout << "Http version Done: " << (*it) << " at: " << _target->getBuffer()[i] << std::endl;
                    _target->incrementParserIndex();
                    _finished = true;
                    if (!_target->getBuffer()[_target->getParserIndex()])
                    {
                        _target->resetParserIndex();
                        throw EagainParser();
                    }
                    skipCRLF();
                    return;
                }
                else if ((*it)[_j] != c)
				{
                    it = _versions.erase(it);
					if (!_versions.size())
						throw BadRequestException();
				}
                else
                    ++it;
            }
			_j++;
			_target->incrementParserIndex();
        }
    }
    _target->resetParserIndex();
    throw EagainParser();
	std::cout << "...................." << std::endl;

}
