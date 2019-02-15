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

			var mainMarker = L.marker([0, 0]).addTo(map);
			var lineTrack = L.polyline([], {color: randomColor()}).addTo(map);
			var lastPoint = null;

			setInterval(function(){
			    $.ajax({
					type: "POST",
					url: "phpFunctions/getLocation.php",
					//async: true,
					//dataType: "json",
					success: function(r){
						var locArray = jQuery.parseJSON(r);
						var locLength = locArray.length;

						var newPoint = [locArray[locLength-1][1], locArray[locLength-1][2]];
						
						if (firstTime){
							map.setView(newPoint);
							firstTime = false;
						}

						if (newPoint != lastPoint){
							lastPoint = newPoint;

							lineTrack.addLatLng(lastPoint);
							mainMarker.setLatLng(lastPoint);
						}
					}
				});
			}, 1000);
		</script>
	</body>
</html>