<?php
	include "phpFunctions/databaseFunctions.php";

	// POST data
	$pass = $_POST["pass"];
	$latitude = $_POST["latitude"];
	$longitude = $_POST["longitude"];
	$trackerID = "(SELECT id FROM trackers WHERE pass = '$pass')";
	$limitVal = 0.0001;

	// Get last location
	$preArray = selectDB("
		SELECT trackerID, ABS(latitude-$latitude) AS laDiff, 
		ABS(longitude-$longitude) AS loDiff, TIMESTAMPDIFF(SECOND, dateTime, NOW()) AS dateDiff,
		latitude, longitude FROM logs
		WHERE trackerID=(SELECT id FROM trackers WHERE pass = '$pass') 
		ORDER BY id DESC LIMIT 1;
	");

	if ($preArray != 0){
		$trackerID = $preArray[0][0];
		$laDiff = $preArray[0][1];
		$loDiff = $preArray[0][2];
		$secDiff = $preArray[0][3];

		// If GPS return a unexpected value send the previous one.
		if ( (($laDiff/$secDiff) > $limitVal) || (($loDiff/$secDiff) > $limitVal) ){
			$latitude = $preArray[0][4];
			$longitude = $preArray[0][5];
		}
	}

	$result = insertDB("
		INSERT INTO logs (trackerID, latitude, longitude)
		VALUES (
			$trackerID,
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