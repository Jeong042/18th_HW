#ifndef ASTAR_HPP
#define ASTAR_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;


class AStar {
public:
    AStar(const vector<vector<int>>& map);
    
    vector<pair<int, int>> findPath(int start_x, int start_y, int goal_x, int goal_y);

private:
    int heuristic(int x, int y, int goal_x, int goal_y);
    
    const vector<vector<int>>& map_c;
    int row_c, col_c;
    static const int directions[4][2];
};



















#endif  // ASTAR_HPP