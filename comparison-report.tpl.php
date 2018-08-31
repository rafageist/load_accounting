<!-- Report -->
<div class="row" style="margin-top:35px;">
  <div class="col col-sm-6">
    <table class="table">
      <thead class="thead-light">
        <tr>
          <th></th>
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
  
  <div class="col col-sm-6">
    <div id="chart_div"></div>
  </div>
</div> 

<!--Load the AJAX API-->
<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
<script type="text/javascript">
  google.charts.load('current', {packages: ['corechart', 'bar']});
  google.charts.setOnLoadCallback(drawBarColors);
  
  function drawBarColors() {
    var data = google.visualization.arrayToDataTable([
      ['Period', 'Current Period', 'Last Period'],
      ['Week', <?php print $report['thisweek']['sold']; ?>, <?php print $report['lastweek']['sold']; ?>],
      ['Month', <?php print $report['thismonth']['sold']; ?>, <?php print $report['lastmonth']['sold']; ?>],
    ]);
 
    var options = {
      title: 'Comparison Chart',
      chartArea: { width: '60%' },
      colors: ['#5cb85c', '#e9314b'],
      hAxis: {
        minValue: 0
      },
    };

    var chart = new google.visualization.BarChart(document.getElementById('chart_div'));
    chart.draw(data, options);
  }
</script>