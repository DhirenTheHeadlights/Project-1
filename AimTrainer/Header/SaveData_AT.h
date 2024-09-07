#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <utility> // For std::pair
#include <algorithm>

class SaveData {
public:
    SaveData(const std::string& filename);
    void addData(const std::vector<std::pair<std::string, int>>& newScores);
    void clearData();
    void file_write();
    std::vector<std::pair<std::string, int>> file_read() const;
    std::string fileName() const { return filename; };
    std::vector<std::pair<std::string, int>> scores() const { return leaderboard; }
    int getSessionHigh() const { return sessionHigh; }
    std::pair<std::string, int> getHighScore() const { return !leaderboard.empty() ? leaderboard.front() : std::make_pair("", 0); }

private:
    std::vector<std::pair<std::string, int>> leaderboard;
    std::string filename;
    int sessionHigh = 0;
};