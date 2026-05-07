#pragma once
#include "../data_structures/LinkedList.h"
#include "../models/Song.h"
#include <string>

class FileManager {
public:
    static LinkedList<Song> loadSongs(const std::string& filename);
    static void saveSongs(const std::string& filename, const LinkedList<Song>& songs);
    static void appendSong(const std::string& filename, const Song& song);
    static void removeSong(const std::string& filename, int songId);
};

