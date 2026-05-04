#include "Player.hpp"
#include "FileManager.hpp"
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
        
        std::cout << "Reproduciendo " << getStatusPrefix();
        if (config.getCurrentSong().isValid()) {
            std::cout << ": " << config.getCurrentSong().getDisplayString();
        } else {
            std::cout << "Reproducción Detenida";
        }
        std::cout << "\n\n";
        
        std::cout << "Opciones:\n";
        std::cout << "W - Reproducir/Pausar\n";
        std::cout << "Q - Pista Anterior\n";
        std::cout << "E - Pista Siguiente\n";
        std::cout << "S - Activar/Desactivar modo aleatorio\n";
        std::cout << "R - Repetición (Desactivado/Repetir una/Repetir todas)\n";
        std::cout << "A - Ver lista de reproducción actual\n";
        std::cout << "L - Listado de canciones\n";
        std::cout << "X - Salir\n";
        std::cout << "Ingrese Opción: ";
        
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
                std::cout << "Opción inválida\n";
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
    }
    else if (!allSongs.isEmpty()) {
        if (config.getRepeatMode() == RepeatMode::ALL) {
            regeneratePlaylist();
            if (!config.getPlaylist().isEmpty()) {
                Song nextSong = config.getPlaylist().get(0);
                config.getPlaylist().pop_front();
                config.setCurrentSong(nextSong);
                config.setPlaying(true);
            }
        } else if (config.getRepeatMode() == RepeatMode::ONE) {
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
    
    bool inSubmenu = true;
    char option;
    
    while (inSubmenu) {
        clearScreen();
        
        std::cout << "Actual " << getStatusPrefix();
        if (config.getCurrentSong().isValid()) {
            std::cout << ": " << config.getCurrentSong().getDisplayString();
        } else {
            std::cout << ": Vacía";
        }
        std::cout << "\n\n";
        
        std::cout << "Lista de reproducción actual:\n";
        if (config.getPlaylist().isEmpty()) {
            std::cout << "Vacía\n\n";
        } else {
            for (int i = 0; i < config.getPlaylist().getSize(); i++) {
                std::cout << (i + 1) << ". " << config.getPlaylist().get(i).getDisplayString() << "\n";
            }
            std::cout << "\n";
        }
        
        std::cout << "Opciones: ";
        if (!config.getPlaylist().isEmpty()) {
            std::cout << "S<num> - Saltar a la canción seleccionada ";
        }
        std::cout << "V - Volver al menú principal\n";
        std::cout << "Ingrese Opción: ";
        
        std::cin >> option;
        
        if (option == 'V' || option == 'v') {
            inSubmenu = false;
        } else if ((option == 'S' || option == 's') && !config.getPlaylist().isEmpty()) {
            int num;
            std::cin >> num;
            if (num >= 1 && num <= config.getPlaylist().getSize()) {
                if (config.getCurrentSong().isValid()) {
                    addToHistory(config.getCurrentSong());
                }
                
                Song selected = config.getPlaylist().get(num - 1);
                
                for (int i = 0; i < num; i++) {
                    config.getPlaylist().pop_front();
                }
                
                config.setCurrentSong(selected);
                config.setPlaying(true);
                inSubmenu = false;
                updateConfigFile();
            }
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void Player::showAllSongsMenu() {
    if (allSongs.isEmpty()) return;
    
    bool inSubmenu = true;
    std::string option;
    
    while (inSubmenu) {
        clearScreen();
        
        std::cout << "Actual " << getStatusPrefix();
        if (config.getCurrentSong().isValid()) {
            std::cout << ": " << config.getCurrentSong().getDisplayString();
        } else {
            std::cout << ": Vacía";
        }
        std::cout << "\n\n";
        
        std::cout << "Canciones registradas:\n";
        for (int i = 0; i < allSongs.getSize(); i++) {
            std::cout << (i + 1) << ". " << allSongs.get(i).getDisplayString() << "\n";
        }
        
        std::cout << "\nOpciones: ";
        std::cout << "R<num> - Reproducir canción seleccionada ";
        std::cout << "A<num> - Agregar canción seleccionada al final ";
        std::cout << "N - Agregar canción al registro ";
        std::cout << "D<num> - Eliminar canción seleccionada ";
        std::cout << "V - Volver al menú principal\n";
        std::cout << "Ingrese Opción: ";
        
        std::getline(std::cin, option);
        
        if (option == "V" || option == "v") {
            inSubmenu = false;
        } else if (option == "N" || option == "n") {
            std::string title, artist, album, filePath;
            int year, duration;
            
            std::cout << "Nombre de la canción: ";
            std::getline(std::cin, title);
            std::cout << "Nombre del artista: ";
            std::getline(std::cin, artist);
            std::cout << "Nombre del álbum: ";
            std::getline(std::cin, album);
            std::cout << "Año de lanzamiento: ";
            std::cin >> year;
            std::cout << "Duración en segundos: ";
            std::cin >> duration;
            std::cin.ignore();
            std::cout << "Ruta del archivo: ";
            std::getline(std::cin, filePath);
            
            Song newSong(config.getNextId(), title, artist, album, year, duration, filePath);
            allSongs.push_back(newSong);
            FileManager::appendSong("music_source.txt", newSong);
            config.setNextId(config.getNextId() + 1);
            updateConfigFile();
            
        } else if (!option.empty() && (option[0] == 'R' || option[0] == 'r')) {
            int num = std::stoi(option.substr(1));
            if (num >= 1 && num <= allSongs.getSize()) {
                Song selected = allSongs.get(num - 1);
                
                LinkedList<Song> newPlaylist;
                for (int i = 0; i < allSongs.getSize(); i++) {
                    if (allSongs.get(i).getId() != selected.getId()) {
                        newPlaylist.push_back(allSongs.get(i));
                    }
                }
                newPlaylist.shuffle();
                
                config.setPlaylist(newPlaylist);
                config.setCurrentSong(selected);
                config.setPlaying(true);
                inSubmenu = false;
                updateConfigFile();
            }
        } else if (!option.empty() && (option[0] == 'A' || option[0] == 'a')) {
            int num = std::stoi(option.substr(1));
            if (num >= 1 && num <= allSongs.getSize()) {
                Song selected = allSongs.get(num - 1);
                config.getPlaylist().push_back(selected);
                updateConfigFile();
            }
        } else if (!option.empty() && (option[0] == 'D' || option[0] == 'd')) {
            int num = std::stoi(option.substr(1));
            if (num >= 1 && num <= allSongs.getSize()) {
                int songId = allSongs.get(num - 1).getId();
                allSongs.remove(num - 1);
                FileManager::removeSong("music_source.txt", songId);
                updateConfigFile();
            }
        }
    }
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
        if (config.getShuffleMode()) {
            prefix += repeatStr;
        } else {
            prefix += "-" + repeatStr;
        }
    }
    
    return prefix;
}