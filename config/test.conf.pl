#STATIC
server:
    port 2000
    host 0.0.0.0
    root  /home/toloandr/Music/test/www/uploadMultipartOneFile
    location:
        path /
        #root  
        methods GET POST
        error_pages 404 /home/toloandr/Music/test/www/error/404.html
        error_pages 400 /home/toloandr/Music/test/www/error/404.html
        # root  /home/toloandr/Music/test/www/uploadMultipartOneFile
        auto_index on
        upload_store /home/toloandr/Music/test/www/uploadMultipartOneFile/uploads
#REDIRECTION
server:
    port 2001
    host 0.0.0.0
    # root /home/toloandr/Music/test/www/uploadMultipartOneFile
    # location:
    #     path /
    #     #root  
    #     methods POST
    #     error_pages 404 /home/toloandr/Music/test/www/error/404.html
    #     error_pages 400 /home/toloandr/Music/test/www/error/404.html
    #     index index.html
    location:
        path /redirection
        return 302 https://google.com
#CGI
server:
    port 2003
    host 0.0.0.0
    # root /home/toloandr/Music/test/www/uploadMultipartOneFile 
    location:
        path /
        root /home/toloandr/Music/test/www/CGI
        methods GET POST
        error_pages 404 /error/404.html
        error_pages 400 /error/400.html
        CGI .php /usr/bin/php-cgi
        index index.php