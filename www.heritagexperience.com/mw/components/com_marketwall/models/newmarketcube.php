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
class marketwallModelnewmarketcube extends JModelLegacy {


    private $_app;
    private $userid;
    protected $params;
    protected  $_db;


    public function __construct($config = array()) {
        parent::__construct($config);

        $user = JFactory::getUser();
        $this->userid = $user->get('id');

        $this->_db = JFactory::getDbo();

        $this->_app = JFactory::getApplication('site');
        $this->params = $this->_app->getParams();

    }

    public function __destruct() {

    }

    public function InsertNewMarketCube($prodottoid){

        $mwid=$this->getMW($this->userid);
        $object = new StdClass;
        $object->id_mw=$mwid;
        $object->id_prodotto=$prodottoid;
        $object->posizione=$this->getNextPosFree($mwid);
        $object->inserito=0;
        $object->timestamp=Date('Y-m-d h:i:s',time());

        $result=$this->_db->insertObject('mc_marketcube',$object);

    }

    private function getNextPosFree($mwid){

        $query=$this->_db->getQuery(true);
        $query->select('max(posizione)');
        $query->from('mc_marketcube');
        $query->where(' id_mw='.$mwid);
        $this->_db->setQuery($query);
        return ($this->_db->loadResult())+1;

    }

    public function getprodotti(){

        $query=$this->_db->getQuery(true);
        $query->select('*');
        $query->from('mc_marketprodotti');
        $this->_db->setQuery($query);
        $result=$this->_db->loadObjectList();
        return $result;

    }

    private function getMW($userid){

        $query=$this->_db->getQuery(true);
        $query->select('id_mw');
        $query->from('mc_marketwall');
        $query->where(' id_user='.$userid);
        $query->order(' timestamp desc');
        $query->setLimit('1');
        $this->_db->setQuery($query);

        return $this->_db->loadResult();

    }
}