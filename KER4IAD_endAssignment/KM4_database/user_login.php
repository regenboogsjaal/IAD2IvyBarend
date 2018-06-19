<?php
include "connect.php";

session_start();

$user_id = $_POST["uname"];
$password = $_POST["psw"];

echo $query = "SELECT id FROM users WHERE password = '$password' AND username = '$user_id'";

if (!($result = $mysqli->query($query))) showerror($mysqli->errno, $mysqli->error); //uitvoeren query/error
$row = $result->fetch_assoc();

echo $row["id"];

if ($row["id"] != 0)
{
    $_SESSION["uname"] = $row["id"];
    header("location: things_overzicht.php");
} else 
{
    echo "login failed";
}







?>