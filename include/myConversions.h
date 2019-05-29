#ifndef MYCONVERSIONS_H_INCLUDED
#define MYCONVERSIONS_H_INCLUDED

#include <sstream>

template<class T>
inline std::string float2str(T n, int no_digits){
    std::ostringstream convert;
    convert.flags(std::ios_base::fmtflags::_S_scientific | std::ios_base::fmtflags::_S_showpos);
    convert.precision(no_digits);
    convert << n;
    return convert.str();
}

template<class T>
inline T str2float(const std::string& in){
    T ret;
    std::istringstream convert(in);
    //convert.flags();
    convert >> ret;
    return ret;
}

template<class T>
inline std::string mysprintf(T n, const char* fmt){
    char retBuffer[50];
    sprintf(retBuffer, fmt, n);
    return std::string(retBuffer);
}

#endif // MYCONVERSIONS_H_INCLUDED
