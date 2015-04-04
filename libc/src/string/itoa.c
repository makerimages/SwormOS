/*
 * Copyright (c) 2015 Yuri Tretyakov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/* itoa() implementation. */

#ifdef __is_kernel

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

char *itoa (unsigned int num, char *str, int radix)
{
    /* Check if `radix' is supported. */
    if (radix != 2 && radix != 8 && radix != 10 && radix != 16)
        return NULL;

    /* Save the string pointer. */
    char *str_saved = str;

    /* Is the number zero? */
    if (!num)
    {
        strcpy (str, "0");
        return str;
    }

    /* Convert the number. */
    while (num > 0)
    {
        int digit = num % radix;        /* current digit */
        num /= radix;
        char ch;                        /* converted digit */

        if (digit >= 10)
            ch = digit + 87;
        else
            ch = digit + 48;

        *str = ch;
        ++str;
    }
    *str = '\0';

    /* Reverse the string. */
    char *left = str_saved;
    char *right = str_saved + strlen (str_saved) - 1;
    while (left < right)
    {
        char tmp;
        tmp = *left;                    /* save the first character */
        *left = *right;                 /* reverse the left character */
        *right = tmp;                   /* reverse the right character */
        ++left; --right;
    }

    return str_saved;
}

char *itoa_s (signed int num, char *str, int radix)
{
    /* Check if `radix' is supported. */
    if (radix != 2 && radix != 8 && radix != 10 && radix != 16)
        return NULL;

    /* Save the string pointer. */
    char *str_saved = str;

    /* Is the number zero? */
    if (!num)
    {
        strcpy (str, "0");
        return str;
    }

    /* Is the numer negative? */
    bool negative = false;
    if (num < 0)
    {
        negative = true;                /* used in the end */
        *str = '-';                     /* add the sign, */
        ++str;                          /* and skip it */
        ++str_saved;
        num *= -1;                      /* make the number positive */
    }

    return itoa (num, str, radix) - negative;
}

#endif
