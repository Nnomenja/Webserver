#include "Encoding.hpp"

Encoding::Encoding(){};
Encoding::~Encoding(){};



bool    Encoding::isHexa(char c)
{
	return (std::isxdigit(static_cast<unsigned char>(c)));
}

int Encoding::HexaToInt(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}

char Encoding::PourcentHexaToChar(char first, char second)
{
	int		msb;
	int		lsb;

	msb = Encoding::HexaToInt(first);
	lsb = Encoding::HexaToInt(second);
	if (msb == -1 || lsb == -1)
		throw BadRequestException();
	return (static_cast<unsigned char>((msb << 4 | lsb)));
}