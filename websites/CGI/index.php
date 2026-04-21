#!/usr/bin/php-cgi
<?php
header("Content-Type: text/html");
?>
<!DOCTYPE html>
<html>
<head><title>PHP CGI Test</title></head>
<body>
<h1>PHP CGI is Working!</h1>
<p>Method: <?php echo $_SERVER['REQUEST_METHOD']; ?></p>
<p>Query String: <?php echo $_SERVER['QUERY_STRING']; ?></p>
<p>Script Name: <?php echo $_SERVER['SCRIPT_NAME']; ?></p>
</body>
</html>