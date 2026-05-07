#include "FileManager.h"
#include <fstream>

LinkedList<Song> FileManager::loadSongs(const std::string& filename) {
    LinkedList<Song> songs;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        return songs;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        try {
            Song song = Song::fromCSV(line);
            if (song.isValid()) {
                songs.push_back(song);
            }
        } catch (const std::exception&) {
            // Ignorar lineas mal formadas.
        }
    }
    
    file.close();
    return songs;
}

void FileManager::saveSongs(const std::string& filename, const LinkedList<Song>& songs) {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    
    for (int i = 0; i < songs.getSize(); i++) {
        file << songs.get(i).toCSV() << "\n";
    }
    
    file.close();
}

void FileManager::appendSong(const std::string& filename, const Song& song) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) return;
    
    file << song.toCSV() << "\n";
    file.close();
}

void FileManager::removeSong(const std::string& filename, int songId) {
    LinkedList<Song> songs = loadSongs(filename);
    LinkedList<Song> newSongs;
    
    for (int i = 0; i < songs.getSize(); i++) {
        if (songs.get(i).getId() != songId) {
            newSongs.push_back(songs.get(i));
        }
    }
    
    saveSongs(filename, newSongs);
}