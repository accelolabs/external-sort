#pragma once


template<typename T>
class ITape {

public:
    
    virtual ~ITape() = default;

    virtual T operator--() = 0;
    virtual T operator++() = 0;

    virtual T read() = 0;
    virtual void write(const T& value) = 0;

};
