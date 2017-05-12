# Final Writeup

<script src="https://www.gstatic.com/charts/loader.js"></script>
<script type="text/javascript">
  google.charts.load('current', {'packages':['corechart']});
  google.charts.setOnLoadCallback(drawChart);

  function drawChart() {
    var data = google.visualization.arrayToDataTable([
      ["Thread Count", "Coarse-grained", "Lock-free"],
      [1, 632.849, 912.562],
      [2, 1171.92, 637.198],
      [4, 1508.99, 335.997],
      [8, 1691.21, 168.594],
      [16, 2266.03, 146.451]
    ]);

    var options = {
      title: 'Performance Under High Contention'
    };

    var chart = new google.visualization.ColumentChart(document.getElementById('high_contention_chart'));

    chart.draw(data, options);
  }
</script>

<div id="high_contention_chart" style="width: 900px; height: 500px"></div>

