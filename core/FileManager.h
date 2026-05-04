#include "../data_structures/LinkedList.hpp"
#include "../models/Song.hpp"
#include <string>
#pragma once

class FileManager {
public:
    static LinkedList<Song> loadSongs(const std::string& filename);
    static void saveSongs(const std::string& filename, const LinkedList<Song>& songs);
    static void appendSong(const std::string& filename, const Song& song);
    static void removeSong(const std::string& filename, int songId);
};