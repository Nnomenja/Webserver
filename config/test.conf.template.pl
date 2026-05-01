#UPLOAD TESTER
server:
    port 2006
    host 0.0.0.0
    root  [webserver path]/www/uploadTools
    max_body_size 1000
    location:         
        path /
        methods GET POST
        error_pages 404 [webserver path]/www/error/404.html
        error_pages 400 [webserver path]/www/error/404.html
        index index.html

#STATIC
server:
    port 2005
    host 0.0.0.0
    root  [webserver path]/www/uploadMultipartOneFile
    max_body_size 1000
    location:         
        path /
        methods GET POST
        error_pages 404 [webserver path]/www/error/404.html
        error_pages 400 [webserver path]/www/error/404.html
        index index.html
#DIRECTORY LISTING
server:
    port 2000
    host 0.0.0.0
    root  [webserver path]/www/uploadMultipartOneFile
    max_body_size 1000
    location:         
        path /
        methods GET POST
        error_pages 404 [webserver path]/www/error/404.html
        error_pages 400 [webserver path]/www/error/404.html
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
        root [webserver path]/www/CGI
        methods GET POST
        error_pages 404 /error/404.html
        error_pages 400 /error/400.html
        CGI .php /usr/bin/php-cgi
        index index.php
#UPLOADS
server:
    port 2004
    host 0.0.0.0
    root  [webserver path]/www/uploadMultipartOneFile
    # max_body_size 1048576
    location:         
        path /
        methods GET POST
        error_pages 404 [webserver path]/www/error/404.html
        error_pages 400 [webserver path]/www/error/404.html
        auto_index on
        upload_store [webserver path]/www/uploadMultipartOneFile/uploads