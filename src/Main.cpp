#include <SDL2/SDL.h>
#include "bot.cpp"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 560;
const int SCREEN_HEIGHT = 480;
const int ROWS = 6;
const int COLS = 7;
const int CELL_SIZE = 80;
const int BORDER_SIZE = 4;
const int PLAYER_1 = 1;
const int PLAYER_2 = 2;
const int WINNING_LENGTH = 4;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

std::string gameState(ROWS * COLS, '-');
int currentPlayer = PLAYER_1;

bool isBoardFull() {
    return gameState.find('-') == std::string::npos;
}

bool checkGameWon(const std::string& state) {
    int rows = ROWS;
    int cols = COLS;

    std::vector<std::vector<char>> grid(rows, std::vector<char>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j] = state[i * cols + j];
        }
    }

    std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (grid[row][col] == '-') {
                continue;
            }
            for (const auto& direction : directions) {
                int dr = direction.first;
                int dc = direction.second;
                for (int step = 1; step <= 3; ++step) {
                    int r = row + step * dr;
                    int c = col + step * dc;
                    if (r >= 0 && r < rows && c >= 0 && c < cols) {
                        if (grid[row][col] == grid[r][c]) {
                            if (step == 3) {
                                return true;
                            }
                        } else {
                            break;
                        }
                    }
                }
            }
        }
    }

    return false;
}

// Function to draw a filled circle
void filledCircle(SDL_Renderer* renderer, int cx, int cy, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            if (dx * dx + dy * dy <= radius * radius) {
                SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
            }
        }
    }
}

std::string getGameStateString() {
    std::string result;
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            result += gameState[row * COLS + col];
        }
    }
    return result;
}


void drawBoard() {
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); 
    SDL_RenderClear(gRenderer);

    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {

            SDL_Rect cellRect = {col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255); 
            SDL_RenderFillRect(gRenderer, &cellRect);

            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); 
            SDL_RenderDrawRect(gRenderer, &cellRect);

            // Draw the game pieces for Player 1 and Player 2
            int centerX = col * CELL_SIZE + CELL_SIZE / 2;
            int centerY = row * CELL_SIZE + CELL_SIZE / 2;

            if (gameState[row * COLS + col] == '1') {
                SDL_Color redColor = {255, 0, 0, 255};
                filledCircle(gRenderer, centerX, centerY, CELL_SIZE / 2 - BORDER_SIZE, redColor);
            } else if (gameState[row * COLS + col] == '2') {
                SDL_Color blueColor = {0, 0, 255, 255};
                filledCircle(gRenderer, centerX, centerY, CELL_SIZE / 2 - BORDER_SIZE, blueColor);
            }
        }
    }

    SDL_RenderPresent(gRenderer);
}

int getTheMove() {

    std::string GS = getGameStateString();
    for (int i = 0 ; i < 42; i++){
        if (GS[i] == '1'){
            GS[i] = 'O';
        }
        if (GS[i] == '2'){
            GS[i] = 'X';
        }
    }
    int ourMovee = botMove(GS);
    
    return ourMovee;
}

void dropPiece(int col, int player) {
    for (int row = ROWS - 1; row >= 0; --row) {
        if (gameState[row * COLS + col] == '-') {
            gameState[row * COLS + col] = (player == PLAYER_1) ? '1' : '2';
            break;
        }
    }
}

int main(int argc, char* args[]) {
    srand(time(0)); 

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    gWindow = SDL_CreateWindow("4-in-a-Row", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    drawBoard();

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && currentPlayer == PLAYER_1) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                int col = mouseX / CELL_SIZE;

                if (col >= 0 && col < COLS && gameState[col] == '-') {
                    dropPiece(col, currentPlayer);
                    drawBoard();

                    if (checkGameWon(gameState)) {
                        std::cout << "Player " << currentPlayer << " wins!" << std::endl;
                        quit = true;
                    } else if (isBoardFull()) {
                        std::cout << "It's a draw!" << std::endl;
                        quit = true;
                    } else {
                        currentPlayer = (currentPlayer == PLAYER_1) ? PLAYER_2 : PLAYER_1;
                    }
                }
            }
        }

        if (currentPlayer == PLAYER_2 && !quit) {
            int botMove = getTheMove();
            dropPiece(botMove, currentPlayer);
            drawBoard();

            if (checkGameWon(gameState)) {
                std::cout << "Player " << currentPlayer << " wins!" << std::endl;
                quit = true;
            } else if (isBoardFull()) {
                std::cout << "It's a draw!" << std::endl;
                quit = true;
            } else {
                currentPlayer = (currentPlayer == PLAYER_1) ? PLAYER_2 : PLAYER_1;
            }
        }
    }

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();

    return 0;
}
