#include "tetris.h"

/**
 * @brief Проверяет, находится ли блок за верхней границей игрового поля.
 *
 * Проверяет, находится ли блок за пределами верхней границы игрового поля.
 *
 * @param block Указатель на двумерный массив (матрицу) тетрамино.
 * @param block_pos Координаты x,y тетрамино.
 * @return true, если блок находится за верхней границей, false в противном
 * случае.
 */

bool isBlockBeyondLimit(int **block, coordinate block_pos) {
  bool result = false;
  if (block) {
    for (int i = 0; !result && i < BLOCK_SIZE; i++) {
      for (int j = 0; !result && j < BLOCK_SIZE; j++) {
        if (block[i][j] != ZERO && block_pos.y + i < 0) {
          result = true;
          break;
        }
      }
    }
  }

  return result;
}
/**
 * @brief Вычисляет необходимую награду по заполненным строкам.
 *
 * Вычисляет количество очков (reward), зависящее от количества уничтоженных
 * линий.
 *
 * @param number_lines Количество уничтоженных линий.
 * @return Количество очков (reward).
 */
int getRewardByCompleteLines(int number_lines) {
  int reward = 0;
  if (number_lines > 0) {
    switch (number_lines) {
      case 1:
        reward = 100;
        break;
      case 2:
        reward = 300;
        break;
      case 3:
        reward = 700;
        break;
      case 4:
        reward = 1500;
        break;

      default:
        reward = 1500;
        break;
    }
  }

  return reward;
}

/**
 * @brief Является хранилищем для структуры tetris_t.
 *
 * При передаче НЕнулевого указателя структура сохраняется. А при передачи NULL
 * сохранённая структура отдаётся. Функция необходима для передачи информации во
 * фронтенд.
 *
 * @param tetris Указатель на основную структуру логики игры.
 * @param remove Параметр для удаления сохранённой структуры.
 */
void tetrisInfoStorage(tetris_t **tetris, bool remove) {
  static tetris_t *static_tetris = NULL;
  if (!remove) {
    if (tetris) {
      if (*tetris) {
        static_tetris = *tetris;
      } else {
        *tetris = static_tetris;
      }
    }
  } else {
    static_tetris = NULL;
  }
}

/**
 * @brief Хранит в себе все виды блоков(Тетромино) и возвращает их при
 * необходимости.
 * Для того, чтобы различать фигуры в фронтенде 'заполнение' фигур
 * отличается НО всегда является положительным числом.
 * mem_cpy Копирование из одного массива в другой всех блоков.
 * @param blocks Массив двумерных фигур(блоков) тетромино. Используется для
 * возвращения в него из функции всех блоков.
 */

void getAllTetramino(int blocks[BLOCK_COUNT][BLOCK_SIZE][BLOCK_SIZE]) {
  {
    static int all_blocks[BLOCK_COUNT][BLOCK_SIZE][BLOCK_SIZE] = {
        // tetremino I
        {{ZERO, ZERO, ZERO, ZERO},
         {IBLOCK, IBLOCK, IBLOCK, IBLOCK},
         {ZERO, ZERO, ZERO, ZERO},
         {ZERO, ZERO, ZERO, ZERO}},
        // tetremino O
        {{ZERO, ZERO, ZERO, ZERO},
         {ZERO, OBLOCK, OBLOCK, ZERO},
         {ZERO, OBLOCK, OBLOCK, ZERO},
         {ZERO, ZERO, ZERO, ZERO}},
        // tetremino T
        {{ZERO, ZERO, ZERO, ZERO},
         {ZERO, TBLOCK, ZERO, ZERO},
         {TBLOCK, TBLOCK, TBLOCK, ZERO},
         {ZERO, ZERO, ZERO, ZERO}},
        // tetremino J
        {{ZERO, ZERO, ZERO, ZERO},
         {JBLOCK, ZERO, ZERO, ZERO},
         {JBLOCK, JBLOCK, JBLOCK, ZERO},
         {ZERO, ZERO, ZERO, ZERO}},
        // tetremino L
        {{ZERO, ZERO, ZERO, ZERO},
         {ZERO, ZERO, LBLOCK, ZERO},
         {LBLOCK, LBLOCK, LBLOCK, ZERO},
         {ZERO, ZERO, ZERO, ZERO}},
        // tetremino Z
        {{ZERO, ZERO, ZERO, ZERO},
         {ZERO, ZBLOCK, ZBLOCK, ZERO},
         {ZBLOCK, ZBLOCK, ZERO, ZERO},
         {ZERO, ZERO, ZERO, ZERO}},
        // tetremino S
        {{ZERO, ZERO, ZERO, ZERO},
         {SBLOCK, SBLOCK, ZERO, ZERO},
         {ZERO, SBLOCK, SBLOCK, ZERO},
         {ZERO, ZERO, ZERO, ZERO}},
    };
    memcpy(blocks, all_blocks,
           BLOCK_COUNT * BLOCK_SIZE * BLOCK_SIZE * sizeof(int));
  }
}

