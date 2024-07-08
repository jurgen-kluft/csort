#ifndef __CSORT_QUAD_SORT_H__
#define __CSORT_QUAD_SORT_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "csort/private/c_quad_sort.inl"

namespace ncore
{
    //----------------------------------------------------------------------------------------------------------------
    // Quad Sort

    // custom compare function
    void quad_sort_with_cmp(void *array, s32 elem_count, s32 elem_size, quad_sort::sort_cmp_fn cmp);

    // u8, s8, u16, s16, u32, s32, u64, s64, f32, f64
    template <typename T>
    s32 g_quad_sort_cmp_prim_fn(void const* item_a, void const* item_b, void* user_data)
    {
        T const* a = (T const*)item_a;
        T const* b = (T const*)item_b;
        return *a > *b;
    }

    // u8, s8, u16, s16, u32, s32, u64, s64, f32, f64
    template<typename T>
    void quad_sort(T *array, s32 elem_count)
    {
        quad_sort_with_cmp(array, elem_count, sizeof(T), g_quad_sort_cmp_prim_fn());
    }
};

#endif
