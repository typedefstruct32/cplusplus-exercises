#ifndef MYSTRING_H_
#define MYSTRING_H_
#include <string.h>

class MyString
{
public:
    MyString();
    MyString(const char *);

    ~MyString();

    MyString &operator = (const char *);
    MyString (const MyString &);

    MyString &operator += (const MyString &);

public:
    int size_;
    char *data_;

};


#endif