<?php

/**
 * Return truck information from a given nid
 *
 * @param $tid
 *
 * @return array
 */
function get_truck_info($tid){
	return db_select('truck_dataset', 't')->fields('t')->condition('truck_nid', $tid, '=')->execute()->fetchAssoc();
}

/**
 * Return loads record from a given nid
 *
 * @param $tid
 *
 * @return array
 */
function get_truck_loads($tid){
	$output = [];
	$loads = db_select('truck_dataset_loads', 't')->fields('t')->condition('truck_nid', $tid, '=')->orderBy('load_created', 'ASC')->execute()->fetchAll();

	// LOADS
	foreach($loads as $value){
		$output[$value->load_nid] = [
			'nid' => $value->load_nid,
			'title' => $value->load_title,
			'status' => $value->load_status,
			'created' => $value->load_created,
			'miles' => $value->load_miles,
			'empty' => $value->load_empty,
			'rate' => $value->load_rate,
			'payment' => $value->load_payment,
			'uid' => $value->load_author_uid,
			'uname' => $value->load_author_user,
			'ufull' => $value->load_author_full,
			'trucknid' => $value->truck_nid,
		];
	}

	return $output;
}

/**
 * Return extra info from a given load
 *
 * @param $lid
 *
 * @return array
 */
function get_load_extra($lid){
	$output = ['from' => '', 'to' => '', 'delivery' => '', 'doc' => ''];

	// ITINERARY
	$points = db_select('truck_dataset_loads_address', 'p')->fields('p')->condition('load_nid', $lid, '=')->execute()->fetchAll();

	if(count($points) > 0){
		$output['from'] = $points[0]->city . ' ' . $points[0]->state;
		$output['to'] = $points[count($points) - 1]->city . ' ' . $points[count($points) - 1]->state;
	}

	// DELIVERY DATE
	$delivery = db_select('truck_dataset_loads_delivery', 'p')->fields('p')->condition('load_nid', $lid, '=')->execute()->fetchAll();
	$output['delivery'] = count($delivery) > 0 ? $delivery[count($delivery) - 1]->date : '';

	// RATECONFIRM
	$rate = db_select('truck_dataset_loads_confirm', 'r')->fields('r')->condition('load_nid', $lid, '=')->execute()->fetchAll();
	$output['doc'] = count($rate) > 0 ? $rate[0]->fid : '';

	// RATECONFIRM
	$bol = db_select('truck_dataset_loads_bol', 'r')->fields('r')->condition('load_nid', $lid, '=')->execute()->fetchAll();
	$output['bol'] = count($bol) > 0 ? $bol[0]->fid : null;

	return $output;
}


/**
 * Return filter load list from a given date range
 *
 * @param $loads
 * @param null $from
 * @param null $to
 *
 * @return mixed
 */
function filter_loads($loads, $from = null, $to = null){
	if( ! is_null($from) && ! is_null($to)){
		foreach($loads as $key => $value){
			if( ! is_null($from) && $value->load_created < $from){
				unset($loads[$key]);
			}
			if( ! is_null($to) && $value->load_created > $to){
				unset($loads[$key]);
			}
		}
	}

	return $loads;
}

/**
 * Return performance report from a given load list
 *
 * @param $loads
 * @param $this_week
 * @param $last_week
 * @param $this_month
 * @param $last_month
 * @param $this_year
 *
 * @return array
 */
