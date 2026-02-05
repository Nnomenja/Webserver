#include <gtest/gtest.h>
#include "../Ft-read-test-file/Test.hpp"
#include <string>
#include <sstream>

typedef struct S_SRQ
{
    std::string contents;
    std::string method;
    std::string pathname;
    std::string httpVesion;
    int         port;
} T_SRQ;

typedef struct S_TRQ
{
    std::string contents;
} T_TRQ;


std::string RequestValidPath = "./File-teste/Valid/"; 
std::string RequestInValidPath = "./File-teste/Invalid/";

//===========

std::string BodyRequestValidPath = "./File-teste/Valid/body/"; 
std::string BodyRequestInValidPath = "./File-teste/Invalid/body/";

/**------------------------------------------------------------------------
 *                           PrintTo
 *------------------------------------------------------------------------**/

void PrintTo(const T_SRQ& param, std::ostream* os) {
    *os << "\n-----------------" << "------------------" << "\n";
    *os << param.contents << "\n";
    *os << "-----------------------------------";
    *os << "\033[0m" << std::endl;
}

void PrintTo(const T_TRQ& param, std::ostream* os) {
    *os << "\n-----------------" << "------------------" << "\n";
    *os << param.contents << "\n";
    *os << "-----------------------------------";
    *os << "\033[0m" << std::endl;
}


/**------------------------------------------------------------------------
 *                           TEST WITH PARAMS
 *------------------------------------------------------------------------**/

class RequestTestSuccess: public ::testing::TestWithParam<T_SRQ>{};
class RequestTestThrow: public ::testing::TestWithParam<T_TRQ>{};


/**------------------------------------------------------------------------
 *                           TEST_P
 *------------------------------------------------------------------------**/

TEST_P(RequestTestSuccess, Success) {
    std::string contents = GetParam().contents;
    Request req(contents, contents.length());
    EXPECT_NO_THROW({
        EXPECT_TRUE(req.getMethod() == GetParam().method);
        EXPECT_TRUE(req.getPathName() == GetParam().pathname);
        EXPECT_TRUE(req.getHTTPversion() == GetParam().httpVesion);
        EXPECT_TRUE(req.getPort() == GetParam().port);
    });
}


TEST_P(RequestTestThrow, BadRequest) {
    std::string params = GetParam().contents;
    EXPECT_THROW(Request req(params, params.length()), Request::BadRequest);
}

/**------------------------------------------------------------------------
 *                           INSTANTIATE_TEST_SUITE_P
 *------------------------------------------------------------------------**/

 
INSTANTIATE_TEST_SUITE_P(
    RequestTest,
    RequestTestSuccess,
    ::testing::Values(

        /**============================================
         *               METHOD
         *=============================================**/ 

        T_SRQ{
            readTestFile(RequestValidPath + "method_0.txt"),
            "GET",
            "/",
            "1.1",
            8080
        },
        T_SRQ{
            readTestFile(RequestValidPath + "method_1.txt"),
            "POST",
            "/",
            "1.1",
            8080
        },
        T_SRQ{
            readTestFile(RequestValidPath + "method_2.txt"),
            "DELETE",
            "/",
            "1.1",
            8080
        },
        
        /**============================================
         *               PATHNAME
         *=============================================**/

            /*------- Normal path -------*/    
            T_SRQ{
                readTestFile(RequestValidPath + "path_1.txt"),
                "GET",
                "/path",
                "1.1",
                8080
            },
            /*------- With query -------*/    
            T_SRQ{
                readTestFile(RequestValidPath + "path_2.txt"),
                "GET",
                "/path",
                "1.1",
                8080
            },
            /*------- Successive slash -------*/    
            T_SRQ{
                readTestFile(RequestValidPath + "path_3.txt"),
                 "GET",
                "/path/path2",
                "1.1",
                8080
            },
            /*------- Normalisation -------*/    
            T_SRQ{
                readTestFile(RequestValidPath + "path_4.txt"), 
                "GET",
                "/path",
                "1.1",
                8080
            },
            T_SRQ{
                readTestFile(RequestValidPath + "path_5.txt"),
                "GET",
                "/path",
                "1.1",
                8080
            },
            T_SRQ{
                readTestFile(RequestValidPath + "path_6.txt"),
                "GET",
                "/path",
                "1.1",
                8080
            },
            T_SRQ{
                readTestFile(RequestValidPath + "path_7.txt"),
                "GET",
                "/path/path",
                "1.1",
                8080
            },
            T_SRQ{
                readTestFile(RequestValidPath + "path_8.txt"),
                "GET",
                "/path3/path4",
                "1.1",
                8080
            },
            /*------- Pourcent format Encode -------*/    
            T_SRQ{
                readTestFile(RequestValidPath + "path_9.txt"),
                "GET",
                "/path#path",
                "1.1",
                8080
            },
        /**============================================
         *               Header
         *=============================================**/
        T_SRQ{
            readTestFile(RequestValidPath + "header_00.txt"),
            "GET",
            "/path",
            "1.1",
            8080
        },
        T_SRQ{
            readTestFile(RequestValidPath + "header_01.txt"),
            "GET",
            "/path",
            "1.1",
            8080
        },
        T_SRQ{
            readTestFile(RequestValidPath + "header_02.txt"),
            "GET",
            "/path",
            "1.1",
            8080
        }
    )
);


INSTANTIATE_TEST_SUITE_P(
    RequestTest,
    RequestTestThrow,
    ::testing::Values(
        /**============================================
         *               REQUEST LINE
         *=============================================**/
        T_TRQ{
            readTestFile(RequestInValidPath + "requestLine_00.txt")
        },
        T_TRQ{
            readTestFile(RequestInValidPath + "requestLine_01.txt")
        },
        T_TRQ{
            readTestFile(RequestInValidPath + "requestLine_02.txt")
        },
        T_TRQ{
            readTestFile(RequestInValidPath + "requestLine_03.txt")
        },
        /**============================================
         *               METHOD
         *=============================================**/
        T_TRQ{
            readTestFile(RequestInValidPath + "method_00.txt")
        },
        T_TRQ{
            readTestFile(RequestInValidPath + "method_01.txt")
        },
        T_TRQ{
            readTestFile(RequestInValidPath + "method_02.txt")
        },
        /**============================================
         *               PATH
         *=============================================**/
            
            /*------- Not Encoded -------*/
            T_TRQ{
                readTestFile(RequestInValidPath + "path_00.txt")
            },
            T_TRQ{
                readTestFile(RequestInValidPath + "path_01.txt")
            },
            T_TRQ{
                readTestFile(RequestInValidPath + "path_02.txt")
            },
            T_TRQ{
                readTestFile(RequestInValidPath + "path_03.txt")
            },
            T_TRQ{
                readTestFile(RequestInValidPath + "path_04.txt")
            },
        /**============================================
         *               Protocol
         *=============================================**/
        T_TRQ{
            readTestFile(RequestInValidPath + "protocol_00.txt")
        },
        T_TRQ{
            readTestFile(RequestInValidPath + "protocol_01.txt")
        },
        T_TRQ{
            readTestFile(RequestInValidPath + "protocol_02.txt")
        },
        /**============================================
         *               HTTP version
         *=============================================**/
        T_TRQ{
            readTestFile(RequestInValidPath + "httpVersion_00.txt")
        },
        T_TRQ{
            readTestFile(RequestInValidPath + "httpVersion_01.txt")
        }
    )
);

