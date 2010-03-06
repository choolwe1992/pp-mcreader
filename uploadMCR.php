<?php

#$target = "";
$target = "/home/testuser/www/uploads/";

$target_path = $target. time() . basename( $_FILES['uploadedfile']['name']); 

#echo $target_path;

$return_vals = array(1,5,10,20,50,100);

$limit_size=2000000;

if(move_uploaded_file($_FILES['uploadedfile']['tmp_name'], $target_path)) {
	#$file_size = $_FILES['uploadedfile']['size'];
	#if($file_size >= $limit_size){
	#	echo "Your file size is over limit<BR>";
	#	echo "Your file size = ".$file_size;
	#	echo " K";
	#	echo "<BR>File size limit = 2000000k";
	#}
	$cmd = "chmod 777 ".$target_path;
	exec($cmd);
	$cmd = "mogrify -resize 800x600! ".$target_path;
	exec($cmd);
	$cmd = "./cr ".$target_path;
	#echo "$cmd <br> <br>";
	$output = exec($cmd);
	echo $output."\n";

	#unlink($target_path);

	#test for now...
	#	$randNum = $return_vals[rand(1,5)];
	#	echo $randNum;
} else{
	echo "There was an error uploading the file, please try again!";
}
?>
