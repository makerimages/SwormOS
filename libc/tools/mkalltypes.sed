# http://www.musl-libc.org/
#
# Copyright © 2005-2014 Rich Felker, et al.
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

/^TYPEDEF/s/TYPEDEF \(.*\) \([^ ]*\);$/#if defined(__NEED_\2) \&\& !defined(__DEFINED_\2)\
typedef \1 \2;\
#define __DEFINED_\2\
#endif\
/
/^STRUCT/s/STRUCT * \([^ ]*\) \(.*\);$/#if defined(__NEED_struct_\1) \&\& !defined(__DEFINED_struct_\1)\
struct \1 \2;\
#define __DEFINED_struct_\1\
#endif\
/
/^UNION/s/UNION * \([^ ]*\) \(.*\);$/#if defined(__NEED_union_\1) \&\& !defined(__DEFINED_union_\1)\
union \1 \2;\
#define __DEFINED_union_\1\
#endif\
/
