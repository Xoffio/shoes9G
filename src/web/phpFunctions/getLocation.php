<?php 
	/*
		Returns the last GPS data in a json format
	*/

	include "databaseFunctions.php";

	/*
	SELECT l01.* FROM logs l01
	INNER JOIN
	(SELECT trackerID, max(dateTime) AS maxDate FROM logs GROUP BY trackerID) l02
	ON l01.trackerID = l02.trackerID AND l01.dateTime = l02.maxDate;
	*/

	$movList = selectDB("
		SELECT dateTime, latitude, longitude 
		FROM `logs` WHERE trackerID = 1
		ORDER BY id DESC LIMIT 1;"
	);

	echo json_encode($movList);
?>