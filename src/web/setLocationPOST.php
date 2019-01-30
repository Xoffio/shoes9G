<?php
  $latitude = $_POST["latitude"];
  $longitude = $_POST["longitude"];
  echo "1";
  exec("echo '".date('Y-m-d h:i:sa').": [$latitude, $longitude]' >> location.txt");
?>
