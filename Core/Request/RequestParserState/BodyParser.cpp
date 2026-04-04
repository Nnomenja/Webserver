#include "./BodyParser.hpp"
#include "../../../Exception/BadRequestException.hpp"
#include "../../../Exception/PayloadTooLarge.hpp"

BodyParser::BodyParser(Request *target, UnitConf_t endpoint):ARequestParserState(BODY, target, endpoint) ,_end(false), _chunkState(CHUNK_SIZE), _chunkBytesRemaining(0), _chunkCR(false){
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

long	BodyParser::parseChunkSize(const std::string &line) const
{
    std::string	metadata;
    long		chunkSize;
    size_t		separator;
    int		digit;

    separator = line.find(';');
    metadata = line.substr(0, separator);
    if (!metadata.size())
        throw BadRequestException();
    chunkSize = 0;
    for (size_t i = 0; i < metadata.size(); i++)
    {
        if (metadata[i] >= '0' && metadata[i] <= '9')
            digit = metadata[i] - '0';
        else if (metadata[i] >= 'a' && metadata[i] <= 'f')
            digit = metadata[i] - 'a' + 10;
        else if (metadata[i] >= 'A' && metadata[i] <= 'F')
            digit = metadata[i] - 'A' + 10;
        else
            throw BadRequestException();
        if (chunkSize > (LONG_MAX - digit) / 16)
            throw BadRequestException();
        chunkSize = (chunkSize * 16) + digit;
    }
    if (chunkSize > _endpoint.max_body_size - static_cast<long>(_target->getBody().size()))
        throw PayloadTooLarge();
    return (chunkSize);
}

void	BodyParser::handleChunkSizeLineChar(char c)
{
    if (!_chunkCR)
    {
        if (c == '\r')
            _chunkCR = true;
        else if (c == '\n')
        {
            _chunkBytesRemaining = parseChunkSize(_chunkSizeLine);
            _chunkSizeLine.clear();
            if (_chunkBytesRemaining == 0)
                _chunkState = CHUNK_FINAL_END;
            else
                _chunkState = CHUNK_DATA;
        }
        else
            _chunkSizeLine.push_back(c);
    }
    else
    {
        if (c != '\n')
            throw BadRequestException();
        _chunkCR = false;
        _chunkBytesRemaining = parseChunkSize(_chunkSizeLine);
        _chunkSizeLine.clear();
        if (_chunkBytesRemaining == 0)
            _chunkState = CHUNK_FINAL_END;
        else
            _chunkState = CHUNK_DATA;
    }
}

void	BodyParser::handleChunkDataDelimiterChar(char c)
{
    if (!_chunkCR)
    {
        if (c == '\r')
            _chunkCR = true;
        else if (c == '\n')
            _chunkState = CHUNK_SIZE;
        else
            throw BadRequestException();
    }
    else
    {
        if (c != '\n')
            throw BadRequestException();
        _chunkCR = false;
        _chunkState = CHUNK_SIZE;
    }
}

void	BodyParser::handleFinalChunkDelimiterChar(char c)
{
    if (!_chunkCR)
    {
        if (c == '\r')
            _chunkCR = true;
        else if (c == '\n')
            _end = true;
        else
            throw BadRequestException();
    }
    else
    {
        if (c != '\n')
            throw BadRequestException();
        _chunkCR = false;
        _end = true;
    }
}

/**==============================================
 * todo                  TODO
 *   - read body as chuck 
 *   - verify if the size exceeds the limit.
 *
 *=============================================**/
void    BodyParser::readBodyAsChuncked()
{
    char	c;

    for (size_t i = _target->getParserIndex(); i < _target->getBufferSize(); i++)
    {
        c = _target->getBuffer()[i];
        switch (_chunkState)
        {
            case CHUNK_SIZE:
                handleChunkSizeLineChar(c);
                break;
            case CHUNK_DATA:
                _target->addBody(c);
                _chunkBytesRemaining--;
                if (_chunkBytesRemaining == 0)
                    _chunkState = CHUNK_DATA_END;
                break;
            case CHUNK_DATA_END:
                handleChunkDataDelimiterChar(c);
                break;
            case CHUNK_FINAL_END:
                handleFinalChunkDelimiterChar(c);
                break;
            default:
                throw BadRequestException();
        }
        _target->incrementParserIndex();
        if (_end)
        {
            std::cout << "***************BODY***********************" << std::endl;
            std::cout << _target->getBody() << std::endl; 
            std::cout << "*****************************************" << std::endl;
            return;
        }
    }
    _target->resetParserIndex();
    throw EagainParser();
}

void BodyParser::execute()
{
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
