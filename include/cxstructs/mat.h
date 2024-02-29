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
#define CX_FINISHED

#ifndef CXSTRUCTS_SRC_CXSTRUCTS_MAT_H_
#define CXSTRUCTS_SRC_CXSTRUCTS_MAT_H_

#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../cxconfig.h"
#include "../cxutil/cxmath.h"
#include "vec.h"

namespace cxstructs {
/**
    <h2>2D Matrix</h2>
    This data structure is an efficient representation of a two-dimensional<b> ROW-MAJOR</b> matrix, using a flattened array for cache efficiency and faster access times.
    <br><br>
    <b>Datatype:</b> The elements of the matrix are of type float.
    <br><br>
    <b>Indexing:</b> The matrix uses 0-based indexing.
    <br><br>
    <b>Initialization:</b> The default constructor initializes all elements to 0.
    <br><br>
    A 2D Matrix is essential in various applications such as graphics transformations, solving systems of linear equations, and data analysis. The flattened array representation ensures that the elements are stored in a contiguous block of memory, which is beneficial for cache performance.<p>

    <b>Use Cases:</b> 2D Matrices are widely used in linear algebra, image processing, computer graphics, and scientific computing.
    */
class mat {
  float* arr;
  uint_32_cx n_rows_;
  uint_32_cx n_cols_;

 public:
  inline mat() : n_cols_(0), n_rows_(0), arr(nullptr){};
  inline mat(std::initializer_list<float> list) : n_rows_(1), n_cols_((uint_32_cx)list.size()) {
    arr = new float[n_cols_];
    uint32_t i = 0;
    for (float val : list) {
      arr[i++] = val;
    }
  }
  inline mat(std::initializer_list<std::initializer_list<float>> list)
      : n_rows_((uint_32_cx)list.size()), n_cols_((uint_32_cx)list.begin()->size()) {
    arr = new float[n_rows_ * n_cols_];
    uint32_t i = 0;
    for (const auto& sublist : list) {
      for (float val : sublist) {
        arr[i++] = val;
      }
    }
  }
  /**
   * Constructs a matrix object with n_rows_ rows and n_cols_ columns <p>
   * Initialized with 0
   * @param rows number of rows
   * @param cols number of columns
   */
  inline mat(const uint_32_cx& n_rows, const uint_32_cx& n_cols)
      : n_rows_(n_rows), n_cols_(n_cols) {
    arr = new float[n_rows * n_cols];
    std::fill(arr, arr + n_rows * n_cols, 0);
  }

