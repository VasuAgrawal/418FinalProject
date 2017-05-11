#ifndef SINGLE_THREADED_TESTS_H_
#define SINGLE_THREADED_TESTS_H_

#include <memory>

#include "testing.h"
#include "bst.h"

#ifdef COARSE
#include "coarse_grained_bst.h"
#elif FINE
#include "fine_grained_bst.h"
#elif LOCKFREE
#include "lock_free_bst.h"
#else
#error Implementation not specified. Valid implementations are "COARSE", "FINE", "LOCKFREE".
#endif



namespace SingleThreaded {

void test_all();

bool test_single_add();
bool test_triple_add();
bool test_single_add_remove();
bool test_bad_remove();
bool test_repeated_add();
bool test_nonexistent_remove();
bool test_leaf_remove();
bool test_single_child_remove();
bool test_double_child_remove();
bool test_single_child_remove_root();
bool test_double_child_remove_root();
bool test_double_child_remove_root_deep();
bool test_root_remove_deep();
bool test_middle_remove_deep();

#ifdef COARSE
bool test_in_order_traversal();
#endif 

} // namespace SingleThreaded

#endif
