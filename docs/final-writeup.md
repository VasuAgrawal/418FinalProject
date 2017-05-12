# Final Writeup

## Summary

We implemented multiple thread-safe binary search trees and analyzed their
performance, focusing on the lock-free implementation. We found that our
implementations with fine-grained locking were not competitive with the baseline
coarse-grained implementation, but the lock-free implementation performed much
better than any of the others.

## Background

## Approach

## Results

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

## References

## Authors

Developed by [Vasu Agrawal](https://github.com/VasuAgrawal) and
[Lukas Peraza](https://github.com/LBPeraza).

## Separation of Work

## Links

- [Project Checkpoint](https://vasuagrawal.github.io/418FinalProject/checkpoint)
- [Final Writeup](https://vasuagrawal.github.io/418FinalProject/final-writeup)
