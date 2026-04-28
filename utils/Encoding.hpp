#ifndef ENCODING_HPP
#define ENCODING_HPP

#include "../Exception/BadRequestException.hpp"

class Encoding
{
    public:
        Encoding();
        ~Encoding();

        static bool    isHexa(char c);
        static int     HexaToInt(char c);
        static char    PourcentHexaToChar(char first, char second);

};

#endif