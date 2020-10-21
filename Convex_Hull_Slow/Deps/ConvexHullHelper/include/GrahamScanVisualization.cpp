#include <set>
#include <utility>
#include <algorithm>
#include <cmath>
#include <vector>
#include <stack>
#include <chrono>
#include "Point.hpp"
#include "GrahamScan.hpp"
#include <iostream>

#define PI 3.141592

std::vector<Point> inputVerticesGraham;
std::stack<Point> currentHull;
int doneGraham = 0;

std::pair<double, double> getCurrentHullPointGraham() {
	if (doneGraham)
		return std::make_pair(NULL, NULL);
	else
		return std::make_pair(currentHull.top().x, currentHull.top().y);
}

std::vector<std::pair<double, double> > getCurrentCheckGraham() {

	std::vector<std::pair<double, double> > check;
	if (inputVerticesGraham.size() >= 1 && !currentHull.empty() && !doneGraham)
	{
		check.push_back(std::make_pair(currentHull.top().x, currentHull.top().y));
		check.push_back(std::make_pair(inputVerticesGraham[0].x, inputVerticesGraham[0].y));
	}
	return check;
}

std::vector<std::pair<double, double> > getCurrentHullGraham() {
	std::vector<std::pair<double, double> > hull;
	std::stack<Point> cpy = currentHull;
	while (!cpy.empty()) {
		hull.push_back(std::make_pair(cpy.top().x, cpy.top().y));
		cpy.pop();
	}
	return hull;
}

void loadPointsGraham(std::set<std::pair<double, double> > inputPoints) {
	inputVerticesGraham.clear();
	while (!currentHull.empty())
		currentHull.pop();
	doneGraham = 0;

	double minYCoord = LLONG_MAX;
	int minIndex = -1, i=0;
	for (auto point : inputPoints)
	{
		Point current;
		current.x = point.first;
		current.y = point.second;
		inputVerticesGraham.push_back(current);

		if ((current.y < minYCoord) || (minYCoord == current.y) && current.x < inputVerticesGraham[minIndex].x) {
			minYCoord = current.y;
			minIndex = i;
		}
		i++;
	}
	std::swap(inputVerticesGraham[0], inputVerticesGraham[minIndex]);
	setReferencePoint(inputVerticesGraham[0]);
	auto st = inputVerticesGraham.begin();
	st++;
	std::sort(st, inputVerticesGraham.end(), customSortByTheta);
}

void nextIterationGraham() {
	if (doneGraham || inputVerticesGraham.empty())
	{
		doneGraham = 1;
		return;
	}
	if (currentHull.size() <= 1 || !orientation(secondPoint(currentHull), currentHull.top(), inputVerticesGraham[0]))
	{
		currentHull.push(inputVerticesGraham[0]);
		inputVerticesGraham.erase(inputVerticesGraham.begin());
	}
	else
	{
		currentHull.pop();
	}
}