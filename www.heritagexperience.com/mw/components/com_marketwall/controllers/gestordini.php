<?php
/**
 * @package     Joomla.Site
 * @subpackage  com_contact
 *
 * @copyright   Copyright (C) 2005 - 2017 Open Source Matters, Inc. All rights reserved.
 * @license     GNU General Public License version 2 or later; see LICENSE.txt
 */

defined('_JEXEC') or die;


require_once JPATH_COMPONENT . '/models/gestordini.php';

/**
 * Controller for single contact view
 *
 * @since  1.5.19
 */
class marketwallControllergestordini extends JControllerLegacy
{
    protected $_db;
    private $_app;
    private $params;
    private $_filterparam;

    public function __construct($config = array())
    {
        parent::__construct($config);
        $this->_app = JFactory::getApplication();
        $this->_filterparam = new stdClass();
        $this->_filterparam->mwid=JRequest::getVar('mwid');
        $this->_filterparam->posizione=JRequest::getVar('posizione');
        $this->_filterparam->userid=JRequest::getVar('userid');


    }

    public function insert(){

        $model=new marketwallModelgestordini();
        if($model->InsertNewOrdine($this->_filterparam->mwid,$this->_filterparam->posizione)) {
            echo "1";
        }else{
            echo "0";
        }
        $this->_app->close();

    }

    public function getReport() {
        $userid=null;
        if($this->_filterparam->userid==null) {
            $user = JFactory::getUser();

            $userid=$user->id;
        }else{
            $userid=$this->_filterparam->userid;
        }
        $model=new marketwallModelgestordini();
        $result=$model->getReport($userid);
        echo json_encode($result);
        $this->_app->close();


    }


}