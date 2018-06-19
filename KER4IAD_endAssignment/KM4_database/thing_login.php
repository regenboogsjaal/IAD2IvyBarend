<?php
include "connect.php";

session_start();

$thing_id = $_GET["thing_id"];
$password = $_GET["pw"];

$query = "SELECT id FROM things WHERE password = '$password' AND id = $thing_id";

if (!($result = $mysqli->query($query))) showerror($mysqli->errno, $mysqli->error); //uitvoeren query/error
$row = $result->fetch_assoc();


if ($thing_id == $row["id"])
{
    $_SESSION["thing_id"] = $row["id"];
    echo session_id();
} else 
{
    echo "login failed";
}







?>