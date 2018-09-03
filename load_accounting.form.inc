<?php
/**
 * Implements hook_form().
 */
function customer_trucks_form($form, &$form_state){
	// VARIABLES
	global $user;
	$usr    = user_load($user->uid);
	$form   = [];
	$trucks = [];

	// GENERAL REPORT
	$report = [
		'thisweek'  => ['count' => 0, 'total' => 0, 'empty' => 0, 'sold' => 0, 'rate' => 0],
		'lastweek'  => ['count' => 0, 'total' => 0, 'empty' => 0, 'sold' => 0, 'rate' => 0],
		'thismonth' => ['count' => 0, 'total' => 0, 'empty' => 0, 'sold' => 0, 'rate' => 0],
		'lastmonth' => ['count' => 0, 'total' => 0, 'empty' => 0, 'sold' => 0, 'rate' => 0],
		'thisyear'  => ['count' => 0, 'total' => 0, 'empty' => 0, 'sold' => 0, 'rate' => 0],
		'ever'      => ['count' => 0, 'total' => 0, 'empty' => 0, 'sold' => 0, 'rate' => 0],
		'bytype'    => ['reefer' => 0, 'dryvan' => 0, 'flatbed' => 0],
		'bynumber'  => ['reefer' => 0, 'dryvan' => 0, 'flatbed' => 0],
	];

	// DATE RANGES
	$current_day     = date('w');
	$this_week_date  = date('M d Y', strtotime('-' . $current_day . ' days'));
	$this_week       = strtotime($this_week_date);
	$last_week_date  = date('M d Y', strtotime('-7 days', strtotime($this_week_date)));
	$last_week       = strtotime($last_week_date);
	$this_month_date = date('M 01 Y');
	$this_month      = strtotime($this_month_date);
	$month           = new DateTime("first day of last month");
	$last_month_date = $month->format('M d Y');
	$last_month      = strtotime($last_month_date);
	$year            = new DateTime('first day of January');
	$this_year_date  = $year->format('M d Y');
	$this_year       = strtotime($this_year_date);

	// CUSTOMER ASSOCIATION
	if( ! empty($usr->field_customer) && isset($usr->field_customer['und'][0]['target_id'])){
		$result = db_select('truck_dataset', 't')->fields('t')->condition('customer_nid', $usr->field_customer['und'][0]['target_id'], '=')->execute()->fetchAll();

		foreach($result as $truck){
			// AVOID ANALIZING A TRUCK TWICE
			if( ! isset($trucks[$truck->truck_nid])){
				// TRUCK INFO
				$trucks[$truck->truck_nid] = [
					'plate'      => $truck->truck_plate,
					'equipment'  => $truck->truck_number,
					'state'      => $truck->truck_home,
					'type'       => $truck->trailer_type,
					'customer'   => $truck->customer_name,
					'carrier'    => $truck->company_name,
					'driver'     => $truck->driver_name,
					'contact'    => $truck->driver_phone,
					'dispatcher' => $truck->dispatcher_name,
				];

				// TRUCK LOADS
				$trucks[$truck->truck_nid]['loads'] = get_truck_loads($truck->truck_nid);
				// REPORT
				$trucks[$truck->truck_nid]['report'] = get_loads_report($trucks[$truck->truck_nid]['loads'], $this_week, $last_week, $this_month, $last_month, $this_year);

				// INCOME BY TYPE
				if($trucks[$truck->truck_nid]['type'] == 'Reefer'){
					$report['bytype']['reefer'] += $trucks[$truck->truck_nid]['report']['ever']['sold'];
					$report['bynumber']['reefer'] ++;
				}
				elseif($trucks[$truck->truck_nid]['type'] == 'Dry Van'){
					$report['bytype']['dryvan'] += $trucks[$truck->truck_nid]['report']['ever']['sold'];
					$report['bynumber']['dryvan'] ++;
				}
				elseif($trucks[$truck->truck_nid]['type'] == 'Flat Bed'){
					$report['bytype']['flatbed'] += $trucks[$truck->truck_nid]['report']['ever']['sold'];
					$report['bynumber']['flatbed'] ++;
				}
				// UPDATE GENERAL REPORT
				$report['thisweek']['count']  += $trucks[$truck->truck_nid]['report']['thisweek']['count'];
				$report['lastweek']['count']  += $trucks[$truck->truck_nid]['report']['lastweek']['count'];
				$report['thismonth']['count'] += $trucks[$truck->truck_nid]['report']['thismonth']['count'];
				$report['lastmonth']['count'] += $trucks[$truck->truck_nid]['report']['lastmonth']['count'];
				$report['thisyear']['count']  += $trucks[$truck->truck_nid]['report']['thisyear']['count'];
				$report['ever']['count']      += $trucks[$truck->truck_nid]['report']['ever']['count'];

				$report['thisweek']['total']  += $trucks[$truck->truck_nid]['report']['thisweek']['total'];
				$report['lastweek']['total']  += $trucks[$truck->truck_nid]['report']['lastweek']['total'];
				$report['thismonth']['total'] += $trucks[$truck->truck_nid]['report']['thismonth']['total'];
				$report['lastmonth']['total'] += $trucks[$truck->truck_nid]['report']['lastmonth']['total'];
				$report['thisyear']['total']  += $trucks[$truck->truck_nid]['report']['thisyear']['total'];
				$report['ever']['total']      += $trucks[$truck->truck_nid]['report']['ever']['total'];

				$report['thisweek']['empty']  += $trucks[$truck->truck_nid]['report']['thisweek']['empty'];
				$report['lastweek']['empty']  += $trucks[$truck->truck_nid]['report']['lastweek']['empty'];
				$report['thismonth']['empty'] += $trucks[$truck->truck_nid]['report']['thismonth']['empty'];
				$report['lastmonth']['empty'] += $trucks[$truck->truck_nid]['report']['lastmonth']['empty'];
				$report['thisyear']['empty']  += $trucks[$truck->truck_nid]['report']['thisyear']['empty'];
				$report['ever']['empty']      += $trucks[$truck->truck_nid]['report']['ever']['empty'];

				$report['thisweek']['sold']  += $trucks[$truck->truck_nid]['report']['thisweek']['sold'];
				$report['lastweek']['sold']  += $trucks[$truck->truck_nid]['report']['lastweek']['sold'];
				$report['thismonth']['sold'] += $trucks[$truck->truck_nid]['report']['thismonth']['sold'];
				$report['lastmonth']['sold'] += $trucks[$truck->truck_nid]['report']['lastmonth']['sold'];
				$report['thisyear']['sold']  += $trucks[$truck->truck_nid]['report']['thisyear']['sold'];
				$report['ever']['sold']      += $trucks[$truck->truck_nid]['report']['ever']['sold'];
			}
		}
	}

	// COMPANIES
	if( ! empty($usr->field_company) && count($usr->field_company['und']) > 0){
		foreach($usr->field_company['und'] as $key => $value){
			$result = db_select('truck_dataset', 't')->fields('t')->condition('company_nid', $value['target_id'], '=')->execute()->fetchAll();

			foreach($result as $truck){
				// AVOID ANALIZING A TRUCK TWICE
				if( ! isset($trucks[$truck->truck_nid])){
					// TRUCK INFO
					$trucks[$truck->truck_nid] = [
						'plate'      => $truck->truck_plate,
						'equipment'  => $truck->truck_number,
						'state'      => $truck->truck_home,
						'type'       => $truck->trailer_type,
						'customer'   => $truck->customer_name,
						'carrier'    => $truck->company_name,
						'driver'     => $truck->driver_name,
						'contact'    => $truck->driver_phone,
						'dispatcher' => $truck->dispatcher_name,
					];
					// TRUCK LOADS
					$trucks[$truck->truck_nid]['loads'] = get_truck_loads($truck->truck_nid);
					// REPORT
					$trucks[$truck->truck_nid]['report'] = get_loads_report($trucks[$truck->truck_nid]['loads'], $this_week, $last_week, $this_month, $last_month, $this_year);
					// INCOME BY TYPE
					if($trucks[$truck->truck_nid]['type'] == 'Reefer'){
						$report['bytype']['reefer'] += $trucks[$truck->truck_nid]['report']['ever']['sold'];
						$report['bynumber']['reefer'] ++;
					}
					elseif($trucks[$truck->truck_nid]['type'] == 'Dry Van'){
						$report['bytype']['dryvan'] += $trucks[$truck->truck_nid]['report']['ever']['sold'];
						$report['bynumber']['dryvan'] ++;
					}
					elseif($trucks[$truck->truck_nid]['type'] == 'Flat Bed'){
						$report['bytype']['flatbed'] += $trucks[$truck->truck_nid]['report']['ever']['sold'];
						$report['bynumber']['flatbed'] ++;
					}
					// UPDATE GENERAL REPORT
					$report['thisweek']['count']  += $trucks[$truck->truck_nid]['report']['thisweek']['count'];
					$report['lastweek']['count']  += $trucks[$truck->truck_nid]['report']['lastweek']['count'];
					$report['thismonth']['count'] += $trucks[$truck->truck_nid]['report']['thismonth']['count'];
					$report['lastmonth']['count'] += $trucks[$truck->truck_nid]['report']['lastmonth']['count'];
					$report['thisyear']['count']  += $trucks[$truck->truck_nid]['report']['thisyear']['count'];
					$report['ever']['count']      += $trucks[$truck->truck_nid]['report']['ever']['count'];

					$report['thisweek']['total']  += $trucks[$truck->truck_nid]['report']['thisweek']['total'];
					$report['lastweek']['total']  += $trucks[$truck->truck_nid]['report']['lastweek']['total'];
					$report['thismonth']['total'] += $trucks[$truck->truck_nid]['report']['thismonth']['total'];
					$report['lastmonth']['total'] += $trucks[$truck->truck_nid]['report']['lastmonth']['total'];
					$report['thisyear']['total']  += $trucks[$truck->truck_nid]['report']['thisyear']['total'];
					$report['ever']['total']      += $trucks[$truck->truck_nid]['report']['ever']['total'];

					$report['thisweek']['empty']  += $trucks[$truck->truck_nid]['report']['thisweek']['empty'];
					$report['lastweek']['empty']  += $trucks[$truck->truck_nid]['report']['lastweek']['empty'];
					$report['thismonth']['empty'] += $trucks[$truck->truck_nid]['report']['thismonth']['empty'];
					$report['lastmonth']['empty'] += $trucks[$truck->truck_nid]['report']['lastmonth']['empty'];
					$report['thisyear']['empty']  += $trucks[$truck->truck_nid]['report']['thisyear']['empty'];
					$report['ever']['empty']      += $trucks[$truck->truck_nid]['report']['ever']['empty'];

					$report['thisweek']['sold']  += $trucks[$truck->truck_nid]['report']['thisweek']['sold'];
					$report['lastweek']['sold']  += $trucks[$truck->truck_nid]['report']['lastweek']['sold'];
					$report['thismonth']['sold'] += $trucks[$truck->truck_nid]['report']['thismonth']['sold'];
					$report['lastmonth']['sold'] += $trucks[$truck->truck_nid]['report']['lastmonth']['sold'];
					$report['thisyear']['sold']  += $trucks[$truck->truck_nid]['report']['thisyear']['sold'];
					$report['ever']['sold']      += $trucks[$truck->truck_nid]['report']['ever']['sold'];
				}
			}
		}
	}

	// EVER RATE
	if($report['ever']['sold'] != 0 && $report['ever']['total'] != 0){
		$rate                   = $report['ever']['sold'] / $report['ever']['total'];
		$report['ever']['rate'] = ! is_nan($rate) ? round($rate, 2) : 0;
	}

	// YEAR RATE
	if($report['thisyear']['sold'] != 0 && $report['thisyear']['total'] != 0){
		$rate                       = $report['thisyear']['sold'] / $report['thisyear']['total'];
		$report['thisyear']['rate'] = ! is_nan($rate) ? round($rate, 2) : 0;
	}

	// LAST MONTH RATE
	if($report['lastmonth']['sold'] != 0 && $report['lastmonth']['total'] != 0){
		$rate                        = $report['lastmonth']['sold'] / $report['lastmonth']['total'];
		$report['lastmonth']['rate'] = ! is_nan($rate) ? round($rate, 2) : 0;
	}

	// THIS MONTH RATE
	if($report['thismonth']['sold'] != 0 && $report['thismonth']['total'] != 0){
		$rate                        = $report['thismonth']['sold'] / $report['thismonth']['total'];
		$report['thismonth']['rate'] = ! is_nan($rate) ? round($rate, 2) : 0;
	}

	// LAST WEEK MONTH RATE
	if($report['lastweek']['sold'] != 0 && $report['lastweek']['total'] != 0){
		$rate                       = $report['lastweek']['sold'] / $report['lastweek']['total'];
		$report['lastweek']['rate'] = ! is_nan($rate) ? round($rate, 2) : 0;
	}

	// THIS WEEK RATE
	if($report['thisweek']['sold'] != 0 && $report['thisweek']['total'] != 0){
		$rate                       = $report['thisweek']['sold'] / $report['thisweek']['total'];
		$report['thisweek']['rate'] = ! is_nan($rate) ? round($rate, 2) : 0;
	}

	// Form
	$form['my_trucks'] = [
		'#markup' => theme('my_trucks', ['report' => $report, 'trucks' => $trucks]),
	];

	return $form;
}


