#include "PathUtils.hpp"

std::string PathUtils::normalizePathname(std::string pathname)
{
    int									i;
	std::string							tmp;
	std::stringstream					segment;
	std::stringstream					res;
	std::vector<std::string>			el;
	std::vector<std::string>::iterator	it;

	i = 0;
	if (pathname.length() == 1)
		return (pathname);
	while (pathname[i])
	{
		if (pathname[i] != '/')
			segment << pathname[i];
		if ((pathname[i] != '/' && pathname[i + 1] == '/') || !pathname[i + 1])
		{
				tmp = segment.str();
				if (tmp.length())
				{
					if (tmp == "..")
					{
						if (!el.empty())
							el.pop_back();
					}
					else if (tmp != ".")
					{
						el.push_back(tmp);
					}
					segment.clear();
					segment.str("");
				}
		}
		i++;
	}
	res << "/";
	if (el.size())
	{
		it = el.begin();
		while (it != el.end())
		{
			res << (*it);
			it++;
			if (it != el.end())
				res << "/";
		}
	}
	return (res.str());
}

bool PathUtils::isPathExist(const std::string& path)
{
    if (path.empty())
        return (false);
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
        return (false);
    if (!S_ISREG(st.st_mode))
        return (false);
    return (true);
}

bool PathUtils::isTraversableDir(const std::string& path)
{
    return (access(path.c_str(), X_OK) == 0);
}

bool PathUtils::isFileReadable(const std::string& path)
{
    return (access(path.c_str(), R_OK) == 0);
}

bool PathUtils::isDirectory(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) == -1)
        return (false);
    return (S_ISDIR(st.st_mode));
}


std::string PathUtils::getFileContentbypath(std::string pathname)
{
    std::ifstream file(pathname.c_str());
    std::string line;
    std::string fileContent = "";
    if (!file.is_open())
        throw std::exception();
    while (std::getline(file, line))
    {
        fileContent  += line + "\n";
    }
    file.close();
    return (fileContent);
}
