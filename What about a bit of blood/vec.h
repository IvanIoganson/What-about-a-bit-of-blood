#pragma once

template <class T = int>
class vec
{
public:
    T x, y;
    vec() : x(0), y(0) {};
    vec(T nx, T ny) : x(nx), y(ny) {};
    vec<T> operator+(const vec<T> &v) const { vec<T> res(x + v.x, y + v.y); return res; }
    bool operator==(const vec<T> &v) const { return x == v.x && y == v.y; }
    vec<T> operator-(const vec<T> &v) const { vec<T> res(x - v.x, y - v.y); return res; }
    vec<T> operator-(void) const { return vec<T>(-x, -y); }
    vec<T> &operator=(const vec<T> &v) { x = v.x, y = v.y;  return *this; }
    vec<T> &operator+=(const vec<T> &v) { x += v.x, y += v.y;  return *this; }
    vec<T> &operator-=(const vec<T> &v) { x -= v.x, y -= v.y;  return *this; }
    T operator!(void) const { return x*x + y*y; }
    T operator*(const vec<T> &v) const { return x * v.x + y * v.y; } //Скалярное произведение
    vec<T> operator*(const T a) const { vec<T> res(a*x, a*y); return res; } 
    vec<T> &operator*=(const T a) { x *= a, y *= a;  return *this; }
    /*vec<double> operator(vec<double>)(void) const { vec<double> t(x, y); return t; }
    vec<int> operator(vec<int>)(void) const { vec<int> t(x, y); return t; }*/
    void Normalize(void) { double len = sqrt(!*this); if (len == 0) { return; } x /= len, y /= len; }
    double operator%(const vec<T> &v) {return x * v.y - y * v.x; } //Векторное произведение
};

