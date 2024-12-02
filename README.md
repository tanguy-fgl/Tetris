Tetris
===
A Tetris clone written in C++ using SFML library

## 💾Installation

| OS | Download |
| ---     | ---   |
| Windows 64bit | [download](https://github.com/tanguy-fgl/Tetris/releases/latest) |

### Tested Platform
- software
  ```
  OS: Windows 11 23H2 (22631.4460)
  ```
- hardware
  ```
  CPU: Intel (R) Core i5-8500 3.00 GHz
  GPU: Intel (R) UHD Graphics 630 (8 GB)
  RAM: 16 GB
  ```

## 🎮Controls
- In a party
  ```
  UP: changes the orientation of the block
  LEFT/RIGHT: moves the block to the left or to the right
  DOWN: moves down the block faster
  ```
- When a game is finished
  ```
  R: starts a new game 
  ```

## 🔨How to build
- IDE
```
CodeBlocks 20.03
```
- Libraries
```
SFML 2.6.2
- sfml-graphics-s
- sfml-window-s
- sfml-audio-s
- sfml-system-s
- winmm
- opengl32
- freetype
- gdi32
- openal32
- flac
- vorbisenc
- vorbisfile
- vorbis
- ogg
```
- Compiler
```
WinLibs MSVCRT 13.1.0 (64-bit)
```

## 📁Directory Hierarchy
```
|—— .gitattributes
|—— .gitignore
|—— Arial.ttf
|—— audio
|    |—— music.ogg
|—— bin
|    |—— Debug
|        |—— Tetris.exe
|    |—— Release
|        |—— Arial.ttf
|        |—— audio
|            |—— music.ogg
|        |—— images
|            |—— background.png
|            |—— frame.png
|            |—— gameover.jpg
|            |—— Thumbs.db
|            |—— tiles.png
|        |—— openal32.dll
|        |—— Tetris.exe
|        |—— tetris.png
|        |—— Thumbs.db
|—— icon.rc
|—— images
|    |—— background.png
|    |—— frame.png
|    |—— gameover.jpg
|    |—— Thumbs.db
|    |—— tiles.png
|—— main.cpp
|—— obj
|    |—— Debug
|        |—— icon.res
|        |—— main.o
|    |—— Release
|        |—— icon.res
|        |—— main.o
|—— openal32.dll
|—— sfml-audio-2.dll
|—— sfml-audio-d-2.dll
|—— sfml-graphics-2.dll
|—— sfml-graphics-d-2.dll
|—— sfml-network-2.dll
|—— sfml-network-d-2.dll
|—— sfml-system-2.dll
|—— sfml-system-d-2.dll
|—— sfml-window-2.dll
|—— sfml-window-d-2.dll
|—— Tetris.cbp
|—— Tetris.depend
|—— tetris.ico
|—— Tetris.layout
|—— tetris.png
|—— Thumbs.db
```


## 📚References
- [Let's make 16 games in C++: TETRIS](https://www.youtube.com/watch?v=zH_omFPqMO4)
- [SFML 2.6.2 documentation](https://www.sfml-dev.org/documentation/2.6.2/topics.php)
  
## 📜License

## ☎️Citing
If you use my Tetris clone game, please add the following information to your blog / video description.
```
Game by Tanguy Frageul - Play it for free: https://tanguy-fgl.github.io/
```
