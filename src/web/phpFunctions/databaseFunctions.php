<?php  
	$dbhost = 'localhost';
	$dbuser = 'YOUR_DB_USER';
	$dbpass = 'YOUR_DB_PASSWORD';
	$conn = NULL;

	function connectToDatabase(){
		echo "HELLO11";
		$conn = new mysqli($dbhost, $dbuser, $dbpass);
		$r = 0;
echo "HELLO";
		if(!$conn->connect_error) $r = 1;
echo "HELLO $r";
		return ($r);
	}

	function insertDB($sql){
		$r = 0;
		if ($conn->query($sql) === TRUE) {
			$r = 1;
		}

		return ($r);
	}

	function closeConnection(){
		$conn->close();
		$conn = NULL;
	}	
?>