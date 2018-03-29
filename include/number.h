#pragma once

#include <sparsepp.h>

struct number_t {
    bool is_float;
    union {
        float floatval;
        int64_t intval;
    };

    number_t(): is_float(false), intval(0) {

    }

    number_t(bool is_float, float floatval): is_float(is_float), floatval(floatval) {

    }

    number_t(bool is_float, int64_t intval): is_float(is_float), intval(intval) {

    }


    number_t(float val): is_float(true), floatval(val) {

    }

    number_t(int64_t val): is_float(false), intval(val) {

    }

    inline void operator = (const float & val) {
        floatval = val;
        is_float = true;
    }

    inline void operator = (const int64_t & val) {
        intval = val;
        is_float = false;
    }

    inline bool operator == (const number_t & rhs) const  {
        if(is_float) {
            return floatval == rhs.floatval;
        }
        return intval == rhs.intval;
    }

    inline bool operator < (const number_t & rhs) const  {
        if(is_float) {
            return floatval < rhs.floatval;
        }
        return intval < rhs.intval;
    }

    inline bool operator > (const number_t & rhs) const {
        if(is_float) {
            return floatval > rhs.floatval;
        }
        return intval > rhs.intval;
    }

    inline number_t operator * (const number_t & rhs) const {
        if(is_float) {
            return number_t((float)(floatval * rhs.floatval));
        }
        return number_t((int64_t)(intval * rhs.intval));
    }

    inline number_t operator-()  {
        if(is_float) {
            floatval = -floatval;
        } else {
            intval = -intval;
        }

        return *this;
    }
};