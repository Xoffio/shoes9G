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
		SELECT l01.trackerID, l01.latitude, l01.longitude FROM logs l01
		INNER JOIN
		(SELECT trackerID, max(dateTime) AS maxDate FROM logs GROUP BY trackerID) l02
		ON l01.trackerID = l02.trackerID AND l01.dateTime = l02.maxDate;"
	);

	echo json_encode($movList);
?>