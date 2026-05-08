<?php

header('Content-Type: application/json');

$body = file_get_contents("php://input");

$response = [
    "success" => true,
    "method" => $_SERVER['REQUEST_METHOD'],
    "content_type" => $_SERVER['CONTENT_TYPE'] ?? null,
    "query" => $_GET,
    "post" => $_POST,
    "raw_body" => $body,
    "json" => json_decode($body, true),
    "server_time" => date('c'),
    "client_ip" => $_SERVER['REMOTE_ADDR'] ?? null,
];

echo json_encode($response, JSON_PRETTY_PRINT);