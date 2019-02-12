<?php  
	$dbhost = "localhost";
	$dbuser = 'YOUR_DB_USER';
	$dbpass = 'YOUR_DB_PASSWORD';
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
?>