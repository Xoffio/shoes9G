<?php 
	/*
		Returns the last GPS data in a json format
	*/

	$lastMov = explode("\n", shell_exec("tail -n 5 location.txt"));

	$movList = array();
	foreach ($lastMov as $line) {
		$tmp = explode(";", $line);
		if ( sizeof($tmp) == 3){
			array_push($movList, $tmp);
		}
	}

	echo json_encode($movList);
?>