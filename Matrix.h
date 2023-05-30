#pragma once

#include <initializer_list>
#include <vector>
#include <iterator>
#include <ostream>

template<typename T>
class Matrix final
{
    template<typename R, typename Fr>
    class ItBasic final
    {
        friend Fr;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = std::vector<R>::iterator::value_type;
        using difference_type = std::vector<R>::iterator::difference_type;
        using pointer = std::vector<R>::iterator::pointer;
        using reference = std::vector<R>::iterator::reference;

    public:
        ItBasic() = default;
        R& operator*() { return *itImpl_; }
        const R& operator*() const { return *itImpl_; }
        bool operator==(const ItBasic& rhs) const { return itImpl_ == rhs.itImpl_; }
        bool operator!=(const ItBasic& rhs) const { return itImpl_ != rhs.itImpl_; }
        bool operator>(const ItBasic& rhs) const { return itImpl_ > rhs.itImpl_; }
        bool operator<(const ItBasic& rhs) const { return itImpl_ < rhs.itImpl_; }
        bool operator>=(const ItBasic& rhs) const { return itImpl_ >= rhs.itImpl_; }
        bool operator<=(const ItBasic& rhs) const { return itImpl_ <= rhs.itImpl_; }
        ItBasic& operator++() {
            ++itImpl_;
            return *this;
        }
        ItBasic operator++(int) {
            ItBasic temp(itImpl_++);
            return temp;
        }
        ItBasic& operator--() {
            --itImpl_;
            return *this;
        }
        ItBasic operator--(int) {
            ItBasic temp(itImpl_--);
            return temp;
        }
        ItBasic& operator+=(std::size_t val) {
            itImpl_ += val;
            return *this;
        }
        ItBasic& operator-=(std::size_t val) {
            itImpl_ -= val;
            return *this;
        }
        difference_type operator-(const ItBasic& rhs) const { return itImpl_ - rhs.itImpl_; }
        R& operator[](int i) { return itImpl_[i]; }
        const R& operator[](int i) const { return itImpl_[i]; }

    private:
        ItBasic(std::vector<R>::iterator it) : itImpl_(it) {}

    private:
        std::vector<R>::iterator itImpl_;
    };

    template<typename R, typename Fr>
    class CItBasic final
    {
        friend Fr;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = std::vector<R>::const_iterator::value_type;
        using difference_type = std::vector<R>::const_iterator::difference_type;
        using pointer = std::vector<R>::const_iterator::pointer;
        using reference = std::vector<R>::const_iterator::reference;

    public:
        CItBasic() = default;
        const R& operator*() const { return *itImpl_; }
        bool operator==(const CItBasic& rhs) const { return itImpl_ == rhs.itImpl_; }
        bool operator!=(const CItBasic& rhs) const { return itImpl_ != rhs.itImpl_; }
        bool operator>(const CItBasic& rhs) const { return itImpl_ > rhs.itImpl_; }
        bool operator<(const CItBasic& rhs) const { return itImpl_ < rhs.itImpl_; }
        bool operator>=(const CItBasic& rhs) const { return itImpl_ >= rhs.itImpl_; }
        bool operator<=(const CItBasic& rhs) const { return itImpl_ <= rhs.itImpl_; }
        CItBasic& operator++() {
            ++itImpl_;
            return *this;
        }
        CItBasic operator++(int) {
            CItBasic temp(itImpl_++);
            return temp;
        }
        CItBasic& operator--() {
            ++itImpl_;
            return *this;
        }
        CItBasic operator--(int) {
            CItBasic temp(itImpl_--);
            return *this;
        }
        CItBasic& operator+=(std::size_t val) {
            itImpl_ += val;
            return *this;
        }
        CItBasic& operator-=(std::size_t val) {
            itImpl_ -= val;
            return *this;
        }
        difference_type operator-(const CItBasic& rhs) const { return itImpl_ - rhs.itImpl_; }
        const R& operator[](int i) const { return itImpl_[i]; }

    private:
        CItBasic(std::vector<R>::const_iterator it) : itImpl_(it) {}

    private:
        std::vector<R>::const_iterator itImpl_;
    };

public:
    class Row final
    {
    public:
        using iterator = ItBasic<T, Row>;
        using const_iterator = CItBasic<T, Row>;

    public:
        Row(std::size_t nColumns) : data_(nColumns) {}
        Row(std::initializer_list<T> ilist) : data_(ilist) {}
        T& operator[](std::size_t i) { return data_[i]; }
        const T& operator[](std::size_t i) const { return data_[i]; }
        iterator begin() { return iterator(data_.begin()); }
        iterator end() { return iterator(data_.end()); }
        const_iterator begin() const { return const_iterator(data_.begin()); }
        const_iterator end() const { return const_iterator(data_.end()); }
        const_iterator cbegin() const { return const_iterator(data_.cbegin()); }
        const_iterator cend() const { return const_iterator(data_.cend()); }
        friend iterator operator+(const iterator& it, int val) {
            iterator temp(it);
            temp += val;
            return temp;
        }
        friend iterator operator+(int val, const iterator& it) { return it + val; }
        friend const_iterator operator+(const const_iterator& it, int val) {
            const_iterator temp(it);
            temp += val;
            return temp;
        }
        friend const_iterator operator+(int val, const const_iterator& it) { return it + val; }

    private:
        std::vector<T> data_;
    };

    using iterator = ItBasic<Row, Matrix>;
    using const_iterator = CItBasic<Row, Matrix>;

public:
    Matrix(std::size_t nRows, std::size_t nColumns) : rows_(nRows, Row(nColumns)) {}
    Matrix(std::initializer_list<std::initializer_list<T>> ilist) {
        for (const auto& irow : ilist)
            rows_.emplace_back(irow);
    }
    Row& operator[](std::size_t j) { return rows_[j]; }
    const Row& operator[](std::size_t j) const { return rows_[j]; }
    iterator begin() { return iterator(rows_.begin()); }
    iterator end() { return iterator(rows_.end()); }
    const_iterator begin() const { return const_iterator(rows_.begin()); }
    const_iterator end() const { return const_iterator(rows_.end()); }
    const_iterator cbegin() const { return const_iterator(rows_.cbegin()); }
    const_iterator cend() const { return const_iterator(rows_.cend()); }
    friend iterator operator+(const iterator& it, int val) {
       iterator temp(it);
        temp += val;
        return temp;
    }
    friend iterator operator+(int val , const iterator& it) { return it + val; }
    friend const_iterator operator+(const const_iterator& it, int val) {
        const_iterator temp(it);
        temp += val;
        return temp;
    }
    friend const_iterator operator+(int val, const const_iterator& it) { return it + val; }
    friend iterator operator-(const iterator& it, int val) {
        iterator temp(it);
        temp -= val;
        return temp;
    }
    friend const_iterator operator-(const const_iterator& it, int val) {
        const_iterator temp(it);
        temp -= val;
        return temp;
    }

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