/**
 * @brief Возвращает случайный блок из доступных в функции
 * 'getAllTetramino'. НЕ выделяет память.
 * @param block Используется для возвращения случайно выбранного блока. Не
 * должно быть NULL.
 * @return tetrisError
 */
int getRandomTetramino(int **block) {
  if (!block) {
    return FAIL;
  }
  int all_blocks[BLOCK_COUNT][BLOCK_SIZE][BLOCK_SIZE];
  getAllTetramino(all_blocks);
  int random_index = (rand() % BLOCK_COUNT);
  for (int i = 0; i < BLOCK_SIZE; i++) {
    memcpy(block[i], all_blocks[random_index][i], BLOCK_SIZE * sizeof(int));
  }
  return SUCCESS;
}

/**
 * @brief Создание блока. Выделяем память для массива строк,
 * затем выделяем память для строки. Проверяем на выделение
 * памяти, если ошибка, то освобождаем память
 * @param block В данном случае - указатель на двумерный массив(матрицу).
 * @return tetrisError
 */
int crtTetramino(int ***block) {
  int return_result = SUCCESS;
  *block = (int **)calloc(BLOCK_SIZE, sizeof(int *));
  if (*block) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
      (*block)[i] = (int *)calloc(BLOCK_SIZE, sizeof(int));
      if (!(*block)[i]) {
        for (int j = 0; j < i; j++) {
          free((*block)[j]);
        }
        free(*block);
        *block = NULL;
        return_result = MEMORYFAIL;
        break;
      }
    }
  } else {
    return_result = MEMORYFAIL;
  }
  return return_result;
}

/**
 * @brief Освобождение памяти, выделенной под тетрамино.
 * @param block  Указатель на двумерный массив(матрицу).
 */
void rmvTetramino(int ***block) {
  if (*block != NULL) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
      free((*block)[i]);

      (*block)[i] = NULL;
    }
    free(*block);
    *block = NULL;
  }
}
/**
 * @brief Копирование блока.
 * @param block  Указатель на двумерный массив(матрицу),
 * который копируем.
 * @param block_copy Указатель на двумерный массив(матрицу),
 * куда копируем.
 */
void cpTetramino(int ***block, int ***block_copy) {
  if (block && block_copy) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
      for (int j = 0; j < BLOCK_SIZE; j++) {
        (*block_copy)[i][j] = (*block)[i][j];
      }
    }
  }
}

/**
 * @brief Поворот блока. Проверяем на != NULL.
 * Через буферный блок поворочиваем блок
 * @param left Если == true то влево.
 * @return tetrisError
 */
int rotateTetramino(int **block, char left) {
  if (!block) {
    return FAIL;
  }
  static int temp_block[BLOCK_SIZE][BLOCK_SIZE] = {};
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      temp_block[i][j] = block[(BLOCK_SIZE - 1) - j][i];
      if (left) {
        temp_block[i][j] = block[j][(BLOCK_SIZE - 1) - i];
      } else {
        temp_block[i][j] = block[(BLOCK_SIZE - 1) - j][i];
      }
    }
  }
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      block[i][j] = temp_block[i][j];
    }
  }
  return SUCCESS;
}

/**
 * @brief Cоздание игрового поля. Выделяем память для массива строк,
 * затем выделяем память для строки. Проверяем на выделение
 * @param field Указатель на двумерный массив(матрицу).
 * @return tetrisError
 */
int crtField(int ***field) {
  int return_result = SUCCESS;
  *field = (int **)calloc(ROWS_WIN, sizeof(int *));
  if (*field) {
    for (int i = 0; i < ROWS_WIN; i++) {
      (*field)[i] = (int *)calloc(COLUMNS_WIN, sizeof(int));
      if (!(*field)[i]) {
        for (int j = 0; j < i; j++) {
          free((*field)[j]);
        }
        free(*field);
        *field = NULL;
        return_result = MEMORYFAIL;
        break;
      }
    }
  } else {
    return_result = MEMORYFAIL;
  }
  return return_result;
}

