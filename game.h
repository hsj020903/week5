
#ifndef GAME_H
#define GAME_H

#include "tetromino.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 40

#define DROP_DELAY 60

class Game {
  private:
    // 게임 판을 나타내는 배열
    // board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을
    // 의미한다
    bool board_[BOARD_WIDTH][BOARD_HEIGHT];
    int count; // DROP_DELAY 프레임마다 작동하기 위한 카운트
    int line_clear_conut;
    int end;   // 게임의 상태
    int now_x, now_y; // tetromino의 좌표를 저장
    int future_y; // tetromino가 그대로 내려갔을 때의 좌표를 저장
    int bottom; //아래 비어있는 줄의 수
    int hold_count;
    int hold;
    bool before_left;  // 이전에 좌측으로 이동하였나?
    bool before_right; // 이전에 우측으로 이동하였나?
  
    Tetromino now_tetromino = rand_tetromino(); // 현재 tetromino
    Tetromino next_tetromino = rand_tetromino(); // 다음 tetromino
    Tetromino hold_tetromino = rand_tetromino(); // 다음 tetromino
    // 랜덤한 tetromino를 리턴한다
    Tetromino rand_tetromino();

    // 아래 혹은 tetromino 위치에 다른 블럭이 몇칸 있는지 리턴한다
    int is_underBlock(int x, int y); 


    // 그대로 내려갔을때 멈추게 되는 y좌표를 리턴한다
    void future_tetromino_y();

    //보드에 블럭추가
    void board_add();

    //보드 그리기
    void board_draw();

    //bottom 초기화
    void bottom_check();

    //꽉찬 라인을 지운다
    void line_clear();




  public:
    // 게임의 한 프레임을 처리한다.
    void update();

    // 게임 화면을 그린다.
    void draw();

    // 게임 루프가 종료되어야 하는지 여부를 반환한다.
    bool shouldExit();

    // 생성자
    Game();
};
#endif