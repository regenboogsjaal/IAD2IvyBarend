<?php

include "connect.php";

$thing = $_GET["thingID"];
$payment = $_GET["payment"];
$location = $_GET["location"];

$paymentInsertQuery = "INSERT INTO `Payments` ( `thing_id` , `payment` , `location`, `time` ) VALUES ( $thing, $payment, '$location', now() ) "; //-

if (!($result = $mysqli->query($paymentInsertQuery))) showerror($mysqli->errno, $mysqli->error); //-

//budget has changed
//match thing id with user id

$getUserQuery = "SELECT user_id FROM Things WHERE id = $thing";
if (!($result = $mysqli->query($getUserQuery))) showerror($mysqli->errno, $mysqli->error);
$row = $result->fetch_assoc();

$userID = $row["user_id"];

//get the latest budget
$getBudgetQuery = "SELECT budget FROM Budgets WHERE user_id = $userID";
if (!($result = $mysqli->query($getBudgetQuery))) showerror($mysqli->errno, $mysqli->error);
$row = $result->fetch_assoc();
$budget = $row["budget"];

$newBudget = $budget - $payment;

//now change budget in Budgets

$updateBudgetQuery = "UPDATE Budgets SET budget = $budget - $payment WHERE user_id = $userID";
if (!($result = $mysqli->query($updateBudgetQuery))) showerror($mysqli->errno, $mysqli->error);
?>