/**
 * @brief свобождение памяти, выделенное под поле
 * @param field Указатель на двумерный массив(матрицу).
 */
void rmvField(int ***field) {
  if (*field != NULL) {
    for (int i = 0; i < ROWS_WIN; i++) {
      free((*field)[i]);

      (*field)[i] = NULL;
    }
    free(*field);
    *field = NULL;
  }
}

/**
 * @brief Проверяет коллизию блока с игровым полем(с его границами и
 * с внутренними блоками). В зависимости от того, с чем столкнулся блок,
 * возвращает положительное число.
 * @param block Указатель на двумерный массив(матрицу) тетрамино
 * @param field Указатель на двумерный массив(матрицу) поле
 * @param block_pos - координаты тетрмины x,y
 * @return  0 == false / 1 == true, 2 == leftBoard, 3 == rightBoard, 4 ==
 * buttomBoard.
 */
char checkingBlockCollisionWithField(int **block, coordinate block_pos,
                                     int **field) {
  if (!block || !field) {
    return 0;
  }
  char result = 0;
  for (int i = 0; !result && i < BLOCK_SIZE; i++) {
    for (int j = 0; !result && j < BLOCK_SIZE; j++) {
      if (block[i][j] != ZERO) {
        if ((block_pos.x + j >= 0 && block_pos.y + i >= 0) &&
            (block_pos.x + j < COLUMNS_WIN && block_pos.y + i < ROWS_WIN) &&
            field[block_pos.y + i][block_pos.x + j]) {
          result = 1;
        } else if (block_pos.x + j < 0) {
          result = 2;
        } else if (block_pos.x + j >= COLUMNS_WIN) {
          result = 3;
        } else if (block_pos.y + i >= ROWS_WIN) {
          result = 4;
        }
      }
    }
  }
  return result;
}

/**
 * @brief Делает копию 'field' и возвращает эту копию в 'field_copy'.
 * @param field Оригинальное игровое поле.
 * @param field_copy Копия игрового поля.
 */
void copyField(int ***field, int ***field_copy) {
  if (field && field_copy) {
    for (int i = 0; i < ROWS_WIN; i++) {
      for (int j = 0; j < COLUMNS_WIN; j++) {
        (*field_copy)[i][j] = (*field)[i][j];
      }
    }
  }
}

/**
 * @brief Вставляет блок внутрь игрового поля.
 * @param block Указатель на двумерный массив(матрицу) тетрамино
 * @param field Указатель на двумерный массив(матрицу) поле
 * @param block_pos - координаты тетрмины x,y
 */
void addBlockInField(int **block, coordinate block_pos, int **field) {
  if (block && field) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
      for (int j = 0; j < BLOCK_SIZE; j++) {
        if (block[i][j] != ZERO &&
            (block_pos.x + j >= 0 && block_pos.y + i >= 0) &&
            (block_pos.x + j < COLUMNS_WIN && block_pos.y + i < ROWS_WIN)) {
          field[block_pos.y + i][block_pos.x + j] = block[i][j];
        }
      }
    }
  }
}
/**
 * @brief Считает количество заполненных строк на игровом поле. Если строка
 * была заполнена то она обнуляется и все строки сверху сдвигаются на неё.
 * @param field Указатель на двумерный массив(матрицу) поле
 * @return количество строк.
 */
int completeLineInField(int **field) {
  int complete_lines_count = 0;
  if (field) {
    for (int i = 0; i < ROWS_WIN; i++) {
      int is_line_complete = 1;
      for (int j = 0; j < COLUMNS_WIN; j++) {
        if (field[i][j] == ZERO) {
          is_line_complete = 0;
          break;
        }
      }
      if (is_line_complete) {
        for (int j = 0; j < COLUMNS_WIN; j++) {
          field[i][j] = ZERO;
        }
        for (int j = i; j > 0; j--) {
          memcpy(field[j], field[j - 1], COLUMNS_WIN * sizeof(int));
        }
        memset(field[0], 0, COLUMNS_WIN * sizeof(int));
        ++complete_lines_count;
      }
    }
  }
  return complete_lines_count;
}

