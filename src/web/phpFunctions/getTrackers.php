<?php  
	include "databaseFunctions.php";

	//$filter = $_POST["filter"];
	$filter = $_GET["filter"];
	$query = NULL;

	switch ($filter) {
		case 'all':
			$query = "SELECT name FROM `trackers` WHERE 1 ";
			break;

		case 'online':
			$query = "SELECT name FROM `trackers` WHERE lastTrack > ( NOW() - INTERVAL 1 MINUTE )";
			break;

		case 'offline':
			$query = "SELECT name FROM `trackers` WHERE lastTrack < ( NOW() - INTERVAL 1 MINUTE )";
			break;
		
		default:
			$query = "SELECT name FROM `trackers` WHERE 1 ";
			break;
	}

	$html = '<div class="btn-group-toggle" data-toggle="buttons">';
	$trackerList = selectDB($query);
	
	for ($t=0; $t<count($trackerList); $t++){
		$html = $html.'<label class="btn btn-secondary active">
							<input type="checkbox" checked autocomplete="off"> '.$trackerList[$t][0].'
						</label>';
	}

	$html = $html."</div>";
	echo $html;
	//echo json_encode($trackerList);
?>