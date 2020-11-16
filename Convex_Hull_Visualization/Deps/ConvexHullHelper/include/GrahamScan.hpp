#ifndef GRAHAMSCAN_HPP
#define GRAHAMSCAN_HPP

#include <stack>
/**
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
void grahamScan(const std::set<std::pair<double, double> > vertices, std::vector<std::pair<double, double> >& convexHullVertices);

double findDistance(Point v1, Point v2);
bool customSortByTheta(const Point& v1, const Point& v2);
Point secondPoint(std::stack<Point>& S);
int orientation(Point p, Point q, Point r);
void setReferencePoint(Point reference);

#endif // !GRAHAMSCAN_HPP