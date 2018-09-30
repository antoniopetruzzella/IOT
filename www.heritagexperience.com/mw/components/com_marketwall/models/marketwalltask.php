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
class marketwallModelmarketwalltask extends JModelLegacy {


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

    public function getuser($mwid){

      $query=$this->_db->getQuery(true);
      $query->select('id_user');
      $query->from('mc_marketwall');
      $query->where(' id_mw='.$mwid);

      $this->_db->setQuery($query);
      return $this->_db->loadResult();


    }

    public function checkmc($mwid){

      $query=$this->_db->getQuery(true);
      $query->select('posizione,descrizione');
      $query->from('mc_marketcube as m');
      $query->join('inner','mc_marketprodotti as p on m.id_prodotto=p.id');
      $query->where('inserito=0 AND id_mw='.$mwid);

      $this->_db->setQuery($query);
      $result=$this->_db->loadAssocList();
      //var_dump($result);
      //echo $query;
      return $result;

    }

    public function confirmmcinsertion($mwid,$posizione){

      $query="UPDATE mc_marketcube SET inserito=1 WHERE id_mw=".$mwid." AND posizione=".$posizione;
      $this->_db->setQuery($query);
      $result=$this->_db->execute();
      return $result;


    }

}
