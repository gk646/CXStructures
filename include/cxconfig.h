// Copyright (c) 2023 gk646
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#define CX_FINISHED
#ifndef CXSTRUCTS_SRC_CONFIG_H_
#define CXSTRUCTS_SRC_CONFIG_H_

/* |-----------------------------------------------------|
 * |                     MACROS                          |
 * |-----------------------------------------------------|
 */

//#define CX_USE_INT            : uses type int for all custom types
//
//
//

/**
 * namespace for all datastructure related headers
 */
namespace cxstructs {}
/**
 * internal namespace for helper methods or structs
 */
namespace cxhelper {}
/**
 * internal namespace for test methods
 */
namespace cxtests {}

#include <cstdint>
#include "cxutil/cxassert.h"

#ifndef CX_USE_INT
typedef uint_fast32_t uint_32_cx;
typedef uint_fast16_t uint_16_cx;
typedef int_fast32_t int_32_cx;
#else
typedef int uint_32_cx;
typedef int uint_16_cx;
typedef int int_32_cx;
#endif

#include "CXAllocator.h"



#endif  //CXSTRUCTS_SRC_CONFIG_H_
