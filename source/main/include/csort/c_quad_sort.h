#ifndef __CSORT_QUAD_SORT_H__
#define __CSORT_QUAD_SORT_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace ncore
{
    //----------------------------------------------------------------------------------------------------------------
    // Quad Sort
    namespace quad_sort
    {
        typedef s8 (*sort_cmp_fn)(void const *inItemA, void const *inItemB, void *inUserData);
    }
};

#endif
