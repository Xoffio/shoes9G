<?php  
	$dbhost = "localhost";
	$dbuser = 'shoes9G';
	$dbpass = 'br00klynTrackers';
	$dbname = "shoes9G";

	function insertDB($sql){
		// Create connection
		$conn = new mysqli($GLOBALS['dbhost'], $GLOBALS['dbuser'], $GLOBALS['dbpass'], $GLOBALS['dbname']);
		$r = 0;

		if(!$conn->connect_error)
			if ($conn->query($sql) === TRUE) $r = 1;
		else $r = -1;
		
		$conn->close();

		return ($r);
	}

	function selectDB($sql){
		// Create connection
		$conn = new mysqli($GLOBALS['dbhost'], $GLOBALS['dbuser'], $GLOBALS['dbpass'], $GLOBALS['dbname']);
		$r = 0;

		if(!$conn->connect_error){
			$result = $conn->query($sql);
			
			if ($result->num_rows > 0) {
				$r = array();
				
				while ($row = $result->fetch_row()) {
					array_push($r, $row);
				}
			}
		}
		else $r = -1;
		
		$conn->close();

		return ($r);
	}	
?>