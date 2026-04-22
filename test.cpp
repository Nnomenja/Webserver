#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <map>

// void    foo(std::string s)
// {
//     std::cout << s.size() << std::endl;
// }

// int main(int ac, char const *av[])
// {
//    int fd = open(av[1], O_RDONLY);
//    if (fd < 0)
//         std::cout << "can't open file" << std::endl;
//     char buff[4];
//     bzero(buff, sizeof(char)  * 4);
//     size_t n;
//     int i = 3;
//     while (i--)
//     {
//         n = read(fd, buff, 4);            
//         std::cout << "n:" << n << std::endl;
//         std::cout <<  buff << std::endl;
//         buff[4] = 0;
//         bzero(buff, sizeof(char)  * 4);
//         if (n < 4 || 0)
//             break;
//     }
//     close(fd);
//     return 0;
// }

void    foo(std::map<int, int>  &m)
{
    m[9] = 3;
    std::cout << "Foo: " << m.size() << std::endl;
}

int main(int argc, char const *argv[])
{
    std::map<int, int> m;

    foo(m);
    std::cout << "main: " << m.size() << std::endl;
    return 0;
}
