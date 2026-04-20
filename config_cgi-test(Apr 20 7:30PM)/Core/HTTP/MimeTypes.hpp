#ifndef MIMETYPES_HPP
#define MIMETYPES_HPP

#include <cstring>
#include <string>

// ---------------------------------------------------------------------------
// MimeTypes
//
// Resolves a Content-Type string from a file extension or filename.
// All data lives in MimeTypes.cpp — no file I/O, no dynamic allocation.
//
// Usage:
//   std::string ct = MimeTypes::fromExtension("html");       // "text/html"
//   std::string ct = MimeTypes::fromFilename("index.html");  // "text/html"
//   std::string ct = MimeTypes::fromFilename("photo.JPG");   // "image/jpeg"
//   std::string ct = MimeTypes::fromFilename("unknown.xyz"); // "application/octet-stream"
// ---------------------------------------------------------------------------

class MimeTypes
{
public:
    // Returns the MIME type for a bare extension (case-insensitive, no dot).
    // Falls back to "application/octet-stream" when not found.
    static std::string fromExtension(const std::string& ext);

    // Extracts the last extension from a filename or path, then calls
    // fromExtension().  "style.min.css" -> "css", "/srv/app.JS" -> "js".
    static std::string fromFilename(const std::string& filename);
    static bool checkMimeType(const std::string& mimeType);
private:
    MimeTypes(); // static-only class, not instantiable

    struct Entry { const char* ext; const char* mime; };

    static const Entry       TABLE[];
    static const std::size_t TABLE_SIZE;

    static std::string toLower(const std::string& s);
};

#endif