  inline explicit mat(std::vector<std::vector<float>> vec)
      : n_rows_(vec.size()), n_cols_((uint_32_cx)vec[0].size()) {
    arr = new float[n_rows_ * n_cols_];
    for (uint_32_cx i = 0; i < n_rows_; i++) {
      std::copy_n(vec[i].begin(), n_cols_, arr + i * n_cols_);
    }
  }
  /**
 * @brief Constructs a matrix with the specified number of rows and columns, and initializes its elements using a provided function.
 *
 * @tparam fill_form A callable object or function that takes a single int as an argument and returns a float
 * @param n_rows The number of rows in the matrix.
 * @param n_cols The number of columns in the matrix.
 * @param form The callable object or function used to initialize the elements of the matrix. It is invoked for each element with the element's index as an argument.
 * @note The `fill_form` function is invoked for each element of the matrix as `form(index)`, where `index` is the linear index of the element in row-major order (0-indexed).
 */
  template <typename fill_form,
            typename = std::enable_if_t<std::is_invocable_r_v<double, fill_form, double>>>
  inline mat(uint_32_cx n_rows, uint_32_cx n_cols, fill_form form)
      : n_rows_(n_rows), n_cols_(n_cols) {
    arr = new float[n_rows * n_cols];
    for (int i = 0; i < n_rows * n_cols; i++) {
      arr[i] = form(i);
    }
  }
  /**
   * Constructs a matrix with dimensions rows and cols by copy from the given float pointer
   * A new array is allocated so the matrix doesnt take ownership of the pointer
   * @param data a float pointer
   * @param rows
   * @param cols
   */
  inline mat(float* data, uint_32_cx rows, uint_32_cx cols)
      : n_rows_(rows), n_cols_(cols), arr(new float[rows * cols]) {
    std::copy(data, data + rows * cols, arr);
  }
  inline mat(const mat& o) : n_rows_(o.n_rows_), n_cols_(o.n_cols_) {
    arr = new float[n_rows_ * n_cols_];
    std::copy(o.arr, o.arr + n_rows_ * n_cols_, arr);
  }
  inline ~mat() { delete[] arr; };
  inline float& operator()(const uint_32_cx& row, const uint_32_cx& col) {
    return arr[row * n_cols_ + col];
  }
  inline float operator()(const uint_32_cx& row, const uint_32_cx& col) const {
    return arr[row * n_cols_ + col];
  }
  /**
   * The underlying array is flattened!
   * A specific row and column can be accessed with:<p>
   * arr_[row * n_cols_ + column] <p>
   * Use with caution!
   * @return a pointer to the underlying array
   */
  float* get_raw() { return arr; }
  //assign
  inline mat& operator=(const mat& other) {
    if (this != &other) {
      delete[] arr;

      n_rows_ = other.n_rows_;
      n_cols_ = other.n_cols_;

      arr = new float[n_rows_ * n_cols_];
      std::copy(other.arr, other.arr + n_rows_ * n_cols_, arr);
    }
    return *this;
  }
  /**
   * Returns a new matrix that is the result of the multiplication
   * of the current matrix with the provided matrix.
   * The number of columns in the first matrix must match the number
   * of rows in the second.
   * @param o other matrix
   * @return the result of the operation
   */
  inline mat operator*(const mat& o) const {
    CX_ASSERT(n_cols_ == o.n_rows_, "invalid dimensions");

    mat result(n_rows_, o.n_cols_);
    for (uint_32_cx i = 0; i < n_rows_; i++) {
      for (uint_32_cx j = 0; j < o.n_cols_; j++) {
        float sum = 0.0f;
        for (uint_32_cx k = 0; k < n_cols_; k++) {
          sum += arr[i * n_cols_ + k] * o.arr[k * o.n_cols_ + j];
        }
        result.arr[i * result.n_cols_ + j] = sum;
      }
    }
    return result;
  }
  /**
   * Alternative in-place scaling with a float
   * @param f a float scalar
   */
  inline void operator*=(const float& f) const {
#pragma omp simd linear(i : 1)
    for (uint_fast32_t i = 0; i < n_rows_ * n_cols_; i++) {
      arr[i] *= f;
    }
  }
  /**
   * Returns a new matrix that
   * is the result of the addition of the current matrix with the provided matrix
   * The matrices must be of the same n_elem
   * @param o other matrix
   * @return the result of the operation
   */
  inline mat operator+(const mat& o) const {
    CX_ASSERT(o.n_cols_ == n_cols_ || o.n_rows_ == n_rows_, "invalid dimensions");
    mat res(n_rows_, n_cols_);
    for (uint_fast32_t i = 0; i < n_rows_ * n_cols_; i++) {
      res.arr[i] = arr[i] + o.arr[i];
    }
    return res;
  };
  /**
   * Returns a new matrix that is the
   * result of the subtraction of the provided matrix from the current matrix.
   * The matrices must be of the same n_elem
   * @param o other matrix
   * @return the result of the operation
   */
  inline mat operator-(const mat& o) const {
    CX_ASSERT(o.n_cols_ == n_cols_ || o.n_rows_ == n_rows_, "invalid dimensions");
    mat res(n_rows_, n_cols_);
    for (uint_fast32_t i = 0; i < n_rows_ * n_cols_; i++) {
      res.arr[i] = arr[i] - o.arr[i];
    }
    return res;
  };
  inline void operator-=(const mat& o) const {
    CX_ASSERT(o.n_cols_ == n_cols_ || o.n_rows_ == n_rows_, "invalid dimensions");
#pragma omp simd linear(i : 1)
    for (uint_fast32_t i = 0; i < n_rows_ * n_cols_; i++) {
      arr[i] -= o.arr[i];
    }
  }
  /**
   * Returns a new matrix that is the matrix Hadamard product (element-wise multiplication).
   * The matrices must be of the same n_elem
   * @param o other matrix
* @return the result of the operation
   */
  inline mat operator%(const mat& o) const {
    CX_ASSERT(o.n_cols_ == n_cols_ || o.n_rows_ == n_rows_, "invalid dimensions");

    mat res(n_rows_, n_cols_);
    for (uint_fast32_t i = 0; i < n_rows_ * n_cols_; i++) {
      res.arr[i] = arr[i] * o.arr[i];
    }
    return res;
  }
  /**
   * Returns a new matrix that is the result of element-wise division of the current matrix by the provided matrix
   * The matrices must be of the same n_elem
   * @param o other matrix
   * @return the result of the operation
   */
  inline mat operator/(const mat& o) const {
    CX_ASSERT(o.n_cols_ == n_cols_ || o.n_rows_ == n_rows_, "invalid dimensions");
    mat res(n_rows_, n_cols_);
    for (uint_fast32_t i = 0; i < n_rows_ * n_cols_; i++) {
      res.arr[i] = arr[i] / o.arr[i];
    }
    return res;
  }
  /**
   * Checks whether the current matrix and the provided matrix are equal, i.e., have the same n_elem and identical elements in corresponding positions
   * @param o other matrix
   * @return true if the matrices are identical
   */
  inline bool operator==(const mat& o) const {
    if (this == &o) {
      return true;
    }
    if (o.n_cols_ == n_cols_ && o.n_rows_ == n_rows_) {
      for (uint_fast32_t i = 0; i < n_rows_ * n_cols_; i++) {
        if (o.arr[i] != arr[i]) {
          return false;
        }
      }
      return true;
    }
    return false;
  };
  /**
   * Checks whether the current matrix and the provided matrix are not equal.
   * @param o
   * @return true if the matrices are not identical
   */
  inline bool operator!=(const mat& o) const {
    if (this == &o) {
      return false;
    }
    if (o.n_cols_ == n_cols_ && o.n_rows_ == n_rows_) {
      for (uint_fast32_t i = 0; i < n_rows_ * n_cols_; i++) {
        if (o.arr[i] != arr[i]) {
          return true;
        }
      }
      return false;
    }
    return true;
  };
  /**
   * This method returns the transpose of the matrix. The transpose of
   * a matrix is a new matrix whose rows are the columns of the original matrix.
   * @return the transpose
   */
  [[nodiscard]] inline mat transpose() const {
    mat retval(n_cols_, n_rows_);

    for (uint_32_cx i = 0; i < n_cols_; i++) {
      for (uint_32_cx j = 0; j < n_rows_; j++) {
        retval.arr[i * n_rows_ + j] = arr[j * n_cols_ + i];
      }
    }
    return retval;
  };
  /**
   * Allows you to perform a lambda function on all values of the given row
   * Both the current column and the value are given: col, val -> lambda()
   * @tparam lambda the function to perform
   * @param row row to perform the operation on
   * @param l the function to determine the new value
   */
  template <typename lambda>
  inline void row_op(uint_32_cx row, lambda l) {
    for (int i = 0; i < n_cols_; i++) {
      arr[row * n_cols_ + i] = l(i, arr[row * n_cols_ + i]);
    }
  }
  /**
   * Allows you to perform a lambda function on all values of the given column
   * Both the current row and the value are given: row, val -> lambda(... return val)
   * @tparam lambda the function to perform
   * @param col the column to perform the operation on
   * @param l the function to determine the new value
   */
  template <typename lambda>
  inline void col_op(uint_32_cx col, lambda l) {
    for (int i = 0; i < n_rows_; i++) {
      arr[i * n_cols_ + col] = l(i, arr[i * n_cols_ + col]);
    }
  }
  template <typename lambda>
  inline void mat_op(lambda l) {
#pragma omp simd linear(i : 1)
    for (uint_fast32_t i = 0; i < n_rows_ * n_cols_; i++) {
      arr[i] = l(arr[i]);
    }
  }
  /**
   * Allows you to perform a function on all values of the matrix
   * The current value is given as input [](float mat_val){return func(mat_val);}
   * @tparam lambda the function to perform
   * @param row row to perform the operation on
   * @param l the function to determine the new value
   */
  inline void mat_op(float (*func)(float)) {
#pragma omp simd linear(i : 1)
    for (uint_fast32_t i = 0; i < n_rows_ * n_cols_; i++) {
      arr[i] = func(arr[i]);
    }
  }
  /**
 * Takes the dot product of each row of the matrix with the given vector and returns a new vector
 * @param vec
 * @return a new row with each index being the result of the vector-matrix dot product
 */
  [[nodiscard]] inline vec<float> dotProduct(const vec<float>& vec) const {
    cxstructs::vec<float> retval(n_rows_, 0);
    for (int i = 0; i < n_rows_; ++i) {
      for (int j = 0; j < n_cols_; ++j) {
        retval[i] += arr[i * n_cols_ + j] * vec[j];
      }
    }
    return retval;
  }
  /**
   *  A unit matrix is a matrix with dimensions n_rows_ and n_cols_, with only one nonzero entry with value 1 at position M(row, col). <p>
   *
   * ij * M -> 0 except for the i-th row which is alpha * M[j, ]<p>
   * M * ij -> 0 except for the j-th column which is alpha * M[ ,i]
   *
   * @param n_rows the amount of row of the matrix unit
   * @param n_cols the amount of cols of the matrix unit
   * @param row row of 1
   * @param col col of 1
   * @return a new matrix with these properties
   */
  static inline mat unit_matrix(uint_32_cx n_rows, uint_32_cx n_cols, uint_32_cx row,
                                uint_32_cx col) {
    mat retval(n_rows, n_cols);
    retval(row, col) = 1;
    return retval;
  }
  /**
   * A scaled unit matrix is a matrix with dimensions n_rows_ and n_cols_, with only one nonzero entry with value 1 * alpha at position M(row, col). <p>
   *
   * ij * M -> 0 except for the i-th row which is alpha * M[j, ]<p>
   * M * ij -> 0 except for the j-th column which is alpha * M[ ,i]
   * @param n_rows the amount of row of the matrix unit
   * @param n_cols the amount of cols of the matrix unit
   * @param row row of 1
   * @param col col of 1
   * @param alpha the scalar value
   * @return a new matrix with these properties
   */
  static inline mat unit_matrix_scaled(uint_32_cx n_rows, uint_32_cx n_cols, uint_32_cx row,
                                       uint_32_cx col, float alpha) {
    mat retval(n_rows, n_cols);
    retval(row, col) = 1 * alpha;
    return retval;
  }
  /**
   *A square matrix with ones on the main diagonal and zeros elsewhere.<p>
   * Has the property that when multiplied with any matrix M returns exactly M
   * @param size side length
   * @return a new identity matrix
   */
  static inline mat eye(uint_32_cx size) {
    mat eye(size, size);
    for (uint_fast32_t i = 0; i < size; i++) {
      eye.arr[i * size + i] = 1;
    }
    return eye;
  }
  /**
   * Multiplies the whole matrix with the given scalar in-place
   * @param s the scalar
   */
  inline void scale(const float& a) {
#pragma omp simd linear(i : 1)
    for (uint_fast32_t i = 0; i < n_rows_ * n_cols_; i++) {
      arr[i] = arr[i] * a;
    }
  };
  [[nodiscard]] inline mat sum_rows() const {
    mat retval(n_rows_, 1);
    for (uint_fast32_t i = 0; i < n_rows_; i++) {
      float sum = 0;
      for (uint_fast32_t j = 0; j < n_cols_; j++) {
        sum += arr[i * n_cols_ + j];
      }
      retval(i, 0) = sum;
    }
    return retval;
  }
  [[nodiscard]] inline mat sum_cols() const {
    mat ret(1, n_cols_);
    for (uint_fast32_t i = 0; i < n_cols_; i++) {
      float sum = 0;
      for (uint_fast32_t j = 0; j < n_rows_; j++) {
        sum += arr[j * n_cols_ + i];
      }
      ret(0, i) = sum;
    }
    return ret;
  }
  /**
   * Creates and returns a new matrix by copying from the given row
   * @param row row of the given matrix to copy from
   * @return a copy of row row from the given matrix
   */
  [[nodiscard]] inline mat split_row(const uint_32_cx& row) const {
    return {arr + row * n_cols_, 1, n_cols_};
  }
  /**Prints out the matrix
   * @param header optional header
   */
  void print(const std::string& header = "") const {
    if (!header.empty()) {
      std::cout << header << std::endl;
      for (uint_32_cx i = 0; i < n_rows_; i++) {
        std::cout << "     [";
        for (uint_32_cx j = 0; j < n_cols_; j++) {
          std::cout << arr[i * n_cols_ + j];
          if (j < n_cols_ - 1) {
            std::cout << ",";
          }
        }
        std::cout << "]\n";
      }
      return;
    }
    std::cout << *this << std::endl;
  }
  friend std::ostream& operator<<(std::ostream& os, const mat& obj) {
    if(obj.n_cols_+obj.n_rows_ == 0 ){
      return os<<"[]";
    }
    for (uint_32_cx i = 0; i < obj.n_rows_; i++) {
      os << "[";
      for (uint_32_cx j = 0; j < obj.n_cols_; j++) {
        os << obj.arr[i * obj.n_cols_ + j];
        if (j != obj.n_cols_ - 1) {
          os << ", ";
        }
      }
      os << "]";
      os << "\n";
    }
    return os;
  }
  /**
   *
   * @return  amount of rows of the matrix
   */
  [[nodiscard]] inline uint_32_cx n_rows() const { return n_rows_; };
  /**
   *
   * @return amount of columns of the matrix
   */
  [[nodiscard]] inline uint_32_cx n_cols() const { return n_cols_; };
  /**
   * Constructs a new vector from the given row<p>
   * This is a fast operation
   * @param row which row data to return
   * @return a vector of length n_cols with elements from the given row
   */
  [[nodiscard]] inline vec<float, false> get_row(uint_32_cx row) const noexcept {
    return vec<float, false>(arr + row * n_cols_, n_cols_);
  }
  /**
   * Constructs a new vector from the given column<p>
   * This is a rather slow operation
   * @param col the column
   * @return a vector of length n_rows with the data of the given column
   */
  inline vec<float, false> get_col(uint_32_cx col) const noexcept {
    vec<float, false> ret(n_rows_, 0);
    for (uint_fast32_t i = 0; i < n_rows_; i++) {
      ret[i] = arr[i * n_cols_ + col];
    }
    return ret;
  }

};
}  // namespace cxstructs
#endif