/**
	* File:    ConvexHull.cpp
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
	*   Program containing calls to convex hull generating algorithms.
*/
#include <set>
#include <utility>
#include <algorithm>
#include <cmath>
#include <vector>
#include <stack>
#include <chrono>
#include "Point.hpp"
#include "GrahamScan.hpp"
#include "JarvisMarch.hpp"
#include "QuickHull.hpp"
#include "AndrewMonotone.hpp"

#include <iostream>

#define PI 3.141592 ///<Value of PI i.e. 3.141592

using namespace std;

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
vector<pair<double, double> > findConvexHullGraham(vector<pair<double, double> > inputPoints) {
	set<pair<double, double> > points;
	vector<pair<double, double> > convexHullPoints;
	for (auto point : inputPoints) {
		points.insert(point);
	}

	auto start = std::chrono::high_resolution_clock::now();
	grahamScan(points, convexHullPoints);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	cout << "Return from function call in : " << duration.count() << " microseconds for Graham Scan\n";

	vector<pair<double, double> > outputHull(convexHullPoints.begin(),convexHullPoints.end());
	return outputHull;
}
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
vector<pair<double, double> > findConvexHullGraham(set<pair<double, double> > inputPoints) {
	vector<pair<double, double> > convexHullPoints;

	auto start = std::chrono::high_resolution_clock::now();
	grahamScan(inputPoints, convexHullPoints);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	cout << "Return from function call in : " << duration.count() << " microseconds for Graham Scan\n";

	vector<pair<double, double> > outputHull(convexHullPoints.begin(), convexHullPoints.end());
	return outputHull;
}
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
vector<pair<double, double> > findConvexHullJarvis(vector<pair<double, double> > inputPoints) {
	set<pair<double, double> > points;
	vector<pair<double, double> > convexHullPoints;
	for (auto point : inputPoints) {
		points.insert(point);
	}

	auto start = std::chrono::high_resolution_clock::now();
	jarvisMarch(points, convexHullPoints);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	cout << "Return from function call in : " << duration.count() << " microseconds for Jarvis March\n";

	vector<pair<double, double> > outputHull(convexHullPoints.begin(), convexHullPoints.end());
	return outputHull;
}
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
vector<pair<double, double> > findConvexHullJarvis(set<pair<double, double> > inputPoints) {
	vector<pair<double, double> > convexHullPoints;

	auto start = std::chrono::high_resolution_clock::now();
	jarvisMarch(inputPoints, convexHullPoints);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	cout << "Return from function call in : " << duration.count() << " microseconds for Jarvis March\n";

	vector<pair<double, double> > outputHull(convexHullPoints.begin(), convexHullPoints.end());
	return outputHull;
}
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
vector<pair<double, double> > findConvexHullQuickHull(vector<pair<double, double> > inputPoints) {
	set<pair<double, double> > points;
	vector<pair<double, double> > convexHullPoints;
	for (auto point : inputPoints) {
		points.insert(point);
	}

	auto start = std::chrono::high_resolution_clock::now();
	quickHull(points, convexHullPoints);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	cout << "Return from function call in : " << duration.count() << " microseconds for Quick Hull\n";

	vector<pair<double, double> > outputHull(convexHullPoints.begin(), convexHullPoints.end());
	return outputHull;
}
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
vector<pair<double, double> > findConvexHullQuickHull(set<pair<double, double> > inputPoints) {
	vector<pair<double, double> > convexHullPoints;

	auto start = std::chrono::high_resolution_clock::now();
	quickHull(inputPoints, convexHullPoints);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	cout << "Return from function call in : " << duration.count() << " microseconds for Quick Hull\n";

	vector<pair<double, double> > outputHull(convexHullPoints.begin(), convexHullPoints.end());
	return outputHull;
}


std::vector<std::pair<double, double> > findConvexHullAndrewMonotone(std::set<std::pair<double, double> > inputPoints)
{
	vector<pair<double, double> > convexHullPoints;

	auto start = std::chrono::high_resolution_clock::now();
	andrewMonotone(inputPoints, convexHullPoints);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	cout << "Return from function call in : " << duration.count() << " microseconds for Andrew's Algorithm\n";

	vector<pair<double, double> > outputHull(convexHullPoints.begin(), convexHullPoints.end());
	return outputHull;
}
vector<pair<double, double> > findConvexHullAndrewMonotone(vector<pair<double, double> > inputPoints) {
	set<pair<double, double> > points;
	vector<pair<double, double> > convexHullPoints;
	for (auto point : inputPoints) {
		points.insert(point);
	}

	auto start = std::chrono::high_resolution_clock::now();
	andrewMonotone(points, convexHullPoints);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	cout << "Return from function call in : " << duration.count() << " microseconds for Andrew's Algorithm\n";

	vector<pair<double, double> > outputHull(convexHullPoints.begin(), convexHullPoints.end());
	return outputHull;
}