#pragma once
#include "../data_structures/LinkedList.h"
#include "Song.h"
#include <string>
enum class RepeatMode { OFF, ONE, ALL };

class Config {
private:
    int nextId;                    // Próximo ID disponible para canciones
    Song currentSong;              // Canción actual
    bool isPlaying;                // Reproduciendo o en pausa
    bool shuffleMode;              // Modo aleatorio activado
    RepeatMode repeatMode;         // Modo repetición
    LinkedList<Song> playlist;     // Lista de reproducción pendiente
    
public:
    Config();
    bool loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;
    
    // Getters y setters
    int getNextId() const;
    void setNextId(int id);
    Song getCurrentSong() const;
    void setCurrentSong(const Song& song);
    bool isPlaying() const;
    void setPlaying(bool playing);
    bool getShuffleMode() const;
    void setShuffleMode(bool shuffle);
    RepeatMode getRepeatMode() const;
    void setRepeatMode(RepeatMode mode);
    LinkedList<Song>& getPlaylist();
    void setPlaylist(const LinkedList<Song>& playlist);
};