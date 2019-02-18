<?php 
	/*
		Returns the middle point between all the trackers
	*/

	include "databaseFunctions.php";

	$avgPoint = selectDB("
		SELECT AVG(l01.latitude), AVG(l01.longitude) FROM logs l01
		INNER JOIN
		(SELECT trackerID, max(dateTime) AS maxDate FROM logs GROUP BY trackerID) l02
		ON l01.trackerID = l02.trackerID AND l01.dateTime = l02.maxDate;"
	);

	echo json_encode($avgPoint);
?>