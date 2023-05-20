#pragma once
#include <ostream>
#include <vector>
#include <initializer_list>

template<typename T = int>
class Matrix final
{
public:
    class Row
    {
    public:
        Row(std::vector<T>::iterator it);
        T& operator[](int i);

    private:
        std::vector<T>::iterator it_;
    };

    class CRow
    {
    public:
        CRow(std::vector<T>::const_iterator it);
        const T& operator[](int i) const;

    private:
        std::vector<T>::const_iterator it_;
    };

public:
    Matrix(int nRows, int nColumns);
    Matrix(std::initializer_list<std::initializer_list<T>> list);
    int nrows() const;
    int ncols() const;
    Row operator[](int j) &;
    CRow operator[](int j) const &;
    void dump(std::ostream& os) const;

private:
    std::vector<T> data_;
    const int nColumns_;
};

// ############################################################################
template<typename T>
Matrix<T>::Matrix(int nRows, int nColumns) :
    data_(nRows * nColumns), nColumns_(nColumns)
{
}

template<typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> ilist) :
    nColumns_(static_cast<int>((*ilist.begin()).size()))
{
    for (auto& row : ilist)
        data_.insert(data_.cend(), row);
}

template<typename T>
int Matrix<T>::nrows() const
{
    return data_.size() / nColumns_;
}

template<typename T>
int Matrix<T>::ncols() const
{
    return nColumns_;
}

template<typename T>
Matrix<T>::Row Matrix<T>::operator[](int j) &
{
    return Row(data_.begin() + j * nColumns_);
}

template<typename T>
Matrix<T>::CRow Matrix<T>::operator[](int j) const &
{
    return CRow(data_.begin() + j * nColumns_);
}

template<typename T>
void Matrix<T>::dump(std::ostream& os) const
{
    const int nRows = data_.size() / nColumns_;
    for (int i = 0; i < data_.size(); ++i) {
        os << data_[i];
        if ((i + 1) % nRows == 0)
            os << std::endl;
        else
            os << "\t";
    }
}

// ############################################################################
template<typename T>
Matrix<T>::Row::Row(std::vector<T>::iterator it) :
    it_(it)
{
}

template<typename T>
T& Matrix<T>::Row::operator[](int i)
{
    return *(it_ + i);
}

// ############################################################################
template<typename T>
Matrix<T>::CRow::CRow(std::vector<T>::const_iterator it) :
    it_(it)
{
}

template<typename T>
const T& Matrix<T>::CRow::operator[](int i) const
{
    return *(it_ + i);
}

// ############################################################################
template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m)
{
    m.dump(os);
    return os;
}
