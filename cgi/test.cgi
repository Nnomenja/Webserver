#!/bin/bash
# test.cgi - minimal CGI script that outputs HTML

sleep 4

echo "Content-Type: text/html"
echo "Status: 200 OK"
echo "----"
echo "<html><body>"
echo "<h1>CGI Test</h1>"
echo "<p>Request method: POST</p>"
echo "<p>Query string: HELLo</p>"
echo "</body></html>"