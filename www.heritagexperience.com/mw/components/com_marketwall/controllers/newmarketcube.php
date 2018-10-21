<?php
/**
 * @package     Joomla.Site
 * @subpackage  com_contact
 *
 * @copyright   Copyright (C) 2005 - 2017 Open Source Matters, Inc. All rights reserved.
 * @license     GNU General Public License version 2 or later; see LICENSE.txt
 */

defined('_JEXEC') or die;


require_once JPATH_COMPONENT . '/models/newmarketcube.php';

/**
 * Controller for single contact view
 *
 * @since  1.5.19
 */
class marketwallControllerNewmarketcube extends JControllerLegacy
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
        $this->_filterparam->id_prodotto = JRequest::getVar('mcid');



    }

    public function insert(){

        $model=new marketwallModelnewmarketcube();
        if($model->InsertNewMarketCube($this->_filterparam->id_prodotto)) {
            echo "inserimento riuscito";
        }


    }




}
