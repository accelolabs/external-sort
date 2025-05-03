#pragma once


template<typename T>
class ITape {

public:
    
    virtual ~ITape() = default;


    virtual T read() const = 0;
    virtual void write(const T& value) = 0;
    virtual void move(int delta) const = 0;

};
