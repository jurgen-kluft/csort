//==============================================================================
// INCLUDES
//==============================================================================

// blitsort 1.2.1.2 - Igor van den Hoven ivdhoven@gmail.com
#include "csort/c_blit_sort.h"

//==============================================================================
// ccore namespace
//==============================================================================
namespace ncore
{
    // u8, s8, u16, s16, u32, s32, u64, s64, f32, f64
    void blitsort(void *array, s32 elem_count, s32 elem_size, bool _signed) {}

    // custom compare function
    void blitsort_cmp(void *array, s32 elem_count, s32 elem_size, sort_cmp_predicate_fn cmp) {}

};  // namespace ncore
