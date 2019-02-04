<?php
  $latitude = $_POST["latitude"];
  $longitude = $_POST["longitude"];
  echo "1\r\n\r\n";
  exec("echo '".date('Y-m-d h:i:sa').";$latitude;$longitude' >> location.txt");
?>
