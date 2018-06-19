<?php
include "connect.php"; //-

$thingID = $_GET["thingID"];
$location = $_GET["location"];

$query = "SELECT COUNT(*) FROM Locations WHERE name = '$location' AND thing_id = $thingID";

//$locked = 0;

if (!($result = $mysqli->query($query))) showerror($mysqli->errno, $mysqli->error);

$row = $result->fetch_assoc();

if ($row["COUNT(*)"] == 1){
    echo "isl: ".$row["COUNT(*)"];
}
else {
   $insertQuery = "INSERT INTO `Locations` ( `thing_id`, `name` ) VALUES ( $thingID, '$location') ";
    if (!($result = $mysqli->query($insertQuery))) showerror($mysqli->errno, $mysqli->error); 
    echo "isl: ".$row["COUNT(*)"];
}

?>