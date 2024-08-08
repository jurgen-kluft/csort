#ifndef __CSORT_QUAD_SORT_INL__
#define __CSORT_QUAD_SORT_INL__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

namespace ncore
{
    namespace quad_sort
    {
        // quadsort 1.2.1.2 - Igor van den Hoven ivdhoven@gmail.com

        //        void *malloc(u64 size);
        //        void  free(void *ptr);

        inline void memory_copy(void *dest, const void *src, u64 numBytes)
        {
            u8       *d = (u8 *)dest;
            u8 const *s = (u8 const *)src;
            while (numBytes--)
            {
                *d++ = *s++;
            }
        }

        inline void memory_set(void *s, int c, u64 n)
        {
            u8 *p = (u8 *)s;
            while (n--)
            {
                *p++ = (u8)c;
            }
        }

        inline void memory_move(void *dest, const void *src, u64 n)
        {
            u8       *d  = (u8 *)dest;
            u8       *de = d + n;
            u8 const *s  = (u8 const *)src;
            u8 const *se = s + n;
            if ((d >= se) || (de <= s))
            {
                memory_copy(d, s, n);
            }
            else if (d < s)
            {
                while (s < se)
                {
                    *d++ = *s++;
                }
            }
            else
            {
                s += n;
                while (de > d)
                {
                    *--de = *--s;
                }
            }
        }

#define head_branchless_merge(ptd, x, ptl, ptr, cmp, user_data) \
    x    = cmp(ptl, ptr, user_data) <= 0;                       \
    *ptd = *ptl;                                                \
    ptl += x;                                                   \
    ptd[x] = *ptr;                                              \
    ptr += !x;                                                  \
    ptd++;

#define tail_branchless_merge(tpd, y, tpl, tpr, cmp, user_data) \
    y    = cmp(tpl, tpr, user_data) <= 0;                       \
    *tpd = *tpl;                                                \
    tpl -= !y;                                                  \
    tpd--;                                                      \
    tpd[y] = *tpr;                                              \
    tpr -= y;

        // guarantee small parity merges are inlined with minimal overhead

#define parity_merge_two(array, swap, x, y, ptl, ptr, pts, cmp, user_data) \
    ptl = array;                                                           \
    ptr = array + 2;                                                       \
    pts = swap;                                                            \
    head_branchless_merge(pts, x, ptl, ptr, cmp, user_data);               \
    *pts = cmp(ptl, ptr, user_data) <= 0 ? *ptl : *ptr;                    \
                                                                           \
    ptl = array + 1;                                                       \
    ptr = array + 3;                                                       \
    pts = swap + 3;                                                        \
    tail_branchless_merge(pts, y, ptl, ptr, cmp, user_data);               \
    *pts = cmp(ptl, ptr, user_data) > 0 ? *ptl : *ptr;

#define parity_merge_four(array, swap, x, y, ptl, ptr, pts, cmp, user_data) \
    ptl = array + 0;                                                        \
    ptr = array + 4;                                                        \
    pts = swap;                                                             \
    head_branchless_merge(pts, x, ptl, ptr, cmp, user_data);                \
    head_branchless_merge(pts, x, ptl, ptr, cmp, user_data);                \
    head_branchless_merge(pts, x, ptl, ptr, cmp, user_data);                \
    *pts = cmp(ptl, ptr, user_data) <= 0 ? *ptl : *ptr;                     \
                                                                            \
    ptl = array + 3;                                                        \
    ptr = array + 7;                                                        \
    pts = swap + 7;                                                         \
    tail_branchless_merge(pts, y, ptl, ptr, cmp, user_data);                \
    tail_branchless_merge(pts, y, ptl, ptr, cmp, user_data);                \
    tail_branchless_merge(pts, y, ptl, ptr, cmp, user_data);                \
    *pts = cmp(ptl, ptr, user_data) > 0 ? *ptl : *ptr;

#define swap_branchless(pta, swap, x, y, cmp, user_data) \
    x      = cmp(pta, pta + 1, user_data) > 0;           \
    y      = !x;                                         \
    swap   = pta[y];                                     \
    pta[0] = pta[x];                                     \
    pta[1] = swap;

        template <typename VAR>
        void quad_reversal(VAR *pta, VAR *ptz);

        // the next seven functions are used for sorting 0 to 31 elements

        template <typename VAR>
        void tiny_sort(VAR *array, u32 nmemb, sort_cmp_fn cmp, void *user_data)
        {
            VAR swap, *pta;
            u32 x, y;

            switch (nmemb)
            {
                case 4:
                    pta    = array;
                    x      = cmp(pta, pta + 1, user_data) > 0;
                    y      = !x;
                    swap   = pta[y];
                    pta[0] = pta[x];
                    pta[1] = swap;
                    pta += 2;
                    x      = cmp(pta, pta + 1, user_data) > 0;
                    y      = !x;
                    swap   = pta[y];
                    pta[0] = pta[x];
                    pta[1] = swap;
                    pta--;

                    if (cmp(pta, pta + 1, user_data) > 0)
                    {
                        swap   = pta[0];
                        pta[0] = pta[1];
                        pta[1] = swap;
                        pta--;

                        x      = cmp(pta, pta + 1, user_data) > 0;
                        y      = !x;
                        swap   = pta[y];
                        pta[0] = pta[x];
                        pta[1] = swap;
                        pta += 2;
                        x      = cmp(pta, pta + 1, user_data) > 0;
                        y      = !x;
                        swap   = pta[y];
                        pta[0] = pta[x];
                        pta[1] = swap;
                        pta--;
                        x      = cmp(pta, pta + 1, user_data) > 0;
                        y      = !x;
                        swap   = pta[y];
                        pta[0] = pta[x];
                        pta[1] = swap;
                    }
                    return;
                case 3:
                    pta    = array;
                    x      = cmp(pta, pta + 1, user_data) > 0;
                    y      = !x;
                    swap   = pta[y];
                    pta[0] = pta[x];
                    pta[1] = swap;
                    pta++;
                    x      = cmp(pta, pta + 1, user_data) > 0;
                    y      = !x;
                    swap   = pta[y];
                    pta[0] = pta[x];
                    pta[1] = swap;
                case 2:
                    pta    = array;
                    x      = cmp(pta, pta + 1, user_data) > 0;
                    y      = !x;
                    swap   = pta[y];
                    pta[0] = pta[x];
                    pta[1] = swap;
                case 1:
                case 0: return;
            }
        }