/**
 * Implements hook_form().
 */
function customer_reports_form($form, &$form_state){
	// check if authorized truck
	// add filter to a view to hide unpublish loads
	// Pendiente validacion de mostrar y ocultar doc

	$allow = 0;
	if(is_numeric(arg(3))){
		$allow = 1;
	}

	// PROCESS
	if($allow){

		// DATE RANGES
		$current_day     = date('w');
		$this_week_date  = date('M d Y', strtotime('-' . $current_day . ' days'));
		$this_week       = strtotime($this_week_date);
		$last_week_date  = date('M d Y', strtotime('-7 days', strtotime($this_week_date)));
		$last_week       = strtotime($last_week_date);
		$this_month_date = date('M 01 Y');
		$this_month      = strtotime($this_month_date);
		$month           = new DateTime("first day of last month");
		$last_month_date = $month->format('M d Y');
		$last_month      = strtotime($last_month_date);
		$year            = new DateTime('first day of January');
		$this_year_date  = $year->format('M d Y');
		$this_year       = strtotime($this_year_date);

		$truck = db_select('truck_dataset', 't')->fields('t')->condition('truck_nid', round(arg(3)), '=')->execute()->fetchObject();

		$loads  = get_truck_loads($truck->truck_nid);
		$report = get_loads_report($loads, $this_week, $last_week, $this_month, $last_month, $this_year);

		load_accounting_save_truck_nickname($form_state);

		$truck_node                     = node_load($truck->truck_nid);
		$truck->truck_nickname          = $truck_node->field_nickname['und'][0]['value'];
		$form_state['storage']['truck'] = $truck;

		$form["#attributes"]['enctype'] = 'multipart/form-data';

		$form['truck_profile'] = [
			'#markup' => theme('truck_profile', ['truck' => ['truck' => $truck]]),
		];

		// Truck Comparison Report
		$form['comparison_report'] = [
			'#markup' => theme('comparison_report', ['report' => $report]),
		];

		// TODO: no se ejecuto en customer_reports_form_submit, deberia estar alli
		load_accounting_upload_bol($form_state, 'accounting/customer/reports');

		$hide_confirmations = is_app_bonus($form_state);

		// Load List
		foreach($loads as $key => $load){
			$is_paid   = false;
			$load_data = get_load_data($load, $is_paid, true);
			if( ! $hide_confirmations) $loads[$key]['confirmation'] = $load_data['confirmation'];
			$loads[$key]['bol']    = $load_data['bol'];
			$loads[$key]['upload'] = $load_data['upload'];
		}

		$form['load_record'] = [
			'#markup' => theme('load_record', ['loads' => $loads, 'hide_confirmations' => $hide_confirmations]),
		];

		return $form;
	}
	else{
		drupal_set_message(t('You are not authorized to access this report.'), 'error');
		// redirect
	}

	return [];
}

