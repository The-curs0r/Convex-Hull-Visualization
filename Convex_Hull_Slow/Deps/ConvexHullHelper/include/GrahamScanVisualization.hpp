#ifndef GRAHAMSCANVISUALIZATION_HPP
#define GRAHAMSCANVISUALIZATION_HPP

#include <vector>
#include <algorithm>
#include <set>
#include <utility>

std::vector<std::pair<double, double> > getCurrentHullGraham();
std::vector<std::pair<double, double> > getCurrentCheckGraham();
std::pair<double, double> getCurrentHullPointGraham();
void nextIterationGraham();
void loadPointsGraham(std::set<std::pair<double, double> > inputPoints);

#endif // !GRAHAMSCANVISUALIZATION_HPP