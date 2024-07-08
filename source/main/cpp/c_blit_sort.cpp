// blitsort 1.2.1.2 - Igor van den Hoven ivdhoven@gmail.com

// Blitsort is an in-place rotate quick/merge sort based on the stable out-of-place
// merge sort quadsort, stable out-of-place quicksort fluxsort, and unstable in-place crumsort.

#include "csort/c_blit_sort.h"
#include "csort/c_quad_sort.h"

#include "csort/private/c_quad_sort.inl"
#include "csort/private/c_blit_sort.inl"

namespace ncore
{
    // u8, s8, u16, s16, u32, s32, u64, s64, f32, f64
    void g_blit_sort(s8 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp, void *user_data) { blit_sort::blitsort<s8>(array, elem_count, cmp, user_data); }
    void g_blit_sort(s16 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp, void *user_data) { blit_sort::blitsort<s16>(array, elem_count, cmp, user_data); }
    void g_blit_sort(s32 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp, void *user_data) { blit_sort::blitsort<s32>(array, elem_count, cmp, user_data); }
    void g_blit_sort(s64 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp, void *user_data) { blit_sort::blitsort<s64>(array, elem_count, cmp, user_data); }

    void g_blit_sort(u8 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp, void *user_data) { blit_sort::blitsort<u8>(array, elem_count, cmp, user_data); }
    void g_blit_sort(u16 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp, void *user_data) { blit_sort::blitsort<u16>(array, elem_count, cmp, user_data); }
    void g_blit_sort(u32 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp, void *user_data) { blit_sort::blitsort<u32>(array, elem_count, cmp, user_data); }
    void g_blit_sort(u64 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp, void *user_data) { blit_sort::blitsort<u64>(array, elem_count, cmp, user_data); }

    void g_blit_sort(f32 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp, void *user_data) { blit_sort::blitsort<f32>(array, elem_count, cmp, user_data); }
    void g_blit_sort(f64 *array, s32 elem_count, blit_sort::sort_cmp_fn cmp, void *user_data) { blit_sort::blitsort<f64>(array, elem_count, cmp, user_data); }

};  // namespace ncore
