<?php
include "connect.php";
//subtract a day from days_left
$updateDaysQuery = "UPDATE Budgets SET days_left = days_left - 1";
if (!($result = $mysqli->query($updateDaysQuery))) showerror($mysqli->errno, $mysqli->error);

//get original budget and current budget with matching user id's, where daysleft = 0
$getBudgetsQuery = "SELECT b.budget AS cur_budget, u.budget AS user_budget FROM Budgets b LEFT JOIN users u ON ( b.user_id = u.id ) WHERE b.days_left = 0";
if (!($result = $mysqli->query($getBudgetsQuery))) showerror($mysqli->errno, $mysqli->error);
$budgetsRow = $result->fetch_assoc();

$cur_budget = $budgetsRow["cur_budget"];
$user_budget = $budgetsRow["user_budget"];
echo $budgetsRow["cur_budget"]." ".$budgetsRow["user_budget"];

do 
{
    $newBudget = $budgetsRow["cur_budget"] + $budgetsRow["user_budget"];
    echo $newBudget;
    
}
while ($row = $result -> fetch_assoc());

//reset days_left and budget
$resetQuery = "UPDATE Budgets SET days_left = 7, budget = $newBudget WHERE days_left = 0";
if (!($result = $mysqli->query($resetQuery))) showerror($mysqli->errno, $mysqli->error);



?>