#include <fstream>
#include <iostream>
#include "Leaderboard.h"
#include <vector>

void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).

    if (!head_leaderboard_entry || new_entry->score > head_leaderboard_entry->score) {
        new_entry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry *current = head_leaderboard_entry;
        LeaderboardEntry *prev = nullptr;

        while (current && new_entry->score <= current->score) {
            prev = current;
            current = current->next_leaderboard_entry;
        }

        prev->next_leaderboard_entry = new_entry;
        new_entry->next_leaderboard_entry = current;
    }

    max_ten_people();
}

void Leaderboard::write_to_file(const string& filename) {
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions
    std::ofstream file(filename);
    if (file.is_open()) {
        LeaderboardEntry* current = head_leaderboard_entry;
        while (current != nullptr) {
            file << current->score << " " << current->last_played << " " << current->player_name << "\n";
            current = current->next_leaderboard_entry;
        }
        file.close();
    }
}

void Leaderboard::read_from_file(const string& filename) {
    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.
    std::ifstream file(filename);
    if (file.is_open()) {

        int score;
        std::time_t timestamp;
        std::string player_name;
        while (file >> score >> timestamp >> player_name) {
            LeaderboardEntry* new_entry = new LeaderboardEntry(score,timestamp, player_name);
            new_entry->last_played = timestamp;
            insert_new_entry(new_entry);
        }
        file.close();
    }
}


void Leaderboard::print_leaderboard() {
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
    std::cout << "Leaderboard\n-----------\n";
    int order = 1;
    LeaderboardEntry* current = head_leaderboard_entry;
    while (current != nullptr) {
        std::cout << order << ". " << current->player_name << " " << current->score << " "
                  << format_timestamp(current->last_played) << std::endl;
        current = current->next_leaderboard_entry;
        order++;
    }
}

Leaderboard::~Leaderboard() {
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
    LeaderboardEntry* current = head_leaderboard_entry;
    while (current != nullptr) {
        LeaderboardEntry* next = current->next_leaderboard_entry;
        delete current;
        current = next;
    }
    head_leaderboard_entry = nullptr;
}

void Leaderboard::max_ten_people() {
    int free_node = 1;
    LeaderboardEntry* current = head_leaderboard_entry;
    LeaderboardEntry* prev = nullptr;
    while (current != nullptr && free_node < 11) {
        prev = current;
        current = current->next_leaderboard_entry;
        free_node++;
    }

    if (free_node > 10) {
        prev->next_leaderboard_entry = nullptr;
        delete current;
    }
}
std::string Leaderboard::format_timestamp(time_t timestamp) const {
    struct tm* time_info = localtime(&timestamp);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%H:%M:%S/%d.%m.%Y", time_info);
    return std::string(buffer);
}
