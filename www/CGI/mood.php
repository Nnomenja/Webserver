<?php
header("Content-Type: text/plain");

// Read previous mood from cookie
$lastMood = $_COOKIE['mood'] ?? "unknown visitor";

// Check if user is setting a new mood
$newMood = $_GET['set'] ?? null;

if ($newMood) {
    // Set cookie (must go before output in real HTTP mode)
    setcookie("mood", $newMood, time() + 3600);

    echo "🎉 Mood updated!\n";
    echo "You are now feeling: $newMood\n\n";
    echo "Come back and I will remember you.\n";
} else {
    echo "👋 Welcome back!\n";
    echo "I remember your last mood: $lastMood\n\n";
    echo "Try:\n";
    echo "?set=happy\n";
    echo "?set=sad\n";
    echo "?set=curious\n";
}
?>