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
#define FINISHED
#ifndef CXSTRUCTS_ARRAYLIST_H
#define CXSTRUCTS_ARRAYLIST_H

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>
#include "../cxconfig.h"

//This implementation is well optimized (from what I can tell) and should be equal or slightly slower than the std::vector in a lot of use cases
namespace cxstructs {
/**
 * <h2>vec</h2>
 * This is an implementation of a dynamic array data structure, similar to the <code>ArrayList</code> in Java or <code>std::vector</code> in C++.
 * <br><br>
 * <p>A dynamic array is a random access, variable-n_elem list data structure that allows elements to be added or removed.
 * It provides the capability to index into the list, add elements to the end, and remove elements from the end in a time-efficient manner.</p>
 */
template <typename T>
class vec {
  T* arr_;
  uint_32_cx size_;
  uint_32_cx len_;
  uint_32_cx minlen_;

  inline void grow() {
    len_ *= 1.5;
    auto n_arr = new T[len_];
    std::move(arr_, arr_ + size_, n_arr);
    delete[] arr_;
    arr_ = n_arr;

    minlen_ = size_ / 6 < 64 ? 0 : size_ / 6;
  }
  inline void shrink() {
    len_ /= 2;
    auto n_arr = new T[len_];
    std::move(arr_, arr_ + size_, n_arr);
    delete[] arr_;
    arr_ = n_arr;
    // lower bound is very lenient to avoid premature downsizing
    minlen_ = size_ / 6 < 64 ? 0 : size_ / 6;
  }