        // This function requires a minimum offset of 2 to work properly

        template <typename VAR>
        void twice_unguarded_insert(VAR *array, u32 offset, u32 nmemb, sort_cmp_fn cmp, void *user_data)
        {
            VAR key, *pta, *end;
            u32 i, top, x, y;

            for (i = offset; i < nmemb; i++)
            {
                pta = end = array + i;

                if (cmp(--pta, end, user_data) <= 0)
                {
                    continue;
                }

                key = *end;

                if (cmp(array + 1, &key, user_data) > 0)
                {
                    top = i - 1;

                    do
                    {
                        *end-- = *pta--;
                    } while (--top);

                    *end-- = key;
                }
                else
                {
                    do
                    {
                        *end-- = *pta--;
                        *end-- = *pta--;
                    } while (cmp(pta, &key, user_data) > 0);

                    end[0] = end[1];
                    end[1] = key;
                }
                x      = cmp(end, end + 1, user_data) > 0;
                y      = !x;
                key    = end[y];
                end[0] = end[x];
                end[1] = key;
            }
        }

        template <typename VAR>
        void quad_swap_four(VAR *array, sort_cmp_fn cmp, void *user_data)
        {
            VAR *pta, swap;
            u32  x, y;

            pta = array;

            swap_branchless(pta, swap, x, y, cmp, user_data);
            pta += 2;
            swap_branchless(pta, swap, x, y, cmp, user_data);
            pta--;

            if (cmp(pta, pta + 1, user_data) > 0)
            {
                swap   = pta[0];
                pta[0] = pta[1];
                pta[1] = swap;
                pta--;

                swap_branchless(pta, swap, x, y, cmp, user_data);
                pta += 2;
                swap_branchless(pta, swap, x, y, cmp, user_data);
                pta--;
                swap_branchless(pta, swap, x, y, cmp, user_data);
            }
        }

        template <typename VAR>
        void parity_swap_eight(VAR *array, VAR *swap, sort_cmp_fn cmp, void *user_data)
        {
            VAR tmp, *ptl, *ptr, *pts;
            u32 x, y;

            ptl = array;

            swap_branchless(ptl, tmp, x, y, cmp, user_data);
            ptl += 2;
            swap_branchless(ptl, tmp, x, y, cmp, user_data);
            ptl += 2;
            swap_branchless(ptl, tmp, x, y, cmp, user_data);
            ptl += 2;
            swap_branchless(ptl, tmp, x, y, cmp, user_data);

            if (cmp(array + 1, array + 2, user_data) <= 0 && cmp(array + 3, array + 4, user_data) <= 0 && cmp(array + 5, array + 6, user_data) <= 0)
            {
                return;
            }
            parity_merge_two(array + 0, swap + 0, x, y, ptl, ptr, pts, cmp, user_data);
            parity_merge_two(array + 4, swap + 4, x, y, ptl, ptr, pts, cmp, user_data);

            parity_merge_four(swap, array, x, y, ptl, ptr, pts, cmp, user_data);
        }

        // left must be equal or one smaller than right

        template <typename VAR>
        void parity_merge(VAR *dest, VAR *from, u32 left, u32 right, sort_cmp_fn cmp, void *user_data)
        {
            VAR *ptl, *ptr, *tpl, *tpr, *tpd, *ptd;
            u32  x, y;

            ptl = from;
            ptr = from + left;
            ptd = dest;
            tpl = ptr - 1;
            tpr = tpl + right;
            tpd = dest + left + right - 1;

            if (left < right)
            {
                *ptd++ = cmp(ptl, ptr, user_data) <= 0 ? *ptl++ : *ptr++;
            }

            *ptd++ = cmp(ptl, ptr, user_data) <= 0 ? *ptl++ : *ptr++;

            {
                while (--left)
                {
                    head_branchless_merge(ptd, x, ptl, ptr, cmp, user_data);
                    tail_branchless_merge(tpd, y, tpl, tpr, cmp, user_data);
                }
            }
            *tpd = cmp(tpl, tpr, user_data) > 0 ? *tpl : *tpr;
        }

