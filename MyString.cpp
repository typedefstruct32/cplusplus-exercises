#include "MyString.h"
#include <iostream>


MyString::MyString() : size_(0), data_(nullptr)
{
}

MyString::MyString(const char *rhs)
{
    if (rhs == nullptr)
    {
        size_ = 0;
        data_ = nullptr;
    }
    else
    {
        size_ = strlen(rhs);
        data_ = new char[size_ + 1];
        strcpy(data_, rhs); //这里是要做深拷贝的，不能直接 data_ = rhs;
        data_[size_] = '\0';
    }
}

MyString::~MyString()
{
    if (data_)
        delete[] data_; //这里是用[]
}

MyString::MyString(const MyString &rhs)
{
    if (rhs.data_ == nullptr)
    {
        size_ = 0;
        data_ = nullptr;
    }
    else
    {
        size_ = strlen(rhs.data_);
        data_ = new char[size_ + 1];
        strcpy(data_, rhs.data_);
        data_[size_] = '\0';
    }
}

MyString& MyString::operator+=(const MyString& rhs)
{
    char *newData;
    newData = new char[strlen(rhs.data_) + size_ + 1];
    memcpy(newData, data_, size_);
    memcpy(newData + size_, rhs.data_, strlen(rhs.data_) + 1);
    delete[] data_;
    data_ = newData;
    return *this;
}

MyString& MyString::operator=(const char *rhs)
{
    if (rhs)
    {
        delete data_;
        size_ = strlen(rhs);
        data_ = new char[size_ + 1];
        strcpy(data_, rhs);
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const MyString & str) {
    os << str.data_;
    return os;
}
int main() 
{
    MyString s("123");
    //copy constructor;
    std::cout<<s;
}