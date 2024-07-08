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

    void g_blit_sort(s8 *array, s32 elem_count, void *user_data);
    void g_blit_sort(s16 *array, s32 elem_count, void *user_data);
    void g_blit_sort(s32 *array, s32 elem_count, void *user_data);
    void g_blit_sort(s64 *array, s32 elem_count, void *user_data);

    void g_blit_sort(u8 *array, s32 elem_count, void *user_data);
    void g_blit_sort(u16 *array, s32 elem_count, void *user_data);
    void g_blit_sort(u32 *array, s32 elem_count, void *user_data);
    void g_blit_sort(u64 *array, s32 elem_count, void *user_data);

    void g_blit_sort(f32 *array, s32 elem_count, void *user_data);
    void g_blit_sort(f64 *array, s32 elem_count, void *user_data);

}; // namespace ncore

#endif
