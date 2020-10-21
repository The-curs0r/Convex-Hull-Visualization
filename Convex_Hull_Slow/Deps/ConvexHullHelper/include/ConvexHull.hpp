#ifndef CONVEXHULL_HPP
#define CONVEXHULL_HPP

#include <vector>
#include <algorithm>
#include <set>
#include <utility>
/**
	*   vector<pair<double, double> > findConvexHullGraham(vector<pair<double, double> > inputPoints)
	*
	*   Summary of findConvexHullGraham(vector<pair<double, double> > inputPoints) function:
	*
	*       Calls the grahamScan function to generate the convex hull
	*
	*   Parameters  :
	*
	*		vector<pair<double, double> > inputPoints : Input point vector array
	*
	*   Return Value :
	*
	*		Vector containing the convex hull
	*
	*   Description :
	*
	*       Removes duplicates (if any) in the input point vector and then calls the grahamScan function to generate the convex hull
*/
std::vector<std::pair<double, double> > findConvexHullGraham(std::vector<std::pair<double, double> > inputPoints);
/**
	*   vector<pair<double, double> > findConvexHullGraham(set<pair<double, double> > inputPoints)
	*
	*   Summary of findConvexHullGraham(set<pair<double, double> > inputPoints) function:
	*
	*       Calls the grahamScan function to generate the convex hull
	*
	*   Parameters  :
	*
	*		set<pair<double, double> > inputPoints : Input point set
	*
	*   Return Value :
	*
	*		Vector containing the convex hull
	*
	*   Description :
	*
	*       Calls the grahamScan function to generate the convex hull
*/
std::vector<std::pair<double, double> > findConvexHullGraham(std::set<std::pair<double, double> > inputPoints);
/**
	*   vector<pair<double, double> > findConvexHullJarvis(vector<pair<double, double> > inputPoints)
	*
	*   Summary of findConvexHullJarvis(vector<pair<double, double> > inputPoints) function:
	*
	*       Calls the jarvisMarch function to generate the convex hull
	*
	*   Parameters  :
	*
	*		vector<pair<double, double> > inputPoints : Input point vector array
	*
	*   Return Value :
	*
	*		Vector containing the convex hull
	*
	*   Description :
	*
	*       Removes duplicates (if any) in the input point vector and then calls the jarvisMarch function to generate the convex hull
*/
std::vector<std::pair<double, double> > findConvexHullJarvis(std::vector<std::pair<double, double> > inputPoints);
/**
	*   vector<pair<double, double> > findConvexHullJarvis(set<pair<double, double> > inputPoints)
	*
	*   Summary of findConvexHullJarvis(set<pair<double, double> > inputPoints) function:
	*
	*       Calls the jarvisMarch function to generate the convex hull
	*
	*   Parameters  :
	*
	*		set<pair<double, double> > inputPoints : Input point set
	*
	*   Return Value :
	*
	*		Vector containing the convex hull
	*
	*   Description :
	*
	*       Calls the jarvisMarch function to generate the convex hull
*/
std::vector<std::pair<double, double> > findConvexHullJarvis(std::set<std::pair<double, double> > inputPoints);
/**
	*   vector<pair<double, double> > findConvexHullQuickHull(vector<pair<double, double> > inputPoints)
	*
	*   Summary of findConvexHullQuickHull(vector<pair<double, double> > inputPoints) function:
	*
	*       Calls the quickHull function to generate the convex hull
	*
	*   Parameters  :
	*
	*		vector<pair<double, double> > inputPoints : Input point vector array
	*
	*   Return Value :
	*
	*		Vector containing the convex hull
	*
	*   Description :
	*
	*       Removes duplicates (if any) in the input point vector and then calls the quickHull function to generate the convex hull
*/
std::vector<std::pair<double, double> > findConvexHullQuickHull(std::vector<std::pair<double, double> > inputPoints);
/**
	*   vector<pair<double, double> > findConvexHullQuickHull(set<pair<double, double> > inputPoints)
	*
	*   Summary of findConvexHullQuickHull(set<pair<double, double> > inputPoints) function:
	*
	*       Calls the quickHull function to generate the convex hull
	*
	*   Parameters  :
	*
	*		set<pair<double, double> > inputPoints : Input point set
	*
	*   Return Value :
	*
	*		Vector containing the convex hull
	*
	*   Description :
	*
	*       Calls the quickHull function to generate the convex hull
*/
std::vector<std::pair<double, double> > findConvexHullQuickHull(std::set<std::pair<double, double> > inputPoints);

#endif // !CONVEXHULL_HPP
