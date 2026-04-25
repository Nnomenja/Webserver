#!/bin/bash
# test.cgi - minimal CGI script that outputs HTML


echo "Content-Type: text/html"
echo "Status: 200 OK"
echo "----"

sleep 4

echo "METHOD: $REQUEST_METHOD"
echo "PORT: $SERVER_PORT"
echo "QUERY: $QUERY_STRING"
echo "SCRIPT: $SCRIPT_NAME"
echo "PATH_INFO: $PATH_INFO"
echo "CONTENT_LENGTH: $CONTENT_LENGTH"
echo "CONTENT_TYPE: $CONTENT_TYPE"
echo "-----------------------------------BODY-----------------------------------"
    # Read the body of the request
# read -n "$CONTENT_LENGTH" POST_DATA
# echo "$POST_DATA"