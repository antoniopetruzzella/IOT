<?php
/* require the user as the parameter */
if(isset($_GET['userid'])&&isset($_GET['mwid'])) {

  $userid = $_GET['userid'];
	$mwid=$_GET['mwid'];



            /* connect to the db */
            include 'db_address.php';
            $db = new PDO ("mysql:host=$hostname;dbname=$dbname", $user, $pass);
            $sql = $db->prepare("INSERT INTO  marketwall (id_user,id_mw) VALUES (:userid,:mwid)");
            $sql->bindParam(':userid',$userid,PDO::PARAM_INT);
            $sql->bindParam(':mwid',$mwid);
 try {
            $sql->execute();
//echo  $userid." ".$mwid;

            //$db = null;


 } catch (PDOException $e) {

          echo $e->getMessage();
 }
}
?>
