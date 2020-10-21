#ifndef QUICKHULL_HPP
#define QUICKHULL_HPP
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

#endif // !QUICKHULL_HPP