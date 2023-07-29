#pragma once

#include <initializer_list>
#include <vector>
#include <ostream>
#include "MatrixExceptions.h"

template<typename T>
class Matrix final
{
public:
    class Row final
    {
    public:
        using iterator = std::vector<T>::iterator;
        using const_iterator = std::vector<T>::const_iterator;

    public:
        Row(std::size_t nColumns) : data_(nColumns) {}
        Row(std::initializer_list<T> ilist) : data_(ilist) {}
        T& operator[](std::size_t i) { return data_[i]; }
        const T& operator[](std::size_t i) const { return data_[i]; }
        iterator begin() { return data_.begin(); }
        iterator end() { return data_.end(); }
        const_iterator begin() const { return data_.begin(); }
        const_iterator end() const { return data_.end(); }
        const_iterator cbegin() const { return data_.cbegin(); }
        const_iterator cend() const { return data_.cend(); }

        std::size_t size() const { return data_.size(); }

    private:
        std::vector<T> data_;
    };

    using iterator = std::vector<Row>::iterator;
    using const_iterator = std::vector<Row>::const_iterator;

public:
    Matrix(std::size_t nRows, std::size_t nColumns) : rows_(nRows, Row(nColumns)) {}
    Matrix(std::initializer_list<std::initializer_list<T>> ilist) {
        for (const auto& irow : ilist)
            rows_.emplace_back(irow);
    }
    Row& operator[](std::size_t j) { return rows_[j]; }
    const Row& operator[](std::size_t j) const { return rows_[j]; }
    iterator begin() { return rows_.begin(); }
    iterator end() { return rows_.end(); }
    const_iterator begin() const { return rows_.begin(); }
    const_iterator end() const { return rows_.end(); }
    const_iterator cbegin() const { return rows_.cbegin(); }
    const_iterator cend() const { return rows_.cend(); }

    bool equal(const Matrix& other) const {
        if (nrows() != other.nrows() || ncols() != other.ncols())
            return false;

        for (int j = 0; j < nrows(); ++j)
            for (int i = 0; i < ncols(); ++i)
                if (rows_[j][i] != other[j][i])
                    return false;
        return true;
    }

    std::size_t nrows() const { return rows_.size(); }
    std::size_t ncols() const { return rows_[0].size(); }
    Matrix& addition(const Matrix& other) {
        if (nrows() != other.nrows() || ncols() != other.ncols())
            throw MatrixException::SizeMismatch();

        for (int j = 0; j < nrows(); ++j)
            for (int i = 0; i < ncols(); ++i)
                rows_[j][i] += other[j][i];

        return *this;
    }
    Matrix addition(const Matrix& other) const {
        Matrix m(*this);
        m.addition(other);
        return m;
    }
    Matrix& subtraction(const Matrix& other) {
        if (nrows() != other.nrows() || ncols() != other.ncols())
            throw MatrixException::SizeMismatch();

        for (int j = 0; j < nrows(); ++j)
            for (int i = 0; i < ncols(); ++i)
                rows_[j][i] -= other[j][i];

        return *this;
    }
    Matrix subtraction(const Matrix& other) const {
        Matrix m(*this);
        m.subtraction(other);
        return m;
    }
    template<typename X>
    Matrix& multByNumber(X number) {
        for (int j = 0; j < nrows(); ++j)
            for (int i = 0; i < ncols(); ++i)
                rows_[j][i] *= number;

        return *this;
    }
    template<typename X>
    Matrix multByNumber(X number) const {
        Matrix m(*this);
        m.multByNumber(number);
        return m;
    }
    Matrix mult(const Matrix& rhs) const {
        if (ncols() != rhs.nrows())
            throw MatrixException::SizeMismatch();

        Matrix m(nrows(), rhs.ncols());
        for (std::size_t j = 0; j < m.nrows(); ++j) {
            for (std::size_t i = 0; i < m.ncols(); ++i) {
                T& cell = m[j][i];
                for (std::size_t k = 0; k < ncols(); ++k)
                    cell += rows_[j][k] * rhs[k][i];
            }
        }

        return m;
    }
    Matrix& transpose() {
        Matrix m(ncols(), nrows());
        for (std::size_t j = 0; j < nrows(); ++j)
            for (std::size_t i = 0; i < ncols(); ++i)
                m[i][j] = rows_[j][i];
        *this = m;

        return *this;
    }
    Matrix transpose() const {
        Matrix m(*this);
        m.transpose();
        return m;
    }
    T det() const {
        if (rows_.size() == 0 || rows_.size() != rows_[0].size())
            throw MatrixException::NotSquare{};
        return detImpl(*this);
    }

private:
    Matrix<T> subMatrix(int n, const Matrix<T>& m) const {
        Matrix<T> retval{m.nrows() - 1, m.ncols() - 1};
        for (int i = 1; i < m.nrows(); ++i) {
            bool fl = false;
            for (int j = 0; j < m.ncols(); ++j) {
                if (j == n)
                    fl = true;
                else
                    retval[i - 1][!fl ? j : j - 1] = m[i][j];
            }
        }
        return retval;
    }

    T detImpl(const Matrix& m) const {
        if (m.nrows() == 1)
            return m[0][0];

        if (m.nrows() == 2)
            return m[0][0] * m[1][1] - m[1][0] * m[0][1];

        T accum{};
        bool sign = true;
        for (int i = 0; i < m.nrows(); ++i) {
            const T value = m[0][i] * detImpl(subMatrix(i, m));
            accum = sign ? accum + value : accum - value;
            sign = !sign;
        }
        return accum;
    }

private:
    std::vector<Row> rows_;
};

template<typename T>
bool operator==(const Matrix<T>& lhs, const Matrix<T>& rhs) { return lhs.equal(rhs); }

template<typename T>
bool operator!=(const Matrix<T>& lhs, const Matrix<T>& rhs) { return !(lhs == rhs); }

template<typename T>
Matrix<T> operator+(const Matrix<T>& lhs , const Matrix<T>& rhs) { return lhs.addition(rhs); }

template<typename T>
Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs) { return lhs.subtraction(rhs); }

template<typename T, typename X>
Matrix<T> operator*(const Matrix<T>& lhs, X number) { return lhs.multByNumber(number); }

template<typename T, typename X>
Matrix<T> operator*(X number, const Matrix<T>& rhs) { return rhs.multByNumber(number); }

template<typename T>
Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs) { return lhs.mult(rhs); }

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m)
{
    for (const auto& row : m) {
        for (const auto& val : row)
            os << val << " ";
        os << std::endl;
    }

    return os;
}
