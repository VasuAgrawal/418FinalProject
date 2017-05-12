# Final Writeup

## Summary

We implemented multiple thread-safe binary search trees and analyzed their
performance, focusing on the lock-free implementation. We found that our
implementations with fine-grained locking were not competitive with the baseline
coarse-grained implementation, but the lock-free implementation performed much
better than any of the others.

## Background

## Approach

### Fine-Grained Implementation

### Lock-Free Implementation

Our lock-free implementation was modeled after the algorithm described in [1].
We will also provide a brief overview of the algorithm here.

In [1], an external binary search tree is used. That is, the values in the tree
are stored only in the leaves, and all internal nodes are used just for routing.
This imposes the restriction that each internal node has exactly two children,
greatly simplifying the logic for node removal. Additionally, the algorithm
uses marked edges instead of marked nodes to designate which nodes are being
accessed (and are thus unsafe to remove).

Each operation (insert, remove, search) begins with a *seek* phase. This phase
traverses the tree (going left if the current value is greater than the searched
for value, right otherwise) until it reaches a leaf. During the seek, we keep
track of the current node, the parent node, and two additional nodes called the
*ancestor* and *successor*. These nodes are the endpoints of the last unmarked
edge in the entire seek operation.

For the search operation, the seek phase is all that is necessary. If the leaf
node found has the same value that we are searching for, return true. Else,
return false.

For the insert operation, we now create two new nodes -- an internal node, and
a leaf node. The internal node's children are the new leaf node and the leaf
node found in the seek phase. We then set the seek's parent node's child to
be the new internal node, thus inserting the new internal node. This update is
done with a compare and swap operation to ensure atomicity. If the CAS fails,
(because the parent or leaf is set to be deleted), we restart the entire
process.

The delete operation is the most complicated (as it is in most BST
implementations). Upon completion of the seek phase, the edge from the parent to
the leaf is marked. Then the edge from the seek's ancestor to the seek's
successor is replaced with an edge from the ancestor to the sibling of the
seek's leaf. This allows for deletion of multiple nodes at once.

### Analysis

## Results

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

## References

[1] [http://dl.acm.org/citation.cfm?id=2555256](http://dl.acm.org/citation.cfm?id=2555256)

## Authors

Developed by [Vasu Agrawal](https://github.com/VasuAgrawal) and
[Lukas Peraza](https://github.com/LBPeraza).

## Separation of Work

## Links

- [Proposal](https://vasuagrawal.github.io/418FinalProject/proposal)
- [Project Checkpoint](https://vasuagrawal.github.io/418FinalProject/checkpoint)

<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
<script type="text/javascript" src="https://code.jquery.com/jquery-3.2.1.min.js"></script>
<script type="text/javascript" src="makeGraph.js"></script>