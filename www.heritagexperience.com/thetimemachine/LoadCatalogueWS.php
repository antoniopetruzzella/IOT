<?php
/* require the user as the parameter */
if(isset($_GET['userid']) && intval($_GET['userid']) ){


	$userid = intval($_GET['userid']); //no default
	
	$applications=array();
	/* connect to the db */
	include 'db_address.php';
	
	
	// QUI IINIZIA L'ESTRAZIONE DELLE APP A CATALOGO
	
	$queryCatalogue = "SELECT Id, name, image, Url, descr FROM application";
	$resultCatalogue = mysql_query($queryCatalogue,$link) or die('Errant query:  '.$queryCatalogue);
	//echo $queryCatalogue;	
	if(mysql_num_rows($resultCatalogue)) {
		while($applicationInCatalogue = mysql_fetch_assoc($resultCatalogue)) {
		
			
			$applicationInCatalogueId=$applicationInCatalogue["Id"];
			$queryPortfolio = "SELECT applicationid FROM visitapplication  
			where  visitapplication.Applicationid=$applicationInCatalogueId 
			and visitapplication.userid=$userid";
			$resultPortfolio = mysql_query($queryPortfolio,$link) or die('Errant query:  '.$queryPortfolio);
			
			if(mysql_num_rows($resultPortfolio)>0) {
				
			$applicationInCatalogue["inportfolio"]=true;
				
				
			}else{
				
			$applicationInCatalogue["inportfolio"]=false;	
			}
		$applications[]=$applicationInCatalogue;
			
		}
		
	header('Content-type: application/json');
	echo json_encode($applications);	
	}else{
			header('Content-type: application/json');
			echo json_encode(null);	
			
		}
	/* disconnect from the db */
	@mysql_close($link);
}else{

header('Content-type: application/json');
echo json_encode(null);	

	
}
?>