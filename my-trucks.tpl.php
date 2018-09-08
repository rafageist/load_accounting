<!-- Report -->
<div class="row">
    <!-- Table Report -->
    <div class="col col-sm-6">
        <table class="table">
            <thead class="thead-light">
            <tr>
                <th class="bg-success">
                    <button type="button" class="btn btn-success">Active Trucks
                        <span class="badge">
                <?php print $report['bynumber']['reefer'] + $report['bynumber']['flatbed'] + $report['bynumber']['dryvan']; ?>
              </span>
                    </button>
                </th>
                <th class="bg-success">This Week</th>
                <th class="bg-success">Last Week</th>
                <th class="bg-success">This Month</th>
                <th class="bg-success">Last Month</th>
                <th class="bg-success">This Year</th>
                <th class="bg-success">Historical</th>
            </tr>
            </thead>
            <tbody>
            <tr>
                <td style="background-color:rgb(233, 49, 75); color:#FFF;">Total Loads</td>
                <td class="bg-success"><?php print $report['thisweek']['count']; ?></td>
                <td class="bg-success"><?php print $report['lastweek']['count']; ?></td>
                <td class="bg-success"><?php print $report['thismonth']['count']; ?></td>
                <td class="bg-success"><?php print $report['lastmonth']['count']; ?></td>
                <td class="bg-success"><?php print $report['thisyear']['count']; ?></td>
                <td class="bg-success"><?php print $report['ever']['count']; ?></td>
            </tr>
            <tr>
                <td style="background-color:rgb(233, 49, 75); color:#FFF;">Rate / Miles</td>
                <td class="bg-success"><?php print $report['thisweek']['rate']; ?></td>
                <td class="bg-success"><?php print $report['lastweek']['rate']; ?></td>
                <td class="bg-success"><?php print $report['thismonth']['rate']; ?></td>
                <td class="bg-success"><?php print $report['lastmonth']['rate']; ?></td>
                <td class="bg-success"><?php print $report['thisyear']['rate']; ?></td>
                <td class="bg-success"><?php print $report['ever']['rate']; ?></td>
            </tr>
            <tr>
                <td style="background-color:rgb(233, 49, 75); color:#FFF;">Empty Miles</td>
                <td class="bg-success"><?php print number_format($report['thisweek']['empty'], 2); ?></td>
                <td class="bg-success"><?php print number_format($report['lastweek']['empty'], 2); ?></td>
                <td class="bg-success"><?php print number_format($report['thismonth']['empty'], 2); ?></td>
                <td class="bg-success"><?php print number_format($report['lastmonth']['empty'], 2); ?></td>
                <td class="bg-success"><?php print number_format($report['thisyear']['empty'], 2); ?></td>
                <td class="bg-success"><?php print number_format($report['ever']['empty'], 2); ?></td>
            </tr>
            <tr>
                <td style="background-color:rgb(233, 49, 75); color:#FFF;">Total Miles</td>
                <td class="bg-success"><?php print number_format($report['thisweek']['total'], 2); ?></td>
                <td class="bg-success"><?php print number_format($report['lastweek']['total'], 2); ?></td>
                <td class="bg-success"><?php print number_format($report['thismonth']['total'], 2); ?></td>
                <td class="bg-success"><?php print number_format($report['lastmonth']['total'], 2); ?></td>
                <td class="bg-success"><?php print number_format($report['thisyear']['total'], 2); ?></td>
                <td class="bg-success"><?php print number_format($report['ever']['total'], 2); ?></td>
            </tr>
            <tr>
                <td style="background-color:rgb(233, 49, 75); color:#FFF;">Gross Income</td>
                <td class="bg-success"><?php print '$ ' . number_format($report['thisweek']['sold'], 2); ?></td>
                <td class="bg-success"><?php print '$ ' . number_format($report['lastweek']['sold'], 2); ?></td>
                <td class="bg-success"><?php print '$ ' . number_format($report['thismonth']['sold'], 2); ?></td>
                <td class="bg-success"><?php print '$ ' . number_format($report['lastmonth']['sold'], 2); ?></td>
                <td class="bg-success"><?php print '$ ' . number_format($report['thisyear']['sold'], 2); ?></td>
                <td class="bg-success"><?php print '$ ' . number_format($report['ever']['sold'], 2); ?></td>
            </tr>
            </tbody>
        </table>
    </div>

    <!-- Income By Type-->
    <div class="col col-sm-6">
        <div id="chart_div"></div>
    </div>
</div>

<!--Load the AJAX API-->
<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
<script type="text/javascript">
    google.charts.load('current', {'packages': ['corechart']});
    google.charts.setOnLoadCallback(drawChart);

    function drawChart() {
        var data = new google.visualization.DataTable();
        data.addColumn('string', 'Truck');
        data.addColumn('number', 'Income');
        data.addRows([
            ['Reefer (' + <?php print $report['bynumber']['reefer']; ?> +')', <?php print $report['bytype']['reefer']; ?> ],
            ['Flat Bed (' + <?php print $report['bynumber']['flatbed']; ?>  +')', <?php print $report['bytype']['flatbed']; ?> ],
            ['Dry Van (' + <?php print $report['bynumber']['dryvan']; ?>  +')', <?php print $report['bytype']['dryvan']; ?> ],
        ]);
        var options = {'title': 'How much income generate my trucks?', 'width': 600, 'height': 300};
        var chart = new google.visualization.PieChart(document.getElementById('chart_div'));
        chart.draw(data, options);
    }
</script>

<!-- Truck List Card -->
<div class="row"> <?php
	global $base_path;
	global $base_url;
	$base_image = $base_path . '/' . drupal_get_path('theme', 'load_subtheme') . '/pictures';
	setlocale(LC_MONETARY, 'en_US.UTF-8');
	$genid = 'gen_rat_rep_' . rand(150, 1500);

	foreach($trucks as $nid => $info){
		$imagetype = 'unknown.jpg';
		if($info['type'] == 'Reefer'){
			$imagetype = 'reefer.jpg';
		}
		if($info['type'] == 'Dry Van'){
			$imagetype = 'dryvan.jpg';
		}
		if($info['type'] == 'Flat Bed'){
			$imagetype = 'flatbed.jpg';
		}
		?>

        <!-- Bootstrap Card -->
        <div class="col-sm-12 col-md-3" style="height: 402px;">
            <div class="thumbnail">
                <img <?php print 'src="' . $base_image . '/' . $imagetype . '"'; ?>>
                <div class="caption bg-success">
                    <h4><?php print $info['customer']; ?></h4>
                    <p><b>Driver:</b> <?php print $info['driver']; ?></p>
                    <p><b>Carrier</b> <?php print $info['carrier']; ?></p>
                    <p><b>Dispatcher</b> <?php print $info['dispatcher']; ?></p>
                    <p><b>Equipment</b> <?php print $info['equipment']; ?></p>
                    <p>
                        <a class="btn btn-danger active" <?php print 'href="' . $base_url . '/accounting/customer/reports/' . $nid . '"'; ?>>Load
                            Reports</a>
                        <a class="btn btn-primary active" <?php print 'href="' . $base_url . '/accounting/customer/payments/' . $nid . '"'; ?>>Payments</a>
                    </p>
                </div>
            </div>
        </div> <?php
	} ?>
</div>