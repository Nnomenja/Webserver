#!/usr/bin/php
<?php
$body = file_get_contents('php://stdin');
echo "Content-Type: text/html\n\n";
echo "<h1>POST Test</h1>";
echo "<p>Body: " . htmlspecialchars($body) . "</p>";
echo "<p>Length: " . strlen($body) . "</p>";
?>
