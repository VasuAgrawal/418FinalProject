# Project Proposal

## Summary

We will be comparing the performance of various implementations of concurrent 
binary trees across various types of workloads. Specifically, we intend to
explore coarse-grained locking, fine-grained locking, and lock-free structures.

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

## The Challenge

The difficulty in implementing the aforementioned binary trees lies primarily in
the lock-free version. A hand over hand locking scheme should perform well for
the fine-grained locking solution, and the coarse-grained locking solution is
simpler still. However, with the lock-free version, difficulty arises due to the
need to atomically modify multiple values in order to perform a given operation.
For example, to delete a value from a conventional implementation of a binary
tree would require the update of the parent's pointers, as well as a read of the
childrens'. This poses a significant challenge as simple single word compare and
exchanges, the staple of lock free programs, do not seem immediately able to
solve the problem completely. If the read and update operations are not atomic,
a second reader may, for example, follow a pointer to an invalid value.

## Resources

We intend to develop all major implementations ourselves. For the coarse-grained
locking solution, we hope that our knowledge of binary search trees will be
sufficient. For the fine-grained locking solution, we intend to utilize the
course notes for inspiration on implementation. The lock-free BST, however, is
substantially more difficult and is beyond our scope to design from scratch.
Rather, we intend to explore one of the approches outlined in these papers, in
descending order of interest:

1. [http://dl.acm.org/citation.cfm?id=2555256]()
1. [http://dl.acm.org/citation.cfm?id=2611500]()
1. [http://dl.acm.org/citation.cfm?id=2312036]()

For development, we intend to work on our personal computers to avoid expected
contention for resources on the GHC machines. However, for the final performance
analysis, we intend to use the GHC machines 

A few relevant hardware details of the GHC machines are provided below:

```
Architecture:          x86_64
CPU op-modes(s):       32-bit, 64-bit
CPU(s):                16
Thread(s) per core:    2
Core(s) per socket:    1
Model name:            Intel(R) Xeon(R) CPU E5-1660 v4 @3.20GHz
L1d cache:             32k
L1i cache:             32k
L2 cache:              256k
L3 cache:              20480k
```

## Goals & Deliverables

The first major deliverable for the project is the completion of 3 different
implementations of concurrent binary search trees. They are:

1. Coarse-grained locking tree
1. Fine-grained locking tree
1. Lock-free tree

The novel part of our project, however, is the evaluation and comparison of the
performance characteristics of the three different tree structures that we will
develop. Specifically, we intend to evaluate the performance under the following
load conditions, varying the number of threads and the amount of contention.

1. Read-only workload
1. Insert-only workload
1. Delete-heavy workload

We hope that our testing metrics cover a wide spectrum of expected use cases for
these trees. From the results of our testing, we hope to be able to find the
optimally performant system in both specific and general cases, and weigh that
against the implementation complexity to determine a balance. We wish for this
to be an incredibly testing-heavy project, which will be a novel experience for
the both of us. As a result, we hope to learn a lot about developing a robust
and easy to use testing and trace framework and playback system.

Time permitting, we wish to explore some subset of the following:

1. Compare performance of our implementations against other existing
   implementations, such as those available in the C++ STL and perhaps in
   popular libraries such as Boost.
1. Extend our implementations of binary trees to function as concurrent balanced
   binary trees.
1. Perform detailed machine-level analysis to explore the reasons behind the
   performance for each of the implentations. For example, explore lock
   contention and cache performance via machine counters.

## Platform

We intend to use a single GHC machine for our final performance testing,
primarily due to its high core count. We can additionally ensure that there are
no other users on our chosen GHC machine during our testing, as other users'
jobs may skew testing results. Having a single physical machine also ensure
performance repeatability, something which is substantially more difficult on a
virtualized machine. The GHC cluster was additionally chosen due to its relative
ease of access.

## Schedule

~~1. **Mon April 17th (11:59 pm):** Proposal for first idea for final project.~~
1. **Tue April 25th (11:59 pm):** Project checkpoint report due. 
1. **Tue May 2nd (11:59 pm):**
1. **Wed May 10th (9:00 am):** Project pages made available.
1. **Thu May 11th (3:00 pm):** Finalists announced, presentation time sign ups.
1. **Fri May 12th (11:59 pm):** Final report due, parallelism competition.

## Authors

Developed by [Vasu Agrawal](https://github.com/VasuAgrawal) and
[Lukas Peraza](https://github.com/LBPeraza).

## Links

- [Project Checkpoint](https://vasuagrawal.github.io/418FinalProject/checkpoint)