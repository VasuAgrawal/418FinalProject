# Final Writeup

<script src="https://www.gstatic.com/charts/loader.js"></script>
<script type="text/javascript">
  google.charts.load('current', {'packages':['corechart']});
  google.charts.setOnLoadCallback(drawChart);

  function drawChart() {
    var data = google.visualization.arrayToDataTable([
      ["Thread Count", "Coarse-grained", "Fine-grained", "Fine-grained RW", "Lock-free"],
      [1, 632.849, 935.024, 1250.48, 912.562],
      [2, 1171.92, 817.272, 1069.36, 637.198],
      [4, 1508.99, 2241.2, 3141.6, 335.997],
      [8, 1691.21, 3087.83, 168.594],
      [16, 2266.03, 3147.46, 146.451]
    ]);

    var options = {
      title: 'Performance Under High Contention'
    };

    var chart = new google.visualization.ColumnChart(document.getElementById('high_contention_chart'));

    chart.draw(data, options);
  }
</script>

<div id="high_contention_chart" style="width: 900px; height: 500px"></div>

