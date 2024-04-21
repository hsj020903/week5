#include "tetromino.h"
#include "console/console.h"


using namespace console;
using namespace std;

Tetromino::Tetromino(std::string name, int size, std::string shape) {
    name_ = name;
    size_ = size;
    int k = 0;
    for (int i = 0; i < size_; i++) {
        for (int j = 0; j < size_; j++) {
            if (shape[k++] == 'O')
                shape_[i][j] = true;
            else
                shape_[i][j] = false;
        }
    }
}

// 시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
Tetromino Tetromino::rotatedCW() {
    string s = "";
    for (int i = 0; i < size_; i++) {
        for (int j = size_ - 1; j >= 0; j--) {
            if (shape_[j][i] == true)
                s += "O";
            else
                s += "X";
        }
    }
    return Tetromino(name(), size(), s);
}

// 반시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
Tetromino Tetromino::rotatedCCW() {
    string s = "";
    for (int i = size_ - 1; i >= 0; i--) {
        for (int j = 0; j < size_; j++) {
            if (shape_[j][i] == true)
                s += "O";
            else
                s += "X";
        }
    }
    return Tetromino(name(), size(), s);
}

void Tetromino::drawAt(std::string s, int x, int y) {
    for (int i = 0; i < size(); i++) {
        for (int j = 0; j < size(); j++) {
            if (shape_[i][j] == true){
                console::draw(x+j, y+i, s);
            }
        }
    }
}
Tetromino Tetromino::I = Tetromino("I", 4, "XXXXOOOOXXXXXXXX");
Tetromino Tetromino::O = Tetromino("O", 2, "OOOO");
Tetromino Tetromino::T = Tetromino("T", 3, "XOXOOOXXX");
Tetromino Tetromino::S = Tetromino("S", 3, "XOOOOXXXX");
Tetromino Tetromino::Z = Tetromino("Z", 3, "OOXXOOXXX");
Tetromino Tetromino::J = Tetromino("J", 3, "OXXOOOXXX");
Tetromino Tetromino::L = Tetromino("L", 3, "XXOOOOXXX");