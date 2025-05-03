#pragma once


template<typename T>
class ITape {

public:
    
    virtual ~ITape() = default;


    virtual T peek() const = 0;
    virtual T pop() = 0;
    virtual void push(const T& value) = 0;

};
