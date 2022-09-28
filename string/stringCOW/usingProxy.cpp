#include <stdio.h>
#include <cstring>
#include <iostream>

using namespace std;

class String {

    //use CharProxy to distinguish operator[] (assign or just access)
    class charProxy {
    public:
        charProxy(size_t idx, String & self) 
        : _idx(idx), _self(self) {}

        charProxy & operator = (const char & ch);
        
        /*key point, overload type conversion operator
        charProxy simulates the behavior of char type by overloading oeprator char, but it has more advantages than char type.
        We can define new operations for charProxy, so that when you use operator= on CharProxy, you can know this meaning writing to charProxy.
        moreover, charProxy saves a reference to the parent object String, and can now perform the behavior of opening up memory and copying the data structure.
        */
        operator char() {
            cout << "charProxy::operator char()" << endl;
            return _self._pstr[_idx];
        }
    private:
        size_t _idx;
        String& _self;

    friend std::ostream & operator<<(std::ostream & os, const charProxy & rhs);
    };

    friend std::ostream & operator<<(std::ostream & os, const charProxy & rhs);

public:
    String() 
    : _pstr(new char[5]()) {
        //first 4 bytes to store the Refcount
        _pstr += 4;
        initRefCount();
    }

    String(const char* pstr) 
    : _pstr(new char[strlen(pstr) + 5]()) {
        _pstr += 4;
        initRefCount();
        strcpy(_pstr, pstr);
    }

    //shallow copy
    String(const String & rhs) : _pstr(rhs._pstr) {
        increaseRefCount();
    }

    String & operator = (const String & rhs) {
        //not copying self
        if (this != & rhs) {
            release();         //release lhs
            _pstr = rhs._pstr; //shallow copy
            increaseRefCount();
        } 
        return *this;
    }

    ~String() {
        release();
    }

    size_t refcount() const {
        return *((int*)(_pstr - 4));
    }

    size_t size() const {
        return strlen(_pstr);
    }

    const char* c_str() const {
        return _pstr;
    }

    charProxy operator[](size_t idx) {
        return charProxy(idx, *this);
    }

    const char & operator [] (size_t idx) const {
        cout<< "const char & operator[](size_t) const" << endl;
        return _pstr[idx];
    }

private:
    void initRefCount() {
        *((int*)(_pstr - 4)) = 1; 
    }

    void increaseRefCount() {
        ++*((int *)(_pstr - 4));
    }

    void decreaseRefCount() {
        --*((int *)(_pstr - 4));
    }

    void release() {
        decreaseRefCount();
        if (refcount() == 0) {
            delete [] (_pstr - 4);
            cout << ">> delete heap data!" << endl;
        }
    }
    
    friend std::ostream & operator<<(std::ostream & os, const String & rhs);

private:
    char* _pstr;
};

//Write operation, e.g.: str[5] = '1';
String::charProxy& String::charProxy::operator = (const char & ch) {
    if (_idx < _self.size()) {
        //Copy on write
        if (_self.refcount() > 1) {
            char* tmp = new char[_self.size() + 5]();
            tmp += 4;
            strcpy(tmp, _self._pstr);
            _self.decreaseRefCount();
            _self._pstr = tmp;
            _self.initRefCount();
        }
    }
    _self._pstr[_idx] = ch;
}

//read operation
std::ostream & operator<<(std::ostream & os, const String::charProxy & rhs)
{
    os << rhs._self._pstr[rhs._idx];
    return os;
}
 
std::ostream & operator<<(std::ostream & os, const String & rhs)
{
    os << rhs._pstr;
    return os;
}

int main()
{
    String s1;
    String s2(s1);
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s1's refcount = " << s1.refcount() << endl;
 
    String s3 = "hello,world";
    String s4(s3);
 
    cout << "s3 = " << s3 << endl;
    cout << "s4 = " << s4 << endl;
    cout << "s3's refcount = " << s3.refcount() << endl;
    printf("s3's address = %p\n", s3.c_str());
    printf("s4's address = %p\n", s4.c_str());
    cout << endl;
 
    String s5 = "hello,shenzheng";
    cout << "s5 = " << s5 << endl;
    s5 = s4;
    cout << endl;
    cout << "s5 = " << s5 << endl;
    cout << "s3 = " << s3 << endl;
    cout << "s4 = " << s4 << endl;
    cout << "s3's refcount = " << s3.refcount() << endl;
    printf("s5's address = %p\n", s5.c_str());
    printf("s3's address = %p\n", s3.c_str());
    printf("s4's address = %p\n", s4.c_str());
    cout << endl;
 
    cout << "执行写操作之后:" << endl;
    s5[0] = 'X';//char& --> 内置类型 
                //CharProxy cp = ch;
    cout << "s5 = " << s5 << endl;
    cout << "s3 = " << s3 << endl;
    cout << "s4 = " << s4 << endl;
    cout << "s5's refcount = " << s5.refcount() << endl;
    cout << "s3's refcount = " << s3.refcount() << endl;
    printf("s5's address = %p\n", s5.c_str());
    printf("s3's address = %p\n", s3.c_str());
    printf("s4's address = %p\n", s4.c_str());
    cout << endl;
 
    cout << "执行读操作: " << endl;
    cout << "s3[0] = " << s3[0] << endl;
    cout << "s5 = " << s5 << endl;
    cout << "s3 = " << s3 << endl;
    cout << "s4 = " << s4 << endl;
    cout << "s5's refcount = " << s5.refcount() << endl;
    cout << "s3's refcount = " << s3.refcount() << endl;
    printf("s5's address = %p\n", s5.c_str());
    printf("s3's address = %p\n", s3.c_str());
    printf("s4's address = %p\n", s4.c_str());
    cout << endl;
 
    const String s6("hello");
    cout << s6[0] << endl;
 
    return 0;
}