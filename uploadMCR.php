<?php
$target_path = "uploads/";

$target_path = $target_path . time() . basename( $_FILES['uploadedfile']['name']); 

$return_vals = array(1,5,10,20,50,100);

if(move_uploaded_file($_FILES['uploadedfile']['tmp_name'], $target_path)) {
    $randNum = $return_vals[rand(1,5)];
    $output = array();
    exec("./mcr ".$target_path, $output, $return);
    echo($output[0]);
    # echo $randNum;
    # echo "The file ". $target_path . " 
    # has been uploaded";
    # unlink($target_path);
} else{
    echo "There was an error uploading the file, please try again!";
}
?>
