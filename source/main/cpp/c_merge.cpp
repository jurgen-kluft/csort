#include "csort/c_merge.h"

namespace ncore
{
    // larray has enough space to hold both arrays
    template <typename T>
    void merge_sorted_arrays_T(T* larray, s32 lcount, T* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data)
    {
        if (lcount == 0 || rcount == 0)
            return;

        T* lend = larray + lcount - 1;
        T* rend = rarray + rcount - 1;

        T* dst = larray + lcount + rcount - 1;
        while (lend >= larray && rend >= rarray)
        {
            if (cmp(lend, rend, user_data) > 0)
                *dst-- = *lend--;
            else
                *dst-- = *rend--;
        }
        while (lend >= larray)
            *dst-- = *lend--;
        while (rend >= rarray)
            *dst-- = *rend--;
    }

    void merge_sorted_arrays(s8* larray, s32 lcount, s8* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data) { merge_sorted_arrays_T<s8>(larray, lcount, rarray, rcount, cmp, user_data); }
    void merge_sorted_arrays(s16* larray, s32 lcount, s16* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data) { merge_sorted_arrays_T<s16>(larray, lcount, rarray, rcount, cmp, user_data); }
    void merge_sorted_arrays(s32* larray, s32 lcount, s32* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data) { merge_sorted_arrays_T<s32>(larray, lcount, rarray, rcount, cmp, user_data); }
    void merge_sorted_arrays(s64* larray, s32 lcount, s64* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data) { merge_sorted_arrays_T<s64>(larray, lcount, rarray, rcount, cmp, user_data); }

    void merge_sorted_arrays(u8* larray, s32 lcount, u8* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data) { merge_sorted_arrays_T<u8>(larray, lcount, rarray, rcount, cmp, user_data); }
    void merge_sorted_arrays(u16* larray, s32 lcount, u16* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data) { merge_sorted_arrays_T<u16>(larray, lcount, rarray, rcount, cmp, user_data); }
    void merge_sorted_arrays(u32* larray, s32 lcount, u32* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data) { merge_sorted_arrays_T<u32>(larray, lcount, rarray, rcount, cmp, user_data); }
    void merge_sorted_arrays(u64* larray, s32 lcount, u64* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data) { merge_sorted_arrays_T<u64>(larray, lcount, rarray, rcount, cmp, user_data); }

    void merge_sorted_arrays(f32* larray, s32 lcount, f32* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data) { merge_sorted_arrays_T<f32>(larray, lcount, rarray, rcount, cmp, user_data); }
    void merge_sorted_arrays(f64* larray, s32 lcount, f64* rarray, s32 rcount, merge_cmp_fn cmp, void* user_data) { merge_sorted_arrays_T<f64>(larray, lcount, rarray, rcount, cmp, user_data); }

};  // namespace ncore
