<?php
include "connect.php";

$thingID = $_GET["thingID"];

//get user ID with thing ID

$findUserQuery = "SELECT user_id FROM Things WHERE id = $thingID";

if (!($result = $mysqli->query($findUserQuery))) showerror($mysqli->errno, $mysqli->error);
$UserRow = $result->fetch_assoc();

$userID = $UserRow["user_id"];

//get user budget

$checkBudgetQuery = "SELECT budget FROM Budgets WHERE user_id = $userID";

if (!($result = $mysqli->query($checkBudgetQuery))) showerror($mysqli->errno, $mysqli->error);
$budgetRow = $result->fetch_assoc();

//return budget

echo "bgt: ".$budgetRow["budget"];


?>

