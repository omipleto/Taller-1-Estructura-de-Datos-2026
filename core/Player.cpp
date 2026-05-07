#include "Player.h"
#include "FileManager.h"
#include <iostream>
#include <limits>
#include <cstdlib>

Player::Player() {}

bool Player::initialize() {
    allSongs = FileManager::loadSongs("music_source.txt");
    
    if (!config.loadFromFile("status.cfg", allSongs)) {
        if (!allSongs.isEmpty()) {
            regeneratePlaylist();
            if (!config.getPlaylist().isEmpty()) {
                Song firstSong = config.getPlaylist().get(0);
                config.getPlaylist().pop_front();
                config.setCurrentSong(firstSong);
            }
            config.setPlaying(false);
            config.setShuffleMode(false);
            config.setRepeatMode(RepeatMode::OFF);
            config.setNextId(allSongs.getSize() + 1);
        }
        updateConfigFile();
    }
    
    return true;
}

void Player::run() {
    char option;
    bool running = true;
    
    while (running) {
        clearScreen();
        
        // Verificar si no hay canciones
        if (allSongs.isEmpty()) {
            std::cout << "No hay canciones disponibles. Agregue canciones con la opcion L" << std::endl;
            std::cout << "\nPresione Enter para continuar...";
            std::cin.get();
            continue;
        }
        std::cout << "\n\n";
        
        std::cout << "Opciones:\n";
        std::cout << "W - Reproducir/Pausar\n";
        std::cout << "Q - Pista Anterior\n";
        std::cout << "E - Pista Siguiente\n";
        std::cout << "S - Activar/Desactivar modo aleatorio\n";
        std::cout << "R - Repeticion\n";
        std::cout << "A - Ver lista de reproduccion actual\n";
        std::cout << "L - Listado de canciones\n";
        std::cout << "X - Salir\n";
        std::cout << "Ingrese Opcion: ";
        
        std::cin >> option;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (option) {
            case 'W': case 'w': playPause(); break;
            case 'Q': case 'q': previousTrack(); break;
            case 'E': case 'e': nextTrack(); break;
            case 'S': case 's': toggleShuffle(); break;
            case 'R': case 'r': toggleRepeat(); break;
            case 'A': case 'a': showCurrentPlaylist(); break;
            case 'L': case 'l': showAllSongsMenu(); break;
            case 'X': case 'x': 
                exit();
                running = false;
                break;
            default:
                std::cout << "Opcion invalida\n";
                std::cin.get();
        }
    }
}

void Player::playPause() {
    if (allSongs.isEmpty()) return;
    config.setPlaying(!config.isPlaying());
    updateConfigFile();
}

void Player::previousTrack() {
    if (allSongs.isEmpty()) return;
    
    Song previous = getPreviousFromHistory();
    if (previous.isValid()) {
        if (config.getCurrentSong().isValid()) {
            addToHistory(config.getCurrentSong());
        }
        config.setCurrentSong(previous);
        config.setPlaying(true);
    }
    updateConfigFile();
}

void Player::nextTrack() {
    if (allSongs.isEmpty()) return;
    
    if (config.getCurrentSong().isValid()) {
        addToHistory(config.getCurrentSong());
    }
    
    if (!config.getPlaylist().isEmpty()) {
        Song nextSong = config.getPlaylist().get(0);
        config.getPlaylist().pop_front();
        config.setCurrentSong(nextSong);
        config.setPlaying(true);
    } else {
        regeneratePlaylist();
        if (!config.getPlaylist().isEmpty()) {
            Song nextSong = config.getPlaylist().get(0);
            config.getPlaylist().pop_front();
            config.setCurrentSong(nextSong);
            config.setPlaying(true);
        }
    }
    
    updateConfigFile();
}

void Player::toggleShuffle() {
    if (allSongs.isEmpty()) return;
    config.setShuffleMode(!config.getShuffleMode());
    if (config.getShuffleMode()) {
        config.getPlaylist().shuffle();
    }
    updateConfigFile();
}

void Player::toggleRepeat() {
    if (allSongs.isEmpty()) return;
    
    switch (config.getRepeatMode()) {
        case RepeatMode::OFF:
            config.setRepeatMode(RepeatMode::ONE);
            break;
        case RepeatMode::ONE:
            config.setRepeatMode(RepeatMode::ALL);
            break;
        case RepeatMode::ALL:
            config.setRepeatMode(RepeatMode::OFF);
            break;
    }
    updateConfigFile();
}

void Player::showCurrentPlaylist() {
    if (allSongs.isEmpty()) return;
    
    std::cout << "\n=== Lista de Reproduccion Actual ===\n";
    if (config.getPlaylist().isEmpty()) {
        std::cout << "Vacia\n";
    } else {
        for (int i = 0; i < config.getPlaylist().getSize(); i++) {
            std::cout << (i+1) << ". " << config.getPlaylist().get(i).getDisplayString() << "\n";
        }
    }
    std::cout << "\nPresione Enter para continuar...";
    std::cin.get();
    std::cin.get(); // Doble get para asegurar
}

void Player::showAllSongsMenu() {
    if (allSongs.isEmpty()) return;
    
    std::cout << "\n=== Todas las Canciones ===\n";
    for (int i = 0; i < allSongs.getSize(); i++) {
        std::cout << (i+1) << ". " << allSongs.get(i).getDisplayString() << "\n";
    }
    std::cout << "\nPresione Enter para continuar...";
    std::cin.get();
    std::cin.get(); // Doble get para asegurar
}

void Player::exit() {
    updateConfigFile();
    std::cout << "Saliendo del reproductor...\n";
}

void Player::updateConfigFile() {
    config.saveToFile("status.cfg");
}

void Player::regeneratePlaylist() {
    LinkedList<Song> newPlaylist;
    for (int i = 0; i < allSongs.getSize(); i++) {
        newPlaylist.push_back(allSongs.get(i));
    }
    if (config.getShuffleMode()) {
        newPlaylist.shuffle();
    }
    config.setPlaylist(newPlaylist);
}

void Player::addToHistory(const Song& song) {
    history.push_front(song);
    while (history.getSize() > 50) {
        history.pop_back();
    }
}

Song Player::getPreviousFromHistory() {
    if (history.isEmpty()) return Song();
    Song previous = history.get(0);
    history.pop_front();
    return previous;
}

void Player::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

std::string Player::getRepeatModeString() const {
    switch (config.getRepeatMode()) {
        case RepeatMode::ONE: return "R1";
        case RepeatMode::ALL: return "RA";
        default: return "";
    }
}

std::string Player::getStatusPrefix() const {
    std::string prefix;
    
    if (!config.isPlaying()) {
        prefix = "En pausa";
    } else if (config.getCurrentSong().isValid()) {
        prefix = "Reproduciendo";
    } else {
        return "";
    }
    
    if (config.getShuffleMode()) {
        prefix += "-S";
    }
    
    std::string repeatStr = getRepeatModeString();
    if (!repeatStr.empty()) {
        prefix += "-" + repeatStr;
    }
    
    return prefix;
}