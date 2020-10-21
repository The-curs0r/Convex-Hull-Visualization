#ifndef QUICKHULLVISUALIZATION_HPP
#define QUICKHULLVISUALIZATION_HPP

#include <vector>
#include <algorithm>
#include <set>
#include <utility>

std::vector<std::pair<double, double> > getCurrentHullQuickHull();

//std::vector<std::pair<double, double> > getCurrentCheckQuickHull();
//std::pair<double, double> getCurrentHullPointQuickHull();
void nextIterationQuickHull();
void loadPointsQuickHull(std::set<std::pair<double, double> > inputPoints);
bool doneLeft();
bool doneRight();
std::pair<double, double> rightMost();
#endif // !QUICKHULLVISUALIZATION_HPP