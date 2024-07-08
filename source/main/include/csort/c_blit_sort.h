#ifndef __CSORT_BLIT_SORT_H__
#define __CSORT_BLIT_SORT_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace blit_sort
    {
        typedef s8 (*sort_cmp_fn)(void const *inItemA, void const *inItemB, void *inUserData);
    }
    
    //----------------------------------------------------------------------------------------------------------------
    // Blit Sort
    template <typename T>
    inline s8 blit_sort_cmp_fn(void const *item_a, void const *item_b, void *user_data)
    {
        T const *a = (T const *)item_a;
        T const *b = (T const *)item_b;
        return *a > *b;
    }

    void g_blit_sort(s8 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp = blit_sort_cmp_fn<s8>, void *user_data = nullptr);
    void g_blit_sort(s16 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp = blit_sort_cmp_fn<s16>, void *user_data = nullptr);
    void g_blit_sort(s32 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp = blit_sort_cmp_fn<s32>, void *user_data = nullptr);
    void g_blit_sort(s64 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp = blit_sort_cmp_fn<s64>, void *user_data = nullptr);

    void g_blit_sort(u8 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp = blit_sort_cmp_fn<u8>, void *user_data = nullptr);
    void g_blit_sort(u16 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp = blit_sort_cmp_fn<u16>, void *user_data = nullptr);
    void g_blit_sort(u32 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp = blit_sort_cmp_fn<u32>, void *user_data = nullptr);
    void g_blit_sort(u64 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp = blit_sort_cmp_fn<u64>, void *user_data = nullptr);

    void g_blit_sort(f32 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp = blit_sort_cmp_fn<f32>, void *user_data = nullptr);
    void g_blit_sort(f64 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp = blit_sort_cmp_fn<f64>, void *user_data = nullptr);

};  // namespace ncore

#endif
