<?php  
	include "databaseFunctions.php";

	//$filter = $_POST["filter"];
	$filter = $_GET["filter"];
	$query = NULL;

	switch ($filter) {
		case 'all':
			$query = "SELECT name, id FROM `trackers` WHERE 1;";
			break;

		case 'online':
			$query = "SELECT name, id FROM `trackers` WHERE lastTrack > ( NOW() - INTERVAL 1 MINUTE )";
			break;

		case 'offline':
			$query = "SELECT name, id FROM `trackers` WHERE lastTrack < ( NOW() - INTERVAL 1 MINUTE )";
			break;
		
		default:
			$query = "SELECT name, id FROM `trackers` WHERE 1 ";
			break;
	}

	$html = '<div class="btn-group-toggle" data-toggle="buttons">';
	$trackerList = selectDB($query);
	
	$checkV = "checked";
	$activeV = "active";
	for ($t=0; $t<count($trackerList); $t++){

		$html = $html.'<label id="trackerBtn'.$trackerList[$t][1].'" class="btn btn-secondary '.$activeV.'" data-id="'.$trackerList[$t][1].'">
							<input type="checkbox" autocomplete="off" '.$checkV.'> '.$trackerList[$t][0].'
							<span class="onlineTag" style="display:none;">Online</span>
							<span class="offlineTag" style="display:none;">Offline</span>
						</label>';

		if ($t == 0){
			$checkV = "";
			$activeV = "";
		}
	}

	$html = $html."</div>";
	echo $html;
	//echo json_encode($trackerList);
?>