<?php
  $currentLocation = $_GET["location"];
  echo $currentLocation;
  echo exec("echo $currentLocation >> location.txt");
?>
