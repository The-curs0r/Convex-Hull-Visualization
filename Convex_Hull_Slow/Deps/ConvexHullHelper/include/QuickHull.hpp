#ifndef QUICKHULL_HPP
#define QUICKHULL_HPP

#include <vector>
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
void quickHull(const std::set<std::pair<double, double> > vertices, std::vector<std::pair<double, double> >& convexHullVertices);

bool notEqual(const Point& a, const Point& b);
int getFarthest(const Point& a, const Point& b, const std::vector<Point>& v);
double distance(const Point& a, const Point& b, const Point& p);
bool isToTheleft(const Point& a, const Point& b);
double antiClockwise(const Point& a, const Point& b, const Point& c);

#endif // !QUICKHULL_HPP