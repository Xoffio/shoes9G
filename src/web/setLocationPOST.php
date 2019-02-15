<?php
	include "phpFunctions/databaseFunctions.php";

	$pass = $_POST["pass"];
	$latitude = $_POST["latitude"];
	$longitude = $_POST["longitude"];

	$result = insertDB("
		INSERT INTO logs (trackerID, latitude, longitude)
		VALUES (
			(SELECT id FROM trackers WHERE pass = '$pass'),
			$latitude,
			$longitude
		);
	");

	if ($result == 1){
		$result = insertDB("UPDATE trackers SET lastTrack=NOW() WHERE pass = '$pass';");
	}

	echo "$result\r\n\r\n";

	//exec("echo '".date('Y-m-d h:i:sa').";$latitude;$longitude' >> location.txt");
?>