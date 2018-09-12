<?php

include 'db_address.php';

if(isset($_GET['id_mw'])) {

    $id_mw=$_GET['id_mw'];
    $db = new PDO ("mysql:host=$hostname;dbname=$dbname", $user, $pass);


    $sql = $db->prepare("SELECT id_user FROM  marketwall WHERE id_mw=" . $id_mw);

    $sql->execute();
    echo $sql->fetch()[0];
}
?>

