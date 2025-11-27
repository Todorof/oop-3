#include <bits/stdc++.h>

using namespace std;

class MyVec {
    int size;
    int capacity;
    int *elements;

    void _copy(const MyVec& orig);

public:
    MyVec(int size = 0, int value = 0);
    ~MyVec();
    MyVec(const MyVec& orig);
    MyVec& operator=(const MyVec& orig);
    void print();
    void push(int val);
    void pop();
    int operator[](int i);
    MyVec operator+(const MyVec & other) const;
    void sort();
    int& front();
    int& back();
    int* data();
    void red_cap();
};

MyVec::MyVec(int size, int value) {
    int capacity = 4;
    while (size >= capacity) {
        capacity*=2;
    }
    elements = new int[capacity];
    this->capacity = capacity;
    this->size = size;
    for (int i=0; i<this->size;i++ ) {
        elements[i] = value;
    }
}
MyVec::~MyVec() {
    delete [] elements;
}

void MyVec::_copy(const MyVec& orig) {
    this->capacity = orig.capacity;
    this->size = orig.size;
    this->elements = new int[this->capacity];
    for (int i=0; i<this->size; i++) {
        this->elements[i] = orig.elements[i];
    }
}

MyVec::MyVec(const MyVec& orig) {
    _copy(orig);
}
MyVec& MyVec::operator=(const MyVec& orig) {
    if (this != &orig) {
        delete [] this->elements;
        _copy(orig);
    }
    return *this;
}

void MyVec::print() {
    cout<<"C: "<< this->capacity << " S: " << this->size <<":\n";
    for (int i=0; i<this->size; i++) {
        cout<<this->elements[i] << " ";
    }
    cout<<endl;
}

void MyVec::push(int val) {
    if (this->size == this->capacity) {
        int newcap = this->capacity*2;
        int *tmp = new int[newcap];
        for (int i=0; i<this->size; i++) {
            tmp[i] = this->elements[i];
        }
        delete [] elements;
        elements = tmp;
        this->capacity = newcap;
    }
    this->elements[this->size] = val;
    this->size++;
}

void MyVec::pop() {
    if (this->size > 0) {
        this->size--;
        red_cap();
    }
}

int MyVec::operator[](int i) {
    if (i<0 || i > this->size-1) {
        return 42;
    }
    return this->elements[i];
}

MyVec MyVec::operator+(const MyVec & other) const {
    MyVec result(this->size + other.size);
    result.size = this->size + other.size;
    for (int i = 0; i < this->size; i++) {
        result.elements[i] = this->elements[i];
    }
    for (int i = 0; i < other.size; i++) {
        result.elements[this->size + i] = other.elements[i];
    }
    return result;
}

void MyVec::sort() {
    for (int i = 0; i < size; i++)
        for (int j = i + 1; j < size; j++)
            if (elements[j] < elements[i])
                swap(elements[i], elements[j]);
}

int& MyVec::front() {
    return elements[0];
}

int& MyVec::back() {
    return elements[size - 1];
}

int* MyVec::data() {
    return elements;
}

void MyVec::red_cap() {
    if (size < capacity / 4 && capacity > 4) {
        int newcap = capacity / 2;
        int* tmp = new int[newcap];
        for (int i = 0; i < size; i++)
            tmp[i] = elements[i];
        delete[] elements;
        elements = tmp;
        capacity = newcap;
    }
}

int main() {
    MyVec v(3, 123);
    v.print();

    v.push(999);
    v.push(777);
    v.pop();
    v.push(777);
    v.push(333);
    v.push(122);
    v.push(10112554);
    cout<<endl;

    v.print();
    cout<<v[6] << "\n";
    cout<<endl;

    v.sort();
    v.print();
    cout<<endl;

    cout << "Front: " << v.front() <<endl;
    cout << "Back: " << v.back() <<endl;
    cout << "Data[0]: " << v.data()[0] <<endl;
    cout<<endl;

    v.pop();
    v.pop();
    v.print();
    cout<<endl;

    MyVec a(2, 7);
    MyVec b(2, 1);
    MyVec c = a + b;
    c.print();

    return 0;
}
