<?php

/**
 * Implements submit function.
 */
function customer_trucks_form_submit($form, &$form_state){
	$form_state['rebuild'] = true;
}

/**
 * Implements submit function.
 */
function customer_reports_form_submit($form, &$form_state){

	$form_state['rebuild'] = true;
}

/**
 * Implements submit function.
 */
function customer_payments_form_submit($form, &$form_state){

	global $user;
	// SET LOADS AS PAID
	if($form_state['values']['op'] == 'Set As Paid'){
		$form_state['storage']['payment']['place'] = 0;
		$count = 0;
		foreach($form_state['values']['pendloads'] as $key => $value){
			if($value != 0){
				$node = node_load($value);
				$node->field_paid['und'][0]['value'] = 1;
				node_save($node);
				$count ++;
				$form_state['storage']['loads']['paid'][$value] = $form_state['storage']['loads']['pending'][$value];
				unset($form_state['storage']['loads']['pending'][$value]);
			}
		}
		drupal_set_message(t('Loads (' . $count . ') set as paid.'), 'status');
	} // SET LOADS AS PENDING
	elseif($form_state['values']['op'] == 'Set As Pending'){
		$form_state['storage']['payment']['place'] = 0;
		$count = 0;
		foreach($form_state['values']['paidloads'] as $key => $value){
			if($value != 0){
				$node = node_load($value);
				$node->field_paid['und'][0]['value'] = 0;
				node_save($node);
				$count ++;
				$form_state['storage']['loads']['pending'][$value] = $form_state['storage']['loads']['paid'][$value];
				unset($form_state['storage']['loads']['paid'][$value]);
			}
		}
		drupal_set_message(t('Loads (' . $count . ') set as pending.'), 'status');
	} // PLACE PAYMENT FORM
	elseif($form_state['values']['op'] == 'Make Payment'){
		$form_state['storage']['payment']['place'] = 1;
		$form_state['storage']['payment']['miles'] = 10;
		$abort = false;
		$missing = [];
		$count = 0;
		$sumToMiles = 0;
		foreach($form_state['values']['pendloads'] as $key => $value){
			if($value != 0){
				$node = node_load($value);

				$pass = true;

				if(empty($node->field_billoflading)){
					$pass = false;
				}
				elseif( ! isset($node->field_billoflading['und'])) $pass = false;
				elseif( ! isset($node->field_billoflading['und'][0])) $pass = false;
				elseif(empty($node->field_billoflading['und'][0])) $pass = false;
				elseif(empty($node->field_rate_confirmation)) $pass = false;
				elseif( ! isset($node->field_rate_confirmation['und'])) $pass = false;
				elseif( ! isset($node->field_rate_confirmation['und'][0])) $pass = false;
				elseif(empty(($node->field_rate_confirmation['und'][0]))) $pass = false;

				if( ! $pass){
					$abort = true;
					$missing[] = $node->title;
				}

				$count ++;

				$sumToMiles += $node->field_total_miles['und'][0]['value'];
			}
		}

		if($abort){
			drupal_set_message('Please upload BOL for the load(s): ' . implode(', ', $missing) . ' in order to generate the payment', 'error');
			drupal_goto("accounting/customer/payments/{$form_state['storage']['truck']->truck_nid}");

			return false;
		}

		if($count == 0){
			drupal_set_message('One load must be selected at least', 'error');
			drupal_goto("accounting/customer/payments/{$form_state['storage']['truck']->truck_nid}");

			return false;
		}

		$form_state['storage']['payment']['miles'] += $sumToMiles;

		// Rate
		$form_state['storage']['payment']['rate'] = 0;
		$form_state['storage']['payment']['payment'] = 0;
		$form_state['storage']['payment']['accesories'] = 0;
		$form_state['storage']['payment']['total'] = 0;

		switch($form_state['storage']['compensation']['type']){
			case 'Rate x Miles':
				$form_state['storage']['payment']['rate'] = $form_state['storage']['compensation']['value'];
				$form_state['storage']['payment']['payment'] = round($form_state['storage']['compensation']['value'] * $form_state['storage']['payment']['miles'], 2);
				$form_state['storage']['payment']['total'] = $form_state['storage']['payment']['payment'];
				break;
			case 'Weekly Salary':
				$form_state['storage']['payment']['payment'] = $form_state['storage']['compensation']['value'];
				$form_state['storage']['payment']['rate'] = round($form_state['storage']['compensation']['value'] / $form_state['storage']['payment']['miles'], 2);
				$form_state['storage']['payment']['total'] = $form_state['storage']['payment']['payment'];
				break;
			default:
				break;
		}
	} // REGISTER PAYMENT
	elseif($form_state['values']['op'] == 'Save'){
		$form_state['storage']['payment']['place'] = 0;
		$entity = entity_create('node', ['type' => 'payment']);
		$entity->uid = $user->uid;
		$wrap = entity_metadata_wrapper('node', $entity);
		$wrap->title->set(time());
		$wrap->field_payment_date->set(! empty($form_state['values']['fpaymentdate']) ? strtotime($form_state['values']['fpaymentdate']) : time());
		$wrap->field_miles->set($form_state['values']['fmiles']);
		$wrap->field_rate->set($form_state['values']['frate']);
		$wrap->field_payment->set($form_state['values']['fpayment']);
		$wrap->field_access->set($form_state['values']['faccesories']);
		$wrap->field_total->set($form_state['values']['ftotal']);
		$wrap->field_memo->set($form_state['values']['fmemo']);
		$wrap->field_customer->set($form_state['storage']['truck']->customer_nid);
		$wrap->field_single_carrier->set($form_state['storage']['truck']->company_nid);
		$wrap->field_single_driver->set($form_state['storage']['truck']->driver_nid);
		$wrap->field_truck->set($form_state['storage']['truck']->truck_nid);
		$count = 0;

		foreach($form_state['values']['pendloads'] as $key => $value){
			if($value != 0){
				$node = node_load($value);
				$node->field_paid['und'][0]['value'] = 1;
				node_save($node);
				$form_state['storage']['loads']['paid'][$value] = $form_state['storage']['loads']['pending'][$value];
				unset($form_state['storage']['loads']['pending'][$value]);
				$wrap->field_loads[$count]->set($value);
				$count ++;
			}
		}

		if($count > 0){
			$wrap->save();
			drupal_set_message('The payment ' . $wrap->title->value() . ' has been successfully saved.', 'status');
		}
	}

	load_accounting_upload_bol($form_state, 'accounting/customer/payments');

	// delete a payment before load payments
	if(isset($form_state['input']['confirm-dialog-flag']) && $form_state['input']['confirm-dialog-flag'] == 'delete-payment'){
		if(isset($form_state['input']['confirm-dialog-submit']) && ! empty($form_state['input']['confirm-dialog-submit'])){
			$payment_id = $form_state['input']['confirm-dialog-submit'];
			load_accounting_delete_payment($payment_id, $form_state);

			if(isset($form_state['storage']['payment']['list'][$payment_id])){
				unset($form_state['storage']['payment']['list'][$payment_id]);
			}

			drupal_goto("accounting/customer/payments/{$form_state['storage']['truck']->truck_nid}");

			return true;
		}
	}

	// delete bulk payment list
	if(isset($form_state['input']['confirm-dialog-flag']) && $form_state['input']['confirm-dialog-flag'] == 'delete-bulk-payment'){
		if(isset($form_state['input']['confirm-dialog-submit']) && ! empty($form_state['input']['confirm-dialog-submit'])){

			if (isset($form_state['input']['paymentlist']))
				if (is_array($form_state['input']['paymentlist']))
				{
					foreach($form_state['input']['paymentlist'] as $key => $value)
					{
						if (!is_null($value))
						{
							load_accounting_delete_payment($key, $form_state);

							if(isset($form_state['storage']['payment']['list'][$key])){
								unset($form_state['storage']['payment']['list'][$key]);
							}
						}
					}

					drupal_goto("accounting/customer/payments/{$form_state['storage']['truck']->truck_nid}");

					return true;
				}
		}
	}


	$form_state['rebuild'] = true;
}