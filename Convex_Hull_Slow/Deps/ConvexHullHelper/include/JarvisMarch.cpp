/**
	* File:    JarvisMarch.cpp
	*
	* Author1:  Akshay Tiwari (2018A7PS0201H)
	*
	* Author2:  Purvika  (2018A7PS0232H)
	*
	* Author3:  Sneh Lohia  (2018A7PS0171H)
	*
	* Date:     05-08-2020
	*
	* Course:   Computational Geometry (CS F402)
	*
	* Summary of File:
	*
	*   Program to find the convex hull of a point set using Jarvis March algorithm.
*/
#include <vector>
#include <set>
#include <iostream>
#include "Point.hpp"
#include "JarvisMarch.hpp"

using namespace std;
/**
	*	double getCos(Point p, Point q, Point r)
	*
	*   Summary of getCos(Point p, Point q, Point r) function:
	*
	*       Finds the cos product of two vectors defined by 3 points
	*
	*   Parameters  :
	*
	*		Point p : First point
	*		Point q : Second point
	*		Point r : Third point
	*
	*   Return Value :
	*
	*		Value of the cos product
	*
	*   Description :
	*
	*       Finds the cos product of vector joining p and q with vector joining r and q.
*/
double getCos(Point p, Point q, Point r)
{
	double ax = q.x - p.x;
	double ay = q.y - p.y;
	double bx = r.x - q.x;
	double by = r.y - q.y;
	return (ax * bx + by * ay) / sqrt((ax * ax + ay * ay) * (bx * bx + by * by));  //cos@ = A.B/|A||B|
}
/**
	*	bool isEqual(Point p, Point q)
	*
	*   Summary of isEqual(Point p, Point q) function:
	*
	*       Checks if two points are equal
	*
	*   Parameters  :
	*
	*		Point p : First point
	*		Point q : Second point
	*
	*   Return Value :
	*
	*		True if points are equal, flase otherwise.
	*
	*   Description :
	*
	*       Checks if two points are equal by comparing their x and y coordinate.
*/
bool isEqual(Point p, Point q)
{
	if (p.x == q.x && p.y == q.y)
	{
		return true;
	}
	return false;
}
/**
	*	void jarvisMarch(const set<pair<double, double> > vertices, vector<pair<double, double> >& convexHullVertices)
	*
	*   Summary of jarvisMarch(const set<pair<double, double> > vertices, vector<pair<double, double> >& convexHullVertices) function:
	*
	*       Finds the convex hull of input point set and stores it in the passed by reference vector
	*
	*   Parameters  :
	*
	*		const set<pair<double, double> > vertices : Input point set
	*		vector<pair<double, double> >& convexHullVertices : Output vector containing convex hull
	*
	*   Return Value :
	*
	*		None
	*
	*   Description :
	*
	*       Implementation of the jarvis march algorithm.
*/
void jarvisMarch(const set<pair<double, double> > vertices, vector<pair<double, double> >& convexHullVertices)
{
	vector<Point> points;
	for (auto point : vertices) {
		Point current;
		current.x = point.first;
		current.y = point.second;
		points.push_back(current);
	}
	if (points.size() <= 3)
	{
		for (auto point : vertices)
			convexHullVertices.push_back(point);
		return ;
	}

	vector<Point> convexHull;
	//getting point with minimum y-coordinate
	int minindex = 0;
	for (unsigned int i = 0;i < points.size();i++)
	{
		if (points[i].y < points[minindex].y)
		{
			minindex = i;
		}
	}

	convexHull.push_back(points[minindex]);//guaranteed to be in Convex Hull
	Point origin = Point(points[minindex].x - 2, points[minindex].y);//defining origin
	int s2 = 0;//index of second point
	double minAngle = 100;
	for (unsigned int i = 0;i < points.size();i++)
	{
		double cosine = getCos(origin, convexHull[0], points[i]);
		if (acos(cosine) < minAngle && i != minindex)
		{
			minAngle = acos(cosine);
			s2 = i;
		}
	}
	convexHull.push_back(points[s2]);


	while (!isEqual(convexHull[convexHull.size() - 1], convexHull[0]))
	{
		int pointOnHull = 0;
		minAngle = 100;
		for (unsigned int i = 0;i < points.size();i++)
		{
			double cosine = getCos(convexHull[convexHull.size() - 2], convexHull[convexHull.size() - 1], points[i]);
			if (acos(cosine) < minAngle)
			{
				minAngle = acos(cosine);
				pointOnHull = i;
			}
		}
		convexHull.push_back(points[pointOnHull]);

	}
	convexHull.erase(convexHull.begin() + convexHull.size() - 1);

	for (auto point : convexHull) {
		convexHullVertices.push_back(make_pair(point.x, point.y));
	}
	return;
}