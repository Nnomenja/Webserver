#include "./BodyParser.hpp"
#include "../../../Exception/BadRequestException.hpp"
#include "../../../Exception/PayloadTooLarge.hpp"

#define CONTENT_LENGTH_MAX 10485760
BodyParser::BodyParser(Request *target, UnitConf_t endpoint):ARequestParserState(BODY, target, endpoint) ,_end(false){
	_tmp = _target->getContentLength();
};
BodyParser::~BodyParser(){
};

void    BodyParser::readBodyThroughContentLength()
{
    char c;

    for (size_t i = _target->getParserIndex(); i < _target->getBufferSize(); i++)
    {
        c = _target->getBuffer()[i];
        _target->addBody(c);
        _target->incrementParserIndex();
        _tmp--;
        if (!_tmp)
        {
            _end = true;
            std::cout << "***************BODY***********************" << std::endl;
            std::cout << _target->getBody() << std::endl;
            std::cout << "*****************************************" << std::endl;
            skipCRLF();
			return;
        }
    }
    _target->resetParserIndex();
	throw EagainParser();
}

/**==============================================
 * todo                  TODO
 *   - read body as chuck 
 *   - verify if the size exceeds the limit.
 *
 *=============================================**/
void    BodyParser::readBodyAsChuncked()
{
	
}

void BodyParser::execute()
{
	std::cout << "BodyParser executing..." << std::endl;
	if (_target->getMethod() == GET)
		return;
	if (_target->getBodyEncode() == BODY_CONTENT_LENGTH)
	{
		if (!_end)
			readBodyThroughContentLength();
		else
			skipCRLF();
	}
	else
		readBodyAsChuncked();
}
