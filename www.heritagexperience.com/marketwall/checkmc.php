<?php

include 'db_address.php';

if(isset($_GET['mwid'])) {

    $mwid=$_GET['mwid'];
    $db = new PDO ("mysql:host=$hostname;dbname=$dbname", $user, $pass);


    $sql = $db->prepare('SELECT posizione,descrizione FROM  marketcube as m
      INNER JOIN marketprodotti as p on m.id_prodotto=p.id
      WHERE inserito=0 and  id_mw= :mwid');
    $sql->bindParam(':mwid',$mwid);
    $sql->execute();
    $result=$sql->fetchAll(PDO::FETCH_ASSOC);
    header('Content-type: application/json');
    echo json_encode($result);

  //  echo count($result);
/*
    $to_return="rows".count($result);

    foreach ($result as $risultato) {
      $to_return=$to_return."record".$risultato["posizione"]."&";
      $to_return=$to_return.$risultato["descrizione"]."&";
    }
    echo $to_return;
*/
    //$db = null;
}
?>