        template <typename VAR>
        void parity_swap_sixteen(VAR *array, VAR *swap, sort_cmp_fn cmp, void *user_data)
        {
            VAR *ptl, *ptr, *pts;
            u32  x, y;

            quad_swap_four(array + 0, cmp, user_data);
            quad_swap_four(array + 4, cmp, user_data);
            quad_swap_four(array + 8, cmp, user_data);
            quad_swap_four(array + 12, cmp, user_data);

            if (cmp(array + 3, array + 4, user_data) <= 0 && cmp(array + 7, array + 8, user_data) <= 0 && cmp(array + 11, array + 12, user_data) <= 0)
            {
                return;
            }
            parity_merge_four(array + 0, swap + 0, x, y, ptl, ptr, pts, cmp, user_data);
            parity_merge_four(array + 8, swap + 8, x, y, ptl, ptr, pts, cmp, user_data);

            parity_merge(array, swap, 8, 8, cmp, user_data);
        }

        template <typename VAR>
        void tail_swap(VAR *array, VAR *swap, u32 nmemb, sort_cmp_fn cmp, void *user_data)
        {
            if (nmemb < 5)
            {
                tiny_sort(array, nmemb, cmp, user_data);
                return;
            }
            if (nmemb < 8)
            {
                quad_swap_four(array, cmp, user_data);
                twice_unguarded_insert(array, 4, nmemb, cmp, user_data);
                return;
            }
            if (nmemb < 12)
            {
                parity_swap_eight(array, swap, cmp, user_data);
                twice_unguarded_insert(array, 8, nmemb, cmp, user_data);
                return;
            }
            if (nmemb >= 16 && nmemb < 24)
            {
                parity_swap_sixteen(array, swap, cmp, user_data);
                twice_unguarded_insert(array, 16, nmemb, cmp, user_data);
                return;
            }

            u32 quad1, quad2, quad3, quad4, half1, half2;

            half1 = nmemb / 2;
            quad1 = half1 / 2;
            quad2 = half1 - quad1;

            half2 = nmemb - half1;
            quad3 = half2 / 2;
            quad4 = half2 - quad3;

            VAR *pta = array;

            tail_swap(pta, swap, quad1, cmp, user_data);
            pta += quad1;
            tail_swap(pta, swap, quad2, cmp, user_data);
            pta += quad2;
            tail_swap(pta, swap, quad3, cmp, user_data);
            pta += quad3;
            tail_swap(pta, swap, quad4, cmp, user_data);

            if (cmp(array + quad1 - 1, array + quad1, user_data) <= 0 && cmp(array + half1 - 1, array + half1, user_data) <= 0 && cmp(pta - 1, pta, user_data) <= 0)
            {
                return;
            }

            parity_merge(swap, array, quad1, quad2, cmp, user_data);
            parity_merge(swap + half1, array + half1, quad3, quad4, cmp, user_data);
            parity_merge(array, swap, half1, half2, cmp, user_data);
        }

        // the next three functions create sorted blocks of 32 elements

        template <typename VAR>
        void quad_reversal(VAR *pta, VAR *ptz)
        {
            VAR *ptb, *pty, tmp1, tmp2;

            u32 loop = (u32)(ptz - pta) / 2;

            ptb = pta + loop;
            pty = ptz - loop;

            if (loop % 2 == 0)
            {
                tmp2   = *ptb;
                *ptb-- = *pty;
                *pty++ = tmp2;
                loop--;
            }

            loop /= 2;

            do
            {
                tmp1   = *pta;
                *pta++ = *ptz;
                *ptz-- = tmp1;
                tmp2   = *ptb;
                *ptb-- = *pty;
                *pty++ = tmp2;
            } while (loop--);
        }

        template <typename VAR>
        void quad_swap_merge(VAR *array, VAR *swap, sort_cmp_fn cmp, void *user_data)
        {
            VAR *pts, *ptl, *ptr;
            u32  x, y;

            parity_merge_two(array + 0, swap + 0, x, y, ptl, ptr, pts, cmp, user_data);
            parity_merge_two(array + 4, swap + 4, x, y, ptl, ptr, pts, cmp, user_data);

            parity_merge_four(swap, array, x, y, ptl, ptr, pts, cmp, user_data);
        }

        template <typename VAR>
        void tail_merge(VAR *array, VAR *swap, u32 swap_size, u32 nmemb, u32 block, sort_cmp_fn cmp, void *user_data);

