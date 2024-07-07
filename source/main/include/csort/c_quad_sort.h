#ifndef __CBASE_QUAD_SORT_H__
#define __CBASE_QUAD_SORT_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "csort/private/c_quad_sort.inl"

namespace ncore
{
    //----------------------------------------------------------------------------------------------------------------
    // Quad Sort

    // u8, s8, u16, s16, u32, s32, u64, s64, f32, f64
    template<typename T>
    void quadsort(T *array, s32 elem_count, s32 elem_size);

    // custom compare function
    void quadsort_cmp(void *array, s32 elem_count, s32 elem_size, sort_cmp_predicate_fn cmp);
};

#endif
