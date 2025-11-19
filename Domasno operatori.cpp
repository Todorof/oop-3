#include<iostream>
using namespace std;

// +, -, *, /, ==, <, >
class Frac {
    int n;
    int d;
public:
    int N () const { return this->n; }
    int D () const { return this->d; }
    void setN(int x) {this->n = x; }
    void setD(int x) {
        if (x==0) {
            throw invalid_argument("GRESKA, NEMOZE 0");
        }
        this->d = x;
    }

    Frac(const int n, const int d=1) {
        this->n = n;
        this->d = d;
    }

    // Copy C-tor
    Frac (const Frac& orig) {
        this->n = orig.n;
        this->d = orig.d;
    }

    Frac& operator=(const Frac& orig) {
        if (this!=&orig) { // No Self-assignment
            this->n = orig.n;
            this->d = orig.d;
        }
        return *this;
    }





    void print() {
        cout<<this->n <<"/" << this->d;
    }

    Frac operator*(const Frac& b) const {
        Frac c(*this);
        c.setN(this->N() * b.N());
        c.setD(this->D() * b.D());
        return c;
    }

    Frac operator+(const Frac& b) const{
        Frac c(*this);
        if (this->D()==b.D()) {
            c.setD(this->D());
            c.setN(this->N()+b.N());
        }else {
            c.setN(this->N()*b.D() + this->D()*b.N());
            c.setD(this->D()*b.D());
        }
        int a=c.N(), d=c.D();
        while (d != 0) {
            int temp = d;
            d = a % d;
            a = temp;
        }
        c.setD(c.D()/a);
        c.setN(c.N()/a);
        return c;
    }
    Frac operator-(const Frac& b) const{
        Frac c(*this);
        if (this->D()==b.D()) {
            c.setD(this->D());
            c.setN(this->N()-b.N());
        }else {
            c.setN(this->N()*b.D() - this->D()*b.N());
            c.setD(this->D()*b.D());
        }
        int a=c.N(), d=c.D();
        while (d != 0) {
            int temp = d;
            d = a % d;
            a = temp;
        }
        c.setD(c.D()/a);
        c.setN(c.N()/a);
        return c;
    }

    Frac operator/(const Frac& b) const {
        Frac c(*this);
        c.setN(this->N()*b.D());
        c.setD(this->D()*b.N());
        int a=c.N(), d=c.D();
        while (d != 0) {
            int temp = d;
            d = a % d;
            a = temp;
        }
        c.setD(c.D()/a);
        c.setN(c.N()/a);
        return c;
    }

    bool operator==(const Frac& b) const {
        Frac c(*this);
        Frac d(b);
        c.setN(this->N()*b.D());
        d.setN(this->D()*b.N());
        return c.N()==d.N();
    }
    bool operator>(const Frac& b) const {
        Frac c(*this);
        Frac d(b);
        c.setN(this->N()*b.D());
        d.setN(this->D()*b.N());
        return c.N()>d.N();
    }
    bool operator<(const Frac& b) const {
        Frac c(*this);
        Frac d(b);
        c.setN(this->N()*b.D());
        d.setN(this->D()*b.N());
        return c.N()<d.N();
    }

    ~Frac() {}

};

ostream& operator<<(ostream& o, const Frac& f) {
    o<<f.N() <<"/" << f.D();
    return o;
}

// Frac operator*(const Frac& a, const Frac& b) {
//     Frac c(a);
//     c.setN(a.N() * b.N());
//     c.setD(a.D() * b.D());
//     return c;
// }

int main() {
    Frac a(7,8);
    Frac b(9,12);
    Frac c(1);

    // Global function operator* (Frac, Frac)
    // Member Function operator* (Frac)
    // a * b
    // operator*(a,b)
    // a.operator*(b)

    //.c.print();

    cout<<(a+b)<<endl;
    cout<<(a-b)<<endl;
    cout<<(a/b)<<endl;
    if (a==b) {
        cout<<"EDNAKVI"<<endl;
    }else cout<<"NEEDNAKVI"<<endl;
    cout<<(a>b)<<endl;
    cout<<(a<b)<<endl;
}