function get_loads_report($loads, $this_week, $last_week, $this_month, $last_month, $this_year){
	// INITIAL REPORT
	$report = [
		'thisweek' => ['count' => 0, 'total' => 0, 'empty' => 0, 'sold' => 0, 'rate' => 0],
		'lastweek' => ['count' => 0, 'total' => 0, 'empty' => 0, 'sold' => 0, 'rate' => 0],
		'thismonth' => ['count' => 0, 'total' => 0, 'empty' => 0, 'sold' => 0, 'rate' => 0],
		'lastmonth' => ['count' => 0, 'total' => 0, 'empty' => 0, 'sold' => 0, 'rate' => 0],
		'thisyear' => ['count' => 0, 'total' => 0, 'empty' => 0, 'sold' => 0, 'rate' => 0],
		'ever' => ['count' => 0, 'total' => 0, 'empty' => 0, 'sold' => 0, 'rate' => 0],
	];

	// LOAD ANALIZER
	foreach($loads as $key => $value){
		if($value['status'] != 'Breakdown' && $value['status'] != 'Canceled' && $value['status'] != 'Standby'){
			// EVER
			$report['ever']['count'] ++;
			$report['ever']['total'] += $value['miles'];
			$report['ever']['empty'] += $value['empty'];
			$report['ever']['sold'] += $value['payment'];

			// YEAR
			if($value['created'] > $this_year){
				$report['thisyear']['count'] ++;
				$report['thisyear']['total'] += $value['miles'];
				$report['thisyear']['empty'] += $value['empty'];
				$report['thisyear']['sold'] += $value['payment'];
			}

			// LAST MONTH
			if($value['created'] >= $last_month && $value['created'] < $this_month){
				$report['lastmonth']['count'] ++;
				$report['lastmonth']['total'] += $value['miles'];
				$report['lastmonth']['empty'] += $value['empty'];
				$report['lastmonth']['sold'] += $value['payment'];
			}

			// THIS  MONTH
			if($value['created'] >= $this_month){
				$report['thismonth']['count'] ++;
				$report['thismonth']['total'] += $value['miles'];
				$report['thismonth']['empty'] += $value['empty'];
				$report['thismonth']['sold'] += $value['payment'];
			}

			// LAST WEEK
			if($value['created'] >= $last_week && $value['created'] < $this_week){
				$report['lastweek']['count'] ++;
				$report['lastweek']['total'] += $value['miles'];
				$report['lastweek']['empty'] += $value['empty'];
				$report['lastweek']['sold'] += $value['payment'];
			}

			// THIS WEEK
			if($value['created'] >= $this_week){
				$report['thisweek']['count'] ++;
				$report['thisweek']['total'] += $value['miles'];
				$report['thisweek']['empty'] += $value['empty'];
				$report['thisweek']['sold'] += $value['payment'];
			}
		}
	}

	// EVER RATE
	if($report['ever']['sold'] != 0 && $report['ever']['total'] != 0){
		$rate = $report['ever']['sold'] / $report['ever']['total'];
		$report['ever']['rate'] = ! is_nan($rate) ? round($rate, 2) : 0;
	}

	// YEAR RATE
	if($report['thisyear']['sold'] != 0 && $report['thisyear']['total'] != 0){
		$rate = $report['thisyear']['sold'] / $report['thisyear']['total'];
		$report['thisyear']['rate'] = ! is_nan($rate) ? round($rate, 2) : 0;
	}

	// LAST MONTH RATE
	if($report['lastmonth']['sold'] != 0 && $report['lastmonth']['total'] != 0){
		$rate = $report['lastmonth']['sold'] / $report['lastmonth']['total'];
		$report['lastmonth']['rate'] = ! is_nan($rate) ? round($rate, 2) : 0;
	}

	// THIS MONTH RATE
	if($report['thismonth']['sold'] != 0 && $report['thismonth']['total'] != 0){
		$rate = $report['thismonth']['sold'] / $report['thismonth']['total'];
		$report['thismonth']['rate'] = ! is_nan($rate) ? round($rate, 2) : 0;
	}

	// LAST WEEK MONTH RATE
	if($report['lastweek']['sold'] != 0 && $report['lastweek']['total'] != 0){
		$rate = $report['lastweek']['sold'] / $report['lastweek']['total'];
		$report['lastweek']['rate'] = ! is_nan($rate) ? round($rate, 2) : 0;
	}

	// THIS WEEK RATE
	if($report['thisweek']['sold'] != 0 && $report['thisweek']['total'] != 0){
		$rate = $report['thisweek']['sold'] / $report['thisweek']['total'];
		$report['thisweek']['rate'] = ! is_nan($rate) ? round($rate, 2) : 0;
	}

	return $report;
}


/**
 * Validate payment profile
 */
function get_payment_profile($truck){

	// Return unknown if empty truck object
	if(empty($truck)) return 'unknown';

	// Security Check
	$profile = 'unknown';
	$truck_customer = isset($truck->customer_nid) ? $truck->customer_nid : 0;
	$truck_company = isset($truck->company_nid) ? $truck->company_nid : 0;

	// Set Profile
	if($truck_customer != 0 || $truck_company != 0){
		global $user;
		$usr = user_load($user->uid);
		$user_customer = 0;

		if(isset($usr->field_customer['und'][0]['target_id'])){
			$user_customer = $usr->field_customer['und'][0]['target_id'];
		}

		// Customer Profile
		if($user_customer != 0 && $user_customer == $truck_customer){
			$profile = 'customer';
		}
		// Carrier Profile
		elseif(isset($usr->field_company['und'])){
			foreach($usr->field_company['und'] as $key => $value){
				if($value['target_id'] == $truck_company){
					$profile = 'carrier';
				}
			}
		}
	}

	return $profile;
}

/**
 * Ajax callback for payment place
 */
function place_wrap_call(&$form, &$form_state){
	return $form['place'];
}

/**
 * Save truck nickname from form_state
 *
 * @param $form_state
 *
 * @throws Exception
 */
