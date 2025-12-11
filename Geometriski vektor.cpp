#include <iostream>
#include <cmath>
using namespace std;

class vec3 {
    float x, y, z;

public:
    vec3(const float &x = 0.0f, const float &y = 0.0f, const float &z = 0.0f) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    vec3(const vec3 &v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    float X() const { return x; }
    float Y() const { return y; }
    float Z() const { return z; }

    void setx(const float &x) { this->x = x; }
    void sety(const float &y) { this->y = y; }
    void setz(const float &z) { this->z = z; }

    ~vec3() {}

    friend istream& operator>>(istream &i, vec3 &v);

    vec3& operator=(const vec3 &v) {
        this->x = v.X();
        this->y = v.Y();
        this->z = v.Z();
        return *this;
    }

    vec3 operator+(vec3 &v) const {
        vec3 r;
        r.setx(this->x + v.X());
        r.sety(this->y + v.Y());
        r.setz(this->z + v.Z());
        return r;
    }

    vec3 operator-(vec3 &v) const {
        vec3 r;
        r.setx(this->x - v.X());
        r.sety(this->y - v.Y());
        r.setz(this->z - v.Z());
        return r;
    }

    vec3 operator*(float k) const {
        vec3 r;
        r.setx(x * k);
        r.sety(y * k);
        r.setz(z * k);
        return r;
    }

    float operator~() const {
        float r = sqrt(x * x + y * y + z * z);
        return r;
    }

    vec3 operator++(int) {      // post
        vec3 temp(*this);
        this->x++;
        this->y++;
        this->z++;
        return temp;
    }

    vec3 operator++() {         // pre
        this->x++;
        this->y++;
        this->z++;
        return *this;
    }

    vec3 operator--(int) {      // post
        vec3 temp(*this);
        this->x--;
        this->y--;
        this->z--;
        return temp;
    }

    vec3 operator--() {         // pre
        this->x--;
        this->y--;
        this->z--;
        return *this;
    }

    bool operator==(const vec3 &v) {
        return sqrt(x*x + y*y + z*z) ==
               sqrt(v.X()*v.X() + v.Y()*v.Y() + v.Z()*v.Z());
    }

    bool operator>=(const vec3 &v) {
        return sqrt(x*x + y*y + z*z) >=
               sqrt(v.X()*v.X() + v.Y()*v.Y() + v.Z()*v.Z());
    }

    bool operator<=(const vec3 &v) {
        return sqrt(x*x + y*y + z*z) <=
               sqrt(v.X()*v.X() + v.Y()*v.Y() + v.Z()*v.Z());
    }

    bool operator!=(const vec3 &v) {
        return sqrt(x*x + y*y + z*z) !=
               sqrt(v.X()*v.X() + v.Y()*v.Y() + v.Z()*v.Z());
    }

    bool operator>(const vec3 &v) {
        return sqrt(x*x + y*y + z*z) >
               sqrt(v.X()*v.X() + v.Y()*v.Y() + v.Z()*v.Z());
    }

    bool operator<(const vec3 &v) {
        return sqrt(x*x + y*y + z*z) <
               sqrt(v.X()*v.X() + v.Y()*v.Y() + v.Z()*v.Z());
    }
};

ostream& operator<<(ostream &o, const vec3 &v) {
    o << "(" << v.X() << "," << v.Y() << "," << v.Z() << ")";
    return o;
}

istream& operator>>(istream &i, vec3 &v) {
    i >> v.x >> v.y >> v.z;
    return i;
}

int main() {
    vec3 a(1, 2, 3);
    vec3 b(4, 5, 6);

    cout << a << " " << b << endl;
    cout << a + b << endl;
    cout << b - a << endl;

    vec3 c;
    c = a;
    cout << c << endl;

    cout << a * 4 << endl;

    cout << ~a << endl;
    cout << ~b << endl;

    cout << (a == b) << endl;
    cout << (a != b) << endl;
    cout << (a < b) << endl;
    cout << (a > b) << endl;
    cout << (a <= b) << endl;
    cout << (a >= b) << endl;

    cout << a++ << endl;
    cout << a-- << endl;
    cout << ++a << endl;
    cout << --a << endl;
}
