/*
 * http://www.musl-libc.org/
 *
 * Copyright © 2005-2014 Rich Felker, et al.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LIBC_H
#define LIBC_H

/* FIXME: #include <stdlib.h> */
/* FIXME: #include <stdio.h> */
#include <limits.h>

/************************************TODO***************************************
struct __locale_map;

struct __locale_struct {
	volatile int ctype_utf8;
	char *messages_name;
	struct __locale_map *volatile cat[4];
};

struct __libc {
	int has_thread_pointer;
	int can_do_threads;
	int threaded;
	int secure;
	size_t *auxv;
	volatile int threads_minus_1;
	FILE *ofl_head;
	volatile int ofl_lock[2];
	size_t tls_size;
	size_t page_size;
	volatile int uselocale_cnt;
	volatile int bytelocale_cnt_minus_1;
	struct __locale_struct global_locale;
};

extern size_t __hwcap;

#ifndef PAGE_SIZE
#define PAGE_SIZE libc.page_size
#endif

#if !defined(__PIC__) || (100*__GNUC__+__GNUC_MINOR__ >= 303 && !defined(__PCC__))

#ifdef __PIC__
#if __GNUC__ < 4
#define BROKEN_VISIBILITY 1
#endif
#define ATTR_LIBC_VISIBILITY __attribute__((visibility("hidden")))
#else
#define ATTR_LIBC_VISIBILITY
#endif

extern struct __libc __libc ATTR_LIBC_VISIBILITY;
#define libc __libc

#else

#define USE_LIBC_ACCESSOR
#define ATTR_LIBC_VISIBILITY
extern struct __libc *__libc_loc(void) __attribute__((const));
#define libc (*__libc_loc())

#endif


/\* Designed to avoid any overhead in non-threaded processes *\/
void __lock(volatile int *) ATTR_LIBC_VISIBILITY;
void __unlock(volatile int *) ATTR_LIBC_VISIBILITY;
int __lockfile(FILE *) ATTR_LIBC_VISIBILITY;
void __unlockfile(FILE *) ATTR_LIBC_VISIBILITY;
#define LOCK(x) __lock(x)
#define UNLOCK(x) __unlock(x)

void __synccall(void (*)(void *), void *);
int __setxid(int, int, int, int);

extern char **__environ;
*******************************************************************************/

#undef weak_alias
#define weak_alias(old, new) \
	extern __typeof(old) new __attribute__((weak, alias(#old)))

#undef LFS64_2
#define LFS64_2(x, y) weak_alias(x, y)

#undef LFS64
#define LFS64(x) LFS64_2(x, x##64)

#endif
