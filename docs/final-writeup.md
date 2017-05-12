# Final Writeup

<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
<script type="text/javascript" src="https://code.jquery.com/jquery-3.2.1.min.js"></script>
<script type="text/javascript" src="makeGraph.js"></script>

<div class="graph" id="coarse_chart" csv="coarse-proc" title="Write-Intensive, Coarse-Grained Locking"></div>
<div class="graph" id="fine_chart" csv="fine-proc" title="Write-Intensive, Fine-Grained Locking"></div>
<div class="graph" id="fine-rw_chart" csv="fine-rw-proc" title="Write-Intensive, Fine-Grained RW Locking"></div>
<div class="graph" id="lockfree_chart" csv="lockfree-proc" title="Write-Intensive, Lock-Free"></div>

<div class="graph" id="coarse-read_chart" csv="coarse-read-proc" title="Read-Only, Coarse-Grained Locking"></div>
<div class="graph" id="fine-read_chart" csv="fine-read-proc" title="Read-Only, Fine-Grained Locking"></div>
<div class="graph" id="fine-rw-read_chart" csv="fine-rw-read-proc" title="Read-Only, Fine-Grained RW Locking"></div>
<div class="graph" id="lockfree-read_chart" csv="lockfree-read-proc" title="Read-Only, Lock-Free"></div>

<div class="graph" id="lockfree-spdup_chart" csv="lockfree-proc-spdup"
     title="Write-Intensive Lock-Free Speedup" y-title="Speedup (vs. 1 thread)"></div>
<div class="graph" id="lockfree-read-spdup_chart" csv="lockfree-read-proc-spdup"
     title="Read-Only Lock-Free Speedup" y-title="Speedup (vs. 1 thread)"></div>

<script type="text/javascript">
google.charts.load('current', {'packages':['corechart']});
google.charts.setOnLoadCallback(function() {

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

});
</script>