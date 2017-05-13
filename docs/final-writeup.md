# Final Writeup

## Summary

We implemented multiple thread-safe binary search trees and analyzed their
performance, focusing on the lock-free implementation. We found that our
implementations, tested on the latedays cluster, with fine-grained locking were
not competitive with the baseline coarse-grained implementation, but the
lock-free implementation performed much better than any of the others.

## Background

Data structures often have to be modified to allow for concurrency. Applying one
lock to the entire data structure is a simple way to prevent incorrect behavior,
but does not permit any concurrency, as only one thread can access the data
structure at any given time. Fine-grained locks, where only specific parts of
the data structure are locked by each thread, allowing more threads to access
the data structure at once.

[Lock-free data structures](
    https://en.wikipedia.org/wiki/Non-blocking_algorithm), on the other hand,
utilize atomic compare-and-swap operations to allow multiple threads to access
the same data at the same time, while still ensuring that no thread is using
invalid values. They are much more complicated to implement, but allow more
concurrency and therefore often result in significant speedups in
high-contention applications, even when compared to fine-grained schemes.

Our chosen use case to compare these approaches is a Binary Search Tree (BST).
We will specificially implement three methods for our BST, which supports
unique integers in the tree:

* Insert: Add new elements to the tree (if not in tree)
* Remove: Delete elements from the tree (if they exist)
* Contains: Check for element membership

BSTs are often used to implement a Set data structure, which provides the same
interface and guarantees (unique elements). They are also often used simply as
trees for their ability to perform extremely quick (logarithmic) lookup. To
support concurrency in both of these cases, a simple coarse grained solution is
often created by combining a widely available, non-threadsafe data structure
available in `stdlib` with a synchronization primitive, such a mutex. However,
this forces all operations on the tree to be serialized.

The potential for parallelism arises from the nature of the data structure. An
optimal tree to exploit for parallelism is one which is as dense as possible,
or balanced. In such a tree, the number of potential nodes to explore in a
data-parallel approach doubles at every level. Further, each node depends only
on its immediate parents, and affects only its immediate children, so there
should be little to no unnecessary communication between threads attempting to
perform operations on different points in the tree. The recursive
nature lends itself very nicely to a parallel approach with very simple code.
Though the parallel nature of this problem is appealing, the fact that all nodes
stem from the root presents issues of contention that we find to be significant.

## Approach

As mentioned previously, we implemented and analyzed three different styles of
trees. We created a coarse-grained implementation to serve as a baseline by
combining a simple, custom binary search tree with a guarding mutex on all
operations. We then focused on the two more interesting implementations, namely
fine-grained and lock-free. All implementations were created in C++11 using
modern C++ features (e.g. smart pointers) wherever possible both as a learning
exercise and for performance reasons. Move semantics in particular often improve
the performance of code simply when switching compilers from an older version to
modern C++11 and beyond.

### Fine-Grained Implementation

Our fine-grained implementation was custom developed using the ideas 
learned in class. It relies on many of the same principles as the
simple coarse-grained implementation, with a notable difference being an
inclusion of a mutex at every node. There is also a mutex on the object itself
to guard access to the entire tree. Each operation
performs some sort of traversal using a hand-over-hand locking technique in
order to find the node it is looking for. In short, after the first lock is
obtained on a node, the traversal will additionally lock the desired child
before releasing the lock on the parent node. This ensures that the traversal
always has a lock on a valid node, which would not be the case if the parent
lock was released prematurely and, say, the child node was removed before a lock
could be obtained on it.

The contains operation is simply a traversal using the aforementioned
hand-over-hand locking technique. The traversal will start at the root, locking
the child left or right depending on where whether or not the value being
searched for is less than the value at the current node. This lets the contains
operation determine where the desired value should be in the tree. If the node
is present, return true. Else, false.

The insert operation is not much more sophisticated. Again, the same type of
traversal is performed in order to locate where in the tree the desired value
should be placed. If the value is already present, the insert operation will
fail and return false. Otherwise, there will be a `nullptr` at the location of
the value, at which point a new node is created to insert the value, and true is
returned.

The most complicated operation is the removal. It begins in the same way as the
other two operations, searching for the value to be removed. During traversal,
the parent is additionally tracked (the previous traversals maintained only a
pointer to the current node). Should the value not be found, false is returned.
Otherwise, there are three cases to consider.

* *Leaf node:* The value intended for removal is a leaf node. This can be removed
  by simply setting the parent's pointer to the node to be `nullptr`. Since all
  nodes have their lifetime controlled by a `std::unique_ptr`, this will also
  force the object to go out of scope and be automatically deleted.
* *Single child:* The value intended for removal has a single child, on either
  the left or right side. This case is simple as well; the parent's pointer to
  the current node can simply be replaced (using `std::move`) with a pointer to
  the left or right child. The nature of the construction of the BST ensures
  that all invariants are maintained in this process. Again, by letting the
  current node fall out of scope (by not being pointed to), it is automatically
  deleted.
* *Dual child:* The difficulty in removal lies in deleting a node which has two
  children. In this case, a suitable replacement must be found for the current
  node, which we've chosen to be the smallest element in the right subtree. the
  largest element in the left subtree would work equally well. While maintaining
  the lock on the current node, the right subtree is traversed to locate the
  smallest element. It is guaranteed that the smallest element will be either a
  leaf node or will have a single (right) child. This element is then deleted
  according to the rules above, and its value is copied into the current node.
  This approach allows the lock on the parent of the current node to be
  released, as no data in it will be modified, promoting higher concurrency.
  Unfortunately, keeping a lock on the current node likely negates all benefits.

After implementing the tree using fine-grained locking, we had the idea to
replace our mutex implementation (`pthread_mutex_t`) with a reader-writer lock 
(`pthread_rwlock_t`), which allows for multiple readers or a single writer to
hold the lock. We theorized that this would significantly improve concurrency on
read-heavy workloads. Implementing this version of the tree was a relatively
simple change; the insert and remove operations took write locks on every node
in the tree, while the contains operation took a read lock on every node in the
tree while traversing. Writing the fine-grained locking solution a second time
utilizing reader-writer locks also allowed us to improve the design slightly by
adding a dummy node as a parent to the root, which eliminated a number of
special cases designed specifically for the root case.

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

## Analysis

We performed analysis under two different types of workloads, namely a
read-only and a write-heavy workload. Each of the two workloads is performed
with a varying number of threads (1, 2, 4, 8, 16, 32), and is performed with a
varying "contention factor". For both of the following workloads, we set `N` to
be approximately 1 million elements. This value was chosen to be sufficiently
large such as to have the operation take a nontrivial amount of time, while also
being small enough to allow the tree to fit in memory. We believe that the value
is sufficiently large such that operations tend to happen reasonably far away
from the root, and that increasing the number of elements would not provide much
additional benefit in this regard. Testing revealed that the same trends applied
at larger values of `N`, and so this value of `N` was chosen as a means of
optimizing for testing time. Also note that all souces of randomness are 
seeded with the same value to ensure consistency across tests.

### Write-heavy Workload

In the write-heavy workload, each thread is given a block of `N / thread_count`
elements to process, from the total of `N` consecutive integers. The
distribution of these chosen elements is defined by the contention factor, as
described below. A total of `N / thread_count` times, each thread will randomly
choose an element from the supplied range, as well as a write operation to
perform with that element. That is, a randomly chosen element from the range
will either be inserted or removed (though it may not yet exist in the tree).
This approach ensures that a total of `N` write operations take place with a
wide variety of potential conflict scenarios. The final tally for operations
done on each element (removed or inserted) is returned by each thread to use for
a correctness test as well.

Performance is measured as the total CPU time taken (on the latedays cluster) to
execute these `N` operations, measured from thread creation to thread join. The
time taken to set up the threads and perform the correctness test are not
included.

### Read-only workload

To create a read-only workload, again a range of `N` consecutive elements is
chosen. Of this range, all of the even numbers are selected, shuffled, and then
placed into a BST. The shuffling is to ensure that the BST doesn't degenerate
into a linked list, which would be the case for consecutive insertions. Then, in
a manner identical to the above, a range of elements is constructed for each of
the `thread_count` threads using the contention factor. Different from the
above, however, is that each of the ranges for each thread is shuffled before
creating the thread. This allows the threads to have the simple task of
iterating through the range and simply checking membership on each element. Half
of these checks will fail as only even elements are in the BST.

Performance is measured as the total CPU time taken (on the latedays cluster) to
execute these `N` contains operations, measured from thread creation to thread
join. The time taken to set up the threads, data structures, shuffle, etc. are
not included.

### Contention Factor

In order to test a variety of scenarios and distributions for work, we created a
"contention factor" parameter which determines the amount of interleaving
between threads' work. With 0% contention, the `N / thread_count` elements for
each thread are chosen to be consecutive. With 100% contention, the elements are
chosen to be completely interleaved. This can also be thought of as each thread
having a chunk size of 1. Contention factors between 0 and 1 smoothly
interpolate between these chunk sizes. Consider an example with 1024 elements 
and 8 threads below.

0% Contention:
* Thread 0: `[  0,   1, …,  126,  127]`
* Thread 7: `[896, 897, …, 1023, 1024]`

100% Contention:
* Thread 0: `[0,  8, 16, …, 1016]`
* Thread 7: `[7, 15, 23, …, 1023]`

## Results

### Write-Heavy Workloads
<div class="graph" id="summary_write_high" csv="high-cont-proc" title="Write-Intensive Summary, High Contention"></div>
<br />
<div class="graph" id="summary_write_low" csv="low-cont-proc" title="Write-Intensive Summary, Low Contention"></div>
<br />

In both of these plots, we see that the fine-grained solutions perform very
poorly. We believe that this is a combination of significant amounts of
contention at every single node, most notably at the top. Every path has to go
through the root node, and so every thread will attempt to lock it and many of
the immediate descendants. This is likely causing more overall waiting than the
coarse grained solution, in which a lock is taken once and then the entire tree
is traversed.

Another interesting observation is that the performance of the fine-grained
implementations drops spectacularly at 4 threads, rather than at 2 as might
otherwise be expected. We suspect that this is because with two threads, an
interleaving is possible in which 2 threads can tradeoff between holding the
locks and performing computation, but as additional threads are added
significantly more time is spent blocking for locks, preventing high
performance. Of the two, the reader-writer locks implementation is generally the
slower, and we believe this to be due to the additional complexity of
maintaining counters for readers and writers, rather than a simple bit for a
standard mutex.

While the fine-grained solution performs incredibly poorly, the lock-free
solution shows remarkable performance. There are no locks to be taken anywhere
in the system, meaning that it is impossible for the algorithm to block at the
top levels of the tree, where all threads are constantly vying for locks. All
operations can and do seem to happen in almost complete parallel.

One final point of note is that there is that the two plots seem almost
identical even though the contention factors are quite different. This suggests
that there are more fundamental issues at play driving the performance of the
implementations than where in the tree nodes are being accessed.

### Read-Only Workloads
<div class="graph" id="summary_read_high" csv="high-cont-read-proc" title="Read-Only Summary, High Contention"></div>
<br />
<div class="graph" id="summary_read_low" csv="low-cont-read-proc" title="Read-Only Summary, Low Contention"></div>
<br />

Here, we again see the fine-grained solution performing extremely poorly when
compared to the coarse and lock-free solutions. We believe this is due to the
same issues of blocking at the root that we observed in the write-heavy
workload, since approximately the same amount of traversal is needed in both
workloads. This time, however, using the reader-writer locks in the fine-grained
solution makes a significant difference in performance, bringing the performance
to approximately the same level as the coarse-grained baseline. This is
explicitly because multiple threads can hold reader locks on the same nodes, so
no blocking is necessary. One might suspect that under this workload we should
see even better performance due to there only being reads, but we believe the
overhead of constantly making syscalls to lock and unlock, even if there is no
blocking, is killing performance.

Similarly, once again the lock-free solution exhibits almost perfect parallelism
when scaling up to multiple threads. This is for the same reasons as described
above, namely that by eliminating contention at the top levels of the tree,
significant performance gains can and are achieved.

Once again, we note that contention factor does not play a significant role in
the performance of the various algorithms.

### Lock-free Speedup

<div class="graph" id="lockfree-spdup_chart" csv="lockfree-proc-spdup"
     title="Write-Intensive Lock-Free Speedup" y-title="Speedup (vs. 1 thread)"></div>
<br />
<div class="graph" id="lockfree-read-spdup_chart" csv="lockfree-read-proc-spdup"
     title="Read-Only Lock-Free Speedup" y-title="Speedup (vs. 1 thread)"></div>
<br />

With lock-free being our highest performer, we wanted to further analyze its
performance characteristics under both the read and write workloads. We find
that the lock-free implementation exhibits an ideal linear speedup in the
read-heavy workloads, and still achieves respectable speedups in the write-heavy
workloads. In the read-heavy case, we see performance peak at around 22-23x
speedup with 32 threads. This is not a coincidence; there are only 24 execution
contexts on the latedays machines, and achieving this speedup means that we are
effectively utilizing the entire compute capability of the machine nearly
perfectly. Adding additional threads beyond 24 offers no additional speedup as
the computer does not have more performance left to offer.

Here, we do see the effects of the contention factor differentiating the various
test cases. We would expect the lowest contention factor to offer the fastest
performance, but this is not the case in either of the plots. We believe this to
be a function of the tree construction, and the specific order of inserts and
removals which greatly affects the topology of the graph in each of the cases.
This suggests that the randomness approach employed to shuffle the data may not
be ideal for performance testing, even though the seeds are kept consistent.

### Results Summary

From our analysis, a few trends start to appear. As mentioned, we found that the
contention factor had relatively little impact compared to the implementation
itself. We found that the coarse-grained solution performs much better than
expected, and thus it may be worth simply wrapping a non-threadsafe BST in a
mutex for a simple and effective threadsafe BST. Unfortunately, we found the
fine-grained implementations to be slower in all tests except for read-only with
the reader-writer locks, where we achieved baseline performance. This indicates
that fine-grained locking offers more overhead than potential concurrency,
suggesting that fine-grained is best left as a theoretical exercise. Finally,
perhaps as expected, we found the lock-free implementation to perform
exceedingly well, though we were still pleasantly surprised to find it
demonstrate near perfect speedup.

In the appendix section below, we have provided plots of all combinations of
contention, workload, and implementation that we tested.

### Future Work

In completing this analysis of BSTs, a number of ideas for potential future
projects arose. Our particular favorites are presented below:

* Templated BSTs to analyze performance of object size. Currently, the trees
  operate only on 32 bit integers. We are curious to see how performance of the
  implementations would change when only a single node is able to fit on a cache
  line, rather than multiple. This would also make the implementations more
  generic.
* Custom memory allocator to increase cache utilization. Nodes are currently
  allocated in insertion order with the creation of a new object on the heap,
  potentially scattering them around memory. We believe that the addition of a
  custom memory allocator would allow nodes close to each other in the tree to
  be contiguous in memory, which is an ideal case for cache usage.
* Distributed lock-free BST. In order to expand on our implementation of the
  lock-free BST, we consider the possibility of distributing it over multiple
  nodes in a cluster. This could take the form of multiple copies over a large
  physical space (e.g. continents) providing eventual consistency, or exploring
  lock-free trees which are too big to fit on a single computer's memory.

## References

[1] [http://dl.acm.org/citation.cfm?id=2555256](http://dl.acm.org/citation.cfm?id=2555256)

## Authors

Developed by [Vasu Agrawal](https://github.com/VasuAgrawal) and
[Lukas Peraza](https://github.com/LBPeraza).

## Separation of Work

Equal work was performed by both project members.

## Links

* [Proposal](https://vasuagrawal.github.io/418FinalProject/proposal)
* [Project Checkpoint](https://vasuagrawal.github.io/418FinalProject/checkpoint)
* [Final Slide
  Deck](https://docs.google.com/presentation/d/1RhBI1PinvXbjd9-OVcO_GpeWBTiM1-2giSM0LqCGFN8/edit?usp=sharing)

## Appendix
### Write-Heavy Workloads
<div class="graph" id="coarse_chart" csv="coarse-proc" title="Write-Intensive, Coarse-Grained Locking"></div>
<br/>
<div class="graph" id="fine_chart" csv="fine-proc" title="Write-Intensive, Fine-Grained Locking"></div>
<br/>
<div class="graph" id="fine-rw_chart" csv="fine-rw-proc" title="Write-Intensive, Fine-Grained RW Locking"></div>
<br/>
<div class="graph" id="lockfree_chart" csv="lockfree-proc" title="Write-Intensive, Lock-Free"></div>
<br/>

### Read-Only Workloads
<div class="graph" id="coarse-read_chart" csv="coarse-read-proc" title="Read-Only, Coarse-Grained Locking"></div>
<br/>
<div class="graph" id="fine-read_chart" csv="fine-read-proc" title="Read-Only, Fine-Grained Locking"></div>
<br/>
<div class="graph" id="fine-rw-read_chart" csv="fine-rw-read-proc" title="Read-Only, Fine-Grained RW Locking"></div>
<br/>
<div class="graph" id="lockfree-read_chart" csv="lockfree-read-proc" title="Read-Only, Lock-Free"></div>

<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
<script type="text/javascript" src="https://code.jquery.com/jquery-3.2.1.min.js"></script>
<script type="text/javascript" src="makeGraph.js"></script>
