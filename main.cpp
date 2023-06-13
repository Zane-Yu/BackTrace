#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

using namespace std;

int task_num, machine_num;
vector<vector<int>> best_schedule;
int best_time = INT_MAX;

// Get the data
vector<int> get_data(const string& path) {
    ifstream ifs(path);
    if (!ifs) {
        cout << "Failed to open the file." << endl;
        return {};
    }
    ifs >> task_num >> machine_num;
    vector<int> times(task_num);
    for (int i = 0; i < task_num; ++i) {
        ifs >> times[i];
    }
    ifs.close();
    return times;
}


// Use greedy algorithms to obtain uboptimal solutions
int greedy(vector<int> times, vector<int> finish_time) {
    for (int i = 0; i < task_num; ++i) {
        int min = INT_MAX, idx = -1;
        for (int j = 0; j < machine_num; ++j) {
            if (finish_time[j] < min) {
                min = finish_time[j];
                idx = j;
            }
        }
        finish_time[idx] += times[i];
    }
    return *max_element(finish_time.begin(), finish_time.end());
}



// Backtracking algorithm to find the best schedule
void backtrack(vector<vector<int>>& schedule, vector<int> times, vector<int>& finish_time, int task_index) {
    if (task_index == task_num) {
        int max_finish_time = *max_element(finish_time.begin(), finish_time.end());
        if (max_finish_time < best_time) {
            best_time = max_finish_time;
            best_schedule = schedule;
        }
        return;
    }

    for (int i = 0; i < machine_num; ++i) {
        if ((finish_time[i] + times[task_index]) >= best_time)
            continue;

        if (i > 0 && finish_time[i] == finish_time[i - 1])
            continue;

        finish_time[i] += times[task_index];
        schedule[i].push_back(task_index);

        backtrack(schedule, times, finish_time, task_index + 1);

        schedule[i].pop_back();
        finish_time[i] -= times[task_index];
    }
}


void implement(string path) {
    // Initialize the best time
    best_time = INT_MAX; 
    vector<int> times = get_data(path);

    // Initialize schedule, current schedule, and finish time
    vector<vector<int>> schedule(machine_num);
    vector<int> finish_time(machine_num, 0);

    sort(times.begin(), times.end(), [=](const int& n1, const int& n2) {
        return n1 > n2;
    });
    // Give the suboptimal solution to best time
    best_time = greedy(times, finish_time);

    backtrack(schedule, times, finish_time, 0);

    // Output optimal schedule and completion time
    cout << "Best Schedule for " << path.substr(0, 5) << ": \n";
    for (int i = 0; i < machine_num; ++i) {
        cout << "Machine " << (i + 1) << ": ";
        for (int task : best_schedule[i]) {
            cout << task + 1 << " ";
        }
        cout << endl;
    }
    cout << "Best Time: " << best_time << endl;
}


int main() {
    implement("test1.txt");
    implement("test2.txt");
    implement("test3.txt");

    return 0;
}
