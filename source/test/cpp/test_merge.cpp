#include "csort/c_merge.h"

#include "cunittest/cunittest.h"

using namespace ncore;

static s8 s32_compare(const void* const a, const void* const b, const void* user_data)
{
    const ncore::s32* aa = (const ncore::s32*)a;
    const ncore::s32* bb = (const ncore::s32*)b;
    if (*aa < *bb)
        return -1;
    else if (*aa > *bb)
        return 1;
    return 0;
}

static s8 f32_compare(const void* const a, const void* const b, const void* user_data)
{
    const ncore::f32* aa = (const ncore::f32*)a;
    const ncore::f32* bb = (const ncore::f32*)b;
    if (*aa < *bb)
        return -1;
    else if (*aa > *bb)
        return 1;
    return 0;
}

UNITTEST_SUITE_BEGIN(merge)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(merge_s32)
        {
            s32 number_list[] = {0,1,2,3,4,5,6,7,0,0};
            s32 other_list[] = {8,9};

            g_merge_sorted_arrays(number_list, 8, other_list, 2, s32_compare, nullptr);

            for (s32 i = 0; i < 9; ++i)
                CHECK_TRUE(number_list[i] <= number_list[i + 1]);

            CHECK_EQUAL(0, number_list[0]);
            CHECK_EQUAL(9, number_list[9]);
        }

        UNITTEST_TEST(merge_f32)
        {
            f32 number_list[] = {0,1,2,4,5,7,8,9};
            f32 other_list[] = {3,6};

            g_merge_sorted_arrays(number_list, 8, other_list, 2, f32_compare, nullptr);

            for (s32 i = 0; i < 9; ++i)
                CHECK_TRUE(number_list[i] <= number_list[i + 1]);

            CHECK_EQUAL(0, number_list[0]);
            CHECK_EQUAL(9, number_list[9]);
        }
    }
}
UNITTEST_SUITE_END
