<?php 
	/*
		Returns the last GPS data in a json format
	*/

	include "phpFunctions/databaseFunctions.php";

	$movList = selectDB("
		SELECT dateTime, latitude, longitude 
		FROM `logs` WHERE trackerID = 1
		ORDER BY id DESC LIMIT 1;"
	);

	echo json_encode($movList);
?>