function load_accounting_save_truck_nickname(&$form_state){
	$form_id = null;

	if(isset($form_state['form_id'])) $form_id = $form_state['form_id'];
	if($form_state['submitted'] || $form_id = 'customer_reports_form'){
		if(isset($form_state['input']['btnSaveTruckNickname']) && $form_state['input']['btnSaveTruckNickname'] == 'save-truck-nickname'){
			if(isset($form_state['storage']['truck'])){
				$truck = node_load($form_state['storage']['truck']->truck_nid);
				$truck->field_nickname['und'][0]['value'] = $form_state['input']['truck-nickname'];
				node_save($truck);
				drupal_set_message('Truck nick name has been successfully updated as <i>' . $form_state['input']['truck-nickname'] . '</i>');
			}
		}
	}
}

function get_load_data($load, &$is_paid, $force_show_upload = false){
	// Extra Information
	$extra = get_load_extra($load['nid']);
	$is_paid = db_select('truck_dataset_loads_paid', 'p')->fields('p')->condition('load_nid', $load['nid'], '=')->execute()->fetchAssoc();
	$is_paid = ! (empty($is_paid['paid']) && ! $is_paid['paid']);

	$node_load = node_load($load['nid']);
	$element = field_view_field('node', $node_load, 'field_billoflading', [
		'label' => 'hidden',
	]);

	$element_confirmation = field_view_field('node', $node_load, 'field_rate_confirmation', [
		'label' => 'hidden',
	]);

	if(isset($element_confirmation['#items'])){
		$i = 0;
		foreach($element_confirmation['#items'] as $key => $item){
			$i ++;

			if($i > 1){
				unset($element_confirmation[$key]);
				continue;
			}

			$element_confirmation[$key]['#theme'] = 'file_link_extended';
			$element_confirmation[$key]['#file']->description = 'confirmation.pdf';
			$element_confirmation[$key]['#attributes'] = ['target' => '_blank'];
		}
	}

	if(isset($element['#items'])){
		$i = 0;
		foreach($element['#items'] as $key => $item){
			$i ++;

			if($i > 1){
				unset($element[$key]);
				continue;
			}

			$element[$key]['#theme'] = 'file_link_extended';
			$element[$key]['#file']->description = 'bol.pdf';
			$element[$key]['#attributes'] = ['target' => '_blank'];
		}
	}

	$bol_id = 'bol_' . $load['nid'];

	$widget = [
		'#title' => null,
		'#type' => 'file',
		'#default_value' => $extra['bol'],
		'#description' => null,
		'#upload_validators' => ['file_validate_extensions' => ['pdf jpg jpeg']],
		'#upload_location' => 'public://loads_bol/',
		'#attributes' => [
			'class' => ['tableselect_upload_file'],
			'name' => 'bol_' . $load['nid'],
		],
		'#prefix' => '<div id="miniform-upload-' . $bol_id . '" role="dialog" tabindex="-1" class="modal fade in">
                                        <div class="modal-dialog modal-sm" role="document">
                                        <div class="modal-content">
                                            <div class="modal-header">
                                            <h4 class="modal-title">' . t('Upload new BOL') . '</h4></div>
                                  <div class="modal-body">
                                      <div class="form-group">',
		'#suffix' => '</div>
                                  </div>

                                  <div class="modal-footer">
                                      <button class="btn btn-default" type="button" data-dismiss="modal">' . t('Close') . '</button>
                                      <button class ="btn btn-primary" type="submit" name="btnUploadBOL" value="upload-' . $bol_id . '">' . t('Save') . '</button>
                                  </div>
                          </div>
                      </div>
                  </div>',
	];

	// Load Data
	$load_data = [
		'title' => $load['title'],
		'miles' => $load['miles'],
		'empty' => $load['empty'],
		'payment' => $load['payment'],
		'rate' => $load['rate'],
		'from' => $extra['from'],
		'to' => $extra['to'],
		'delivery' => format_date(strtotime($extra['delivery']), 'views'),
		'created' => format_date($load['created'], 'views'),
		'dispatcher' => $load['ufull'],
		'confirmation' => render($element_confirmation),
		'bol' => render($element) . (($is_paid && ! $force_show_upload) ? '' : (empty($extra['bol']) ? '' : '<br/>') . render($widget)),
		'upload' => '<a href="#" class="btn btn-default btn-upload-bol" id="btn-upload-' . $bol_id . '"><span class="glyphicon glyphicon-upload" aria-hidden="true"></span></a>',
	];

	return $load_data;
}

/**
 * Upload a BOL file
 *
 * @param $form_state
 * @param $redirect
 */
