<?php
  $latitude = $_GET["latitude"];
  $longitude = $_GET["longitude"];
  echo "1";
  exec("echo '".date('Y-m-d h:i:sa').";$latitude;$longitude' >> location.txt");
?>