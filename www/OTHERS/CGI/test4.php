#!/usr/bin/php
<?php
$body = file_get_contents('php://stdin');

// Output proper HTTP headers FIRST
header("Content-Type: text/html");
header("Content-Length: " . strlen($body));

// Then output content
?>
<!DOCTYPE html>
<html>
<head><title>POST Test</title></head>
<body>
    <h1>POST Test</h1>
    <p>Body: <?php echo htmlspecialchars($body); ?></p>
    <p>Length: <?php echo strlen($body); ?></p>
</body>
</html>