function load_accounting_upload_bol(&$form_state, $redirect){
	global $user;

	// Process upload of BOL files
	if(isset($form_state['input']['btnUploadBOL'])){
		$prefix = 'upload-bol_';
		$value = $form_state['input']['btnUploadBOL'];
		if(strpos($value, $prefix) == 0){
			$load_id = substr($value, strlen($prefix));
			$node = node_load($load_id);

			if(empty($node->field_billoflading)){
				$node->field_billoflading['und'] = [];
			}

			$post_var = 'bol_' . $load_id;
			$filename = $_FILES[$post_var]['name'];//form_state['input'][$post_var ];
			$url = 'public://bol/' . $filename;
			$url_full = file_create_url($url);
			$path = DRUPAL_ROOT . "/" . substr($url_full, strpos($url_full, 'sites/default/files'));
			$destination = file_destination($path, FILE_EXISTS_RENAME);

			$file = (object) [
				'fid' => null,
				'uid' => $user->uid,
				'filename' => basename($destination),
				'uri' => 'public://bol/' . basename($destination),
				'filemime' => $_FILES[$post_var]['type'],
				'filesize' => $_FILES[$post_var]['size'],
				'status' => FILE_STATUS_PERMANENT,
				'timestamp' => REQUEST_TIME,
			];

			$errors = file_validate_extensions($file, 'jpg pdf jpeg');

			if( ! empty($errors)){

				foreach($errors as $error){
					drupal_set_message($error, 'error', false);
				}

				drupal_goto("accounting/customer/payments/{$form_state['storage']['truck']->truck_nid}");
			}

			move_uploaded_file($_FILES[$post_var]['tmp_name'], $destination);

			file_save($file);
			$file = file_load($file->fid);
			$file->display = true;
			$file->description = basename($destination);

			// delete old files
			if(isset($node->field_billoflading['und'])){
				foreach($node->field_billoflading['und'] as $item){
					$old = file_load($item['fid']);
					if($old !== false) file_delete($old, true);
				}
			}

			$node->field_billoflading['und'] = [get_object_vars($file)];

			try{
				node_save($node);

				drupal_set_message('BOL File uploaded successful');
			} catch(Exception $e){
				drupal_set_message('Error saving the BOL for the Load. Contact with your administrator. Error message: ' . $e->getMessage(), 'error');
			}

			drupal_goto("$redirect/{$form_state['storage']['truck']->truck_nid}");
		}
	}
}

/**
 * Check app bonus from $form_state
 *
 * @return bool
 */
function is_app_bonus(){

	global $user;
	$usr = user_load($user->uid);

	if (isset($usr->field_appbonus['und'][0]['value']) && intval($usr->field_appbonus['und'][0]['value']) == 1)
		return true;

	if (isset($usr->field_customer['und'][0]['target_id']))
	{
		$customer = node_load($usr->field_customer['und'][0]['target_id']);
		if(isset($customer->field_appbonus['und'][0]['value']) && intval($customer->field_appbonus['und'][0]['value']) == 1)
			return true;
	}

	if (isset($usr->field_company['und'][0]['target_id']))
	{
		$company = node_load($usr->field_company['und'][0]['target_id']);
		if(isset($company->field_appbonus['und'][0]['value']) && intval($company->field_appbonus['und'][0]['value']) == 1)
			return true;
	}

	return false;
}

/**
 * Delete a payment
 *
 * @param $payment_id
 * @param $form_state
 *
 * @return bool
 */
function load_accounting_delete_payment($payment_id, &$form_state){
	$node = node_load($payment_id);

	if($node === false) return false;

	if(isset($node->field_loads['und'])){

		$loads = $node->field_loads['und'];

		if(is_array($loads)){
			foreach($loads as $target){

				$load = node_load($target['target_id']);

				if($load === false) continue;

				switch($form_state['storage']['payment']['profile']){
					case 'customer':
						$load->field_paid['und'][0]['value'] = 0;
						break;
					case 'carrier':
						$load->field_customer_paid['und'][0]['value'] = 0;
						break;
				}

				try{
					node_save($load);
				} catch(Exception $e){

				}
			}
		}
	}

	node_delete($payment_id);

	drupal_set_message('Payment ' . $node->title . ' was deleted successful');

	return true;
}

function get_loads_header($hide_confirmations = false){
	$loads_header = [
		'title' => t('4G ID'),
		'miles' => t('Miles'),
		'empty' => t('Empty'),
		'payment' => t('Payment'),
		'rate' => t('Rate'),
		'from' => t('From'),
		'to' => t('To'),
		'delivery' => t('Delivery Date'),
		'created' => t('Created'),
		'dispatcher' => t('Dispatcher'),
	];

	if( ! $hide_confirmations){
		$loads_header['confirmation'] = t('Confirmation');
	}

	$loads_header['bol'] = t('BOL');
	$loads_header['upload'] = '';

	return $loads_header;
}