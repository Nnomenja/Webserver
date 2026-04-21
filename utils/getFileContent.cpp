
#include "utils.hpp" 

std::string getFileContent(std::string fileName)
{
    std::ifstream file(fileName.c_str());
    std::string line;
    std::string fileContent = "";
    if (!file.is_open())
    {
        throw std::runtime_error("Error: could not open configuration file.");
    }
    while (std::getline(file, line))
    {
        fileContent  += line + "\n";
    }
    file.close();
    return (fileContent);
}
