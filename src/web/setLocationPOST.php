<?php
	include "phpFunctions/databaseFunctions.php";

	$pass = $_POST["pass"];
	$latitude = $_POST["latitude"];
	$longitude = $_POST["longitude"];

	if (connectToDatabase()){
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