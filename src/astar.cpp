#include "../include/astar/astar.hpp"

using namespace std;

const int AStar::directions[4][2] = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1} 
};

AStar::AStar(const vector<vector<int>> &map) : map_c(map)
{
    row_c = map_c.size();
    col_c = map_c[0].size();
}

int AStar::heuristic(int x, int y, int goal_x, int goal_y)
{
    int dx = abs(x - goal_x);
    int dy = abs(y - goal_y);
    return std::max(dx, dy);
}

vector<pair<int, int>> AStar::findPath(int start_x, int start_y, int goal_x, int goal_y)
{
    vector<vector<int>> g(row_c, vector<int>(col_c, 10000));
    vector<vector<bool>> visited(row_c, vector<bool>(col_c, false));
    vector<vector<std::pair<int, int>>> parent(row_c, vector<pair<int, int>>(col_c, {-1, -1}));

    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> openList;
    g[start_x][start_y] = 0;
    openList.push({heuristic(start_x, start_y, goal_x, goal_y), start_x, start_y});

    while (!openList.empty())
    {
        auto [f, x, y] = openList.top();
        openList.pop();

        if (x == goal_x && y == goal_y)
        {
            vector<pair<int, int>> path;
            while (x != -1 && y != -1)
            {
                path.push_back({x, y});
                tie(x, y) = parent[x][y];
            }
            reverse(path.begin(), path.end());
            return path;
        }

        if (visited[x][y])
            continue;
        visited[x][y] = true;

        for (int i = 0; i < 4; ++i) 
        {
            int new_x = x + directions[i][0];
            int new_y = y + directions[i][1];

            if (new_x >= 0 && new_x < row_c && new_y >= 0 && new_y < col_c && map_c[new_x][new_y] != 1 && !visited[new_x][new_y])
            {
                int cost = 1;
                int newG = g[x][y] + cost;

                if (newG < g[new_x][new_y])
                {
                    g[new_x][new_y] = newG;
                    parent[new_x][new_y] = {x, y};
                    int h = heuristic(new_x, new_y, goal_x, goal_y);
                    openList.push({newG + h, new_x, new_y});
                }
            }
        }
    }

    return {}; 
}
