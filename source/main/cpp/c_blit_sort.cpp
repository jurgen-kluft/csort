// blitsort 1.2.1.2 - Igor van den Hoven ivdhoven@gmail.com

// Blitsort is an in-place rotate quick/merge sort based on the stable out-of-place
// merge sort quadsort, stable out-of-place quicksort fluxsort, and unstable in-place crumsort.

#include "csort/c_blit_sort.h"

#include "csort/private/c_quad_sort.inl"
#include "csort/private/c_blit_sort.inl"

namespace ncore
{
    // u8, s8, u16, s16, u32, s32, u64, s64, f32, f64
    template <typename T>
    inline s8 sort_cmp_prim_fn(void const *item_a, void const *item_b, void *user_data)
    {
        T const *a = (T const *)item_a;
        T const *b = (T const *)item_b;
        return *a > *b;
    }

    void g_blit_sort(s8 *array, s32 elem_count, void *user_data)
    {
        blit_sort::blitsort<s8>(array, elem_count,  sort_cmp_prim_fn<s8>, user_data);
    }

    void g_blit_sort(s16 *array, s32 elem_count, void *user_data)
    {
        blit_sort::blitsort<s16>(array, elem_count,  sort_cmp_prim_fn<s16>, user_data);
    }
    
    void g_blit_sort(s32 *array, s32 elem_count, void *user_data)
    {
        blit_sort::blitsort<s32>(array, elem_count,  sort_cmp_prim_fn<s32>, user_data);
    }
    
    void g_blit_sort(s64 *array, s32 elem_count, void *user_data)
    {
        blit_sort::blitsort<s64>(array, elem_count,  sort_cmp_prim_fn<s64>, user_data);
    }
    
    void g_blit_sort(u8 *array, s32 elem_count, void *user_data)
    {
        blit_sort::blitsort<u8>(array, elem_count,  sort_cmp_prim_fn<u8>, user_data);
    }

    void g_blit_sort(u16 *array, s32 elem_count, void *user_data)
    {
        blit_sort::blitsort<u16>(array, elem_count,  sort_cmp_prim_fn<u16>, user_data);
    }
    
    void g_blit_sort(u32 *array, s32 elem_count, void *user_data)
    {
        blit_sort::blitsort<u32>(array, elem_count,  sort_cmp_prim_fn<u32>, user_data);
    }
    
    void g_blit_sort(u64 *array, s32 elem_count, void *user_data)
    {
        blit_sort::blitsort<u64>(array, elem_count,  sort_cmp_prim_fn<u64>, user_data);
    }


    void g_blit_sort(f32 *array, s32 elem_count, void *user_data)
    {
        blit_sort::blitsort<f32>(array, elem_count,  sort_cmp_prim_fn<f32>, user_data);
    }
    
    void g_blit_sort(f64 *array, s32 elem_count, void *user_data)
    {
        blit_sort::blitsort<f64>(array, elem_count,  sort_cmp_prim_fn<f64>, user_data);
    }

}; // namespace ncore
