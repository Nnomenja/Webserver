#STATIC
server:
    port 2000
    host 0.0.0.0
    root /home/toloandr/SCHOOL/Webserver/websites/website00
    error_pages 404 /error/404.html
    location:
        path /
        methods GET POST
        error_pages 404 /error/404.html

        error_pages 400 /error/400.html

        auto_index on




#REDIRECTION
server:
    port 2001

    host 0.0.0.0

    root /home/toloandr/SCHOOL/Webserver/websites/website00

    location:
        path /redirection
        return 302 https://google.com



#CGI
server:
    port 2003
    host 0.0.0.0
    root /home/toloandr/SCHOOL/Webserver/websites/CGI
    location:
        path /
        root /home/toloandr/SCHOOL/Webserver/websites/CGI
        methods GET POST
        error_pages 404 /error/404.html
        error_pages 400 /error/400.html
        CGI .php /usr/bin/php-cgi
        index index.php