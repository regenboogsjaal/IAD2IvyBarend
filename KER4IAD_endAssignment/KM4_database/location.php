<?php
include "connect.php";

$thingID = $_GET["thingID"];
$cur_location = $_GET["location"];

$query = "SELECT COUNT(*) FROM Locations WHERE name = '$cur_location' AND thing_id = $thingID";

//$locked = 0;

if (!($result = $mysqli->query($query))) showerror($mysqli->errno, $mysqli->error);

$row = $result->fetch_assoc();

echo "loc: ".$row["COUNT(*)"];

?>