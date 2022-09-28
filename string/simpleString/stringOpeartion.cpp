#include <iostream>
#include <assert.h>
#include <string.h>

class str {
public:
    //不考虑内存重叠
    char* strcpy(char* dest, char* src) {
        assert(dest != NULL);   //点1：检查参数
        assert(src != NULL);
        char * ret = dest;
        while (*src != '\0') {
            *(dest++) = *(src++);
        }
        *dest = '\0';   //点2：结尾补上\0
        return ret;
    }

    //考虑内存重叠
    char* strcpy2(char* dest, char* src) {
        char* ret = dest;
        assert(dest != NULL);
        assert(src != NULL);
        memmove(dest, src, strlen(src) + 1); //strlen不包括\0,所以+1
    }

    char* strcat(char* dest, const char* src) {
        //1.将目的字符串的起始位置先保存，需要返回其头指针
        //2.找到dest的结束位置，再把src拷贝到dest中，最后记得加'\0'
        char* ret = dest; 
        assert(dest != NULL);
        assert(src != NULL);
        while (*dest != '\0') 
            dest++;
        while (*src != '\0') 
            *(dest++) = *(src++);  //*和++同一运算符优先级，但这里是右结合
        *dest = '\0';
        return ret;

    }
};

int main() {
    str string;
    char* src = "abc";
    //char* dest = (char*)malloc(sizeof(char) * 10);
    char* dest = new char(4);
    dest[0] = '1';
    dest[1] = '2';
    //string.strcpy(dest, src);
    string.strcat(dest, src);
    std::cout<<dest;
}