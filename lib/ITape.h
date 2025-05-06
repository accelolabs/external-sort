#pragma once

#include <cstddef>


template<typename T>
class ITape {

public:
    
    virtual ~ITape() = default;

    virtual T operator--() = 0;
    virtual T operator++() = 0;
    virtual size_t index() = 0;
    virtual size_t size() = 0;
    virtual void rewind() = 0;

    virtual T read() = 0;
    virtual void write(const T& value) = 0;

};
