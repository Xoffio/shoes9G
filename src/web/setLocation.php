<?php
	include "phpFunctions/databaseFunctions.php";

	// POST data
	$pass = $_GET["pass"];
	$latitude = $_GET["latitude"];
	$longitude = $_GET["longitude"];
	$trackerID = "(SELECT id FROM trackers WHERE pass = '$pass')";
	$limitVal = 0.0004;
	echo "la: ".$latitude."<BR>";

	echo "
		SELECT trackerID, ABS(latitude-$latitude) AS laDiff, 
		ABS(longitude-$longitude) AS loDiff, TIMESTAMPDIFF(SECOND, dateTime, NOW()) AS dateDiff,
		latitude, longitude FROM logs
		WHERE trackerID=(SELECT id FROM trackers WHERE pass = '$pass') 
		ORDER BY id DESC LIMIT 1;
	";

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

		echo "lastla: ".$latitude."<BR>";
		// If GPS return a unexpected value send the previous one.
		if ( (($laDiff/$secDiff) > $limitVal) || (($loDiff/$secDiff) > $limitVal) ){
			echo "HERE<BR>";
			$latitude = $preArray[0][4];
			$longitude = $preArray[0][5];
			echo "la: ".$latitude."<BR>";
		}
	}

	echo "la: ".$latitude."<BR>";
	echo ($laDiff/$secDiff)." > ".$limitVal." = ".(($laDiff/$secDiff) > $limitVal)."<br>";
	echo "INSERT INTO logs (trackerID, latitude, longitude)
		VALUES (
			$trackerID,
			$latitude,
			$longitude
		);";
	/*$result = insertDB("
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

	echo "$result\r\n\r\n";*/

	//exec("echo '".date('Y-m-d h:i:sa').";$latitude;$longitude' >> location.txt");
?>