/**
 * @brief Вычисляет следующее состояние игры.
 * Главная функция, в которой происходят все процессы.
 * Каждая стадия вынесена в отдельную функцию.
 * Вычисление последующих состояний происходит внутри этих функций.
 * @param tetris Главная структура логики игры. Хранит всю необходимую
 * информацию.
 * @return tetrisError
 */
int selectNextState(tetris_t *tetris) {
  if (!tetris) {
    return FAIL;
  }
  int return_result = SUCCESS;
  switch (tetris->state) {
    case START_STATE:
      startState(tetris);
      break;
    case SPAWN_STATE:
      spawnTetramino(tetris);
      break;
    case MOVING_STATE:
      movingState(tetris);
      break;
    case ATTACHING_STATE:
      attachingState(tetris);
      break;
    case END_GAME_STATE:
      endGameState(tetris);
      break;
    case TERMINATE_STATE:
      break;
    default:
      return_result = FAIL;
      break;
  }
  return return_result;
}

/**
 * @brief START_STATE = Подготовка к новой игре.
 * Обнуление уровня и очков. Очистка игрового поля.
 * Вычисление следующего блока.
 * Переход возможен в 'SPAWN_STATE'
 * Переход будет ТОЛЬКО после нажатия кнопки 'Start'.
 * @param tetris структура логики игры.
 */
void startState(tetris_t *tetris) {
  tetris->pause = 0;
  tetris->level = LEVEL_START;
  if (tetris->last_user_input.hold &&
      (tetris->last_user_input.action == Start)) {
    tetris->level = LEVEL_MIN;
    tetris->score = 0;
    for (int i = 0; i < ROWS_WIN; i++) {
      memset(tetris->field[i], 0, COLUMNS_WIN * sizeof(int));
    }
    getRandomTetramino(tetris->next_block);
    tetris->state = SPAWN_STATE;
  }
}

/**
 * @brief SPAWNTetramino = Создание нового блока.
 * Перемещение нового блока в следующий(next),
 * а следующий в нынешний:
 * new -> next -> current.
 * Обнуление всех действий. А также вычисление будущей
 * скорости падения по уровню.
 * Переход возможен в 'MOVING_STATE'
 * @param tetris стуктура логики тетриса.
 */
void spawnTetramino(tetris_t *tetris) {
  cpTetramino(&tetris->next_block, &tetris->current_block);
  tetris->current_block_pos.x = SPAWN_X;
  tetris->current_block_pos.y = SPAWN_Y;
  getRandomTetramino(tetris->next_block);
  tetris->last_user_input.hold = false;
  tetris->is_fall_time = 0;
  tetris->speed = tetris->level;
  tetris->state = MOVING_STATE;
}

/**
 * @brief MOVING_STATE = Перемещение блока по игровому полю а также считывание
 * пользовательских действий.
 * Если блок не может упасть то он закрепляется -
 * переносится внутрь игрового поля и игра либо
 * заканчивается либо спавнится новый блок.
 * - Переход возможен в 'ATTACHING_STATE' или 'TERMINATE_STATE'
 * Переход в следущее состояние НЕ обязателен.
 * Если блок упал вниз насколько возможно то состояние переходит в
 * 'ATTACHING_STATE'. Если пользователь нажал 'Terminate'
 * то состояние переходит в 'TERMINATE_STATE'.
 * @param tetris основная стуктура логики тетриса.
 */
void movingState(tetris_t *tetris) {
  if (tetris->last_user_input.hold) {
    switch (tetris->last_user_input.action) {
      case Pause:
        tetris->pause = !tetris->pause;
        break;
      case Terminate:
        tetris->state = TERMINATE_STATE;
        break;
      case Left:
        if (tetris->pause == 0) {
          mvLeft(tetris);
        }
        break;
      case Right:
        if (tetris->pause == 0) {
          mvRight(tetris);
        }
        break;
      case Down:
        if (tetris->pause == 0) {
          mvDown(tetris);
        }
        break;
      case Action:
        if (tetris->pause == 0) {
          mvAction(tetris);
        }
        break;
      default:
        break;
    }
    tetris->last_user_input.hold = 0;
  }
  if (tetris->is_fall_time && tetris->pause == 0) {
    ++tetris->current_block_pos.y;
    tetris->is_fall_time = false;
  }
  if (checkingBlockCollisionWithField(
          tetris->current_block, tetris->current_block_pos, tetris->field)) {
    --tetris->current_block_pos.y;
    tetris->state = ATTACHING_STATE;
  }
}
/**
 * @brief Сдвгигает Блок по оси X влево. Далее проверяет на
 * столкновение с с границами поля. Если было касание -
 * обратно сдвигает блок по X вправо
 * @param tetris основная стуктура логики тетриса.
 */
