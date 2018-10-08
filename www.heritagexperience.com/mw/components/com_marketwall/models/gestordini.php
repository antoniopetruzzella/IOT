<?php

/**
 * WebTVContenuto Model
 *
 * @package    Joomla.Components
 * @subpackage WebTV
 */
defined('_JEXEC') or die('Restricted access');

jimport('joomla.application.component.model');



/**
 * WebTVContenuto Model
 *
 * @package    Joomla.Components
 * @subpackage WebTV
 */
class marketwallModelgestordini extends JModelLegacy {


    private $_app;
    private $userid;
    protected $params;
    protected  $_db;


    public function __construct($config = array()) {
        parent::__construct($config);


        $this->_db = JFactory::getDbo();

        $this->_app = JFactory::getApplication('site');
        $this->params = $this->_app->getParams();

    }

    public function __destruct() {

    }

    public function InsertNewOrdine($mwid,$posizione){

        $mcid=$this->getMcId($mwid,$posizione);
        $object = new StdClass;
        $object->idmc=$mcid;
        $object->stato_ordine=1;
        $object->timestamp=Date('Y-m-d h:i:s',time());

        $result=$this->_db->insertObject('mc_ordini',$object);
        return $result;

    }


    private function getMcId($mwid,$posizione){

        $query=$this->_db->getQuery(true);
        $query->select('id');
        $query->from('mc_marketcube');
        $query->where(' id_mw='.$mwid.' AND posizione='.$posizione);
        $this->_db->setQuery($query);

        return $this->_db->loadResult();

    }

    public function getReport($userid){

        $query=$this->_db->getQuery(true);
        $query->select('prodotti.descrizione as descrizione, stato.stato as stato');
        $query->from('mc_ordini as o');
        $query->join('inner','mc_marketcube as mc on mc.id=o.idmc');
        $query->join('inner','mc_marketprodotti as prodotti on mc.id_prodotto=prodotti.id');
        $query->join('inner','mc_marketwall as mw on mc.id_mw=mw.id_mw');
        $query->join('inner','mc_stato_ordini as stato on o.stato_ordine=stato.id');
        $query->where(' mw.id_user='.$userid);
        $this->_db->setQuery($query);

        $result=$this->_db->loadAssocList();

        return $result;


    }
}