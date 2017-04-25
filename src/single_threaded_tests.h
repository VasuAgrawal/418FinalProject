#include <memory>

#include "testing.h"
#include "bst.h"

#ifdef COARSE
#include "coarse_grained_bst.h"
#elif FINE
#include "fine_grained_bst.h"
#else
#error Implementation not specified. Valid implementations are "COARSE".
#endif

bool test_single_add();
bool test_single_add_remove();
bool test_bad_remove();