void mvLeft(tetris_t *tetris) {
  --tetris->current_block_pos.x;
  if (checkingBlockCollisionWithField(
          tetris->current_block, tetris->current_block_pos, tetris->field)) {
    ++tetris->current_block_pos.x;
  }
}
/**
 * @brief Сдвгигает Блок по оси X вправо. Далее проверяет на
 * столкновение с с границами поля. Если было касание -
 * обратно сдвигает блок по X влево
 * @param tetris основная стуктура логики тетриса.
 */
void mvRight(tetris_t *tetris) {
  ++tetris->current_block_pos.x;
  if (checkingBlockCollisionWithField(
          tetris->current_block, tetris->current_block_pos, tetris->field)) {
    --tetris->current_block_pos.x;
  }
}
/**
 * @brief Если is_fall_time = true, двигает блок вниз
 * @param tetris основная стуктура логики тетриса.
 */
void mvDown(tetris_t *tetris) { tetris->is_fall_time = true; }

/**
 * @brief В зависимости от return checkingBlockCollisionWithField
 * перемещает блок вправо, влево, вниз или поворачивает
 * @param tetris основная стуктура логики тетриса.
 */
void mvAction(tetris_t *tetris) {
  rotateTetramino(tetris->current_block, 0);
  for (int i = 0; i < BLOCK_SIZE; i++) {
    const char temp_collision_return = checkingBlockCollisionWithField(
        tetris->current_block, tetris->current_block_pos, tetris->field);
    if (temp_collision_return == 1) {
      rotateTetramino(tetris->current_block, 1);
    } else if (temp_collision_return == 2) {
      ++tetris->current_block_pos.x;
    } else if (temp_collision_return == 3) {
      --tetris->current_block_pos.x;
    } else if (temp_collision_return == 4) {
      --tetris->current_block_pos.y;
      tetris->state = ATTACHING_STATE;
    }
  }
}

/**
 * @brief ATTACHING_STATE = Закрепление блока. Блок упал вниз насколько
 * возможно.
 *
 * Блок 'вставляется' внутрь игрового поля. Поле обновляется и убираются
 * заполненные строки. Прибавляются очки 'score' и, если необходимо,
 * обновляется 'HighScore'. Обновляется уровень по 'score'.
 *
 * - Переход возможен в 'SPAWN_STATE' или 'END_GAME_STATE'
 * Переход в следущее состояние НЕ обязателен.
 * Если блок находился за верхней линией то состояние переходит в
 * 'END_GAME_STATE', если нет, то в 'SPAWN_STATE'.
 *
 * @param tetris Экземпляр стуктуры логики тетриса.
 */
void attachingState(tetris_t *tetris) {
  addBlockInField(tetris->current_block, tetris->current_block_pos,
                  tetris->field);
  tetris->score += getRewardByCompleteLines(completeLineInField(tetris->field));
  setHighScore(tetris->score);
  tetris->level = (tetris->score / LEVEL_STEP) + 1;
  tetris->level = tetris->level > MAX_LEVEL ? MAX_LEVEL : tetris->level;
  if (isBlockBeyondLimit(tetris->current_block, tetris->current_block_pos)) {
    tetris->state = END_GAME_STATE;
  } else {
    tetris->state = SPAWN_STATE;
  }
}

/**
 * @brief END_GAME_STATE = Игра окончена. Игровое поле было заполнено до
 * верхней линии.
 * - Переход возможен в 'START_STATE' или 'TERMINATE_STATE'
 * @param tetris Экземпляр стуктуры логики тетриса.
 */
void endGameState(tetris_t *tetris) {
  tetris->level = LEVEL_END;
  if (tetris->last_user_input.hold) {
    if (tetris->last_user_input.action == Start) {
      tetris->state = START_STATE;
    } else if (tetris->last_user_input.action == Terminate) {
      tetris->state = TERMINATE_STATE;
    }
  }
}

/**
 * @brief Создание экземпляра структуры tetris_t. Инициализируем rand()
 * В программе может быть ТОЛЬКО один инициализированный экземпляр tetris_t.
 * Так как во время инициализации указатель на данный экземпляр сохраняется
 * 'глобально' и используется во всех функциях, где необходимо.
 * @return tetrisError
 */