        template <typename VAR>
        u32 quad_swap(VAR *array, u32 nmemb, sort_cmp_fn cmp, void *user_data)
        {
            VAR           tmp, swap[32];
            u32           count;
            VAR          *pta, *pts;
            unsigned char v1, v2, v3, v4, x;
            pta = array;

            count = nmemb / 8;

            while (count--)
            {
                v1 = cmp(pta + 0, pta + 1, user_data) > 0;
                v2 = cmp(pta + 2, pta + 3, user_data) > 0;
                v3 = cmp(pta + 4, pta + 5, user_data) > 0;
                v4 = cmp(pta + 6, pta + 7, user_data) > 0;

                switch (v1 + v2 * 2 + v3 * 4 + v4 * 8)
                {
                    case 0:
                        if (cmp(pta + 1, pta + 2, user_data) <= 0 && cmp(pta + 3, pta + 4, user_data) <= 0 && cmp(pta + 5, pta + 6, user_data) <= 0)
                        {
                            goto ordered;
                        }
                        quad_swap_merge(pta, swap, cmp, user_data);
                        break;

                    case 15:
                        if (cmp(pta + 1, pta + 2, user_data) > 0 && cmp(pta + 3, pta + 4, user_data) > 0 && cmp(pta + 5, pta + 6, user_data) > 0)
                        {
                            pts = pta;
                            goto reversed;
                        }

                    default:
                    not_ordered:
                        x      = !v1;
                        tmp    = pta[x];
                        pta[0] = pta[v1];
                        pta[1] = tmp;
                        pta += 2;
                        x      = !v2;
                        tmp    = pta[x];
                        pta[0] = pta[v2];
                        pta[1] = tmp;
                        pta += 2;
                        x      = !v3;
                        tmp    = pta[x];
                        pta[0] = pta[v3];
                        pta[1] = tmp;
                        pta += 2;
                        x      = !v4;
                        tmp    = pta[x];
                        pta[0] = pta[v4];
                        pta[1] = tmp;
                        pta -= 6;

                        quad_swap_merge(pta, swap, cmp, user_data);
                }
                pta += 8;

                continue;

            ordered:

                pta += 8;

                if (count--)
                {
                    if ((v1 = cmp(pta + 0, pta + 1, user_data) > 0) | (v2 = cmp(pta + 2, pta + 3, user_data) > 0) | (v3 = cmp(pta + 4, pta + 5, user_data) > 0) | (v4 = cmp(pta + 6, pta + 7, user_data) > 0))
                    {
                        if (v1 + v2 + v3 + v4 == 4 && cmp(pta + 1, pta + 2, user_data) > 0 && cmp(pta + 3, pta + 4, user_data) > 0 && cmp(pta + 5, pta + 6, user_data) > 0)
                        {
                            pts = pta;
                            goto reversed;
                        }
                        goto not_ordered;
                    }
                    if (cmp(pta + 1, pta + 2, user_data) <= 0 && cmp(pta + 3, pta + 4, user_data) <= 0 && cmp(pta + 5, pta + 6, user_data) <= 0)
                    {
                        goto ordered;
                    }
                    quad_swap_merge(pta, swap, cmp, user_data);
                    pta += 8;
                    continue;
                }
                break;

            reversed:

                pta += 8;

                if (count--)
                {
                    if ((v1 = cmp(pta + 0, pta + 1, user_data) <= 0) | (v2 = cmp(pta + 2, pta + 3, user_data) <= 0) | (v3 = cmp(pta + 4, pta + 5, user_data) <= 0) | (v4 = cmp(pta + 6, pta + 7, user_data) <= 0))
                    {
                        // not reversed
                    }
                    else
                    {
                        if (cmp(pta - 1, pta, user_data) > 0 && cmp(pta + 1, pta + 2, user_data) > 0 && cmp(pta + 3, pta + 4, user_data) > 0 && cmp(pta + 5, pta + 6, user_data) > 0)
                        {
                            goto reversed;
                        }
                    }
                    quad_reversal(pts, pta - 1);

                    if (v1 + v2 + v3 + v4 == 4 && cmp(pta + 1, pta + 2, user_data) <= 0 && cmp(pta + 3, pta + 4, user_data) <= 0 && cmp(pta + 5, pta + 6, user_data) <= 0)
                    {
                        goto ordered;
                    }
                    if (v1 + v2 + v3 + v4 == 0 && cmp(pta + 1, pta + 2, user_data) > 0 && cmp(pta + 3, pta + 4, user_data) > 0 && cmp(pta + 5, pta + 6, user_data) > 0)
                    {
                        pts = pta;
                        goto reversed;
                    }

                    x      = !v1;
                    tmp    = pta[v1];
                    pta[0] = pta[x];
                    pta[1] = tmp;
                    pta += 2;
                    x      = !v2;
                    tmp    = pta[v2];
                    pta[0] = pta[x];
                    pta[1] = tmp;
                    pta += 2;
                    x      = !v3;
                    tmp    = pta[v3];
                    pta[0] = pta[x];
                    pta[1] = tmp;
                    pta += 2;
                    x      = !v4;
                    tmp    = pta[v4];
                    pta[0] = pta[x];
                    pta[1] = tmp;
                    pta -= 6;

                    if (cmp(pta + 1, pta + 2, user_data) > 0 || cmp(pta + 3, pta + 4, user_data) > 0 || cmp(pta + 5, pta + 6, user_data) > 0)
                    {
                        quad_swap_merge(pta, swap, cmp, user_data);
                    }
                    pta += 8;
                    continue;
                }

                switch (nmemb % 8)
                {
                    case 7:
                        if (cmp(pta + 5, pta + 6, user_data) <= 0)
                            break;
                    case 6:
                        if (cmp(pta + 4, pta + 5, user_data) <= 0)
                            break;
                    case 5:
                        if (cmp(pta + 3, pta + 4, user_data) <= 0)
                            break;
                    case 4:
                        if (cmp(pta + 2, pta + 3, user_data) <= 0)
                            break;
                    case 3:
                        if (cmp(pta + 1, pta + 2, user_data) <= 0)
                            break;
                    case 2:
                        if (cmp(pta + 0, pta + 1, user_data) <= 0)
                            break;
                    case 1:
                        if (cmp(pta - 1, pta + 0, user_data) <= 0)
                            break;
                    case 0:
                        quad_reversal(pts, pta + nmemb % 8 - 1);

                        if (pts == array)
                        {
                            return 1;
                        }
                        goto reverse_end;
                }
                quad_reversal(pts, pta - 1);
                break;
            }
            tail_swap(pta, swap, nmemb % 8, cmp, user_data);

        reverse_end:

            pta = array;

            for (count = nmemb / 32; count--; pta += 32)
            {
                if (cmp(pta + 7, pta + 8, user_data) <= 0 && cmp(pta + 15, pta + 16, user_data) <= 0 && cmp(pta + 23, pta + 24, user_data) <= 0)
                {
                    continue;
                }
                parity_merge(swap, pta, 8, 8, cmp, user_data);
                parity_merge(swap + 16, pta + 16, 8, 8, cmp, user_data);
                parity_merge(pta, swap, 16, 16, cmp, user_data);
            }

            if (nmemb % 32 > 8)
            {
                tail_merge(pta, swap, 32, nmemb % 32, 8, cmp, user_data);
            }
            return 0;
        }

