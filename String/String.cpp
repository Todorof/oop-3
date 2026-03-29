#include <iostream>
#include <cstring>
#include <cstdint>
using namespace std;

class String {
private:
    char* content;
    uint32_t size;

    void copy(const String& other) {
        size = other.size;
        content = new char[size + 1];
        strcpy(content, other.content);
    }

public:
    String(const char* str = "") {
        size = strlen(str);
        content = new char[size + 1];
        strcpy(content, str);
    }

    String(const String& other) {
        copy(other);
    }

    String& operator=(const String& other) {
        if (this != &other) {
            delete[] content;
            copy(other);
        }
        return *this;
    }

    uint32_t length() const {
        return size;
    }

    String operator+(const String& other) const {
        uint32_t newSize = size + other.size;
        char* temp = new char[newSize + 1];

        strcpy(temp, content);
        strcat(temp, other.content);

        String result(temp);
        delete[] temp;

        return result;
    }

    String& operator+=(const String& other) {
        uint32_t newSize = size + other.size;
        char* temp = new char[newSize + 1];

        strcpy(temp, content);
        strcat(temp, other.content);

        delete[] content;
        content = temp;
        size = newSize;

        return *this;
    }

    friend ostream& operator<<(ostream& os, const String& str);
    friend istream& operator>>(istream& is, String& str);

    ~String() {
        delete[] content;
    }
};

ostream& operator<<(ostream& os, const String& str) {
    os << str.content;
    return os;
}

istream& operator>>(istream& is, String& str) {
    char buffer[10000];
    is.getline(buffer, 10000);

    delete[] str.content;
    str.size = strlen(buffer);
    str.content = new char[str.size + 1];
    strcpy(str.content, buffer);

    return is;
}

int main() {
    String s1, s2 = "Hello";

    cout << "Enter a string: ";
    cin >> s1;

    cout << "The strings are: " << s1 << " , " << s2 << endl;

    cout << "length: " << s2.length()
         << "; operator+: " << (s1 + s2)
         << "; operator+=: " << (s2 += s1)
         << endl;

    return 0;
}