int initTetris(tetris_t *tetris) {
  int return_result = FAIL;
  srand(time(NULL));
  if (tetris) {
    memset(tetris, 0, sizeof(*tetris));
    {
      crtField(&tetris->field);
      crtField(&tetris->preview_field);
      crtTetramino(&tetris->current_block);
      crtTetramino(&tetris->next_block);
    }
    tetris->is_game_start = false;
    tetris->state = START_STATE;
    tetris->level = LEVEL_START;
    tetris->last_user_input.action = Start;
    tetris->last_user_input.hold = false;
    tetrisInfoStorage(&tetris, false);
    return_result = SUCCESS;
  }
  return return_result;
}

/**
 * @brief Удаление экземпляра структуры tetris_t.
 * Заносим в хранилище
 */
void removeTetris(tetris_t *tetris) {
  if (tetris) {
    rmvField(&tetris->field);
    rmvField(&tetris->preview_field);
    rmvTetramino(&tetris->current_block);
    rmvTetramino(&tetris->next_block);
    memset(tetris, 0, sizeof(*tetris));
    tetrisInfoStorage(&tetris, true);
  }
}

/**
 * @brief Принимается пользовательский ввод.
 * @param hold Нажата ли сейчас клавиша
 */
void userInput(UserActionButton action, bool hold) {
  tetris_t *tetris = NULL;
  tetrisInfoStorage(&tetris, false);
  if (tetris) {
    tetris->last_user_input.action = action;
    tetris->last_user_input.hold = hold;
  }
}

/**
 * @brief Обновление состаяния игры
 * @param tetris Главная структура логики игры. Хранит всю необходимую
 * информацию.
 * @return Структура, которая хранит краткое сведение об игре и необходима для
 * отображения текущего состояния игры.
 */
GameInfo_tetris updCurrState(void) {
  GameInfo_tetris return_game_info = {};
  tetris_t *tetris = NULL;
  tetrisInfoStorage(&tetris, false);
  if (tetris && selectNextState(tetris) == SUCCESS) {
    return_game_info = crtGameInfoTetris(tetris);
  }
  return return_game_info;
}

/**
 * @brief Создание/составление структуры GameInfo_tetris c помощью главной
 * структуры tetris_t. Перенос необходимой информации из одной структуры в
 * другую. Является частью функции 'updCurrState'
 * @param tetris Главная структура логики игры. Хранит всю необходимую
 * информацию.
 */
GameInfo_tetris crtGameInfoTetris(tetris_t *tetris) {
  GameInfo_tetris return_game_info = {};
  if (tetris) {
    copyField(&tetris->field, &tetris->preview_field);
    addBlockInField(tetris->current_block, tetris->current_block_pos,
                    tetris->preview_field);
    return_game_info.field = tetris->preview_field;
    return_game_info.next = (int **)tetris->next_block;
    return_game_info.high_score = tetris->high_score;
    return_game_info.score = tetris->score;
    return_game_info.level = tetris->level;
    return_game_info.speed = tetris->speed;
    return_game_info.pause = tetris->pause;
  }
  return return_game_info;
}

/**
 * @brief Проверка на конец игры
 */
bool isGameEnd(void) {
  tetris_t *tetris = NULL;
  tetrisInfoStorage(&tetris, false);
  return tetris ? tetris->state == TERMINATE_STATE : true;
}

/**
 * @brief Иницилизация начало игры.
 */
void gameStart(void) {
  tetris_t *tetris = NULL;
  tetrisInfoStorage(&tetris, false);

  if (tetris) {
    tetris->is_game_start = true;
  }
}

/**
 * @brief Обновление рекорда
 * @param score если рекорд не обновлён то ничего не делает
 */
void setHighScore(int score) {
  tetris_t *tetris = NULL;
  tetrisInfoStorage(&tetris, false);

  if (tetris && score > tetris->high_score) {
    tetris->high_score = score;
  }
}
/**
 * @brief Работа с рекордом
 */
int getHighScore(void) {
  tetris_t *tetris = NULL;
  tetrisInfoStorage(&tetris, false);
  return tetris ? tetris->high_score : 0;
}
/**
 * @brief pause true\false
 */
bool isPause(void) {
  bool result = 0;
  tetris_t *tetris = NULL;
  tetrisInfoStorage(&tetris, false);
  if (tetris) {
    result = !!tetris->pause;
  }
  return result;
}
