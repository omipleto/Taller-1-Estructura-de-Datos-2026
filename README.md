# Taller-1-Estructura-de-Datos-2026
Gaspar Navarro 
08/05/2026


## Descripción

Proyecto de un reproductor de música base desarrollado en C++ como parte del Taller 1 de la asignatura Estructura de Datos. La aplicación demuestra el uso de estructuras de datos personalizadas (listas enlazadas) para gestionar listas de reproducción, modos aleatorio y repetición, todo a través de una interfaz de consola.

El programa carga canciones desde un archivo de texto y guarda el estado de la reproducción (canción actual, modo, lista de reproducción) en un archivo de configuración.

## Características 
- **Gestión de Canciones**: Carga la biblioteca desde _music_source.txt_. Permite agregar nuevas canciones y eliminar las existentes.
- **Control de Reproducción**: Funciones de **Reproducir/Pausar** (W) , **Pista Siguiente** (E) y **Pista Anterior** (Q).
- **Modo Aleatorio (Shuffle)**: Activar/desactivar con la tecla S. Al activarse, mezcla las canciones restantes de la lista de reproducción.
- **Modos de Repetición**: Cambia entre _OFF_ -> Repetir una (R1) -> Repetir todas (RA) con la tecla R.
- **Lista de Reproducción Actual (A)**: Muestra las canciones pendientes y permite saltar (S<número>) a una canción específica, descartando las intermedias.
- **Listado de Canciones (L)**: Muestra toda la biblioteca y ofrece un submenú para reproducir, agregar a la lista de reproducción, agregar y eliminar canciones.
- **Persistencia**: Guarda el estado actual (canción, modo, lista) en status.cfg al salir con la tecla X.
  
  

## Compilar
Este proyecto se compiló con g++ utilizando C++14

_g++ -std=c++14 models/Song.cpp models/Config.cpp core/FileManager.cpp core/Player.cpp core/main.cpp -o reproductor_

