#!/bin/bash
# test.cgi - minimal CGI script that outputs HTML

sleep 12

echo "Content-Type: text/html"
echo "Status: 200 OK"
echo
echo "<html><body>"
echo "<h1>CGI Test</h1>"
echo "<p>Request method: $REQUEST_METHOD</p>"
echo "<p>Query string: $QUERY_STRING</p>"
echo "</body></html>"