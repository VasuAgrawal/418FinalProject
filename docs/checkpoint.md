# Project Checkpoint

## Schedule

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
