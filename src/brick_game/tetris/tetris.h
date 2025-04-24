/**
 * @mainpage micheilb
 *
 * @section intro_sec Введение
 * Этот проект является объединением двух проектов. Предыдущего с языка С игрой
 * Тетрис и нового на языке С++ Змейка
 *
 * @section arch_sec Архитектура проекта
 * Архетектура представляет из себя реализацию MVC Паттерн.
 *
 */
#ifndef C7_BRICKGAME_V1_0_1_TETRIC_H
#define C7_BRICKGAME_V1_0_1_TETRIC_H
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

/**
 * @brief Координаты объекта по x,y
 * используется для логики и отображения
 * тетрамино
 * \details Подробное описание
 */
typedef struct {
  int x;
  int y;
} coordinate;

typedef struct {
  WINDOW *game_win;
  coordinate game_win_size;
  coordinate game_win_pos;
  WINDOW *stat_win;
  coordinate stat_win_size;
  coordinate stat_win_pos;

} cliTetris;

/**
 * @brief Общие константы
 */
#define ROWS_WIN 20
#define COLUMNS_WIN 10
#define BLOCK_SIZE 4
#define BLOCK_COUNT 7

/**
 * @brief Уровни и "скорость"
 * @param LEVEL_MIN - минимальный уровень
 * @param LEVEL_START - для фронтенда, чтобы показать
 * загрузочное окно
 * @param LEVEL_END - уровень для фронденда, чтобы показать
 * конец игры
 * @param DEFAULT_FALL_TIME_MS - Стандартное время падения блока
 * @param SPEED_STEP_TIME_MS - Шаг увеличения скорости с каждым уровнем
 */
#define LEVEL_MIN 1
#define LEVEL_START -1
#define LEVEL_END -2
#define DEFAULT_FALL_TIME_MS 1070
#define SPEED_STEP_TIME_MS 70

/**
 * @brief Бекенд константы
 * @param SPAWN_X - Координата появление фигуры сверху
 * @param SPAWN_Y - Координата появление фигуры сверху
 * @param LEVEL_STEP - необходимый счёт, чтобы перейти на след. уровень
 * @param MAX_LEVEL - максимальный уровень сложности
 */
#define SPAWN_X (COLUMNS_WIN / 2 - BLOCK_SIZE / 2)
#define SPAWN_Y -2
#define LEVEL_STEP 600
#define MAX_LEVEL 10

/**
 * @brief Фронтенд константы
 * @param WIN_X - размер границы
 * @param WIN_Y - размер границы
 * @param VIEW_CHAR_SIZE_X - корректировка для отображение
 * @param VIEW_CHAR_SIZE_Y - отображение Y символов
 * @param EMPTY_SYMBOL - пустой символ
 * @param FULL_SYMBOL - заполненный символ для отрисовки границ
 */

#define WIN_X 1
#define WIN_Y 1
#define VIEW_CHAR_SIZE_X 2
#define VIEW_CHAR_SIZE_Y 1

#define EMPTY_SYMBOL ' '
#define FULL_SYMBOL '|'

#include <inttypes.h>
#include <ncurses.h>
#include <stdbool.h>

// --- Общие объекты -------------|
/**
 * @brief Все кнопки через которые происходит взаимодействия игрока с программой
 */
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserActionButton;

/**
 * @brief Основная структура, в которой хранятся
 * все данные о текущей игре
 */
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_tetris;

/**
 * @brief Структура для передачи "зажимания" клавишы
 * пользователем
 */
typedef struct {
  UserActionButton action;
  bool hold;
} UserInput_t;

/**
 * @brief Структура для заполнения тетрамино случайным
 * паттерном
 */
typedef enum {
  ZERO = 0,
  FULL = 1,
  IBLOCK = 2,
  OBLOCK = 3,
  TBLOCK = 4,
  JBLOCK = 5,
  LBLOCK = 6,
  ZBLOCK = 7,
  SBLOCK = 8,
} cageTetis;

// --- Бекенд объекты -------------|

/// @brief Все состояния игры.
typedef enum {
  START_STATE,
  SPAWN_STATE,
  MOVING_STATE,
  ATTACHING_STATE,
  END_GAME_STATE,
  TERMINATE_STATE,
} tetrisState;

/**
 * @brief Эта структура хранит всю информацию о игровой логике
 * is_game_start - true/false началась ли игра
 * tetrisState state - Текущее состояние игры
 * **field - Основное игровое поле
 * **preview_field -   Временное игровое поле, которое используется
 * для того, чтобы отдавать в фронтенд. (Соединяя с блоком на каждом кадре)
 * **current_block - Текущий блок (Блок не привязан к игровому полю и не
 * находится в нём)
 * **current_block -   Позиция текущего блока (Блок не привязан к игровому
 * полю и не находится в нём)
 * **next_block; - Следующий блок (Просто отображение)
 * UserInput_t last_user_input - Последние действия пользователя
 * bool is_fall_time - Нужно ли в следующем кадре упасть блоку.
 */
