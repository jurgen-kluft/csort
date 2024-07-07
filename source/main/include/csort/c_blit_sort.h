#ifndef __CBASE_BLIT_SORT_H__
#define __CBASE_BLIT_SORT_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "csort/private/c_blit_sort.inl"

namespace ncore
{
    //----------------------------------------------------------------------------------------------------------------
    // Blit Sort

    // u8, s8, u16, s16, u32, s32, u64, s64, f32, f64
    template<typename T>
    void blitsort(T *array, s32 elem_count, s32 elem_size);

    // custom compare function
    void blitsort_cmp(void *array, s32 elem_count, s32 elem_size, blit_sort::blit_sort_cmp cmp);
};

#endif