 public:
  explicit vec(uint_32_cx n_elem = 64)
      : len_(n_elem),
        size_(0),
        arr_(new T[n_elem]),
        minlen_(n_elem / 6 < 64 ? 0 : n_elem / 6) {}
  vec(uint_32_cx n_elem, const T val)
      : len_(n_elem),
        size_(n_elem),
        arr_(new T[n_elem]),
        minlen_(n_elem / 6 < 64 ? 0 : n_elem / 6) {
    std::fill(arr_, arr_ + n_elem, val);
  }
  /**
* @brief Constructs a vec with the specified number of elements, and initializes them using a provided function.
*
* @tparam fill_form A callable object or function that takes a single int as an argument and returns a float
* @param n_elem The number of elements in the vec.
* @param form The callable object or function used to initialize the elements of the vec. It is invoked for each element with the element's index as an argument.
**/
  template <typename fill_form,
            typename = std::enable_if_t<
                std::is_invocable_r_v<double, fill_form, double>>>
  vec(uint_32_cx n_elem, fill_form form)
      : len_(n_elem),
        size_(n_elem),
        arr_(new T[n_elem]),
        minlen_(n_elem / 6 < 64 ? 0 : n_elem / 6) {
    for (uint_32_cx i = 0; i < n_elem; i++) {
      arr_[i] = form(i);
    }
  }
  explicit vec(const std::vector<T>& vector)
      : len_(vector.size() * 1.5),
        size_(vector.size()),
        arr_(new T[vector.size() * 1.5]),
        minlen_(vector.size() / 6 < 64 ? 0 : vector.size() / 6) {
    std::copy(vector.begin(), vector.end(), arr_);
  }
  explicit vec(const std::vector<T>&& vector)
      : len_(vector.size()),
        size_(vector.size()),
        arr_(vector.data()),
        minlen_(vector.size() / 6 < 64 ? 0 : vector.size() / 6) {}
  vec(std::initializer_list<T> init_list)
      : size_(init_list.size()),
        len_(init_list.size() * 2),
        arr_(new T[init_list.size()]),
        minlen_(0) {
    std::copy(init_list.begin(), init_list.end(), arr_);
  }
  vec(const vec<T>& o) : size_(o.size_), len_(o.len_), minlen_(o.minlen_) {
    arr_ = new T[len_];
    std::copy(o.arr_, o.arr_ + size_, arr_);
  }
  vec& operator=(const vec<T>& o) {
    if (this != &o) {
      delete[] arr_;

      size_ = o.size_;
      len_ = o.len_;
      minlen_ = o.minlen_;
      arr_ = new T[size_];
      std::copy(o.arr_, o.arr_ + size_, arr_);
    }
    return *this;
  }
  //move constructor
  vec(vec&& o) noexcept
      : arr_(o.arr_), size_(o.size_), len_(o.len_), minlen_(o.minlen_) {
    o.arr_ = nullptr;
    o.size_ = 0;
    o.len_ = 0;
    o.minlen_ = 0;
  }
  //move assignment
  vec& operator=(vec&& o) noexcept {
    if (this != &o) {
      delete[] arr_;

      arr_ = o.arr_;
      size_ = o.size_;
      len_ = o.len_;
      minlen_ = o.minlen_;

      o.arr_ = nullptr;
      o.size_ = 0;
      o.len_ = 0;
      o.minlen_ = 0;
    }
    return *this;
  }
  ~vec() { delete[] arr_; }
  /**
   * Direct access to the underlying array
   * @param index the accessed index
   * @return a reference to the value
   */
  [[nodiscard]] inline T& operator[](const uint_32_cx& index) const {
    return arr_[index];
  }
  /**
   * Allows direct access at the specified index starting with index 0 <p>
   * Negative indices can be used to access the list from the last element onwards starting with -1
   * Throws std::out_of_range on invalid index
   * @param T a reference to the value at the given index
   */
  inline T& at(const int_32_cx& index) {
    if (index < 0) {
      int_32_cx access = size_ + index;
      if (access >= 0) {
        return arr_[access];
      }
      throw std::out_of_range("index out of bounds");
    } else if (index < size_) {
      return arr_[index];
    }
    throw std::out_of_range("index out of bounds");
  }
  /**
   * Adds a element to the list
   * @param e the element to be added
   */
  inline void add(const T& e) {
    if (size_ == len_) {
      grow();
    }
    arr_[size_++] = e;
  }
  /**
   * Construct a new T element at the end of the list
   * Will produce a leaking vector if the T constructor throws an error
   * @param args T constructor arguments
   */
  template <typename... Args>
  inline void emplace_back(Args&&... args) {
    if (size_ == len_) {
      grow();
    }
    arr_[size_++] = T(std::forward<Args>(args)...);
  }
  /**
   * Removes the first occurrence of the given element from the list
   * @param e element to be removed
   */
  inline void remove(const T& e) {
    if (size_ < minlen_) {
      shrink();
    }
    for (uint_32_cx i = 0; i < len_; i++) {
      if (arr_[i] == e) {
        std::move(arr_ + i + 1, arr_ + size_, arr_ + i);
        size_--;
        return;
      }
    }
  }
  /**
   * Removes the element at the given index
   * @param index index of removal
   */
  inline void removeAt(const uint_32_cx& index) {
    if (size_ < minlen_) {
      shrink();
    }
    std::move(arr_ + index + 1, arr_ + size_--, arr_ + index);
  }
  /**
 *
 * @return the current n_elem of the list
 */
  [[nodiscard]] inline uint_32_cx size() const { return size_; }
  /**
   * Clears the list of all its elements <br>
   * Resets the length back to its starting value
   */
  void clear() {
    minlen_ = 0;
    size_ = 0;
    len_ = 32;
    delete[] arr_;
    arr_ = new T[len_];
  }
  /**
   * Provides access to the underlying array which can be used for sorting
   * Use with caution! Valid data is only guaranteed from 0 up to n_elem
   * @return a pointer to the data array
   */
  T* get_raw() { return arr_; }
  /**
   * Searches the whole vector starting from the beginning (default)
   * @param val value to search for
   * @param startFront whether to start from the front
   * @return true if the value was found
   */
  bool contains(const T& val, bool startFront = true) {
    if (startFront) {
      for (int i = 0; i < size_; i++) {
        if (arr_[i] == val) {
          return true;
        }
      }
      return false;
    } else {
      for (int i = size_ - 1; i > -1; i++) {
        if (arr_[i] == val) {
          return true;
        }
      }
      return false;
    }
  }
  /**
 * Appends the contents of another vec at the end of this vec <p>
 * If necessary, the capacity of this vec is expanded
 * @param vec  the vec to append
 */
  void append(vec<T>& vec) {
    while (len_ - size_ < vec.size_) {
      grow();
    }
    std::copy(vec.arr_, vec.arr_ + vec.size_, arr_ + size_);
    size_ += vec.size_;
  }
  /**
 * Appends a specified range of elements from another vec to this vec.
 * If necessary, the capacity of this vec is expanded.
 * Throws a std::out_of_range exception if the start or end indices are not valid.
 *
 * @param list the list to append
 * @param end index of the last element (exclusive)
 * @param start the index of the first element (inclusive)
 */
  void append(vec<T>& list, uint_32_cx endIndex,
              uint_32_cx startIndex = 0) {
    if (startIndex >= endIndex || endIndex > list.size_) {
      throw std::out_of_range("index out of bounds");
    }
    while (len_ - size_ < endIndex - startIndex) {
      grow();
    }
    std::copy(list.arr_ + startIndex, list.arr_ + endIndex, arr_ + size_);
    size_ += endIndex - startIndex;
  }
  /**
  * Attempts to print the complete list to std::cout
  * @param prefix optional prefix
  */
  void print(const std::string& prefix) {
    if (!prefix.empty()) {
      std::cout << prefix << std::endl;
      std::cout << "   ";
    }
    std::cout << this << std::endl;
  }
  class Iterator {
    T* ptr;

