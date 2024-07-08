#ifndef __CSORT_MERGE_ARRAY_H__
#define __CSORT_MERGE_ARRAY_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    typedef s8 (*merge_cmp_fn)(void const* inItemA, void const* inItemB, void* inUserData);

    // larray has enough space to hold both arrays
    void merge_sorted_arrays(s8* larray, s32 lcount, s8* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data);
    void merge_sorted_arrays(s16* larray, s32 lcount, s16* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data);
    void merge_sorted_arrays(s32* larray, s32 lcount, s32* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data);
    void merge_sorted_arrays(s64* larray, s32 lcount, s64* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data);

    void merge_sorted_arrays(u8* larray, s32 lcount, u8* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data);
    void merge_sorted_arrays(u16* larray, s32 lcount, u16* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data);
    void merge_sorted_arrays(u32* larray, s32 lcount, u32* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data);
    void merge_sorted_arrays(u64* larray, s32 lcount, u64* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data);

    void merge_sorted_arrays(f32* larray, s32 lcount, f32* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data);
    void merge_sorted_arrays(f64* larray, s32 lcount, f64* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data);

};  // namespace ncore

#endif
