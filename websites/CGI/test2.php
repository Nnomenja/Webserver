#!/usr/bin/php
<?php
// This works with PHP CLI (not just CGI)
$post_body = file_get_contents('php://stdin');

header("Content-Type: text/html");
?>
<!DOCTYPE html>
<html>
<body>
    <h1>POST Test</h1>
    <p>Content-Length: <?php echo strlen($post_body); ?></p>
    <p>Data: <?php echo htmlspecialchars($post_body); ?></p>
</body>
</html>
