#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>  // Pour memset
#include <ctime>    // Pour srand et rand
#include <fstream>  // Pour les fichiers

using namespace sf;

const int M = 20;  // Hauteur du terrain
const int N = 10;  // Largeur du terrain

int field[M][N] = {0};  // Terrain de jeu
int score = 0;
int highScore = 0;

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
    2, 3, 4, 5   // O
};

const int initialX = N / 2 - 1;
int currentSpawnX = -1;
int initialSpawnX = -1;

bool check() {
    for (int i = 0; i < 4; i++) {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return false;
        if (field[a[i].y][a[i].x]) return false;
    }
    return true;
}

bool canSpawnAt(int n, int offsetX) {
    for (int i = 0; i < 4; i++) {
        int x = offsetX + (figures[n][i] % 2);
        int y = figures[n][i] / 2;
        if (x < 0 || x >= N || field[y][x]) return false;
    }
    return true;
}

std::vector<int> findAlternativeSpawn(int n) {
    std::vector<int> possiblePositions;
    for (int offsetX = 0; offsetX < N; offsetX++) {
        if (canSpawnAt(n, offsetX)) possiblePositions.push_back(offsetX);
    }
    return possiblePositions;
}

void loadHighScore() {
    std::ifstream inFile("data.txt");
    if (inFile.is_open()) {
        inFile >> highScore;
        inFile.close();
    } else {
        highScore = 0;
    }
}

void saveHighScore() {
    std::ofstream outFile("data.txt");
    if (outFile.is_open()) {
        outFile << highScore;
        outFile.close();
    }
}

void resetGame() {
    memset(field, 0, sizeof(field));
    srand(static_cast<unsigned>(time(0)));
    score = 0;

    int n = rand() % 7;
    if (initialSpawnX == -1) {
        if (canSpawnAt(n, initialX)) {
            initialSpawnX = initialX;
        } else {
            auto alternativePositions = findAlternativeSpawn(n);
            if (!alternativePositions.empty()) {
                initialSpawnX = alternativePositions.front();
            } else {
                std::cout << "Game Over: No valid spawn positions.\n";
                exit(0);
            }
        }
    }

    currentSpawnX = initialSpawnX;
    for (int i = 0; i < 4; i++) {
        a[i].x = currentSpawnX + (figures[n][i] % 2);
        a[i].y = figures[n][i] / 2;
    }
}

void checkLines() {
    int linesCleared = 0;
    for (int i = M - 1; i >= 0; i--) {
        bool lineFull = true;
        for (int j = 0; j < N; j++) {
            if (!field[i][j]) {
                lineFull = false;
                break;
            }
        }
        if (lineFull) {
            linesCleared++;
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < N; j++) {
                    field[k][j] = field[k - 1][j];
                }
            }
            for (int j = 0; j < N; j++) field[0][j] = 0;
            i++; // Re-vérifier la ligne après le décalage
        }
    }
    switch (linesCleared) {
        case 1: score += 100; break;
        case 2: score += 300; break;
        case 3: score += 500; break;
        case 4: score += 800; break;
    }
}

int main() {
    loadHighScore();
    srand(static_cast<unsigned>(time(0)));
    RenderWindow window(VideoMode(320, 480), "Tetris - Tanguy Frageul");

    Texture t1, t2, t3, tGameOver;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/frame.png");
    tGameOver.loadFromFile("images/gameover.jpg");

    Sprite s(t1), background(t2), frame(t3), gameOverSprite(tGameOver);

    Music music;
    if (!music.openFromFile("audio/music.ogg")) return -1;
    music.setLoop(true);
    music.play();

    Font font;
    Text scoreText, highScoreText, gameOverText, restartText;
    if (!font.loadFromFile("fonts/arial.ttf")) {
        std::cerr << "Erreur : Impossible de charger la police.\n";
        return -1;
    }

    Font superPixelFont;
    if (!superPixelFont.loadFromFile("fonts/Daydream.ttf")) {
        std::cerr << "Erreur : Impossible de charger la police Daydream.ttf\n";
        return -1;
    }

    scoreText.setFont(superPixelFont);
    scoreText.setCharacterSize(14);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(10, 420);

    highScoreText.setFont(superPixelFont);
    highScoreText.setCharacterSize(14);
    highScoreText.setFillColor(Color::White);
    highScoreText.setPosition(10, 450);

    gameOverText.setFont(superPixelFont);
    gameOverText.setString("Game Over!");
    gameOverText.setCharacterSize(24);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(40, 200);

    restartText.setFont(superPixelFont);
    restartText.setString("Press 'R' to Restart");
    restartText.setCharacterSize(16);
    restartText.setFillColor(Color::Green);
    restartText.setPosition(15, 270);

    int dx = 0;
    bool rotate = false;
    int colorNum = 1;
    float timer = 0, delay = 0.3;

    Clock clock;
    resetGame();

    bool gameOver = false;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) window.close();
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Up) rotate = true;
                else if (e.key.code == Keyboard::Left) dx = -1;
                else if (e.key.code == Keyboard::Right) dx = 1;
                else if (e.key.code == Keyboard::R && gameOver) {
                    gameOver = false;
                    resetGame();
                }
            }
        }

        if (gameOver) {
            window.clear();
            window.draw(gameOverSprite);
            window.draw(gameOverText);
            window.draw(restartText);
            window.display();
            continue;
        }

        if (score > highScore) {
            highScore = score;
            saveHighScore();
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;

        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
            a[i].x += dx;
        }
        if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

        if (rotate) {
            Point p = a[1];
            for (int i = 0; i < 4; i++) {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
        }

        if (timer > delay) {
            for (int i = 0; i < 4; i++) {
                b[i] = a[i];
                a[i].y += 1;
            }
            if (!check()) {
                for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;
                checkLines();
                colorNum = 1 + rand() % 7;
                int n = rand() % 7;
                if (!canSpawnAt(n, currentSpawnX)) {
                    auto alternativePositions = findAlternativeSpawn(n);
                    if (alternativePositions.empty()) gameOver = true;
                    else currentSpawnX = alternativePositions.front();
                }
                for (int i = 0; i < 4; i++) {
                    a[i].x = currentSpawnX + (figures[n][i] % 2);
                    a[i].y = figures[n][i] / 2;
                }
            }
            timer = 0;
        }

        dx = 0;
        rotate = false;
        delay = 0.3;

        window.clear(Color::White);
        window.draw(background);

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (field[i][j] == 0) continue;
                s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                s.setPosition(j * 18, i * 18);
                s.move(28, 31);
                window.draw(s);
            }
        }


        for (int i = 0; i < 4; i++) {
            s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
            s.setPosition(a[i].x * 18, a[i].y * 18);
            s.move(28, 31);
            window.draw(s);
        }

        window.draw(frame);

        scoreText.setString("Score: " + std::to_string(score));
        highScoreText.setString("High Score: " + std::to_string(highScore));
        window.draw(scoreText);
        window.draw(highScoreText);

        window.display();
    }

    return 0;
}
