#include <string>
#include <vector>
#include <map>


/**============================================
 *               DATA CLASS
 *=============================================**/

 // client's config


 enum EndpointType {
    REDIRECTION,
    STATIC,
    DYNAMIC,
    DIRECTORY,
    ERROR
};

enum RequestParserState {
    METHOD = 100,
    URI,
    VERSION,
    BODY,
    COMPLETE
};


typedef struct UnitConf
{
    std::string     host;
    int             port;
    int             methods;
    EndpointType    type;
    // additionnal informations.
} UnitConf_t;



class Request
{

};

class Response
{

};


class Client
{
    private:
        Request             _req;
        Response            _res;
        UnitConf            _data_config;
        RequestParserState  _state; // used by HttpRequestParser in incremental parsing.
};

/**============================================
 *               SETTING
 *=============================================**/

class Config
{
    std::map<int, UnitConf_t> endpoints; // [id, value]
    public:
        void load(std::string &value);
        void getEndointByFd(int fd); // used by Socket
};

/**============================================
 *               NETWORK
 *=============================================**/

class Socket
{
    public:
        void    init(Config &config);
};


/**============================================
 *               HTTP Request
 *=============================================**/

class HttpRequestParser
{
    public:
        void    parse(Client &client, std::string buffer);
};


class HttpRequestProcessor
{
    private:
        IRequestStrategy   *createStrategy(Client &client); // detect client's strategy and create it.
    public:
        void    processRequest(Client &client); // create stategy and process request;
};

/**============================================
 *       HTTP Request Process Strategy
 *=============================================**/

class IRequestStrategy
{
    public:
        virtual void process() = 0;
};

class DirectoryStrategy: public IRequestStrategy
{
    public:
        void process();
    
};


class StaticStrategy: public IRequestStrategy
{
    public:
        void process();
    
};


class DynamicStrategy: public IRequestStrategy
{
    public:
        void process();
    
};

// ... more

/**============================================
 *               WRAPPER
 *=============================================**/

class Webserver
{
    private:
        Config                  _configs;
        std::map<int, Client>   _clients; // [id, data]
        Socket                  _socket;

        public:
        void    start(std::string &config_path);

};


int main(int ac, char *av[])
{
    if (ac != 2)
        //error;
    Webserver httpServer;

    httpServer.start(av[1]);
}