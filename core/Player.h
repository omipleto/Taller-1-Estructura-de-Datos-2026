#pragma once 
#include "../data_structures/LinkedList.hpp"
#include "../models/Song.hpp"
#include "../models/Config.hpp"
#include <string>

class Player {
private:
    LinkedList<Song> allSongs;
    Config config;
    LinkedList<Song> history;
    
    void updateConfigFile();
    void regeneratePlaylist();
    void addToHistory(const Song& song);
    Song getPreviousFromHistory();
    void clearScreen();
    std::string getRepeatModeString() const;
    std::string getStatusPrefix() const;
    
public:
    Player();
    bool initialize();
    void run();
    
    void playPause();
    void previousTrack();
    void nextTrack();
    void toggleShuffle();
    void toggleRepeat();
    void showCurrentPlaylist();
    void showAllSongsMenu();
    void exit();
};