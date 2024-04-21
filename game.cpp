#include "game.h"
#include "console/console.h"
#include <iostream>
#include <time.h>

using namespace std;
using namespace console;

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#define DROP_DELAY 60

Game::Game() {
    line_clear_conut = LINES;
    count = 0;
    hold = 0;
    end = false;
    now_x = 5;
    now_y = 1;
    before_left = false;
    before_right = false;
    future_y = BOARD_HEIGHT + 1 - now_tetromino.size();
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++)
            board_[i][j] = false;
    }
    bottom_check();
    hold_count = 0;
}

// 게임의 한 프레임을 처리한다.
void Game::update() {
    count++;

    // 60프레임당 한번 작동
    if (count == DROP_DELAY) {
        count = 0;
        ++now_y;
    }

    if (key(K_ESC)) {
        end = true;
    } else if (key(K_Z)) {
        now_tetromino = now_tetromino.rotatedCCW();
    } else if (key(K_X)) {
        now_tetromino = now_tetromino.rotatedCW();
    }

    else if (key(K_LEFT)) {
        if (1 < now_x)
            now_x--;
    } else if (key(K_RIGHT)) {
        if (now_x < BOARD_WIDTH + 1 - now_tetromino.size())
            now_x++;
    }

    else if (key(K_DOWN)) {
        if (now_y < BOARD_HEIGHT)
            now_y++;
    } else if (key(K_UP)) {
        now_y = future_y + bottom;
    } else if (key(K_SPACE) && hold == 0) {
        hold = 1;
        if (hold_count == 0) {
            hold_count = 1;
            hold_tetromino = now_tetromino;
            now_tetromino = next_tetromino;
            next_tetromino = rand_tetromino();
        } else {
            Tetromino tetromino = hold_tetromino;
            hold_tetromino = now_tetromino;
            now_tetromino = tetromino;
        }
    }
    if (0 < is_underBlock(now_x, now_y) ||
        now_y == BOARD_HEIGHT + 1 + bottom - now_tetromino.size()) {
        board_add();
        now_tetromino = next_tetromino;
        next_tetromino = rand_tetromino();
        now_y = 1;
        now_x = 5;
        hold = 0;
    }
    future_tetromino_y();
    bottom_check();
    line_clear();
    if (line_clear_conut == 0) {
        console::draw(3, 11, "You Win");
        end = true;
    }
}

// 게임 화면을 그린다.
void Game::draw() {
    // 보드그리기
    console::drawBox(0, 0, BOARD_WIDTH + 1, BOARD_HEIGHT + 1);
    console::drawBox(BOARD_WIDTH + 3, 0, BOARD_WIDTH + 8, 6);
    console::drawBox(BOARD_WIDTH + 9, 0, BOARD_WIDTH + 14, 6);

    // 게임 블록 그리기
    now_tetromino.drawAt(SHADOW_STRING, now_x, future_y + bottom);
    now_tetromino.drawAt(BLOCK_STRING, now_x, now_y);
    if (hold_count == 1)
        hold_tetromino.drawAt(BLOCK_STRING, 20, 2);
    next_tetromino.drawAt(BLOCK_STRING, 14, 2);
    board_draw();

    console::draw(14, 0, "Next");
    console::draw(20, 0, "Hold");
    string s = to_string(line_clear_conut) + " lines left";
    console::draw(0, 22, s);
    
}

// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit() { return end; }

// 랜덤 tetromino
Tetromino Game::rand_tetromino() {
    srand(time(NULL));
    switch (rand() % 6) {
    case 0:
        return Tetromino::I;
    case 1:
        return Tetromino::O;
    case 2:
        return Tetromino::T;
    case 3:
        return Tetromino::S;
    case 4:
        return Tetromino::Z;
    case 5:
        return Tetromino::L;
    }
}
// 아래 혹은 tetromino 위치에 다른 블럭이 몇칸 있는지 리턴한다
int Game::is_underBlock(int x, int y) {
    int block_count = 0;
    for (int i = 0; i < now_tetromino.size(); i++) {
        for (int j = 0; j < now_tetromino.size(); j++) {
            if (now_tetromino.check(i, j)) {
                if (board_[x + i - 1][y + j - 1] == true)
                    block_count++;
                if (board_[x + i - 1][y + j] == true && y != 18)
                    block_count++;
            }
        }
    }
    return block_count;
}
// 그대로 내려갔을때 멈추게 되는 y좌표를 설정
void Game::future_tetromino_y() {
    int underBlock = -1;
    int n = -1;
    future_y = BOARD_HEIGHT + 1 - now_tetromino.size();
    while (underBlock != 0) {
        n++;
        underBlock = is_underBlock(now_x, future_y - n);
    }
    future_y -= n;
}

void Game::board_add() {
    for (int i = 0; i < now_tetromino.size(); i++) {
        for (int j = 0; j < now_tetromino.size(); j++) {
            if (now_tetromino.check(i, j)) {
                board_[now_x + j - 1][now_y + i - 1] = true;
            }
        }
    }
}

void Game::board_draw() {
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            if (board_[i][j] == true) {
                console::draw(i + 1, j + 1, BLOCK_STRING);
            }
        }
    }
}

void Game::bottom_check() {
    bottom = 0;
    for (int i = now_tetromino.size() - 1; i >= 0; i--) {
        for (int j = 0; j < now_tetromino.size(); j++) {
            if (now_tetromino.check(i, j)) {
                return;
            }
        }
        bottom++;
    }
}

void Game::line_clear() {
    int block_count = 0;
    for (int i = BOARD_HEIGHT -1; 0 < i; i--) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (board_[j][i] == true) {
                block_count++;
            }
        }
        if (block_count == BOARD_WIDTH) {
            line_clear_conut--;
            for (int h = i; 0 < h; h--) {
                for (int k = 0; k < BOARD_WIDTH; k++) {
                    board_[h][k] = board_[h][k - 1];
                }
            }
            i++;
        } else
            block_count = 0;
    }
}