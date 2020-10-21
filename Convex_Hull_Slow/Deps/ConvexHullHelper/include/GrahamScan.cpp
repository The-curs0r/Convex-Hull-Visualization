/**
	* File:    GrahamScan.cpp
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
	*   Program to find the convex hull of a point set using Graham Scan algorithm.
*/
#include <set>
#include <utility>
#include <algorithm>
#include <cmath>
#include <vector>
#include <stack>
#include "Point.hpp"

#include <iostream>

#define PI 3.141592///<PI

using namespace std;

Point referencePoint;///<Reference point for angle calculations

/**
	*   double findDistance(Point v1, Point v2)
	*
	*   Summary of findDistance(Point v1, Point v2) function:
	*
	*       Returns the squared distance between two points
	*
	*   Parameters  :
	*
	*		Point v1 : First Point
	*		Point v2 : Second Point
	*
	*   Return Value :
	*
	*		Square of distance between the two points
	*
	*   Description :
	*
	*       Returns the squared distance between two points
*/
double findDistance(Point v1, Point v2) {
	return ((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}
/**
	*   bool customSortByTheta(const Point& v1, const Point& v2)
	*
	*   Summary of customSortByTheta(const Point& v1, const Point& v2) function:
	*
	*       Compares two points with respect to angle
	*
	*   Parameters  :
	*
	*		Point v1 : First Point
	*		Point v2 : Second Point
	*
	*   Return Value :
	*
	*		True if first point makes smaller angle, flase otherwise.
	*
	*   Description :
	*
	*       Compares two points with respect to angle made by the line joining point to reference point and the poisitive x axis
*/
bool customSortByTheta(const Point& v1, const Point& v2)
{
	double v1Distance = findDistance(v1, referencePoint);
	double v2Distance = findDistance(v2, referencePoint);
	double v1Angle = acos((v1.x - referencePoint.x) / sqrt(v1Distance));
	double v2Angle = acos((v2.x - referencePoint.x) / sqrt(v2Distance));
	return v1Angle == v2Angle ? v1Distance < v2Distance : v1Angle < v2Angle;
}
/**
	*   Point secondPoint(stack<Point>& S)
	*
	*   Summary of secondPoint(stack<Point>& S) function:
	*
	*       Returns the second point in stack
	*
	*   Parameters  :
	*
	*		stack<Point>& S : Stack of points
	*
	*   Return Value :
	*
	*		Second point on the stack
	*
	*   Description :
	*
	*       Returns the second point in stack without modifying it
*/
Point secondPoint(stack<Point>& S)
{
	Point p = S.top();
	S.pop();
	Point res = S.top();
	S.push(p);
	return res;
}
/**
	*   int orientation(Point p, Point q, Point r)
	*
	*   Summary of orientation(Point p, Point q, Point r) function:
	*
	*       Checks if 3 point are in anti clockwise order or not
	*
	*   Parameters  :
	*
	*		Point p : First point 
	*		Point q : Second point
	*		Point r : Third point
	*
	*   Return Value :
	*
	*		True if points are in anticlockwise order, false otherwise.
	*
	*   Description :
	*
	*       Checks if 3 point are in anti clockwise order or not using sign of cross product
*/
int orientation(Point p, Point q, Point r)
{
	double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
	return (val > 0) ? 1 : 0; // Return true if ccw
}
/**
	*   void grahamScan(const set<pair<double, double> > vertices, vector<pair<double, double> >& convexHullVertices)
	*
	*   Summary of grahamScan(const set<pair<double, double> > vertices, vector<pair<double, double> >& convexHullVertices) function:
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
	*       Implementation of the graham scan algorithm using stack data structure.
*/
void grahamScan(const set<pair<double, double> > vertices, vector<pair<double, double> >& convexHullVertices) {
	if (vertices.size() == 0) return;
	else if (vertices.size() <= 3) {
		for (auto i : vertices)
			convexHullVertices.push_back(i);
		return;
	}

	vector<Point> inputVertices;
	auto it = vertices.begin();
	double minYCoord = it->second;
	int minIndex = 0, i = 0;
	Point first;
	first.x = it->first;
	first.y = it->second;
	inputVertices.push_back(first);
	it++, i++;
	while (it != vertices.end()) {
		Point current;
		current.x = it->first;
		current.y = it->second;
		inputVertices.push_back(current);
		if ((current.y < minYCoord) || (minYCoord == current.y) && current.x < inputVertices[minIndex].x) {
			minYCoord = current.y;
			minIndex = i;
		}
		it++, i++;
	}
	swap(inputVertices[0], inputVertices[minIndex]);

	referencePoint = inputVertices[0];
	auto st = inputVertices.begin();
	st++;
	sort(st, inputVertices.end(), customSortByTheta);

	stack<Point> currentConvexHull;
	currentConvexHull.push(inputVertices[0]);
	currentConvexHull.push(inputVertices[1]);
	currentConvexHull.push(inputVertices[2]);

	for (unsigned int i = 3; i < inputVertices.size(); i++)
	{
		while (orientation(secondPoint(currentConvexHull), currentConvexHull.top(), inputVertices[i]))
			currentConvexHull.pop();
		currentConvexHull.push(inputVertices[i]);
	}
	while (!currentConvexHull.empty())
	{
		Point p = currentConvexHull.top();
		convexHullVertices.push_back(make_pair(p.x, p.y));
		currentConvexHull.pop();
	}
	return;
}