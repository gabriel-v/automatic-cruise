/*
 * @file $FILE
 * @brief 
 * @author Gabriel Vijiala < gabriel.vijiala@gmail.com >
 * @date 4/9/16
 */

#ifndef LEC_ACC_CPP_ERROR_H
#define LEC_ACC_CPP_ERROR_H

#include <exception>
#include <string>

struct Error : public std::exception {
    std::string s;

    Error(std::string ss) : s(ss) { }

    ~Error() throw() { }

    const char *what() const throw() { return s.c_str(); }
};


#endif
