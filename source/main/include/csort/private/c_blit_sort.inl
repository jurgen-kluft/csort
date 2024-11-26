#ifndef __CSORT_BLIT_SORT_INL__
#define __CSORT_BLIT_SORT_INL__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace blit_sort
    {

#define BLIT_AUX   512  // set to 0 for sqrt(n) cache size
#define BLIT_OUT   96   // should be smaller or equal to BLIT_AUX
#define QUAD_CACHE 32

        template <typename VAR>
        void memory_copy(VAR *dest, VAR const *src, u32 nmemb)
        {
            for (u32 i = 0; i < nmemb; i++)
            {
                dest[i] = src[i];
            }
        }

        template <typename VAR>
        void blit_partition(VAR *array, VAR *swap, u32 swap_size, u32 nmemb, sort_cmp_fn cmp, void *user_data);

        template <typename VAR>
        void blit_analyze(VAR *array, VAR *swap, u32 swap_size, u32 nmemb, sort_cmp_fn cmp, void *user_data)
        {
            unsigned char loop, asum, bsum, csum, dsum;
            unsigned int  astreaks, bstreaks, cstreaks, dstreaks;
            u32           quad1, quad2, quad3, quad4, half1, half2;
            u32           cnt, abalance, bbalance, cbalance, dbalance;
            VAR          *pta, *ptb, *ptc, *ptd;

            half1 = nmemb / 2;
            quad1 = half1 / 2;
            quad2 = half1 - quad1;
            half2 = nmemb - half1;
            quad3 = half2 / 2;
            quad4 = half2 - quad3;

            pta = array;
            ptb = array + quad1;
            ptc = array + half1;
            ptd = array + half1 + quad3;

            astreaks = bstreaks = cstreaks = dstreaks = 0;
            abalance = bbalance = cbalance = dbalance = 0;

            for (cnt = nmemb; cnt > 132; cnt -= 128)
            {
                for (asum = bsum = csum = dsum = 0, loop = 32; loop; loop--)
                {
                    asum += cmp(pta, pta + 1, user_data) > 0;
                    pta++;
                    bsum += cmp(ptb, ptb + 1, user_data) > 0;
                    ptb++;
                    csum += cmp(ptc, ptc + 1, user_data) > 0;
                    ptc++;
                    dsum += cmp(ptd, ptd + 1, user_data) > 0;
                    ptd++;
                }
                abalance += asum;
                astreaks += asum = (asum == 0) | (asum == 32);
                bbalance += bsum;
                bstreaks += bsum = (bsum == 0) | (bsum == 32);
                cbalance += csum;
                cstreaks += csum = (csum == 0) | (csum == 32);
                dbalance += dsum;
                dstreaks += dsum = (dsum == 0) | (dsum == 32);

                if (cnt > 516 && asum + bsum + csum + dsum == 0)
                {
                    abalance += 48;
                    pta += 96;
                    bbalance += 48;
                    ptb += 96;
                    cbalance += 48;
                    ptc += 96;
                    dbalance += 48;
                    ptd += 96;
                    cnt -= 384;
                }
            }

            for (; cnt > 7; cnt -= 4)
            {
                abalance += cmp(pta, pta + 1, user_data) > 0;
                pta++;
                bbalance += cmp(ptb, ptb + 1, user_data) > 0;
                ptb++;
                cbalance += cmp(ptc, ptc + 1, user_data) > 0;
                ptc++;
                dbalance += cmp(ptd, ptd + 1, user_data) > 0;
                ptd++;
            }

            if (quad1 < quad2)
            {
                bbalance += cmp(ptb, ptb + 1, user_data) > 0;
                ptb++;
            }
            if (quad1 < quad3)
            {
                cbalance += cmp(ptc, ptc + 1, user_data) > 0;
                ptc++;
            }
            if (quad1 < quad4)
            {
                dbalance += cmp(ptd, ptd + 1, user_data) > 0;
                ptd++;
            }

            cnt = abalance + bbalance + cbalance + dbalance;

            if (cnt == 0)
            {
                if (cmp(pta, pta + 1, user_data) <= 0 && cmp(ptb, ptb + 1, user_data) <= 0 && cmp(ptc, ptc + 1, user_data) <= 0)
                {
                    return;
                }
            }

            asum = quad1 - abalance == 1;
            bsum = quad2 - bbalance == 1;
            csum = quad3 - cbalance == 1;
            dsum = quad4 - dbalance == 1;

            if (asum | bsum | csum | dsum)
            {
                unsigned char span1 = (asum && bsum) * (cmp(pta, pta + 1, user_data) > 0);
                unsigned char span2 = (bsum && csum) * (cmp(ptb, ptb + 1, user_data) > 0);
                unsigned char span3 = (csum && dsum) * (cmp(ptc, ptc + 1, user_data) > 0);

                switch (span1 | span2 * 2 | span3 * 4)
                {
                    case 0: break;
                    case 1:
                        quad_sort::quad_reversal(array, ptb);
                        abalance = bbalance = 0;
                        break;
                    case 2:
                        quad_sort::quad_reversal(pta + 1, ptc);
                        bbalance = cbalance = 0;
                        break;
                    case 3:
                        quad_sort::quad_reversal(array, ptc);
                        abalance = bbalance = cbalance = 0;
                        break;
                    case 4:
                        quad_sort::quad_reversal(ptb + 1, ptd);
                        cbalance = dbalance = 0;
                        break;
                    case 5:
                        quad_sort::quad_reversal(array, ptb);
                        quad_sort::quad_reversal(ptb + 1, ptd);
                        abalance = bbalance = cbalance = dbalance = 0;
                        break;
                    case 6:
                        quad_sort::quad_reversal(pta + 1, ptd);
                        bbalance = cbalance = dbalance = 0;
                        break;
                    case 7: quad_sort::quad_reversal(array, ptd); return;
                }

                if (asum && abalance)
                {
                    quad_sort::quad_reversal(array, pta);
                    abalance = 0;
                }
                if (bsum && bbalance)
                {
                    quad_sort::quad_reversal(pta + 1, ptb);
                    bbalance = 0;
                }
                if (csum && cbalance)
                {
                    quad_sort::quad_reversal(ptb + 1, ptc);
                    cbalance = 0;
                }
                if (dsum && dbalance)
                {
                    quad_sort::quad_reversal(ptc + 1, ptd);
                    dbalance = 0;
                }
            }

            cnt = nmemb / 512;  // more than 25% ordered

            asum = astreaks > cnt;
            bsum = bstreaks > cnt;
            csum = cstreaks > cnt;
            dsum = dstreaks > cnt;

            if (quad1 > QUAD_CACHE)
            {
                asum = bsum = csum = dsum = 1;
            }

            switch (asum + bsum * 2 + csum * 4 + dsum * 8)
            {
                case 0: blit_partition(array, swap, swap_size, nmemb, cmp, user_data); return;
                case 1:
                    if (abalance)
                        quad_sort::quadsort_swap(array, swap, swap_size, quad1, cmp, user_data);
                    blit_partition(pta + 1, swap, swap_size, quad2 + half2, cmp, user_data);
                    break;
                case 2:
                    blit_partition(array, swap, swap_size, quad1, cmp, user_data);
                    if (bbalance)
                        quad_sort::quadsort_swap(pta + 1, swap, swap_size, quad2, cmp, user_data);
                    blit_partition(ptb + 1, swap, swap_size, half2, cmp, user_data);
                    break;
                case 3:
                    if (abalance)
                        quad_sort::quadsort_swap(array, swap, swap_size, quad1, cmp, user_data);
                    if (bbalance)
                        quad_sort::quadsort_swap(pta + 1, swap, swap_size, quad2, cmp, user_data);
                    blit_partition(ptb + 1, swap, swap_size, half2, cmp, user_data);
                    break;
                case 4:
                    blit_partition(array, swap, swap_size, half1, cmp, user_data);
                    if (cbalance)
                        quad_sort::quadsort_swap(ptb + 1, swap, swap_size, quad3, cmp, user_data);
                    blit_partition(ptc + 1, swap, swap_size, quad4, cmp, user_data);
                    break;
                case 8:
                    blit_partition(array, swap, swap_size, half1 + quad3, cmp, user_data);
                    if (dbalance)
                        quad_sort::quadsort_swap(ptc + 1, swap, swap_size, quad4, cmp, user_data);
                    break;
                case 9:
                    if (abalance)
                        quad_sort::quadsort_swap(array, swap, swap_size, quad1, cmp, user_data);
                    blit_partition(pta + 1, swap, swap_size, quad2 + quad3, cmp, user_data);
                    if (dbalance)
                        quad_sort::quadsort_swap(ptc + 1, swap, swap_size, quad4, cmp, user_data);
                    break;
                case 12:
                    blit_partition(array, swap, swap_size, half1, cmp, user_data);
                    if (cbalance)
                        quad_sort::quadsort_swap(ptb + 1, swap, swap_size, quad3, cmp, user_data);
                    if (dbalance)
                        quad_sort::quadsort_swap(ptc + 1, swap, swap_size, quad4, cmp, user_data);
                    break;
                case 5:
                case 6:
                case 7:
                case 10:
                case 11:
                case 13:
                case 14:
                case 15:
                    if (asum)
                    {
                        if (abalance)
                            quad_sort::quadsort_swap(array, swap, swap_size, quad1, cmp, user_data);
                    }
                    else
                        blit_partition(array, swap, swap_size, quad1, cmp, user_data);
                    if (bsum)
                    {
                        if (bbalance)
                            quad_sort::quadsort_swap(pta + 1, swap, swap_size, quad2, cmp, user_data);
                    }
                    else
                        blit_partition(pta + 1, swap, swap_size, quad2, cmp, user_data);
                    if (csum)
                    {
                        if (cbalance)
                            quad_sort::quadsort_swap(ptb + 1, swap, swap_size, quad3, cmp, user_data);
                    }
                    else
                        blit_partition(ptb + 1, swap, swap_size, quad3, cmp, user_data);
                    if (dsum)
                    {
                        if (dbalance)
                            quad_sort::quadsort_swap(ptc + 1, swap, swap_size, quad4, cmp, user_data);
                    }
                    else
                        blit_partition(ptc + 1, swap, swap_size, quad4, cmp, user_data);
                    break;
            }

            if (cmp(pta, pta + 1, user_data) <= 0)
            {
                if (cmp(ptc, ptc + 1, user_data) <= 0)
                {
                    if (cmp(ptb, ptb + 1, user_data) <= 0)
                    {
                        return;
                    }
                }
                else
                {
                    quad_sort::rotate_merge_block(array + half1, swap, swap_size, quad3, quad4, cmp, user_data);
                }
            }
            else
            {
                quad_sort::rotate_merge_block(array, swap, swap_size, quad1, quad2, cmp, user_data);

                if (cmp(ptc, ptc + 1, user_data) > 0)
                {
                    quad_sort::rotate_merge_block(array + half1, swap, swap_size, quad3, quad4, cmp, user_data);
                }
            }
            quad_sort::rotate_merge_block(array, swap, swap_size, half1, half2, cmp, user_data);
        }

        // The next 4 functions are used for pivot selection

        template <typename VAR>
        VAR blit_binary_median(VAR const *pta, VAR const *ptb, u32 len, sort_cmp_fn cmp, void *user_data)
        {
            while (len /= 2)
            {
                if (cmp(pta + len, ptb + len, user_data) <= 0)
                    pta += len;
                else
                    ptb += len;
            }
            return cmp(pta, ptb, user_data) > 0 ? *pta : *ptb;
        }

        template <typename VAR>
        void blit_trim_four(VAR *pta, sort_cmp_fn cmp, void *user_data)
        {
            VAR swap;
            s32 x;

            x      = cmp(pta, pta + 1, user_data) > 0;
            swap   = pta[!x];
            pta[0] = pta[x];
            pta[1] = swap;
            pta += 2;
            x      = cmp(pta, pta + 1, user_data) > 0;
            swap   = pta[!x];
            pta[0] = pta[x];
            pta[1] = swap;
            pta -= 2;

            x      = (cmp(pta, pta + 2, user_data) <= 0) * 2;
            pta[2] = pta[x];
            pta++;
            x      = (cmp(pta, pta + 2, user_data) > 0) * 2;
            pta[0] = pta[x];
        }

        template <typename VAR>
        VAR blit_median_of_nine(VAR *array, VAR *swap, u32 nmemb, sort_cmp_fn cmp, void *user_data)
        {
            VAR *pta;
            u32  x, y, z;

            z = nmemb / 9;

            pta = array;

            for (x = 0; x < 9; x++)
            {
                swap[x] = *pta;

                pta += z;
            }

            blit_trim_four(swap, cmp, user_data);
            blit_trim_four(swap + 4, cmp, user_data);

            swap[0] = swap[5];
            swap[3] = swap[8];

            blit_trim_four(swap, cmp, user_data);

            swap[0] = swap[6];

            x = cmp(swap + 0, swap + 1, user_data) > 0;
            y = cmp(swap + 0, swap + 2, user_data) > 0;
            z = cmp(swap + 1, swap + 2, user_data) > 0;

            return swap[(x == y) + (y ^ z)];
        }

        template <typename VAR>
        VAR blit_median_of_cbrt(VAR *array, VAR *swap, u32 swap_size, u32 nmemb, int *generic, sort_cmp_fn cmp, void *user_data)
        {
            VAR *pta, *ptb, *pts;
            u32  cnt, div, cbrt;

            for (cbrt = 32; nmemb > cbrt * cbrt * cbrt && cbrt < swap_size; cbrt *= 2) {}

            div = nmemb / cbrt;

            pta = array;  // + (s32) &div / 16 % div; // for a non-deterministic offset
            pts = swap;

            for (cnt = 0; cnt < cbrt; cnt++)
            {
                pts[cnt] = *pta;

                pta += div;
            }
            pta = pts;
            ptb = pts + cbrt / 2;

            for (cnt = cbrt / 8; cnt; cnt--)
            {
                blit_trim_four(pta, cmp, user_data);
                blit_trim_four(ptb, cmp, user_data);

                pta[0] = ptb[1];
                pta[3] = ptb[2];

                pta += 4;
                ptb += 4;
            }
            cbrt /= 4;

            quad_sort::quadsort_swap(pts, pts + cbrt * 2, cbrt, cbrt, cmp, user_data);
            quad_sort::quadsort_swap(pts + cbrt, pts + cbrt * 2, cbrt, cbrt, cmp, user_data);

            *generic = cmp(pts + cbrt * 2 - 1, pts, user_data) <= 0;

            return blit_binary_median(pts, pts + cbrt, cbrt, cmp, user_data);
        }

        // As per suggestion by Marshall Lochbaum to improve generic data handling

        template <typename VAR>
        u32 blit_reverse_partition(VAR *array, VAR *swap, VAR *piv, u32 swap_size, u32 nmemb, sort_cmp_fn cmp, void *user_data)
        {
            if (nmemb > swap_size)
            {
                u32 l, r, h = nmemb / 2;

                l = blit_reverse_partition(array + 0, swap, piv, swap_size, h, cmp, user_data);
                r = blit_reverse_partition(array + h, swap, piv, swap_size, nmemb - h, cmp, user_data);

quad_sort::                trinity_rotation(array + l, swap, swap_size, h - l + r, h - l);

                return l + r;
            }
            u32  cnt, m;
            VAR *tmp, *ptx = array, *pta = array, *pts = swap;

            for (cnt = nmemb / 4; cnt; cnt--)
            {
                tmp  = cmp(piv, ptx, user_data) > 0 ? pta++ : pts++;
                *tmp = *ptx++;
                tmp  = cmp(piv, ptx, user_data) > 0 ? pta++ : pts++;
                *tmp = *ptx++;
                tmp  = cmp(piv, ptx, user_data) > 0 ? pta++ : pts++;
                *tmp = *ptx++;
                tmp  = cmp(piv, ptx, user_data) > 0 ? pta++ : pts++;
                *tmp = *ptx++;
            }

            for (cnt = nmemb % 4; cnt; cnt--)
            {
                tmp  = cmp(piv, ptx, user_data) > 0 ? pta++ : pts++;
                *tmp = *ptx++;
            }
            m = (u32)(pta - array);

            memory_copy(array + m, swap, (nmemb - m) * sizeof(VAR));

            return m;
        }

        template <typename VAR>
        s32 blit_default_partition(VAR *array, VAR *swap, VAR *piv, u32 swap_size, u32 nmemb, sort_cmp_fn cmp, void *user_data)
        {
            if (nmemb > swap_size)
            {
                s32 l, r, h = nmemb / 2;

                l = blit_default_partition(array + 0, swap, piv, swap_size, h, cmp, user_data);
                r = blit_default_partition(array + h, swap, piv, swap_size, nmemb - h, cmp, user_data);

                quad_sort::trinity_rotation(array + l, swap, swap_size, h - l + r, h - l);

                return l + r;
            }

            s32  cnt, m;
            VAR *tmp, *ptx = array, *pta = array, *pts = swap;

            for (cnt = nmemb / 4; cnt; cnt--)
            {
                tmp  = cmp(ptx, piv, user_data) <= 0 ? pta++ : pts++;
                *tmp = *ptx++;
                tmp  = cmp(ptx, piv, user_data) <= 0 ? pta++ : pts++;
                *tmp = *ptx++;
                tmp  = cmp(ptx, piv, user_data) <= 0 ? pta++ : pts++;
                *tmp = *ptx++;
                tmp  = cmp(ptx, piv, user_data) <= 0 ? pta++ : pts++;
                *tmp = *ptx++;
            }

            for (cnt = nmemb % 4; cnt; cnt--)
            {
                tmp  = cmp(ptx, piv, user_data) <= 0 ? pta++ : pts++;
                *tmp = *ptx++;
            }
            m = (u32)(pta - array);

            memory_copy(array + m, swap, sizeof(VAR) * (nmemb - m));

            return m;
        }

        template <typename VAR>
        void blit_partition(VAR *array, VAR *swap, u32 swap_size, u32 nmemb, sort_cmp_fn cmp, void *user_data)
        {
            s32 a_size = 0, s_size;
            VAR piv, max = 0;
            int generic = 0;

            while (1)
            {
                if (nmemb <= 2048)
                {
                    piv = blit_median_of_nine(array, swap, nmemb, cmp, user_data);
                }
                else
                {
                    piv = blit_median_of_cbrt(array, swap, swap_size, nmemb, &generic, cmp, user_data);

                    if (generic)
                    {
                        quad_sort::quadsort_swap(array, swap, swap_size, nmemb, cmp, user_data);
                        return;
                    }
                }

                if (a_size && cmp(&max, &piv, user_data) <= 0)
                {
                    a_size = blit_reverse_partition(array, swap, &piv, swap_size, nmemb, cmp, user_data);
                    s_size = nmemb - a_size;

                    if (s_size <= a_size / 16 || a_size <= BLIT_OUT)
                    {
                        quad_sort::quadsort_swap(array, swap, swap_size, a_size, cmp, user_data);
                        return;
                    }
                    nmemb  = a_size;
                    a_size = 0;
                    continue;
                }

                a_size = blit_default_partition(array, swap, &piv, swap_size, nmemb, cmp, user_data);
                s_size = nmemb - a_size;

                if (a_size <= s_size / 16 || s_size <= BLIT_OUT)
                {
                    if (s_size == 0)
                    {
                        a_size = blit_reverse_partition(array, swap, &piv, swap_size, a_size, cmp, user_data);
                        s_size = nmemb - a_size;

                        if (s_size <= a_size / 16 || a_size <= BLIT_OUT)
                        {
                            return quad_sort::quadsort_swap(array, swap, swap_size, a_size, cmp, user_data);
                        }
                        nmemb  = a_size;
                        a_size = 0;
                        continue;
                    }
                    quad_sort::quadsort_swap(array + a_size, swap, swap_size, s_size, cmp, user_data);
                }
                else
                {
                    blit_partition(array + a_size, swap, swap_size, s_size, cmp, user_data);
                }

                if (s_size <= a_size / 16 || a_size <= BLIT_OUT)
                {
                    quad_sort::quadsort_swap(array, swap, swap_size, a_size, cmp, user_data);
                    return;
                }
                nmemb = a_size;
                max   = piv;
            }
        }

        template <typename VAR>
        void blitsort(void *array, s32 nmemb, sort_cmp_fn cmp, void *user_data)
        {
            if (nmemb <= 132)
            {
                quad_sort::quadsort<VAR>(array, nmemb, cmp, user_data);
            }
            else
            {
                VAR *pta = (VAR *)array;
                const s32 swap_size = BLIT_AUX;
                VAR swap[swap_size];
                blit_analyze(pta, swap, swap_size, nmemb, cmp, user_data);
            }
        }

        template <typename VAR>
        void blitsort_swap(void *array, void *swap, s32 swap_size, s32 nmemb, sort_cmp_fn cmp, void *user_data)
        {
            if (nmemb <= 132)
            {
                quad_sort::quadsort_swap<VAR>(array, swap, swap_size, nmemb, cmp, user_data);
            }
            else
            {
                VAR *pta = (VAR *)array;
                VAR *pts = (VAR *)swap;

                blit_analyze(pta, pts, swap_size, nmemb, cmp, user_data);
            }
        }

#undef BLIT_AUX
#undef BLIT_OUT
#undef QUAD_CACHE
    }  // namespace blit_sort
}  // namespace ncore

#endif
