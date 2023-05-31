#pragma once

#include <stdexcept>
#include <string>

namespace MatrixException {
    class Exception : public std::runtime_error
    {
    public:
        Exception(const std::string& str) :
            std::runtime_error("Matrix exception. " + str) {}
    };

    class SizeMismatch : public Exception
    {
    public:
        SizeMismatch() : Exception("Size mismatch") {}
    };
}
