<?php
/**
 * @package     Joomla.Site
 * @subpackage  com_contact
 *
 * @copyright   Copyright (C) 2005 - 2017 Open Source Matters, Inc. All rights reserved.
 * @license     GNU General Public License version 2 or later; see LICENSE.txt
 */

defined('_JEXEC') or die;


require_once JPATH_COMPONENT . '/models/marketwalltask.php';

/**
 * Controller for single contact view
 *
 * @since  1.5.19
 */
class marketwallControllerMarketwalltask extends JControllerLegacy
{
    protected $_db;
    private $_app;
    private $params;
    private $_filterparam;
    private $mwid;

    public function __construct($config = array())
    {
        parent::__construct($config);
        $this->_app = JFactory::getApplication();
        $this->_filterparam = new stdClass();
        $this->mwid = JRequest::getVar('mwid');



    }


    public function getuser(){
      $model=new marketwallModelmarketwalltask();
      $user=$model->getuser($this->mwid);
      echo ($user);
      $this->_app->close();
    }

    public function checkmc(){
      $model=new marketwallModelmarketwalltask();

      $result=$model->checkmc($this->mwid);
      echo json_encode($result);
      $this->_app->close();

    }




}
