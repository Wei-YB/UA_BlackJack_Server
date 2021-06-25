#pragma once
#include <iostream>
#include <memory>
#include <string>
class Poker
{
public:
    typedef std::shared_ptr<Poker> ptr;

    Poker(int value) : _value(value){};
    void showMessage(void) const;
    inline int getValue(void) const { return _value; }
    inline bool isHide(void) const { return _isHide; }
    inline void setHide(void) { _isHide = true; }
    inline void cancelHide(void) { _isHide = false; }

private:
    bool _isHide = false;
    int _value;
};