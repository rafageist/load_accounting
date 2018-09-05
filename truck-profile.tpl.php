<?php
  drupal_add_css(drupal_get_path('module', 'load_accounting') . '/css/load_accounting.css');
  global $base_path;
  $truck_info = isset($truck['truck']) ? $truck['truck'] : array();
  $compesation_info = isset($truck['compensation']) ? $truck['compensation'] : array();
?>

<!-- Truck Information -->
<div class="row">
  <!-- General Information-->
  <div class="col col-sm-4">
    <div class="panel panel-danger">
      <div class="panel-heading">        
        <b> General Information</b>
      </div>
      <table class="table">
        <thead>                    
          <tr>
            <th>Customer</th>
            <th>Carrier</th>
            <th>Dispatcher</th>
           </tr>
        </thead> 
        <tbody>                       
          <tr>
            <td><?php print $truck_info->customer_name; ?></td>
            <td><?php print $truck_info->company_name; ?></td>
            <td><?php print $truck_info->dispatcher_name; ?></td>
          </tr>
        </tbody>
      </table>
    </div>    
  </div>
   <!-- Driver Information-->
  <div class="col col-sm-4">
    <div class="panel panel-danger">
      <div class="panel-heading">        
         <b> Driver Information</b>
        <?php if(isset($truck_info->profile) && $truck_info->profile == 'customer'): ?>
          <a id="btnChangeCompensation" href="#">
              <span class="glyphicon glyphicon-edit" aria-hidden="true"></span>
          </a>
        <?php endif; ?>
      </div>

      <table class="table">
        <thead>                    
          <tr>
            <th>Driver</th>
            <th>Contact</th>
            <?php if(isset($truck_info->profile) && $truck_info->profile == 'customer') { ?> <th>Compensation</th> <?php } ?>
          </tr>
        </thead> 
        <tbody>                       
          <tr>
            <td><?php print $truck_info->driver_name; ?></td>
            <td><?php print $truck_info->driver_phone; ?></td>
            <?php if(isset($truck_info->profile) && $truck_info->profile == 'customer') { ?>
              <td>
                 <?php print $compesation_info['type'] . ' - ' . $compesation_info['value'];?>
                  <div id="miniform-compensation" role="dialog" tabindex="-1" class="modal fade in">
                      <div class="modal-dialog modal-sm" role="document">
                          <div class="modal-content">
                              <div class="modal-header">
                                  <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">×</span></button>
                                  <h4 class="modal-title"><?php echo t('Change Driver\'s compensation'); ?></h4></div>
                                  <div class="modal-body">
                                      <div class="form-group">
                                          <label class="control-label"><?php echo t('Type'); ?></label>
                                          <select class="form-control" name="driver-compensation-type" id="driver-compensation-type">
                                              <?php foreach($comp_types as $comp_type => $caption): ?>
                                              <option value="<?php echo $comp_type; ?>" <?php if ($truck['compensation']['type'] == $comp_type) print "selected"; ?>><?php print $caption; ?></option>
                                              <?php endforeach; ?>
                                          </select>
                                      </div>
                                      <div class="form-group">
                                          <label class="control-label"><?php echo t('Value'); ?></label>
                                          <input type="numeric" class="form-control" name="driver-compensation-value" id="driver-compensation-value" value="<?php print $truck['compensation']['value']; ?>" />
                                      </div>
                                  </div>

                                  <div class="modal-footer">
                                      <button class="btn btn-default" type="button" data-dismiss="modal"><?php echo t('Close'); ?></button>
                                      <button class ="btn btn-primary" type="submit" name="btnSaveCompensation" value="save-driver-compensation"><?php echo t('Save'); ?></button>
                                  </div>
                          </div>
                      </div>
                  </div>

              </td>
            <?php } ?>
          </tr>
        </tbody>
      </table>
    </div>    
  </div>  
  <!-- Truck Information -->
  <div class="col col-sm-4">
    <div class="panel panel-danger">
      <div class="panel-heading">        
        <b> Truck Information </b>
          <a id="btnChangeNickname" href="#" >
              <span class="glyphicon glyphicon-edit" aria-hidden="true"></span>
          </a>
      </div>
      <table class="table">
        <thead>
          <tr>
            <th>Plate</th>
            <th>Equipment</th>
            <th>Nickname</th>
            <th>Status</th>
            <th>State</th>
            <th>Type</th>
          </tr>
        </thead> 
        <tbody>                       
          <tr>
            <td><?php print $truck_info->truck_plate; ?></td>
            <td><?php print $truck_info->truck_number; ?></td>
            <td>
               <?php print empty($truck_info->truck_nickname) ? '--': $truck_info->truck_nickname; ?>
                <div id="miniform-nickname" role="dialog" tabindex="-1" class="modal fade in">
                    <div class="modal-dialog modal-sm" role="document">
                        <div class="modal-content">
                            <div class="modal-header">
                                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">×</span></button>
                                <h4 class="modal-title"><?php echo t('Change Truck\'s nickname'); ?></h4></div>
                            <div class="modal-body">
                                <div class="form-group">
                                    <label class="control-label"><?php echo t('Nickname'); ?></label>
                                    <input maxlength="30" type="text" class="form-control" name="truck-nickname" id="truck-nickname" value="<?php print $truck_info->truck_nickname; ?>"/>
                                </div>
                            </div>
                            <div class="modal-footer">
                                <button class="btn btn-default" type="button" data-dismiss="modal"><?php echo t('Close'); ?></button>
                                <button class ="btn btn-primary" type="submit" name="btnSaveTruckNickname" value="save-truck-nickname"><?php echo t('Save'); ?></button>
                            </div>
                        </div>
                    </div>
                </div>
            </td>
            <td><?php print $truck_info->truck_status; ?></td>
            <td><?php print $truck_info->truck_home; ?></td>
            <td><?php print $truck_info->trailer_type; ?></td>
          </tr>
        </tbody>
      </table>
    </div>
  </div>  
