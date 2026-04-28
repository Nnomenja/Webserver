#!/usr/bin/php-cgi
<?php
// Method 1: Try different ways to read stdin
$post_body = '';

// Try reading from stdin directly
$stdin = fopen('php://stdin', 'r');
if ($stdin) {
    while ($line = fgets($stdin)) {
        $post_body .= $line;
    }
    fclose($stdin);
}

// If that didn't work, try file_get_contents
if (empty($post_body)) {
    $post_body = file_get_contents('php://stdin');
}

// If still empty, try reading from HTTP_RAW_POST_DATA (deprecated but sometimes works)
if (empty($post_body) && isset($GLOBALS['HTTP_RAW_POST_DATA'])) {
    $post_body = $GLOBALS['HTTP_RAW_POST_DATA'];
}

header("Content-Type: text/html");
?>
<!DOCTYPE html>
<html>
<head>
    <title>POST Test</title>
</head>
<body>
    <h1>POST Request Test</h1>
    
    <h2>Request Information</h2>
    <ul>
        <li>Method: <strong><?php echo $_SERVER['REQUEST_METHOD']; ?></strong></li>
        <li>Content-Type: <strong><?php echo isset($_SERVER['CONTENT_TYPE']) ? $_SERVER['CONTENT_TYPE'] : ''; ?></strong></li>
        <li>Content-Length (from server): <strong><?php echo isset($_SERVER['CONTENT_LENGTH']) ? $_SERVER['CONTENT_LENGTH'] : '0'; ?></strong></li>
        <li>Content-Length (actual): <strong><?php echo strlen($post_body); ?></strong></li>
    </ul>
    
    <h2>POST Data Received:</h2>
    <pre><?php echo htmlspecialchars($post_body ?: '(empty)'); ?></pre>
    
    <h2>Debug Info:</h2>
    <pre>
Method used to read: <?php echo $post_body ? 'Success' : 'Failed'; ?>
</pre>
</body>
</html>
