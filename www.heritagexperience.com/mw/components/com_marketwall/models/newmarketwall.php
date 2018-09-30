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
class marketwallModelnewmarketwall extends JModelLegacy {


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

    public function InsertNewMarketWall($mwid){

        $object = new StdClass;
        $object->id_user=$this->userid;
        $object->id_mw=$mwid;
        $object->timestamp=Date('Y-m-d h:i:s',time());

        $result=$this->_db->insertObject('mc_marketwall',$object);




    }

}