        // quad merge support routines

        template <typename VAR>
        void cross_merge(VAR *dest, VAR *from, u32 left, u32 right, sort_cmp_fn cmp, void *user_data)
        {
            VAR *ptl, *tpl, *ptr, *tpr, *ptd, *tpd;
            u32  loop;
            u32  x, y;

            ptl = from;
            ptr = from + left;
            tpl = ptr - 1;
            tpr = tpl + right;

            if (left + 1 >= right && right + 1 >= left && left >= 32)
            {
                if (cmp(ptl + 15, ptr, user_data) > 0 && cmp(ptl, ptr + 15, user_data) <= 0 && cmp(tpl, tpr - 15, user_data) > 0 && cmp(tpl - 15, tpr, user_data) <= 0)
                {
                    parity_merge(dest, from, left, right, cmp, user_data);
                    return;
                }
            }
            ptd = dest;
            tpd = dest + left + right - 1;

            while (tpl - ptl > 8 && tpr - ptr > 8)
            {
            ptl8_ptr:
                if (cmp(ptl + 7, ptr, user_data) <= 0)
                {
                    memory_copy(ptd, ptl, 8 * sizeof(VAR));
                    ptd += 8;
                    ptl += 8;

                    if (tpl - ptl > 8)
                    {
                        goto ptl8_ptr;
                    }
                    break;
                }

            ptl_ptr8:
                if (cmp(ptl, ptr + 7, user_data) > 0)
                {
                    memory_copy(ptd, ptr, 8 * sizeof(VAR));
                    ptd += 8;
                    ptr += 8;

                    if (tpr - ptr > 8)
                    {
                        goto ptl_ptr8;
                    }
                    break;
                }

            tpl_tpr8:
                if (cmp(tpl, tpr - 7, user_data) <= 0)
                {
                    tpd -= 7;
                    tpr -= 7;
                    memory_copy(tpd--, tpr--, 8 * sizeof(VAR));

                    if (tpr - ptr > 8)
                    {
                        goto tpl_tpr8;
                    }
                    break;
                }

            tpl8_tpr:
                if (cmp(tpl - 7, tpr, user_data) > 0)
                {
                    tpd -= 7;
                    tpl -= 7;
                    memory_copy(tpd--, tpl--, 8 * sizeof(VAR));

                    if (tpl - ptl > 8)
                    {
                        goto tpl8_tpr;
                    }
                    break;
                }
                {
                    loop = 8;
                    do
                    {
                        head_branchless_merge(ptd, x, ptl, ptr, cmp, user_data);
                        tail_branchless_merge(tpd, y, tpl, tpr, cmp, user_data);
                    } while (--loop);
                }
            }

            if (cmp(tpl, tpr, user_data) <= 0)
            {
                while (ptl <= tpl)
                {
                    *ptd++ = cmp(ptl, ptr, user_data) <= 0 ? *ptl++ : *ptr++;
                }
                while (ptr <= tpr)
                {
                    *ptd++ = *ptr++;
                }
            }
            else
            {
                while (ptr <= tpr)
                {
                    *ptd++ = cmp(ptl, ptr, user_data) <= 0 ? *ptl++ : *ptr++;
                }
                while (ptl <= tpl)
                {
                    *ptd++ = *ptl++;
                }
            }
        }

        // main memory: [A][B][C][D]
        // swap memory: [A  B]       step 1
        // swap memory: [A  B][C  D] step 2
        // main memory: [A  B  C  D] step 3

        template <typename VAR>
        void quad_merge_block(VAR *array, VAR *swap, u32 block, sort_cmp_fn cmp, void *user_data)
        {
            VAR *pt1, *pt2, *pt3;
            u32  block_x_2 = block * 2;

            pt1 = array + block;
            pt2 = pt1 + block;
            pt3 = pt2 + block;

            switch ((cmp(pt1 - 1, pt1, user_data) <= 0) | (cmp(pt3 - 1, pt3, user_data) <= 0) * 2)
            {
                case 0:
                    cross_merge(swap, array, block, block, cmp, user_data);
                    cross_merge(swap + block_x_2, pt2, block, block, cmp, user_data);
                    break;
                case 1:
                    memory_copy(swap, array, block_x_2 * sizeof(VAR));
                    cross_merge(swap + block_x_2, pt2, block, block, cmp, user_data);
                    break;
                case 2:
                    cross_merge(swap, array, block, block, cmp, user_data);
                    memory_copy(swap + block_x_2, pt2, block_x_2 * sizeof(VAR));
                    break;
                case 3:
                    if (cmp(pt2 - 1, pt2, user_data) <= 0)
                        return;
                    memory_copy(swap, array, block_x_2 * 2 * sizeof(VAR));
            }
            cross_merge(array, swap, block_x_2, block_x_2, cmp, user_data);
        }

