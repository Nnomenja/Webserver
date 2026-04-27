server:
    port 2000
    host 127.1.1.1
    root /home/toloandr/Music/Webserver/websites/website00
    location:
        path /
        root /home/toloandr/Music/Webserver/websites/website00
        auto_index off
        methods GET
#         error_pages 404 /error/404.html
#         error_pages 400 /error/400.html
    location:
       path /t
#     #    root test
       
       index index.html
       methods GET POST
#     location:
#        path /up
#        methods POST
#        root /home/toloandr/Music/Webserver_implement/websites/website00
#        upload_store /home/toloandr/Music/Webserver_implement/websites/website00/uploads
#     #location:
#     #    path /redirection
#     #    return 302 https://google.com
#     #location:
#     #    path /cgi
#     #    root test
#     #    methods GET POST
#     #    error_pages 400 401 /4xx.html
#     #    error_pages 500 501 /5xx.html
#     #    CGI .py /usr/bin/python3




# #CGI
server:
    port 2001
    host 0.0.0.0
    root /home/toloandr/Music/Webserver/websites/website00
    location:
        path /cgi
        root /home/toloandr/Music/Webserver/websites/website00
        error_pages 404 /error/404.html
        error_pages 400 /error/400.html
        CGI .py /usr/bin/python3
        methods GET POST
    location:
        path redirection
        return 302 https://google.com
# server:
#     port 2002
#     host 0.0.0.0
#     root /home/toloandr/Music/Webserver_implement/websites/website00
#     location:
#         path /up
#         root /home/toloandr/Music/Webserver_implement/websites/website00
#         methods GET POST
#         error_pages 404 /error/404.html
#         error_pages 400 /error/400.html
#         CGI .py /usr/bin/python3
server:
    port 2003
    host 0.0.0.0
    root /home/toloandr/Music/Webserver/websites/website00
    location:
        path /cgi
        root /home/toloandr/Music/Webserver/websites/website00
        error_pages 404 /error/404.html
        error_pages 400 /error/400.html
        CGI .py /usr/bin/python3
        methods GET POST
