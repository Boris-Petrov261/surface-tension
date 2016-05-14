#ifndef ST_LIBRARY_H
#define ST_LIBRARY_H
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include "bitmap\\bitmap_image.hpp"
using namespace std;
class Point
{
public:
	double x, y, fi;
	Point(double _x = 0, double _y = 0, double _fi = 0) : x(_x), y(_y), fi(_fi){}
	void print()
	{
		cout << x << "  " << y << endl;
	}
	friend istream& operator>>(istream& in, Point& p)
	{
	    return in >> p.x >> p.y;
	}
	friend ostream& operator<<(ostream& out, const Point& p)
	{
	    return out << "  " << p.x << "  " << p.y << endl;
	}
};

class Vector2D
{
public:
	double x, y;
	Vector2D(double _x = 0, double _y = 0) : x(_x), y(_y){}
	Vector2D operator-(const Vector2D& v) const { return Vector2D(x - v.x, y - v.y); }
	Vector2D operator*(double scalar) const { return Vector2D(scalar*x, scalar*y); }

    double magnitude() const { return sqrt(x*x + y*y);}
    Vector2D normalizedVector() const { return (*this)*(1/magnitude());}
	friend Vector2D operator*(double scalar, Vector2D v) { return Vector2D(scalar*v.x, scalar*v.y); }
	friend ostream& operator<<(ostream& out, const Vector2D& v){ return out << v.x << " " << v.y << endl; }
};

enum class drop_type
{
    PENDANT,
    ROTATED
};

double dist(Point, Point);
double dist(vector<Point> experimentalPts, Point p);
double errorFunct(vector<Point> experimentalPts, vector<Point> theoreticalPts);
vector<Point> createTheoreticalModel(Vector2D params, drop_type dropType, Vector2D dimLimits);
Vector2D findInitialValues(vector<Point> experimentalPts, drop_type dropType);
Vector2D calcGradient(vector<Point> experimentalPts, Vector2D params, drop_type dropType, Vector2D dimLimits);
Vector2D preciseError(Vector2D params, vector<Point> experimentalPts, drop_type dropType, Vector2D dimLimits);
Vector2D identifyParams(string fileInput, string fileOutput, drop_type dropType);

void drawToBMP(vector<Point> & , string);

void inverseGraph(vector<Point> & points, Point & reversePoint);
void reverseInverseGraph(vector<Point> & points, Point & reversePoint);

#endif // ST_LIBRARY_H
