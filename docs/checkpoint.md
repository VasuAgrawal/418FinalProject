# Project Checkpoint

## Schedule

1. ~~**Mon April 17th (11:59 pm):** Proposal for first idea for final project.~~
1. ~~**Tue April 25th (11:59 pm):** Project checkpoint report due. By this point,
   we should have a completed test harness (Vasu) as well as a coarse-grained
   implementation of binary search trees completed (Lukas).~~
1. **Fri April 28 (11:49 pm):** Fine-grained locking implementation completed (Vasu).
1. **Tue May 2nd (11:59 pm):** Lock-free implementation started (Lukas). Test
   harness upgraded to include full multithreading and load simulation support
   (Vasu).
1. **Sat May 6th (11:59 pm):** Lock-free implementation completed (Lukas).
1. **Wed May 10th (9:00 am):** Analysis of different implementations complete.
   Work on stretch goals started.
1. **Thu May 11th (3:00 pm):** Finalists announced, presentation time sign ups.
1. **Fri May 12th (11:59 pm):** Final report due, parallelism competition.
   Analysis and report completed.

## Summary of Work

By Tuesday, 25th April's checkpoint we have completed a coarse grained
implementation of our binary search trees. In doing so, we've established a
framework which should allow the other implementations to function as a drop-in
replacement, thus requiring minimal code changes. We've additionally begun
development of our own testing harness which has both a single-threaded and
multi-threaded implementation testing for correctness. The combination of this
test suite with the coarse grained reference solution should make the
implementations of the other two implementations simpler and faster to develop.

## Goals and Deliverables

## Concerns

In developing the test harness, we had no difficulty testing for correctness in
a single-threaded case. However, ensuring consistency across test iterations in
the multi-threaded case is proving to be challenging. We are attempting to
mitigate these concerns by using the law of large numbers, and simply making
long, repeated attempts in order to attempt to tease out any potential race
conditions or similar concurrency issues. While we're reasonably confident in
the test harness' ability to find bugs, we are somewhat concerned that
determining accurate performance scores may be more difficult than we had
originally anticipated.

We have yet to begin working with the fine grained locking and lock free
implementations, so it is difficult to judge the expected level of difficulty.
It appears that the fine-grained locking version should not be significantly
more complex than the coarse grained locking implementation, and that the large
majority of the difficulty will be found in the lock-free implementation. For
this reason, we've allocated approximately half a week of schedule time for the
fine-grained locking solution while giving an entire week to the lock-free
implementation.

## Authors

Developed by [Vasu Agrawal](https://github.com/VasuAgrawal) and
[Lukas Peraza](https://github.com/LBPeraza).

## Links

- [Project Proposal](https://vasuagrawal.github.io/418FinalProject/proposal)
