<?php
  $currentLocation = $_POST["location"];
  echo $currentLocation;
  echo exec("echo '".date('Y-m-d h:i:sa').": $currentLocation' >> location.txt");
?>
