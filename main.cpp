#include <time.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstring>  // pour utiliser memset
#include <iostream>

using namespace sf;

const int M = 20;
const int N = 10;

int field[M][N] = {0};

struct Point {
  int x, y;
} a[4], b[4];

int figures[7][4] = {
    1, 3, 5, 7,  // I
    2, 4, 5, 7,  // Z
    3, 5, 4, 6,  // S
    3, 5, 4, 7,  // T
    2, 3, 5, 7,  // L
    3, 5, 7, 6,  // J
    2, 3, 4, 5,  // O
};

// Calculer la position x de départ pour centrer les blocs
const int initialX = N / 2 - 1;
int currentSpawnX =
    -1;  // Initialise à -1 pour indiquer qu'aucune position n'a été choisie
int initialSpawnX =
    -1;  // Initialise à -1 pour indiquer qu'aucune position n'a été choisie

bool canSpawn(int n) {
  for (int i = 0; i < 4; i++) {
    int x = initialX + (figures[n][i] % 2);  // Colonnes
    int y = figures[n][i] / 2;               // Lignes
    if (field[y][x])                         // Si l'endroit est déjà occupé
      return false;
  }
  return true;
}

bool canSpawnAt(int n, int offsetX) {
  for (int i = 0; i < 4; i++) {
    int x = offsetX + (figures[n][i] % 2);  // Colonnes
    int y = figures[n][i] / 2;              // Lignes
    if (x < 0 || x >= N ||
        field[y][x])  // Si l'endroit est déjà occupé ou hors de l'aire de jeu
      return false;
  }
  return true;
}

bool check() {
  for (int i = 0; i < 4; i++)
    if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
      return 0;
    else if (field[a[i].y][a[i].x])
      return 0;

  return 1;
}

std::vector<int> findAlternativeSpawn(int n) {
  std::vector<int> possiblePositions;
  for (int offsetX = 0; offsetX <= N - 2;
       offsetX++) {  // Ajuster pour éviter les débordements
    if (canSpawnAt(n, offsetX)) {
      possiblePositions.push_back(offsetX);
    }
  }
  return possiblePositions;
}

void resetGame() {
  // Réinitialiser le champ de jeu et autres variables
  memset(field, 0, sizeof(field));
  srand(time(0));

  int n = rand() % 7;

  if (initialSpawnX == -1) {
    // Définir initialSpawnX lors de la première partie
    if (canSpawnAt(n, initialX)) {
      initialSpawnX = initialX;
    } else {
      std::vector<int> alternativePositions = findAlternativeSpawn(n);
      if (!alternativePositions.empty()) {
        std::random_shuffle(alternativePositions.begin(),
                            alternativePositions.end());
        initialSpawnX = alternativePositions.front();
      } else {
        std::cout << "Game Over: No valid spawn positions." << std::endl;
        exit(0);  // ou toute autre action de fin de jeu
      }
    }
  }

  currentSpawnX =
      initialSpawnX;  // Utiliser initialSpawnX à chaque nouvelle partie

  for (int i = 0; i < 4; i++) {
    a[i].x = currentSpawnX + (figures[n][i] % 2);  // Colonnes
    a[i].y = figures[n][i] / 2;                    // Lignes
  }
}