</div>

<?php  drupal_add_js(drupal_get_path('module', 'load_accounting') . '/js/load_accounting.js'); ?>

<!-- confirm modal dialog -->
<div id="confirm-dialog" tabindex="-1" class="modal fade in">
    <div class="modal-dialog modal-sm" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">×</span></button>
                <h4 class="modal-title" id="confirm-title"></h4></div>
            <div class="modal-body" id="confirm-text">
            </div>
            <div class="modal-footer">
                <input type="hidden" name = 'confirm-dialog-flag' id= 'confirm-dialog-flag'>
                <button class ="btn btn-primary" type="submit" name="confirm-dialog-submit" id="confirm-dialog-submit" value=""><?php echo t('Yes'); ?></button>
                <button class="btn btn-default" type="button" data-dismiss="modal"><?php echo t('No'); ?></button>
            </div>
        </div>
    </div>
</div>

<!-- payment edit form -->
<div role="dialog" tabindex="-1" class="modal fade in" id="payment-edit-form">
    <div class="modal-dialog" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">×</span></button>
                <h4 id="payment-reference" class="modal-title">Payment #</h4></div>
            <div class="modal-body">
                <input type="hidden" name="payment-id" class="form-control" id="payment-id" />
                <div class="row">
                    <div class="col col-lg-6">
                        <div class="form-group">
                            <label for="payment-miles" class="control-label"><?php echo t('Miles'); ?>: </label>
                            <input type="text" name="payment-miles" class="form-control" id="payment-miles" />
                        </div>
                    </div>
                    <div class="col col-lg-6">
                        <div class="form-group">
                            <label for="payment-payment" class="control-label"><?php echo t('Payment'); ?>: </label>
                            <input type="text" name="payment-payment" class="form-control" id="payment-payment" />
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col col-lg-6">
                        <div class="form-group">
                            <label for="payment-accessories" class="control-label"><?php echo t('Accessories'); ?>:</label>
                            <input type="text" name="payment-accessories" class="form-control" id="payment-accessories" />
                        </div>
                    </div>
                    <div class="col col-lg-6">
                        <div class="form-group">
                            <label for="payment-memo" class="control-label"><?php echo t('Memo'); ?>: </label>
                            <input type="text" name="payment-memo" class="form-control" id="payment-memo"/>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col col-lg-6">
                        <div class="form-group">
                            <label for="payment-total" class="control-label"><?php echo t('Total'); ?>: </label>
                            <input type="text" disabled="true" name="payment-total" class="form-control" id="payment-total"/>
                        </div>
                    </div>
                    <div class="col col-lg-6">
                        <div class="form-group">
                            <label for="payment-rate" class="control-label"><?php echo t('Rate'); ?>: </label>
                            <input type="text" disabled="true" name="payment-rate" class="form-control" id="payment-rate"/>
                        </div>
                    </div>
                </div>
            </div>
            <div class="modal-footer">
                <button class="btn btn-default" type="button" data-dismiss="modal">Close</button>
                <button class="btn btn-primary" type="submit" name="btn-update-payment" value="update-payment">Save</button>
            </div>
        </div>
    </div>
</div>