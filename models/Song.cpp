#include "Song.h"
#include <iostream>

Song::Song() : id(-1), year(0), durationSeconds(0) {}

Song::Song(int id, const std::string& title, const std::string& artist,
           const std::string& album, int year, int durationSeconds, const std::string& filePath)
    : id(id), title(title), artist(artist), album(album), 
      year(year), durationSeconds(durationSeconds), filePath(filePath) {}

int Song::getId() const { return id; }
std::string Song::getTitle() const { return title; }
std::string Song::getArtist() const { return artist; }
std::string Song::getAlbum() const { return album; }
int Song::getYear() const { return year; }
int Song::getDurationSeconds() const { return durationSeconds; }
std::string Song::getFilePath() const { return filePath; }

std::string Song::getDisplayString() const {
    return title + " - " + artist;
}

std::string Song::toCSV() const {
    return std::to_string(id) + "," + title + "," + artist + "," + album + "," +
           std::to_string(year) + "," + std::to_string(durationSeconds) + "," + filePath;
}

Song Song::fromCSV(const std::string& line) {
    Song song;
    
    size_t start = 0;
    size_t end = line.find(',');
    if (end == std::string::npos) return song;
    song.id = std::stoi(line.substr(start, end - start));
    
    start = end + 1;
    end = line.find(',', start);
    if (end == std::string::npos) return song;
    song.title = line.substr(start, end - start);
    
    start = end + 1;
    end = line.find(',', start);
    if (end == std::string::npos) return song;
    song.artist = line.substr(start, end - start);
    
    start = end + 1;
    end = line.find(',', start);
    if (end == std::string::npos) return song;
    song.album = line.substr(start, end - start);
    
    start = end + 1;
    end = line.find(',', start);
    if (end == std::string::npos) return song;
    song.year = std::stoi(line.substr(start, end - start));
    
    start = end + 1;
    end = line.find(',', start);
    if (end == std::string::npos) return song;
    song.durationSeconds = std::stoi(line.substr(start, end - start));
    
    start = end + 1;
    song.filePath = line.substr(start);
    
    return song;
}

bool Song::isValid() const {
    return id != -1 && !title.empty() && !artist.empty();
}