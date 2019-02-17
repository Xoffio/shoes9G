<!DOCTYPE html>
<html>
	<head>
		<title>TRACKER</title>

		<link rel="stylesheet" href="bootstrap/css/bootstrap.css">
		<link rel="stylesheet" href="leaflet/leaflet.css">
		<link rel="stylesheet" href="getLocation.css">
		<script type="text/javascript" src="jquery-3.3.1.min.js"></script>
		<script type="text/javascript" src="bootstrap/js/bootstrap.min.js"></script>
		<script type="text/javascript">
			function randomColor(){
				var color = '#'+Math.floor(Math.random()*16777215).toString(16);
				return (color);
			}
		</script>
	</head>
	<body>
		<div class="container-fluid">
			<!--<dir id="sidebar">
				<div class="sidebar-wrapper">
					<div id="features" class="panel panel-default">
						<div class="panel-heading">
							<h3 class="panel-title"> SIDEBAR</h3>
						</div>
					</div>
				</div>
			</dir>-->
			<div class="row">
				<div class="col col-10">
					<div id="mainMap"></div>
				</div>
				<div id="sidebar" class="col col-2">
					<div class="card text-center">
						<div class="card-header">
							Trackers

							<div class="trackerFilters btn-group btn-group-toggle" data-toggle="buttons">
								<label class="btn btn-secondary active">
									<input type="radio" name="tFilter" id="tFilterAll" checked> All
								</label>

								<label class="btn btn-secondary">
									<input type="radio" name="tFilter" id="tFilterOnline"> Online
								</label>

								<label class="btn btn-secondary">
									<input type="radio" name="tFilter" id="tFilterOffline"> Offline
								</label>
							</div>
						</div>
					</div>

					<div class="card-body">

						<!-- <div class="btn-group-toggle" data-toggle="buttons">
							<label class="btn btn-secondary active">
								<input type="checkbox" checked autocomplete="off"> DEV-01
								<spam class="onlineTag" >
									online
								</spam>
							</label>
						</div>-->

						<?php
							include "phpFunctions/getTrackers.php";
						?>

					</div>
				</div>
			</div>
			
		</div>


		
		<script src="leaflet/leaflet.js"></script>
		<script>
			var map = L.map('mainMap').setView([0, 0], 18);
			var firstTime = true;

			/*
				Dark theme: https://{s}.basemaps.cartocdn.com/dark_all/{z}/{x}/{y}{r}.png
				https://leaflet-extras.github.io/leaflet-providers/preview/
			*/
			L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
			    attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
			}).addTo(map);

			/*var mainMarker = L.marker([0, 0]).addTo(map);
			var lineTrack = L.polyline([], {color: randomColor()}).addTo(map);*/
			var markers = {};
			var paths = {};
			var lastPoints = {};

			$("[id^=trackerBtn]").each(function(){
				var currentID = $(this).attr("data-id").toString();
				var currentName = $(this).text();
				var rColor = randomColor();

				markers[currentID] = L.marker([0, 0]).bindPopup(currentName).addTo(map);
				paths[currentID] = L.polyline([], {color: rColor}).addTo(map);
				lastPoints[currentID] = null;

				console.log(markers);

				$(this).css("text-shadow", "1px 1px 5px"+rColor);
			});

			setInterval(function(){
			    $.ajax({
					type: "POST",
					url: "phpFunctions/getLocation.php",
					//async: true,
					//dataType: "json",
					success: function(r){
						var locArray = jQuery.parseJSON(r);
						var locLength = locArray.length;

						if (firstTime){
							//map.setView(newPoint);
							firstTime = false;
						}

						for (var tracker=0; tracker<locLength; tracker++){
							var trackerID = locArray[tracker][0].toString();
							var newPoint = [locArray[tracker][1], locArray[tracker][2]];

							if (newPoint != lastPoints[trackerID]){
								lastPoints[trackerID] = newPoint;

								paths[trackerID].addLatLng(newPoint);
								markers[trackerID].setLatLng(newPoint);
							}
						}
					}
				});
			}, 1000);

			$("[id^=trackerBtn]").on("click", function(){
				var tracker = $(this);
				var trackerID = tracker.attr("data-id").toString();

				// If the tracker was active. Remove the tracker marker and path.
				if (tracker.hasClass("active")){
					map.removeLayer(paths[trackerID]);
					map.removeLayer(markers[trackerID]);
				}
				else{
					paths[trackerID].addTo(map);
					markers[trackerID].addTo(map);
				}
			});
		</script>
	</body>
</html>