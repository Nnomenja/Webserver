server:

    #UPLOAD
    port 2000
    host 0.0.0.0
    # root 
    location:
        auto_index on
        path /upload
        methods GET POST DELETE
        # upload_store

    #STATIC
    location:
        path /
        methods GET
        # root 
        auto_index on
        index index.html

    #REDIRECTION
    location:
        path /redirection
        return 302 https://google.com

    #CGI
    location:
        path /CGI
        # root 
        methods GET POST
        error_pages 404 /error/404.html
        error_pages 400 /error/400.html
        CGI .php /usr/bin/php-cgi
        index index.php
        auto_index on