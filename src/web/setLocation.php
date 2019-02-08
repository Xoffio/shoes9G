<?php
	include "phpFunctions/databaseFunctions.php";
	
	$pass = $_GET["pass"];
	$latitude = $_GET["latitude"];
	$longitude = $_GET["longitude"];

	if (connectToDatabase()){
		echo "HELLO";
		$result = NULL;
		
		$result = insertDB("
			INSERT INTO logs (trackerID, latitude, longitude)
			VALUES (
				(SELECT id FROM trackers WHERE pass = '$pass'),
				$latitude,
				$longitude
			)
		");

		closeConnection();

		if ($result == 1) echo "1\r\n\r\n";
		else echo "0\r\n\r\n";
	}

	//exec("echo '".date('Y-m-d h:i:sa').";$latitude;$longitude' >> location.txt");
?>