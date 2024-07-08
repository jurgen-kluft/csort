#include "csort/c_quad_sort.h"
#include "csort/private/c_quad_sort.inl"

namespace ncore
{
    // u8, s8, u16, s16, u32, s32, u64, s64, f32, f64
    void g_quad_sort(s8 *array, s32 elem_count, quad_sort::sort_cmp_fn cmp, void *user_data) { quad_sort::quadsort<s8>(array, elem_count, cmp, user_data); }
    void g_quad_sort(s16 *array, s32 elem_count, quad_sort::sort_cmp_fn cmp, void *user_data) { quad_sort::quadsort<s16>(array, elem_count, cmp, user_data); }
    void g_quad_sort(s32 *array, s32 elem_count, quad_sort::sort_cmp_fn cmp, void *user_data) { quad_sort::quadsort<s32>(array, elem_count, cmp, user_data); }
    void g_quad_sort(s64 *array, s32 elem_count, quad_sort::sort_cmp_fn cmp, void *user_data) { quad_sort::quadsort<s64>(array, elem_count, cmp, user_data); }

    void g_quad_sort(u8 *array, s32 elem_count, quad_sort::sort_cmp_fn cmp, void *user_data) { quad_sort::quadsort<u8>(array, elem_count, cmp, user_data); }
    void g_quad_sort(u16 *array, s32 elem_count, quad_sort::sort_cmp_fn cmp, void *user_data) { quad_sort::quadsort<u16>(array, elem_count, cmp, user_data); }
    void g_quad_sort(u32 *array, s32 elem_count, quad_sort::sort_cmp_fn cmp, void *user_data) { quad_sort::quadsort<u32>(array, elem_count, cmp, user_data); }
    void g_quad_sort(u64 *array, s32 elem_count, quad_sort::sort_cmp_fn cmp, void *user_data) { quad_sort::quadsort<u64>(array, elem_count, cmp, user_data); }

    void g_quad_sort(f32 *array, s32 elem_count, quad_sort::sort_cmp_fn cmp, void *user_data) { quad_sort::quadsort<f32>(array, elem_count, cmp, user_data); }
    void g_quad_sort(f64 *array, s32 elem_count, quad_sort::sort_cmp_fn cmp, void *user_data) { quad_sort::quadsort<f64>(array, elem_count, cmp, user_data); }

}; // namespace ncore
