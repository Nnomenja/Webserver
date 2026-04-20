#include "Config.hpp"

int main(int ac, char** av)
{
    if (ac != 2)
        return (1);
    try
    {
        Config config(av[1]);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return (0);
}