   public:
    explicit Iterator(T* p) : ptr(p) {}
    T& operator*() { return *ptr; }
    Iterator& operator++() {
      ++ptr;
      return *this;
    }
    bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
  };
  Iterator begin() { return Iterator(arr_); }
  Iterator end() { return Iterator(arr_ + size_); }
  friend std::ostream& operator<<(std::ostream& os, const vec<T>& list) {
    if (list.size_ == 0) {
      return os << "[]";
    }
    os << "[" << list.arr_[0];

    for (int i = 1; i < list.size_; i++) {
      os << "," << list.arr_[i];
    }
    return os << "]";
  }

  static void TEST() {
    std::cout << "TESTING ARRAY LIST" << std::endl;

    // Test 1: Testing add and remove
    std::cout << "   Test 1: Testing add and remove..." << std::endl;
    vec<int> list1;
    list1.add(5);
    list1.add(10);
    list1.add(15);

    list1.remove(10);
    assert(list1.size() == 2);
    assert(list1[1] == 15);

    // Test 2: Testing List access
    std::cout << "   Test 2: Testing List access..." << std::endl;
    assert(list1[0] == 5);
    assert(list1.at(-1) == 15);
    assert(list1.at(-2) == 5);
    assert(list1[1] == 15);

    // Test 3: Testing iterator
    std::cout << "   Test 3: Testing iterator..." << std::endl;
    list1.clear();
    list1.add(5);
    list1.add(10);
    list1.add(15);

    int checkNum = 0;
    for (auto& num : list1) {
      checkNum += 5;
      assert(num == checkNum);
    }
    assert(checkNum == 15);

    // Test 4: Testing resizing
    std::cout << "   Test 4: Testing resizing..." << std::endl;
    list1.clear();
    for (int i = 0; i < 10000; i++) {
      list1.add(i);
    }
    for (int i = 0; i < 10000; i++) {
      list1.remove(i);
    }
    assert(list1.size() == 0);

    // Test 5: Testing contains
    std::cout << "   Test 5: Testing contains..." << std::endl;
    list1.clear();
    list1.add(5);
    assert(list1.contains(5) == true);
    assert(list1.contains(5, false) == true);

    // Test 6: Testing append
    std::cout << "   Test 6: Testing append..." << std::endl;
    list1.clear();

    list1.add(5);
    list1.add(10);
    vec<int> list2;
    for (int i = 0; i < 1000000; i++) {
      list2.add(i);
    }
    assert(list2.size() == 1000000);

    list1.append(list2);
    assert(list1.size() == 1000002);
    assert(list2[10] == 10);

    list1.clear();

    list1.append(list2, 10, 1);
    int check = 1;
    for (auto num : list1) {
      assert(check++ == num);
    }
    assert(list1.size() == 9);

    // Test 7: copy constructor
    std::cout << "   Test 7: Testing copy constructor..." << std::endl;
    vec<int> list5(10);
    for (uint_32_cx i = 0; i < 10; i++) {
      list5.add(i);
    }
    vec<int> list6 = list5;  // copy constructor
    for (uint_32_cx i = 0; i < 10; i++) {
      assert(list6[i] == i);
    }

    // Test 8: copy assignment
    std::cout << "   Test 8: Testing copy assignment..." << std::endl;
    vec<int> list7(10);
    list7 = list5;  // copy assignment
    for (uint_32_cx i = 0; i < 10; i++) {
      assert(list7[i] == i);
    }

    // Test 9: move constructor
    std::cout << "   Test 9: Testing move constructor..." << std::endl;
    vec<int> list8 = std::move(list5);  // move constructor
    for (uint_32_cx i = 0; i < 10; i++) {
      assert(list8[i] == i);
    }

    // Test 10: move assignment
    std::cout << "   Test 10: Testing move assignment..." << std::endl;
    vec<int> list9(10);
    list9 = std::move(list6);  // move assignment
    for (uint_32_cx i = 0; i < 10; i++) {
      assert(list9[i] == i);
    }

    // Test 11: Checking for memory leaks
    std::cout << "   Test 11: Checking for memory leaks..." << std::endl;
    list1.clear();
    for (int i = 0; i < 10000000; i++) {
      list1.add(i);
    }
  }
};
}  // namespace cxstructs
#endif  // CXSTRUCTS_ARRAYLIST_H
