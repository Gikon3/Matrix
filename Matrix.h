#pragma once

#include <initializer_list>
#include <vector>
#include <ostream>

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

private:
    std::vector<Row> rows_;
};

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
