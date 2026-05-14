#HOMEPAGE
server:
    port 1234
    host 0.0.0.0
    root  [webserv path]/www/SHOWCASE/homePage
    # max_body_size 1000
    location:         
        path /
        methods GET
        error_pages 404 /error/404.html
        index index.html
    location:        
        root  [webserv path]/www/SHOWCASE 
        path /testing-lab
        methods GET
        error_pages 404 /error/404.html
        index index.html

#TESTING-LAB
server:
    port 1235
    host 0.0.0.0
    root  [webserv path]/www/SHOWCASE/testing-lab
    # max_body_size 1000
    location:         
        path /
        methods GET
        index index.html

#SITE
server:
    port 1236
    host 0.0.0.0
    root  [webserv path]/www/OTHERS/site
    # max_body_size 1000
    location:         
        path /
        methods GET
        index index.html


#UPLOAD TESTER
server:
    port 2006
    host 0.0.0.0
    root  [webserv path]/www/OTHERS/uploadTools
    # max_body_size 1000
    location:         
        path /
        methods GET POST
        error_pages 404 /error/404.html
        error_pages 400 /error/404.html
        index index.html

#STATIC
server:
    port 2005
    host 0.0.0.0
    root  [webserv path]/www/OTHERS/uploadMultipartOneFile
    # max_body_size 1000
    location:         
        path /
        methods GET POST
        error_pages 404 /error/404.html
        error_pages 400 /error/400.html
        index index.html
#DIRECTORY LISTING
server:
    port 2000
    host 0.0.0.0
    root  [webserv path]/www/OTHERS/uploadMultipartOneFile
    max_body_size 1000
    location:         
        path /
        methods GET POST
        error_pages 404 /error/404.html
        error_pages 400 /error/404.html
        auto_index on
        index index.html
#REDIRECTION
server:    
    port 2001
    host 0.0.0.0
    location:
        path /redirection
        return 302 https://google.com
#CGI
server:
    port 2003
    host 0.0.0.0
    location:
        path /
        root [webserv path]/www/OTHERS/CGI
        methods GET POST
        error_pages 404 /error/404.html
        error_pages 400 /error/400.html
        CGI .php /usr/bin/php-cgi
        index index.php
#UPLOADS
server:
    port 2004
    host 0.0.0.0
    root  [webserv path]/www/OTHERS/uploadMultipartOneFile
    error_pages 404 /errors/404.html
    # max_body_size 1048576
    location:         
        path /
        methods GET POST
        # error_pages 404 /error/404.html
        error_pages 400 /error/400.html
        auto_index on
        upload_store [webserv path]/www/OTHERS/uploadMultipartOneFile/uploads