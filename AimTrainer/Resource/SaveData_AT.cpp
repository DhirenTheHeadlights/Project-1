#include "SaveData_AT.h"

SaveData::SaveData(const std::string& givenFileName) : filename(givenFileName) {
    leaderboard = file_read();
    if (!leaderboard.empty()) {
        sessionHigh = leaderboard.front().second;
    }
    else {
        sessionHigh = 0;
    }
}

void SaveData::addData(const std::vector<std::pair<std::string, int>>& newScores) {
    for (const auto& entry : newScores) {
        leaderboard.push_back(entry);
        if (entry.second > sessionHigh) {
            sessionHigh = entry.second;
        }
    }
    std::sort(leaderboard.begin(), leaderboard.end(),
        [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second > b.second;
        });
    file_write();
}

void SaveData::clearData() {
    leaderboard.clear();
    sessionHigh = 0;
    file_write();
}

void SaveData::file_write() {
    std::ofstream outdata(filename);
    if (!outdata) {
        std::cerr << "Error: file could not be opened" << std::endl;
        exit(1);
    }

    for (const auto& [name, score] : leaderboard) {
        outdata << name << "," << score << "\n";
    }
    outdata.close();
}

std::vector<std::pair<std::string, int>> SaveData::file_read() {
    std::fstream indata(filename);
    std::vector<std::pair<std::string, int>> data;
    if (!indata) {
        return data; // If the file couldn't be opened or doesn't exist, return an empty vector
    }

    std::string filetext;
    int lineCount = 0; // Initialize a counter for the number of lines read
    while (getline(indata, filetext) && lineCount < 10) { // Check both for EOF and line count
        if (!filetext.empty()) {
            size_t commaPos = filetext.find(',');
            std::string name = filetext.substr(0, commaPos);
            int score = std::stoi(filetext.substr(commaPos + 1));
            data.emplace_back(name, score);
        }
        lineCount++; // Increment the counter for each line read
    }

    indata.close();
    // Sort and return the read data
    std::sort(data.begin(), data.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return a.second > b.second;
        });

    return data;
}
