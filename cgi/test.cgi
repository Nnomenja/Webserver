#!/bin/bash
# test.cgi - minimal CGI script that outputs HTML


echo "Content-Type: text/html"
echo "Status: 200 OK"
echo "----"
echo "METHOD: $REQUEST_METHOD"
echo "PORT: $SERVER_PORT"
echo "QUERY: $QUERY_STRING"
echo "SCRIPT: $SCRIPT_NAME"
echo "PATH_INFO: $PATH_INFO"
echo "CONTENT_LENGTH: $CONTENT_LENGTH"
echo "CONTENT_TYPE: $CONTENT_TYPE"
echo ""echo "<html><body><h1>Hello from CGI!</h1></body></html>"