typedef struct {
  bool is_game_start;
  tetrisState state;
  int **field;
  int **preview_field;
  int **current_block;
  coordinate current_block_pos;
  int **next_block;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  UserInput_t last_user_input;
  bool is_fall_time;
} tetris_t;

/**
 *  @brief Все виды ошибок.
 * SUCCESS - успех
 * FAIL - ошибка
 * MEMORYFAIL - ошибка памяти
 */
typedef enum {
  SUCCESS,
  FAIL,
  MEMORYFAIL,
} tetrisError;

// --- Фронтенд объекты -------------|

/**
 * @brief Эта структура хранит всю информацию о отображении игры в консоли.
 * WINDOW *game_win - окно игрового поля
 * WINDOW *stat_win - окно статистики игры
 */

/**
 *@brief Цвета,
 * Цвета для ncurses;
 */
typedef enum {
  ColorGray = 100,
  ColorRed = 101,
  ColorOrange = 102,
  ColorYellow = 103,
  ColorPink = 104,
  ColorGreen = 105,
  ColorBlue = 106,
  ColorPurple = 107,
} cli_colors_t;

//

typedef struct {
  int **field;     // игровое поле
  int **next;      // следующая фигура
  int score;       // текущий счет
  int high_score;  // лучший счет
  int level;       // текущий уровень
  int speed;       // скорость падения фигур
  int pause;       // флаг паузы
} GameInfo_t;

typedef struct {
  int x;
  int y;
  int **matrix;
} Figure;

// Перечисление для состояний игры
typedef enum {
  GAME_PRESTART,
  GAME_START,
  GAME_SPAWN,
  GAME_MOVING,
  GAME_ATTACHING,
  GAME_OVER,
  GAME_PAUSED,
  GAME_EXIT,
  GAME_WON
} GameState_t;

void rotate_figure(Figure *figure, GameInfo_t *gameInfo);

// ALL Methods

int initTetris(tetris_t *tetris);
void removeTetris(tetris_t *tetris);

void gameStart(void);
bool isGameEnd(void);

void userInput(UserActionButton action, bool hold);

GameInfo_tetris updCurrState(void);
GameInfo_tetris crtGameInfoTetris(tetris_t *tetris);

void setHighScore(int score);
int getHighScore(void);

bool isPause(void);

int crtTetramino(int ***block);
void rmvTetramino(int ***block);

void cpTetramino(int ***block, int ***block_copy);
int rotateTetramino(int **block, char left);
void getAllTetramino(int blocks[BLOCK_COUNT][BLOCK_SIZE][BLOCK_SIZE]);
int getRandomTetramino(int **block);

int crtField(int ***field);
void rmvField(int ***field);
void copyField(int ***field, int ***field_copy);
char checkingBlockCollisionWithField(int **block, coordinate block_pos,
                                     int **field);
void addBlockInField(int **block, coordinate block_pos, int **field);
int completeLineInField(int **field);

int getRewardByCompleteLines(int number_lines);
bool isBlockBeyondLimit(int **block, coordinate block_pos);
void tetrisInfoStorage(tetris_t **tetris, bool remove);

int selectNextState(tetris_t *tetris);
void startState(tetris_t *tetris);
void spawnTetramino(tetris_t *tetris);
void movingState(tetris_t *tetris);
void mvLeft(tetris_t *tetris);
void mvRight(tetris_t *tetris);
void mvDown(tetris_t *tetris);
void mvAction(tetris_t *tetris);
void attachingState(tetris_t *tetris);
void endGameState(tetris_t *tetris);

void InitCli(cliTetris *cli);
void initCurses(void);
void removeCli(cliTetris *cli);
void crtWin(WINDOW **window, coordinate window_size, coordinate window_pos);
UserInput_t getUserInput(void);
void printCurrentState(GameInfo_tetris game_info);
void statWinPrint(cliTetris *cli, GameInfo_tetris *game_info);
void sliInfoStorage(cliTetris **cli, bool remove);
void printFieldCeil(WINDOW *win, chtype symbol);
void printStartPreview();
void printEndPreview();
void printPausePreview();

void main_loop(tetris_t *tetris);
int **create_matrix(int rows, int cols);
void generate_next_figure(GameInfo_t *gameInfo);
void move_figure_down(Figure *figure, GameInfo_t *gameInfo);
void rotate_figure(Figure *figure, GameInfo_t *gameInfo);
bool is_figure_collide(Figure *figure, GameInfo_t *gameInfo);
#endif  // C7_BRICKGAME_V1_0_1_TETRIC_H
