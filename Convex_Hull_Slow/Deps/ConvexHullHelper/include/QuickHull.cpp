/**
    * File:    QuickHull.cpp
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
    *   Program to find the convex hull of a point set using Quick Hull algorithm.
*/
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <map>
#include "Point.hpp"
#include "QuickHull.hpp"


using namespace std;
/**
    *   Summary of double antiClockwise(const Point& a, const Point& b, const Point& c)
    *
    *       Returns signed cross product of segments ac and bc
    *
    *   Parameters  :
    *
    *		Point a : Starting point of directed line ab
    *		Point b : Endpoint of line ab
    *       Point c : Point for which status is to be found
    *
    *   Return Value :
    *
    *		double result : negative if lies to left of ab, positive if lies to right of ab, zero if collinear
    *
    *   Description :
    *
    *       Checks whether the cross product of vector ab and vector ac is positive or not.
*/
double antiClockwise(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}
/**
    *   Summary of bool isToTheLeft(const Point& a, const Point& b) 
    *
    *       Checks wether point a is to the left of point b
    *
    *   Parameters  :
    *
    *		Point a : Point to be checked
    *		Point b : Point to be checked against
    *
    *   Return Value :
    *
    *		bool result : true if a lies to left of b, false otherwise
    *
    *   Description :
    *
    *       Checks wether point a is to the left of point b, if same x coordinate then checks which one lies lower.
*/
bool isToTheleft(const Point& a, const Point& b) {
    return (a.x < b.x || (a.x == b.x && a.y < b.y));
}
/**
    *   Summary of double distance(const Point& a, const Point& b, const Point& p)
    *
    *       Returns the absolute distance of p from line segment ab
    *
    *   Parameters  :
    *
    *		Point a : Left endpoint of segment
    *		Point b : Right endpoint of segment
    *       Point p : Input point
    *
    *   Return Value :
    *
    *		 absolute distance of p from line segment ab
    *
    *   Description :
    *
    *       Returns the absolute distance of p from line segment ab using cross product
*/
double distance(const Point& a, const Point& b, const Point& p) {
    return fabs((b.x - a.x) * (a.y - p.y) - (b.y - a.y) * (a.x - p.x)) / sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}
/**
    *   Summary of int getFarthest(const Point& a, const Point& b, const vector<Point>& v)
    *
    *       Gives index of farthest point from the line segment ab
    *
    *   Parameters  :
    *
    *		Point a : Left endpoint of segment
    *		Point b : Right endpoint of segment
    *
    *   Return Value :
    *
    *		returns index of farthest point from the line segment ab
    *
    *   Description :
    *
    *       Gives index of farthest point from the line segment ab by comparing area of each triangle
*/
int getFarthest(const Point& a, const Point& b, const vector<Point>& v) {
    int farthestIndex = 0;
    double farthest = distance(a, b, v[farthestIndex]);

    for (unsigned int i = 1; i < v.size(); ++i) {
        double distCurr = distance(a, b, v[i]);
        if (distCurr > farthest) {
            farthestIndex = i;
            farthest = distCurr;
        }
    }
    return farthestIndex;
}
/**
    *   Summary of bool notEqual(const Point& a, const Point& b)
    *
    *       Checks wether point a is not equal to point b
    *
    *   Parameters  :
    *
    *		Point a : Point to be checked
    *		Point b : Point to be checked against
    *
    *   Return Value :
    *
    *		bool result : true if a is not equal b, false otherwise
    *
    *   Description :
    *
    *       Checks wether point a is not equal to point b by comparing x and y values of both
*/
bool notEqual(const Point& a, const Point& b) {
    return (a.x != b.x) || (a.y != b.y);
}
/**
    *   Summary of getHull(vector<Point> pointSet, vector<Point>& H, Point& P, Point& Q) function:
    *
    *       Uses quickHull algorithm to find the points on convex hull and store in vector H
    *
    *   Parameters  :
    *
    *       vector<Point> pointSet : set of points under discussion
    *       vector<Point>& H : set of points on the convex hull
    *       Point& P : start point of baseline pq
    *       Point& Q :  endpoint of baseline pq
    *
    *   Return Value :
    *
    *       None
    *
    *   Description :
    *
    *       Implementation of the quick hull algorithm.
*/

void getHull(const vector<Point>& pointSet, vector<Point>& hull, const Point& leftEndpoint, const Point& rightEndpoint
    ) {
    if (pointSet.empty()) {
        return;
    }
    Point farthestPoint = pointSet[getFarthest(leftEndpoint, rightEndpoint, pointSet)];
    vector<Point> left;
    for (auto p : pointSet) {
        if (notEqual(farthestPoint,p) && notEqual(leftEndpoint, p) && antiClockwise(leftEndpoint, farthestPoint, p) >= 0) {
            left.push_back(p);
        }
    }
    getHull(left,hull, leftEndpoint, farthestPoint);
    hull.push_back(farthestPoint);
    vector<Point> right;
    for (auto p : pointSet) {
        if (antiClockwise(farthestPoint, rightEndpoint, p) > 0) {
            right.push_back(p);
        }
    } 
    getHull(right, hull, farthestPoint, rightEndpoint);
}

/**
    *   Summary of quickHull(const std::set<std::pair<double, double> > vertices, std::vector<std::pair<double, double> >& convexHullVertices) function:
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
    *       Implementation of the quick hull algorithm.
*/
void quickHull(const std::set<std::pair<double, double> > vertices, std::vector<std::pair<double, double> >& convexHullVertices)
{
    vector<Point> points;
    for (auto point : vertices) {
        Point current = Point(point.first, point.second);
        points.push_back(current);
    }

    if (points.size() <= 3)
    {   
        for (auto point : vertices) {
            convexHullVertices.push_back(point);
        }
        return;
    }
    vector<Point> convexHull;

    Point leftmost = *min_element(points.begin(), points.end(), isToTheleft);
    Point rightmost = *max_element(points.begin(), points.end(), isToTheleft);

    vector<Point> left, right;
    for (auto p : points) {
        antiClockwise(leftmost, rightmost, p) > 0 ? left.push_back(p) : right.push_back(p);
    }

    convexHull.push_back(leftmost);

    getHull(left, convexHull, leftmost, rightmost);

    convexHull.push_back(rightmost);

    getHull(right, convexHull ,rightmost, leftmost);

    map<std::pair<double, double>, int> hashMap;
    std::vector<std::pair<double, double> > hullVertices;

    for (auto point : convexHull) {
        hullVertices.push_back(make_pair(point.x, point.y));
        hashMap[hullVertices[hullVertices.size()-1]] = 0;
    }
    for (auto point : hullVertices) {
        if (hashMap[point] != 0)
            continue;
        convexHullVertices.push_back(make_pair(point.first, point.second));
        hashMap[point]++;
    }
    return;
}