<?php global $base_path; ?>

<!-- Load Record -->
<div class="row">
  <div class="col col-sm-12">
    <div class="panel panel-danger" style="margin-top:10px; margin-bottom:10px;">
      <div class="panel-heading">Load Record <span class="badge"><?php print count($loads); ?></span></div>
      <table class="table">
        <thead>
          <tr>
            <th>4G ID</th>
            <th>Status</th>
            <th>From</th>
            <th>To</th>
            <th>Miles</th>
            <th>Empty</th>
            <th>Payment</th>
            <th>Rate</th>
            <th>Delivery Date</th>
            <th>Created</th>
            <th>Dispatcher</th>
            <th>Confirmation</th>
            <th>BOL</th>
          </tr>
        </thead> 
        <tbody>
          <?php foreach (array_reverse($loads) as $key => $value):
            $extra = get_load_extra($value['nid']);
            $class = 'bg-success';
            if($value['status'] == 'Breakdown' || $value['status'] == 'Canceled' || $value['status'] == 'Standby'): 
              $class = 'bg-danger'; 
            endif; ?>
            <tr <?php print "class='".$class."'"; ?>>
              <td> <?php print $value['title']; ?> </td>
              <td> <?php print $value['status']; ?> </td>
              <td> <?php print $extra['from']; ?> </td>
              <td> <?php print $extra['to']; ?> </td>
              <td> <?php print $value['miles']; ?> </td>
              <td> <?php print $value['empty']; ?> </td>
              <td> <?php print $value['payment']; ?> </td>
              <td> <?php print $value['rate']; ?> </td>
              <td> <?php if(isset($extra['delivery']) && !empty($extra['delivery'])) { 
                print date('M d Y', strtotime($extra['delivery'])); } ?> 
              </td>
              <td> <?php print date('M d Y', $value['created']); ?> </td>
              <td> <?php print $value['ufull']; ?> </td>
                <td> <?php print $value['confirmation']; ?> </td>

                <td><?php print $value['bol']; ?></td>
            </tr>
          <?php endforeach; ?>
        </tbody>
      </table>
    </div>
  </div>
</div>