        template <typename VAR>
        u32 quad_merge(VAR *array, VAR *swap, u32 swap_size, u32 nmemb, u32 block, sort_cmp_fn cmp, void *user_data)
        {
            VAR *pta, *pte;

            pte = array + nmemb;

            block *= 4;

            while (block <= nmemb && block <= swap_size)
            {
                pta = array;

                do
                {
                    quad_merge_block(pta, swap, block / 4, cmp, user_data);

                    pta += block;
                } while (pta + block <= pte);

                tail_merge(pta, swap, swap_size, (u32)(pte - pta), block / 4, cmp, user_data);

                block *= 4;
            }

            tail_merge(array, swap, swap_size, nmemb, block / 4, cmp, user_data);

            return block / 2;
        }

        template <typename VAR>
        void partial_forward_merge(VAR *array, VAR *swap, u32 swap_size, u32 nmemb, u32 block, sort_cmp_fn cmp, void *user_data)
        {
            VAR *ptl, *ptr, *tpl, *tpr;
            u32  x, y;

            if (nmemb == block)
            {
                return;
            }

            ptr = array + block;
            tpr = array + nmemb - 1;

            if (cmp(ptr - 1, ptr, user_data) <= 0)
            {
                return;
            }

            memory_copy(swap, array, block * sizeof(VAR));

            ptl = swap;
            tpl = swap + block - 1;

            while (ptl < tpl - 1 && ptr < tpr - 1)
            {
                if (cmp(ptl, ptr + 1, user_data) > 0)
                {
                    *array++ = *ptr++;
                    *array++ = *ptr++;
                }
                else if (cmp(ptl + 1, ptr, user_data) <= 0)
                {
                    *array++ = *ptl++;
                    *array++ = *ptl++;
                }
                else
                {
                    x        = cmp(ptl, ptr, user_data) <= 0;
                    y        = !x;
                    array[x] = *ptr;
                    ptr += 1;
                    array[y] = *ptl;
                    ptl += 1;
                    array += 2;
                    head_branchless_merge(array, x, ptl, ptr, cmp, user_data);
                }
            }

            while (ptl <= tpl && ptr <= tpr)
            {
                *array++ = cmp(ptl, ptr, user_data) <= 0 ? *ptl++ : *ptr++;
            }

            while (ptl <= tpl)
            {
                *array++ = *ptl++;
            }
        }

        template <typename VAR>
        void partial_backward_merge(VAR *array, VAR *swap, u32 swap_size, u32 nmemb, u32 block, sort_cmp_fn cmp, void *user_data)
        {
            VAR *tpl, *tpa, *tpr;  // tail pointer left, array, right
            u32  right, loop, x, y;

            if (nmemb == block)
            {
                return;
            }

            tpl = array + block - 1;
            tpa = array + nmemb - 1;

            if (cmp(tpl, tpl + 1, user_data) <= 0)
            {
                return;
            }

            right = nmemb - block;

            if (nmemb <= swap_size && right >= 64)
            {
                cross_merge(swap, array, block, right, cmp, user_data);

                memory_copy(array, swap, nmemb * sizeof(VAR));

                return;
            }

            memory_copy(swap, array + block, right * sizeof(VAR));

            tpr = swap + right - 1;

            while (tpl > array + 16 && tpr > swap + 16)
            {
            tpl_tpr16:
                if (cmp(tpl, tpr - 15, user_data) <= 0)
                {
                    loop = 16;
                    do
                        *tpa-- = *tpr--;
                    while (--loop);

                    if (tpr > swap + 16)
                    {
                        goto tpl_tpr16;
                    }
                    break;
                }

            tpl16_tpr:
                if (cmp(tpl - 15, tpr, user_data) > 0)
                {
                    loop = 16;
                    do
                        *tpa-- = *tpl--;
                    while (--loop);

                    if (tpl > array + 16)
                    {
                        goto tpl16_tpr;
                    }
                    break;
                }

                loop = 8;
                do
                {
                    if (cmp(tpl, tpr - 1, user_data) <= 0)
                    {
                        *tpa-- = *tpr--;
                        *tpa-- = *tpr--;
                    }
                    else if (cmp(tpl - 1, tpr, user_data) > 0)
                    {
                        *tpa-- = *tpl--;
                        *tpa-- = *tpl--;
                    }
                    else
                    {
                        x = cmp(tpl, tpr, user_data) <= 0;
                        y = !x;
                        tpa--;
                        tpa[x] = *tpr;
                        tpr -= 1;
                        tpa[y] = *tpl;
                        tpl -= 1;
                        tpa--;
                        tail_branchless_merge(tpa, y, tpl, tpr, cmp, user_data);
                    }
                } while (--loop);
            }

            while (tpr > swap + 1 && tpl > array + 1)
            {
                if (cmp(tpl, tpr - 1, user_data) <= 0)
                {
                    *tpa-- = *tpr--;
                    *tpa-- = *tpr--;
                }
                else if (cmp(tpl - 1, tpr, user_data) > 0)
                {
                    *tpa-- = *tpl--;
                    *tpa-- = *tpl--;
                }
                else
                {
                    x = cmp(tpl, tpr, user_data) <= 0;
                    y = !x;
                    tpa--;
                    tpa[x] = *tpr;
                    tpr -= 1;
                    tpa[y] = *tpl;
                    tpl -= 1;
                    tpa--;
                    tail_branchless_merge(tpa, y, tpl, tpr, cmp, user_data);
                }
            }

            while (tpr >= swap && tpl >= array)
            {
                *tpa-- = cmp(tpl, tpr, user_data) > 0 ? *tpl-- : *tpr--;
            }

            while (tpr >= swap)
            {
                *tpa-- = *tpr--;
            }
        }

