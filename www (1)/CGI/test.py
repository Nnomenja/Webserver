#!/usr/bin/env python3

import os
import sys

print("Content-Type: text/html\n")

method = os.environ.get("REQUEST_METHOD", "")

print("<html><body>")
print(f"<h2>Method: {method}</h2>")

if method == "GET":
    query = os.environ.get("QUERY_STRING", "")
    print(f"<p>Query string: {query}</p>")

elif method == "POST":
    length = int(os.environ.get("CONTENT_LENGTH", 0))
    body = sys.stdin.read(length)
    print(f"<p>POST body: {body}</p>")

else:
    print("<p>Unknown method</p>")

print("</body></html>")
