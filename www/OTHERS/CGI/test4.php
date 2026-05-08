#!/usr/bin/env php-cgi
<?php

header("Content-Type: text/html\r\n\r\n");

$method = getenv("REQUEST_METHOD");

echo "<html><body>";
echo "<h2>Method: $method</h2>";

if ($method === "GET") {
    $query = getenv("QUERY_STRING");
    echo "<p>Query string: $query</p>";
}

elseif ($method === "POST") {
    $length = (int)getenv("CONTENT_LENGTH");

    $body = "";
    if ($length > 0) {
        $body = file_get_contents("php://stdin");
        // optional strict version:
        $body = substr($body, 0, $length);
    }

    echo "<p>POST body: $body</p>";
}

else {
    echo "<p>Unknown method</p>";
}

echo "</body></html>";
?>