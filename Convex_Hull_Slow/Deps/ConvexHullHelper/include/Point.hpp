/**
	* File:    Point.hpp
	*
	* Author1:  Akshay Tiwari (2018A7PS0201H)
	* Author2:  Purvika  (2018A7PS0232H)
	* Author3:  Sneh Lohia  (2018A7PS0171H)
	* Date:     05-08-2020
	* Course:   Computational Geometry (CS F402)
	*
	* Summary of File:
	*
	*   Header file containing Point structure with members and constructors.
*/
#ifndef POINT_HPP
#define POINT_HPP

struct Point {
	double x;///<x Coordinate of the point
	double y;///<y Coordinate of the point
	/**
*   Point()
*
*   Summary of Point() constructor:
*
*       Default constructor for Point struct
*
*   Parameters  :
*
*		None
*
*   Description :
*
*       Creates a new point and initializes both the coordinates to 0.
*/
	Point() {
		x = 0; 
		y = 0;
	}
	/**
*   Point(double x, double y)
*
*   Summary of Point() constructor:
*
*       Default constructor for Point struct
*
*   Parameters  :
*
*		double x - x Coordinate of the new point
*		double y - y Coordinate of the new point
*
*   Description :
*
*       Creates a new point and initializes the coordinates to the given arguments.
*/
	Point(double x, double y)
	{
		this->x = x;
		this->y = y;
	}
	struct Point& operator+=(const Point& rhs) { x += rhs.x; y += rhs.y; return *this; }
	struct Point& operator+=(const double& k) { x += k; y += k; return *this; }
	struct Point& operator-=(const Point& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
	struct Point& operator-=(const double& k) { x -= k; y -= k; return *this; }
	bool operator==(const Point& rhs) const { return (x == rhs.x && y == rhs.y); }
	double cross(const Point& rhs) { return (x * rhs.y - y * rhs.x); }
};
inline Point operator+(Point lhs, const Point& rhs) { return lhs += rhs; }
inline Point operator+(Point lhs, const double k) { return lhs += k; }
inline Point operator+(const double k, Point rhs) { return rhs += k; }
inline Point operator-(Point lhs, const Point& rhs) { return lhs -= rhs; }
inline Point operator-(Point lhs, const double k) { return lhs -= k; }
inline Point operator-(const double k, Point rhs) { return rhs -= k; }
#endif // !POINT_HPP
