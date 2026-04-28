#ifndef MIMETYPES_HPP
#define MIMETYPES_HPP

#include <cstring>
#include <string>

class MimeTypes
{
public:
    static std::string fromExtension(const std::string& ext);

    static std::string fromFilename(const std::string& filename);
    static bool checkMimeType(const std::string& mimeType);
private:
    MimeTypes();

    struct Entry { const char* ext; const char* mime; };

    static const Entry       TABLE[];
    static const std::size_t TABLE_SIZE;

    static std::string toLower(const std::string& s);
};

#endif