        template <typename VAR>
        void tail_merge(VAR *array, VAR *swap, u32 swap_size, u32 nmemb, u32 block, sort_cmp_fn cmp, void *user_data)
        {
            VAR *pta, *pte;

            pte = array + nmemb;

            while (block < nmemb && block <= swap_size)
            {
                for (pta = array; pta + block < pte; pta += block * 2)
                {
                    if (pta + block * 2 < pte)
                    {
                        partial_backward_merge(pta, swap, swap_size, block * 2, block, cmp, user_data);

                        continue;
                    }
                    partial_backward_merge(pta, swap, swap_size, (u32)(pte - pta), block, cmp, user_data);

                    break;
                }
                block *= 2;
            }
        }

        // the next four functions provide in-place rotate merge support

        template <typename VAR>
        void trinity_rotation(VAR *array, VAR *swap, u32 swap_size, u32 nmemb, u32 left)
        {
            VAR temp;
            u32 bridge, right = nmemb - left;

            if (swap_size > 65536)
            {
                swap_size = 65536;
            }

            if (left < right)
            {
                if (left <= swap_size)
                {
                    memory_copy(swap, array, left * sizeof(VAR));
                    memory_move(array, array + left, right * sizeof(VAR));
                    memory_copy(array + right, swap, left * sizeof(VAR));
                }
                else
                {
                    VAR *pta, *ptb, *ptc, *ptd;

                    pta = array;
                    ptb = pta + left;

                    bridge = right - left;

                    if (bridge <= swap_size && bridge > 3)
                    {
                        ptc = pta + right;
                        ptd = ptc + left;

                        memory_copy(swap, ptb, bridge * sizeof(VAR));

                        while (left--)
                        {
                            *--ptc = *--ptd;
                            *ptd   = *--ptb;
                        }
                        memory_copy(pta, swap, bridge * sizeof(VAR));
                    }
                    else
                    {
                        ptc = ptb;
                        ptd = ptc + right;

                        bridge = left / 2;

                        while (bridge--)
                        {
                            temp   = *--ptb;
                            *ptb   = *pta;
                            *pta++ = *ptc;
                            *ptc++ = *--ptd;
                            *ptd   = temp;
                        }

                        bridge = (u32)(ptd - ptc) / 2;

                        while (bridge--)
                        {
                            temp   = *ptc;
                            *ptc++ = *--ptd;
                            *ptd   = *pta;
                            *pta++ = temp;
                        }

                        bridge = (u32)(ptd - pta) / 2;

                        while (bridge--)
                        {
                            temp   = *pta;
                            *pta++ = *--ptd;
                            *ptd   = temp;
                        }
                    }
                }
            }
            else if (right < left)
            {
                if (right <= swap_size)
                {
                    memory_copy(swap, array + left, right * sizeof(VAR));
                    memory_move(array + right, array, left * sizeof(VAR));
                    memory_copy(array, swap, right * sizeof(VAR));
                }
                else
                {
                    VAR *pta, *ptb, *ptc, *ptd;

                    pta = array;
                    ptb = pta + left;

                    bridge = left - right;

                    if (bridge <= swap_size && bridge > 3)
                    {
                        ptc = pta + right;
                        ptd = ptc + left;

                        memory_copy(swap, ptc, bridge * sizeof(VAR));

                        while (right--)
                        {
                            *ptc++ = *pta;
                            *pta++ = *ptb++;
                        }
                        memory_copy(ptd - bridge, swap, bridge * sizeof(VAR));
                    }
                    else
                    {
                        ptc = ptb;
                        ptd = ptc + right;

                        bridge = right / 2;

                        while (bridge--)
                        {
                            temp   = *--ptb;
                            *ptb   = *pta;
                            *pta++ = *ptc;
                            *ptc++ = *--ptd;
                            *ptd   = temp;
                        }

                        bridge = (u32)(ptb - pta) / 2;

                        while (bridge--)
                        {
                            temp   = *--ptb;
                            *ptb   = *pta;
                            *pta++ = *--ptd;
                            *ptd   = temp;
                        }

                        bridge = (u32)(ptd - pta) / 2;

                        while (bridge--)
                        {
                            temp   = *pta;
                            *pta++ = *--ptd;
                            *ptd   = temp;
                        }
                    }
                }
            }
            else
            {
                VAR *pta, *ptb;

                pta = array;
                ptb = pta + left;

                while (left--)
                {
                    temp   = *pta;
                    *pta++ = *ptb;
                    *ptb++ = temp;
                }
            }
        }

        template <typename VAR>
        u32 monobound_binary_first(VAR *array, VAR *value, u32 top, sort_cmp_fn cmp, void *user_data)
        {
            VAR *end;
            u32  mid;

            end = array + top;

            while (top > 1)
            {
                mid = top / 2;

                if (cmp(value, end - mid, user_data) <= 0)
                {
                    end -= mid;
                }
                top -= mid;
            }

            if (cmp(value, end - 1, user_data) <= 0)
            {
                end--;
            }
            return (u32)(end - array);
        }

