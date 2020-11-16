#ifndef ANDREWMONOTONE_HPP
#define ANDREWMONOTONE_HPP

void andrewMonotone(const std::set<std::pair<double, double> > vertices, std::vector<std::pair<double, double> >& convexHullVertices);

bool customSortByCoordinate(const Point& v1, const Point& v2);
#endif // !ANDREWMONOTONE_HPP