int main() {

  srand(time(0));

  RenderWindow window(VideoMode(320, 480), "Tetris - Tanguy Frageul");

  // Charger l'image de l'icône
  Image icon;
  if (!icon.loadFromFile("tetris.png")) {
    std::cerr << "Erreur : Impossible de charger l'icône." << std::endl;
    return -1;
  }

  // Définir l'icône pour la fenêtre
  window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

  Texture t1, t2, t3, tGameOver;
  t1.loadFromFile("images/tiles.png");
  t2.loadFromFile("images/background.png");
  t3.loadFromFile("images/frame.png");
  tGameOver.loadFromFile("images/gameover.jpg"); // Charger la texture de Game Over


  Sprite s(t1), background(t2), frame(t3), gameOverSprite(tGameOver);

  // Charger la musique
  Music music;
  if (!music.openFromFile("audio/music.ogg"))  // Thème
    return -1;
  music.setLoop(true);  // La musique sera jouée en boucle
  music.play();         // Démarre la musique

  int dx = 0;
  bool rotate = 0;
  int colorNum = 1;
  float timer = 0, delay = 0.3;

  Clock clock;

  // Initialisation de la première pièce
  resetGame();

  bool gameOver = false;
  Text gameOverText, restartText;
  Font font;
  if (!font.loadFromFile("arial.ttf")) {
    std::cerr << "Erreur : Impossible de charger la police." << std::endl;
    return -1;
  }

  gameOverText.setFont(font);
  gameOverText.setString("Game Over!");
  gameOverText.setCharacterSize(24);
  gameOverText.setFillColor(Color::Red);
  gameOverText.setPosition(90, 200);

  restartText.setFont(font);
  restartText.setString("Press 'R' to play again");
  restartText.setCharacterSize(18);
  restartText.setFillColor(Color::Green);
  restartText.setPosition(70, 240);

  while (window.isOpen()) {
    float time = clock.getElapsedTime().asSeconds();
    clock.restart();
    timer += time;

    Event e;
    while (window.pollEvent(e)) {
        if (e.type == Event::Closed)
            window.close();

        if (e.type == Event::KeyPressed) {
            if (e.key.code == Keyboard::Up) rotate = true;
            else if (e.key.code == Keyboard::Left) dx = -1;
            else if (e.key.code == Keyboard::Right) dx = 1;
            else if (e.key.code == Keyboard::R && gameOver) {
                // Recommencer le jeu
                gameOver = false;
                resetGame();
            }
        }
    }

    if (gameOver) {
        // Si le jeu est terminé, on affiche l'image de Game Over
        window.clear();
        window.draw(gameOverSprite); // Afficher l'image de Game Over
        window.draw(gameOverText);
        window.draw(restartText);
        window.display();
        continue;
    }

    if (Keyboard::isKeyPressed(Keyboard::Down))
        delay = 0.05;

    //// <- Move -> ///
    for (int i = 0; i < 4; i++) {
        b[i] = a[i];
        a[i].x += dx;
    }
    if (!check())
        for (int i = 0; i < 4; i++)
            a[i] = b[i];

    //////Rotate//////
    if (rotate) {
        Point p = a[1]; // centre de rotation
        for (int i = 0; i < 4; i++) {
            int x = a[i].y - p.y;
            int y = a[i].x - p.x;
            a[i].x = p.x - x;
            a[i].y = p.y + y;
        }
        if (!check())
            for (int i = 0; i < 4; i++)
                a[i] = b[i];
    }

    ///////Tick//////
    if (timer > delay) {
        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
            a[i].y += 1;
        }

        if (!check()) {
            for (int i = 0; i < 4; i++)
                field[b[i].y][b[i].x] = colorNum;

            // Nouvelle pièce
            colorNum = 1 + rand() % 7;
            int n = rand() % 7;
            if (canSpawnAt(n, currentSpawnX)) {
                for (int i = 0; i < 4; i++) {
                    a[i].x = currentSpawnX + (figures[n][i] % 2);
                    a[i].y = figures[n][i] / 2;
                }
            } else {
                std::vector<int> alternativePositions = findAlternativeSpawn(n);
                if (alternativePositions.empty()) {
                    gameOver = true;
                } else {
                    std::random_shuffle(alternativePositions.begin(), alternativePositions.end());
                    currentSpawnX = alternativePositions.front();
                    for (int i = 0; i < 4; i++) {
                        a[i].x = currentSpawnX + (figures[n][i] % 2);
                        a[i].y = figures[n][i] / 2;
                    }
                }
            }
        }

        timer = 0;
    }

    ///////check lines//////////
    int k = M - 1;
    for (int i = M - 1; i > 0; i--) {
        int count = 0;
        for (int j = 0; j < N; j++) {
            if (field[i][j])
                count++;
            field[k][j] = field[i][j];
        }
        if (count < N)
            k--;
    }

    dx = 0;
    rotate = 0;
    delay = 0.3;

    /////////draw//////////
    window.clear(Color::White);
    window.draw(background);

    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++) {
            if (field[i][j] == 0)
                continue;
            s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
            s.setPosition(j * 18, i * 18);
            s.move(28, 31); // offset
            window.draw(s);
        }

    for (int i = 0; i < 4; i++) {
        s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
        s.setPosition(a[i].x * 18, a[i].y * 18);
        s.move(28, 31); // offset
        window.draw(s);
    }

    window.draw(frame);
    window.display();
}


  return 0;
}
