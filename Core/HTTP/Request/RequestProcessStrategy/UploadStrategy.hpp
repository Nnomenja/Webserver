/*
   File: UploadStrategy.hpp
   By: Azaria
   Created: 2026/04/08 16:13:50
*/

#ifndef UPLOADSTRATEGY_HPP
#define UPLOADSTRATEGY_HPP

# include "IRequestStrategy.hpp"
# include "../../../../Data/Client.hpp"
# include "../../../../Data/Request.hpp"


typedef struct SMultipartHeader
{
    std::string name;           // Nom du champ (ex: "file", "caption") - OBLIGATOIRE
    std::string filename;       // Nom du fichier (ex: "ft_lock.sh") - OBLIGATOIRE pour les fichiers
    std::string contentType;   // Type MIME (ex: "application/x-sh", "image/png") - OBLIGATOIRE
    std::string contentTransferEncoding; // Optionnel: "binary", "base64", "7bit", "8bit"
    std::string contentId;     // Optionnel: identifiant unique pour la partie
    std::string contentLocation; // Optionnel: URI où trouver le contenu
    std::map<std::string, std::string> customParams; // Pour extensions futures
    
} MultipartHeader;

enum BodyType
{
    NONE,
    DIRECT,
    MULTIPART,
    URLENCODED,
    UNKNOWN
};

class UploadStrategy : public IRequestStrategy
{
    public :
        UploadStrategy();
        ~UploadStrategy();

        void process(Client* client);

    private :

        BodyType bodyTypeDetection(Request* request);
        std::string creatUploadFileName(Request* request);

        void     handleDirectUpload(Client* client);
        void     handleMultipartUpload(Client* client);
};

#endif /* UPLOADSTRATEGY_HPP */

