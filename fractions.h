#include <iostream>
using namespace std;

namespace error {
    enum frac {DEN_EQ_0=0xF, INVALID_FRACTION_INPUT, INVALID_SEPARATOR, INVALID_USEPARATOR};
}

template <class T>
T egcd(T a, T b) { return b? egcd(b, a%b) : a;  }

template <class T>
class frac {

    T N, D, E;
    T GCD;

    string SEP, USEP;

public:

    bool UN, SIMPL;

    frac(): SEP("/"), USEP("/"), UN(0), SIMPL(1) {}
    frac(const T& n, const T& d = 1) : N(n), D(d), SEP("/"), USEP("/"), UN(0), SIMPL(1) {simplify();}
    frac(const frac& f) : N(f.N), D(f.D), SEP(f.SEP), USEP(f.USEP), UN(f.UN), SIMPL(f.SIMPL)  {simplify();}

    T n() const { return SIMPL? N : N*GCD; }
    T d() const { return SIMPL? D : D*GCD; }
    T gcd() const { return GCD; }

    string separator() {return SEP;}

    frac invert() { N ^= D; D ^= N; N ^= D; return *this; }
    double aprox() const { return N/D; }

    inline frac operator*(const T& k) const { return frac(N*k,D); }

    inline frac operator+(const frac& f) const { return frac(N*f.D+f.N*D, D*f.D); }
    inline frac operator-(const frac& f) const { return *this + (-1)*f; }
    inline frac operator*(const frac& f) const { return frac(N*f.N, D*f.D); }
    inline frac operator/(const frac& f) const { return frac(N*f.D, D*f.N); }

    inline frac operator+=(const frac& f) { *this = *this + f; return *this; }
    inline frac operator-=(const frac& f) { *this = *this - f; return *this; }
    inline frac operator*=(const frac& f) { *this = *this * f; return *this; }
    inline frac operator/=(const frac& f) { *this = *this / f; return *this; }

    inline bool operator==(const frac& f) const { return (N==f.N and D==f.N); }
    inline bool operator>=(const frac& f) const { return (N*f.D >= f.N*D); }
    inline bool operator<=(const frac& f) const { return (N*f.D <= f.N*D); }
    inline bool operator<(const frac& f) const { return (N*f.D > f.N*D); }
    inline bool operator>(const frac& f) const { return (N*f.D < f.N*D); }

    friend ostream& operator<< (ostream& out, const frac& f) {
        out << f.n() << f.SEP << f.d();
        return out;
    }
    friend istream& operator>> (istream& in, frac& f) {
        string s;
        in >> s;
        T n=0, d=0;
        uint i;
        bool neg = false;
        for (i = 0; i < s.size(); i++) {
            if (s[i]>='0' and s[i]<='9') n=n*10+(s[i]-'0');
            else if (s[i]=='-' and i == 0) neg = true;
            else break;
        }
        if (neg) n*= -1;
        neg = false;
        for (auto j = 0; j < f.SEP.size(); i++,j++ ) {
            if (i >= s.size()) throw error::INVALID_FRACTION_INPUT;
            if (f.SEP[j]!=s[i]) throw error::INVALID_FRACTION_INPUT;
        }
        for (auto j = i; j < s.size(); j++) {
            if (s[j]>='0' and s[j]<='9') d=d*10+(s[j]-'0');
            else if (s[j]=='-' and j==i) neg = true;
            else break;
        }
        if (neg) d*=-1;
        f.N = n;
        f.D = d;
        f.simplify();
        return in;
    }

    frac runfunc(T (*func)(T)) const { return frac <T> (func(n()), func(d())); }
    frac applyfunc(T (*func)(T)) { *this = runfunc(func); return *this; }

    void n(T n) {N = n; simplify();}
    void d(T d) {D = d; simplify();}
    void gcd(T gcd) {GCD = gcd; }

    void separator(string s) {
        for (auto i : s) if (i >='0' and i <='9') throw error::INVALID_SEPARATOR;
        SEP = s;
    }
    void useparator(string s) {
        for (auto i : s) if (i >='0' and i <='9') throw error::INVALID_SEPARATOR;
        USEP = s;
    }

    bool simplify() {
        if (N > 0 and  D < 0) {N*=-1; D*=-1;}
        else if ( N < 0 and D < 0 ) {N*=-1; D*=-1; }
        if (D == 0) throw error::DEN_EQ_0;
        GCD = egcd(N,D);
        N /= GCD;
        D /= GCD;
        return true;
    }
};

