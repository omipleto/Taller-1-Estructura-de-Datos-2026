#pragma once
#include <string>

class Song {
private:
    int id;
    std::string title;
    std::string artist;
    std::string album;
    int year;
    int durationSeconds;
    std::string filePath;
    
public:
    Song();
    Song(int id, const std::string& title, const std::string& artist,
         const std::string& album, int year, int durationSeconds, const std::string& filePath);
    
    int getId() const;
    std::string getTitle() const;
    std::string getArtist() const;
    std::string getAlbum() const;
    int getYear() const;
    std::string getFilePath() const;
    int getDurationSeconds() const;
    
    std::string getDisplayString() const;
    std::string toCSV() const;
    
    static Song fromCSV(const std::string& line);
    bool isValid() const;
};

