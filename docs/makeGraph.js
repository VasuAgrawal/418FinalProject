
function processData(data) {
    var lines = data.split("\n");
    var newData = lines.map(function(x, i) {
        var info = x.split(",");
        if (i == 0) {
            return info;
        }
        return [parseInt(info[0])].concat(
            info.slice(1).map(parseFloat));
    });
    return newData;
}

function makeGraph(csvFile, graphTitle, xTitle, yTitle, divId) {
    $.ajax("data/" + csvFile + ".csv", {
        datatype: "text",
        mimeType: "text/plain"
    }).done(function(data) {

        var dataTable = google.visualization.arrayToDataTable(processData(data));
        var options = {
            title: graphTitle,
            pointSize: 5,
            hAxis: {
              title: xTitle
            },
            vAxis: {
              title: yTitle
            }
        };
        var chart = new google.visualization.LineChart(
            document.getElementById(divId));
        chart.draw(dataTable, options);

    });
}

function drawAllCharts() {

    $('.graph').each(function () {
        var $this = $(this);
        $this.css({
            width : '100%',
            height : '400px'
        });
        var xTitle = $this.attr('x-title');
        xTitle = xTitle ? xTitle : 'Threads';
        var yTitle = $this.attr('y-title');
        yTitle = yTitle ? yTitle : 'Execution time (ms)';
        makeGraph($this.attr('csv'), $this.attr('title'), xTitle, yTitle, this.id);
    });

}

google.charts.load('current', {'packages':['corechart']});
google.charts.setOnLoadCallback(drawAllCharts);