/**
 * Implements hook_form().
 * @throws Exception
 */
function customer_payments_form($form, &$form_state){

	// Truck Information ($form_state['storage']['truck'])
	if( ! isset($form_state['storage']['truck'])){
		if(is_numeric(arg(3))){
			$result = db_select('truck_dataset', 't')->fields('t')->condition('truck_nid', round(arg(3)), '=')->execute()->fetchObject();
			if( ! empty($result)){
				$form_state['storage']['truck'] = $result;
			}
			else{
				drupal_set_message(t('There is no information available.'), 'error');
				drupal_goto('accounting/customer/trucks');
			}
		}
	}

	$profile = null;

	// Payment Profile ($form_state['storage']['payment']['profile'])
	if( ! isset($form_state['storage']['payment']['profile'])){
		$profile                                     = get_payment_profile($form_state['storage']['truck']);
		$form_state['storage']['payment']['profile'] = $profile;
	}
	else{
		$profile = $form_state['storage']['payment']['profile'];
	}

	$form_state['storage']['truck']->profile = $profile;

	$hide_confirmations = is_app_bonus($form_state);

	// Set Compensation Type ($form_state['storage']['compensation'])
	if($form_state['storage']['payment']['profile'] == 'customer'){

		load_accounting_save_truck_nickname($form_state);

		if( ! isset($form_state['storage']['compensation']) || (isset($form_state['input']['btnSaveCompensation']) && $form_state['submitted']) && $form_state['input']['btnSaveCompensation'] == 'save-driver-compensation'){

			$comp_type  = 'Rate x Miles';
			$comp_value = 0.5;

			$driver = node_load($form_state['storage']['truck']->driver_nid);

			if($driver->type == 'driver'){

				// Compensation Type
				if(isset($driver->field_compensation['und'][0]['value'])){
					$comp_type = $driver->field_compensation['und'][0]['value'];
				}

				if(isset($form_state['input']['driver-compensation-type'])){
					$comp_type = $form_state['input']['driver-compensation-type'];
					if($form_state['submitted']){
						drupal_set_message('The compensation type was updated as <i>' . $comp_type . '</i>');
					}
				}

				// Compensation Value
				if(isset($driver->field_compensation_value['und'][0]['value'])){
					$comp_value = $driver->field_compensation_value['und'][0]['value'];
				}

				if(isset($form_state['input']['driver-compensation-value'])){
					$comp_value = $form_state['input']['driver-compensation-value'] * 1;
					if($form_state['submitted']){
						drupal_set_message('The compensation value was updated as <i>' . $comp_value . '</i>');
					}
				}

				$driver->field_compensation['und'][0]['value']       = $comp_type;
				$driver->field_compensation_value['und'][0]['value'] = $comp_value;

				try{
					node_save($driver);
				} catch(Exception $e){
					drupal_set_message('Internal error saving compensation. Contact with your administrator.', 'error');
				}
			}

			$form_state['storage']['compensation']['type']  = $comp_type;
			$form_state['storage']['compensation']['value'] = $comp_value;
		}
	}

	// Load Analisys ($form_state['storage']['loads']['pending'], $form_state['storage']['load']['paid'])
	if($profile != 'unknown'){
		if( ! isset($form_state['storage']['loads'])){
			$form_state['storage']['loads']['pending'] = [];
			$form_state['storage']['loads']['paid']    = [];
			$loads                                     = get_truck_loads($form_state['storage']['truck']->truck_nid);
			foreach($loads as $load){
				if($load['status'] == 'Completed'){
					$is_paid   = false;
					$load_data = get_load_data($load, $is_paid);

					if($hide_confirmations){
						unset($load_data['confirmation']);
					}

					// Pending Loads
					if( ! $is_paid){
						$form_state['storage']['loads']['pending'][$load['nid']] = $load_data;
					}
					else{
						$form_state['storage']['loads']['paid'][$load['nid']] = $load_data;
					}
				}
			}
		}
	} // Unknown Profile
	else{
		drupal_set_message(t('You are not authorized to access this page.'), 'error');
		drupal_goto('accounting/customer/trucks');
	}

	// Header Load Panels ($form_state['storage']['loads']['header'])
	if( ! isset($form_state['storage']['loads']['header'])){
		$loads_header = [
			'title'      => t('4G ID'),
			'miles'      => t('Miles'),
			'empty'      => t('Empty'),
			'payment'    => t('Payment'),
			'rate'       => t('Rate'),
			'from'       => t('From'),
			'to'         => t('To'),
			'delivery'   => t('Delivery Date'),
			'created'    => t('Created'),
			'dispatcher' => t('Dispatcher'),
		];

		if( ! $hide_confirmations){
			$loads_header['confirmation'] = t('Confirmation');
		}

		$loads_header['bol']    = t('BOL');
		$loads_header['upload'] = '';

		$form_state['storage']['loads']['header'] = $loads_header;
	}

	// Header Payment Panels ($form_state['storage']['loads']['header'])
	if( ! isset($form_state['storage']['payment']['header'])){
		$form_state['storage']['payment']['header'] = [
			'date'        => t('Date'),
			'reference'   => t('Reference'),
			'miles'       => t('Miles'),
			'rate'        => t('Rate'),
			'payment'     => t('Payment'),
			'accessories' => t('Accessories'),
			'total'       => t('Total'),
			'memo'        => t('Memo'),
			// 'carrier' => t('Carrier'),
			// 'customer' => t('Customer'),
			// 'truck' => t('Truck'),
			// 'driver' => t('Driver'),
			// 'loads' => t('Loads'),
		];
	}

	// Payment List ($form_state['storage']['payment']['list'])
	if( ! isset($form_state['storage']['payment']['list'])){
		$form_state['storage']['payment']['list'] = [];
	}

	$payments = db_select('field_data_field_truck', 't')->fields('t', ['entity_id'])->condition('bundle', 'payment', '=')->condition('field_truck_target_id', $form_state['storage']['truck']->truck_nid, '=')->execute()->fetchAll();

	foreach(array_reverse($payments) as $payment){
		$p                                                             = node_load($payment->entity_id);
		$form_state['storage']['payment']['list'][$payment->entity_id] = [
			'date'        => date('M d Y', $p->created),
			'reference'   => $p->title,
			'miles'       => $p->field_miles['und'][0]['value'],
			'rate'        => $p->field_rate['und'][0]['value'],
			'payment'     => $p->field_payment['und'][0]['value'],
			'accessories' => $p->field_access['und'][0]['value'],
			'total'       => $p->field_total['und'][0]['value'],
			'memo'        => $p->field_memo['und'][0]['value'],
			// 'carrier' => $p->field_miles['und'][0],
			// 'customer' => $p->field_miles['und'][0],
			// 'truck' => $p->field_miles['und'][0],
			// 'driver' => $p->field_miles['und'][0],
			// 'loads' => $p->field_miles['und'][0],
		];
	}

	// Form
	if( ! empty($form_state['storage']['loads'])){
		// Set Payment Information
		if( ! isset($form_state['storage']['payment']['place'])){
			$form_state['storage']['payment']['place'] = 0;
		}
		$form_state['storage']['payment']['to'] = $form_state['storage']['truck']->customer_name;
		if($form_state['storage']['truck']->profile == 'customer'){
			$form_state['storage']['payment']['to'] = $form_state['storage']['truck']->driver_name;
		}

		// Form Markup
		$form = [];

		$form["#attributes"]['enctype'] = 'multipart/form-data';
		$truck                          = $form_state['storage'];
		$truck_node                     = node_load($form_state['storage']['truck']->truck_nid);
		$truck['truck']->truck_nickname = $truck_node->field_nickname['und'][0]['value'];
		$form['truck_profile']          = [
			'#markup' => theme('truck_profile', [
				'truck'      => $truck,
				'comp_types' => [
					'Rate x Miles'  => 'Rate x Miles',
					'Weekly Salary' => 'Weekly Salary',
				],
			]),
		];

		// Show Payment Form
		if($form_state['storage']['payment']['place']){
			if (isset($form_state['input']['_triggering_element_name']))
			{
				// Ajax Rate Event
				if($form_state['input']['_triggering_element_name'] == 'frate'){
					$form_state['storage']['payment']['rate']    = $form_state['values']['frate'];
					$newpayment                                  = round($form_state['storage']['payment']['miles'] * $form_state['values']['frate'], 2);
					$form_state['storage']['payment']['payment'] = $newpayment;
					$form_state['storage']['payment']['total']   = $newpayment + $form_state['values']['faccesories'];
					unset($form_state['input']['fpayment']);
					unset($form_state['input']['ftotal']);
				}

				// Ajax Payment Event
				if($form_state['input']['_triggering_element_name'] == 'fpayment'){
					$form_state['storage']['payment']['payment'] = $form_state['values']['fpayment'];
					$newrate                                     = round($form_state['values']['fpayment'] / $form_state['storage']['payment']['miles'], 2);
					$form_state['storage']['payment']['rate']    = $newrate;
					$form_state['storage']['payment']['total']   = $form_state['values']['fpayment'] + $form_state['values']['faccesories'];
					unset($form_state['input']['frate']);
					unset($form_state['input']['ftotal']);
				}

				// Ajax Payment Event
				if($form_state['input']['_triggering_element_name'] == 'faccesories'){
					$form_state['storage']['payment']['accesories'] = $form_state['values']['faccesories'];
					$newtotal                                       = $form_state['values']['faccesories'] + $form_state['storage']['payment']['payment'];
					$form_state['storage']['payment']['total']      = $newtotal;
					unset($form_state['input']['ftotal']);
				}
			}

			// Form
			$form['place'] = [
				'#type'       => 'container',
				'#prefix'     => '<div id="place_wrap" class="row" style="margin-top:20px;"><div class="col col-sm-12 col-md-12"><div class="panel panel-danger"><div class="panel-heading" style="color:#a94442;"><b> Make a Payment</b></div><div class="panel-body" style="background-color:#a94442; color:#FFF;"><div class="row"">',
				'#suffix'     => '</div></div></div></div></div>',
				'fmiles'      => [
					'#title'         => 'Total Miles',
					'#type'          => 'textfield',
					'#disabled'      => true,
					'#default_value' => isset($form_state['storage']['payment']['miles']) ? $form_state['storage']['payment']['miles'] : 0,
					'#prefix'        => '<div class="col col-sm-12 col-md-1">',
					'#suffix'        => '</div>',
				],
				'frate'       => [
					'#title'         => 'Rate',
					'#type'          => 'textfield',
					'#default_value' => isset($form_state['storage']['payment']['rate']) ? $form_state['storage']['payment']['rate'] : 0,
					'#ajax'          => [
						'callback' => 'place_wrap_call',
						'wrapper'  => 'place_wrap',
						'effect'   => 'fade',
					],
					'#prefix'        => '<div class="col col-sm-12 col-md-1">',
					'#suffix'        => '</div>',
				],
				'fpayment'    => [
					'#title'         => 'Payment',
					'#type'          => 'textfield',
					'#default_value' => isset($form_state['storage']['payment']['payment']) ? $form_state['storage']['payment']['payment'] : 0,
					'#ajax'          => [
						'callback' => 'place_wrap_call',
						'wrapper'  => 'place_wrap',
						'effect'   => 'fade',
					],
					'#prefix'        => '<div class="col col-sm-12 col-md-1">',
					'#suffix'        => '</div>',
				],
				'faccesories' => [
					'#title'         => 'Accesories',
					'#type'          => 'textfield',
					'#default_value' => isset($form_state['storage']['payment']['accesories']) ? $form_state['storage']['payment']['accesories'] : 0,
					'#ajax'          => [
						'callback' => 'place_wrap_call',
						'wrapper'  => 'place_wrap',
						'effect'   => 'fade',
						'method'   => 'replace',
					],
					'#prefix'        => '<div class="col col-sm-12 col-md-1">',
					'#suffix'        => '</div>',
				],
				'ftotal'      => [
					'#title'         => 'Total',
					'#type'          => 'textfield',
					'#disabled'      => true,
					'#default_value' => isset($form_state['storage']['payment']['total']) ? $form_state['storage']['payment']['total'] : 0,
					'#prefix'        => '<div class="col col-sm-12 col-md-1">',
					'#suffix'        => '</div>',
				],
				'fmemo'       => [
					'#title'         => 'Memo',
					'#type'          => 'textfield',
					'#default_value' => isset($form_state['storage']['payment']['memo']) ? $form_state['storage']['payment']['memo'] : '',
					'#prefix'        => '<div class="col col-sm-12 col-md-6">',
					'#suffix'        => '</div>',
				],
				'fcalculate'  => [
					'#type'   => 'submit',
					'#value'  => t('Save'),
					'#prefix' => '<div class="col col-sm-12 col-md-1" style="padding-top:24px;">',
					'#suffix' => '</div>',
				],
			];
		}

		// Pending Loads
		$form['pending'] = [
			'#type'     => 'container',
			'#prefix'   => '<div class="row" style="margin-top:20px;"><div class="col col-sm-12">',
			'#suffix'   => '</div></div>',
			'pendloads' => [
				'#type'    => 'tableselect',
				'#header'  => $form_state['storage']['loads']['header'],
				'#options' => $form_state['storage']['loads']['pending'],
				'#empty'   => t('No pending loads.'),
				'#prefix'  => '<div class="panel panel-danger">
								<div class="panel-heading">
									<a data-toggle="collapse" data-target="#PendingPanel" style="text-decoration:none; color:#a94442; cursor:pointer;">' . '<span class="badge">' . count($form_state['storage']['loads']['pending']) . '</span>' . '<b> Pending Payments --- ' . $form_state['storage']['payment']['to'] . '</b>' . '</a>' . '</div>' . '<div id="PendingPanel" class="panel-collapse collapse in">' . '<div class="panel-body" style="max-height: 300px; overflow-y: scroll;">',
				'#suffix'  => '</div></div>',
			],
			'setaspaid' => [
				'#type'   => 'submit',
				'#value'  => t('Set As Paid'),
				'#prefix' => '<div class="panel-footer">',
			],
			'payment'   => [
				'#type'   => 'submit',
				'#value'  => t('Make Payment'),
				'#suffix' => '</div></div>',

			],
		];

		// Paid Loads
		$form['paid'] = [
			'#type'        => 'container',
			'#prefix'      => '<div class="row" style="margin-top:20px;"><div class="col col-sm-12">',
			'#suffix'      => '</div></div>',
			'paidloads'    => [
				'#type'    => 'tableselect',
				'#header'  => $form_state['storage']['loads']['header'],
				'#options' => $form_state['storage']['loads']['paid'],
				'#empty'   => t('No paid loads.'),
				'#prefix'  => '<div class="panel panel-success"><div class="panel-heading"><a data-toggle="collapse" data-target="#PaidPanel" style="text-decoration:none; color:#3c763d; cursor:pointer;"><span class="badge">' . count($form_state['storage']['loads']['paid']) . '</span><b> Paid Loads </b></a></div><div id="PaidPanel" class="panel-collapse collapse"><div class="panel-body" style="max-height: 500px; overflow-y: scroll;">',
			],
			'setaspending' => [
				'#type'   => 'submit',
				'#value'  => t('Set As Pending'),
				'#suffix' => '</div></div></div>',
			],
		];

		// Payment List
		$form['payment'] = [
			'#type'       => 'container',
			'#prefix'     => '<div class="row" style="margin-top:20px;"><div class="col col-sm-12">',
			'#suffix'     => '</div></div>',
			'paymentlist' => [
				'#type'    => 'tableselect',
				'#header'  => $form_state['storage']['payment']['header'],
				'#options' => $form_state['storage']['payment']['list'],
				'#empty'   => t('No payment record.'),
				'#prefix'  => '<div class="panel panel-success"><div class="panel-heading"><a data-toggle="collapse" data-target="#PaymentsRecord" style="text-decoration:none; color:#3c763d; cursor:pointer;"><span class="badge">' . count($form_state['storage']['payment']['list']) . '</span><b> Payments Record</b></a></div><div id="PaymentsRecord" class="panel-collapse collapse"><div class="panel-body" style="max-height: 300px; overflow-y: scroll;">',
				'#suffix'  => '</div></div></div>',
			],
		];


		return $form;
	}
	else{
		drupal_set_message(t('There is no loads associated to this truck.'), 'error');
		drupal_goto('accounting/customer/trucks');
	}
}

