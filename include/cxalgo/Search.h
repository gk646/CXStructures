// Copyright (c) 2023 gk646
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#define FINISHED
#ifndef CXSTRUCTS_BINARYSEARCH_H
#define CXSTRUCTS_BINARYSEARCH_H

#include <cassert>
#include <cstdint>
#include <iostream>

namespace cxhelper { // helper methods to provide clean calling interface
template <typename T>
bool binarySearch_recursive_internal(T *arr, T target, int_fast32_t low,
                                     int_fast32_t high) {
  if (low > high) {
    return false;
  }
  int_fast32_t mid = low + (high - low) / 2;

  if (arr[mid] == target) {
    return true;
  } else if (arr[mid] < target) {
    return binarySearch_recursive_internal(arr, target, mid + 1, high);
  } else {
    return binarySearch_recursive_internal(arr, target, low, mid - 1);
  }
}
} // namespace cxhelper

namespace cxalgo {
/**
 * Binary search on the specified ASCENDED SORTED array without recursion <p>
 * runtime: O(log(n))
 * @tparam T the used datatype
 * @param arr search array
 * @param target target value to search for
 * @param len the length of the given array
 * @return true if the target was found inside arr_
 */
template <typename T> bool binarySearch(T *arr, T target, int_fast32_t len) {
  int_fast32_t low = 0;
  int_fast32_t high = len - 1;
  int_fast32_t mid;
  while (low <= high) {
    mid = low + (high - low) / 2;
    if (arr[mid] == target) {
      return true;
    } else if (arr[mid] < target) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }
  return false;
}

/**
 * Binary search on the specified ASCENDED SORTED array with recursion <p>
* runtime: O(log(n))
 * @tparam T the used datatype
 * @param arr search array
 * @param target target value to search for
 * @param len the length of the given array
 * @return true if the target was found inside arr_
 */
template <typename T>
bool binarySearch_recursive(T *arr, T target, int_fast32_t len) {
  if (len == 0) {
    return false;
  }
  return cxhelper::binarySearch_recursive_internal(arr, target, 0, len - 1);
}

} // namespace cxalgo

namespace cxtests {

using namespace cxalgo;

static void TEST_SEARCH() {
  std::cout << "TESTING BINARY SEARCH" << std::endl;
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  assert(binarySearch(arr, 5, 9) == true);
  assert(binarySearch(arr, -1, 9) == false);

  std::cout << "TESTING BINARY SEARCH RECURSIVE" << std::endl;
  assert(binarySearch_recursive(arr, 5, 9) == true);
  assert(binarySearch_recursive(arr, -1, 9) == false);
}
} // namespace cxtests
#endif // CXSTRUCTS_BINARYSEARCH_H
