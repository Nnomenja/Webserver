#include "./Test.hpp"

std::string readTestFile(std::string path)
{
    std::stringstream ss;
    char tmp;
    std::ifstream file(path, std::ios::binary);  
    bool bs;

    bs = false;
    if (!file.is_open()) {
        std::cerr << "Could not open the file!: " << path << std::endl;
        throw std::exception();
    }
    while (file.get(tmp)) {
        if(tmp == '\\')
            bs = true;
        else if (bs)
        {
            if(tmp == '\\')
                ss << '\\';
            else if (tmp == 'r')
                ss << '\r';
            bs = false;
        }
        else
            ss << tmp;
    }
    file.close();
    return (ss.str());
}
