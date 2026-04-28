#!/usr/bin/php-cgi
<?php
header("Content-Type: text/html; charset=UTF-8");

// Get or update mood
$mood = $_COOKIE['mood'] ?? "ghost";

if (isset($_GET['set'])) {
    $mood = $_GET['set'];
    setcookie("mood", $mood, time() + 3600);
}

// Visual mood map
$moods = [
    "happy"   => ["emoji" => "😄", "color" => "#FFD93D", "bg" => "linear-gradient(135deg,#ffe259,#ffa751)"],
    "sad"     => ["emoji" => "🌧️", "color" => "#6FA3FF", "bg" => "linear-gradient(135deg,#4facfe,#00f2fe)"],
    "curious" => ["emoji" => "🧠", "color" => "#B983FF", "bg" => "linear-gradient(135deg,#8e2de2,#4a00e0)"],
    "calm"    => ["emoji" => "🌿", "color" => "#7DCE82", "bg" => "linear-gradient(135deg,#a8e063,#56ab2f)"],
    "ghost"   => ["emoji" => "👻", "color" => "#ffffff", "bg" => "linear-gradient(135deg,#232526,#414345)"]
];

$data = $moods[$mood] ?? $moods["ghost"];
?>

<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Mood Universe</title>

<style>
body {
    margin: 0;
    font-family: "Arial Black", sans-serif;
    background: <?= $data['bg'] ?>;
    color: white;
    text-align: center;
    overflow-x: hidden;
}

.title {
    font-size: 34px;
    margin-top: 40px;
    text-shadow: 2px 2px 10px rgba(0,0,0,0.4);
}

.card {
    margin: 40px auto;
    width: 340px;
    padding: 25px;
    border-radius: 25px;
    background: rgba(255,255,255,0.15);
    box-shadow: 0 12px 30px rgba(0,0,0,0.3);
    backdrop-filter: blur(10px);
}

.big-emoji {
    font-size: 90px;
    animation: bounce 1.5s infinite;
}

@keyframes bounce {
    0%,100% { transform: translateY(0); }
    50% { transform: translateY(-10px); }
}

.mood-text {
    font-size: 20px;
    margin-top: 10px;
}

.buttons {
    margin-top: 15px;
}

a {
    display: inline-block;
    margin: 6px;
    padding: 10px 12px;
    border-radius: 14px;
    text-decoration: none;
    font-size: 18px;
    background: rgba(0,0,0,0.25);
    color: white;
    transition: 0.2s;
}

a:hover {
    transform: scale(1.1);
    background: rgba(255,255,255,0.3);
}

.floating {
    position: absolute;
    font-size: 22px;
    animation: float 6s infinite linear;
    opacity: 0.6;
}

@keyframes float {
    0% { transform: translateY(100vh) translateX(0); }
    100% { transform: translateY(-10vh) translateX(40px); }
}
</style>
</head>

<body>

<div class="title">🌈 Mood Universe CGI</div>

<div class="card">
    <div class="big-emoji"><?= $data['emoji'] ?></div>

    <div class="mood-text">
        You are feeling: <b><?= htmlspecialchars($mood) ?></b>
    </div>

    <div class="buttons">
        <a href="?set=happy">😄 Happy</a>
        <a href="?set=sad">🌧️ Sad</a>
        <a href="?set=curious">🧠 Curious</a>
        <a href="?set=calm">🌿 Calm</a>
    </div>
</div>

<!-- Floating emojis background -->
<div class="floating" style="left:10%;">✨</div>
<div class="floating" style="left:25%;">🍪</div>
<div class="floating" style="left:40%;">🌈</div>
<div class="floating" style="left:65%;">💫</div>
<div class="floating" style="left:80%;">🎈</div>

</body>
</html>