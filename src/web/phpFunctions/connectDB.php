<?php  
	$dbhost = 'localhost';
	$dbuser = 'YOUR_DB_USER';
	$dbpass = 'YOUR_DB_PASSWORD';
	$conn = new mysqli($dbhost, $dbuser, $dbpass);

	if($conn->connect_error) {
	  die('Could not connect: '.$conn->connect_error);
	}

	echo 'Connected successfully';
	$conn->close();
?>