        template <typename VAR>
        void rotate_merge_block(VAR *array, VAR *swap, u32 swap_size, u32 lblock, u32 right, sort_cmp_fn cmp, void *user_data)
        {
            u32 left, rblock, unbalanced;

            if (cmp(array + lblock - 1, array + lblock, user_data) <= 0)
            {
                return;
            }

            rblock = lblock / 2;
            lblock -= rblock;

            left = monobound_binary_first(array + lblock + rblock, array + lblock, right, cmp, user_data);

            right -= left;

            // [ lblock ] [ rblock ] [ left ] [ right ]

            if (left)
            {
                if (lblock + left <= swap_size)
                {
                    memory_copy(swap, array, lblock * sizeof(VAR));
                    memory_copy(swap + lblock, array + lblock + rblock, left * sizeof(VAR));
                    memory_move(array + lblock + left, array + lblock, rblock * sizeof(VAR));

                    cross_merge(array, swap, lblock, left, cmp, user_data);
                }
                else
                {
                    trinity_rotation(array + lblock, swap, swap_size, rblock + left, rblock);

                    unbalanced = (left * 2 < lblock) | (lblock * 2 < left);

                    if (unbalanced && left <= swap_size)
                    {
                        partial_backward_merge(array, swap, swap_size, lblock + left, lblock, cmp, user_data);
                    }
                    else if (unbalanced && lblock <= swap_size)
                    {
                        partial_forward_merge(array, swap, swap_size, lblock + left, lblock, cmp, user_data);
                    }
                    else
                    {
                        rotate_merge_block(array, swap, swap_size, lblock, left, cmp, user_data);
                    }
                }
            }

            if (right)
            {
                unbalanced = (right * 2 < rblock) | (rblock * 2 < right);

                if ((unbalanced && right <= swap_size) || right + rblock <= swap_size)
                {
                    partial_backward_merge(array + lblock + left, swap, swap_size, rblock + right, rblock, cmp, user_data);
                }
                else if (unbalanced && rblock <= swap_size)
                {
                    partial_forward_merge(array + lblock + left, swap, swap_size, rblock + right, rblock, cmp, user_data);
                }
                else
                {
                    rotate_merge_block(array + lblock + left, swap, swap_size, rblock, right, cmp, user_data);
                }
            }
        }

        template <typename VAR>
        void rotate_merge(VAR *array, VAR *swap, u32 swap_size, u32 nmemb, u32 block, sort_cmp_fn cmp, void *user_data)
        {
            VAR *pta, *pte;

            pte = array + nmemb;

            if (nmemb <= block * 2 && nmemb - block <= swap_size)
            {
                partial_backward_merge(array, swap, swap_size, nmemb, block, cmp, user_data);

                return;
            }

            while (block < nmemb)
            {
                for (pta = array; pta + block < pte; pta += block * 2)
                {
                    if (pta + block * 2 < pte)
                    {
                        rotate_merge_block(pta, swap, swap_size, block, block, cmp, user_data);

                        continue;
                    }
                    rotate_merge_block(pta, swap, swap_size, block, (u32)(pte - pta - block), cmp, user_data);

                    break;
                }
                block *= 2;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // ┌─────────────────────────────────────────────────────────────────────────┐//
        // │    ██████┐ ██┐   ██┐ █████┐ ██████┐ ███████┐ ██████┐ ██████┐ ████████┐  │//
        // │   ██┌───██┐██│   ██│██┌──██┐██┌──██┐██┌────┘██┌───██┐██┌──██┐└──██┌──┘  │//
        // │   ██│   ██│██│   ██│███████│██│  ██│███████┐██│   ██│██████┌┘   ██│     │//
        // │   ██│▄▄ ██│██│   ██│██┌──██│██│  ██│└────██│██│   ██│██┌──██┐   ██│     │//
        // │   └██████┌┘└██████┌┘██│  ██│██████┌┘███████│└██████┌┘██│  ██│   ██│     │//
        // │    └──▀▀─┘  └─────┘ └─┘  └─┘└─────┘ └──────┘ └─────┘ └─┘  └─┘   └─┘     │//
        // └─────────────────────────────────────────────────────────────────────────┘//
        ///////////////////////////////////////////////////////////////////////////////

        template <typename VAR>
        void quadsort(void *array, u32 nmemb, sort_cmp_fn cmp, void *user_data)
        {
            VAR *pta = (VAR *)array;
            if (nmemb < 32)
            {
                VAR swap[32];
                tail_swap(pta, swap, nmemb, cmp, user_data);
            }
            else if (quad_swap(pta, nmemb, cmp, user_data) == 0)
            {
                VAR *swap = nullptr;
                u32  block, swap_size = nmemb;

                //		for (swap_size = 32 ; swap_size * 4 <= nmemb ; swap_size *= 4) {}

                //swap = (VAR *)malloc(swap_ size * sizeof(VAR));

                if (swap == nullptr)
                {
                    VAR stack[512];

                    tail_merge(pta, stack, 32, nmemb, 32, cmp, user_data);
                    rotate_merge(pta, stack, 32, nmemb, 64, cmp, user_data);

                    return;
                }

                block = quad_merge(pta, swap, swap_size, nmemb, 32, cmp, user_data);
                rotate_merge(pta, swap, swap_size, nmemb, block, cmp, user_data);

                //free(swap);
            }
        }

        template <typename VAR>
        void quadsort_swap(VAR *array, VAR *swap, u32 swap_size, u32 nmemb, sort_cmp_fn cmp, void *user_data)
        {
            VAR *pta = (VAR *)array;
            VAR *pts = (VAR *)swap;

            if (nmemb <= 96)
            {
                tail_swap(pta, pts, nmemb, cmp, user_data);
            }
            else if (quad_swap(pta, nmemb, cmp, user_data) == 0)
            {
                u32 block = quad_merge(pta, pts, swap_size, nmemb, 32, cmp, user_data);

                rotate_merge(pta, pts, swap_size, nmemb, block, cmp, user_data);
            }
        }
    }  // namespace quad_sort
}  // namespace ncore

#endif
