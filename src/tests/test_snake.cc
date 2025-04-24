#include "test_snake.h"

s21::SnakeLogic logic;

// Тест на инициализацию игры
TEST(SnakeLogicTest, InitGame) {
  logic.initGame();
  EXPECT_EQ(logic.getGameInfo().score, 0);
  EXPECT_EQ(logic.getGameInfo().level, 1);
  EXPECT_NE(logic.getSnake().size(), 0U);
}

// Тест на создание матрицы
TEST(SnakeLogicTest, CreateMatrixField) {
  int** field = logic.createMatrixField();
  ASSERT_NE(field, nullptr);
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      EXPECT_EQ(field[i][j], 0);
    }
  }

  for (int i = 0; i < FIELD_HEIGHT; i++) {
    delete[] field[i];
  }
  delete[] field;
}

// Тест на движение змейки
TEST(SnakeLogicTest, MoveSnake) {
  logic.initGame();
  s21::Coordinates head = logic.getSnake().front();
  logic.moveSnake();
  s21::Coordinates newHead = logic.getSnake().front();

  EXPECT_EQ(newHead.y, head.y - 1);
}

// Тест на генерацию яблока
TEST(SnakeLogicTest, GenerateApple) {
  logic.initGame();
  s21::Coordinates apple = logic.apple;
  logic.generateApple();
  s21::Coordinates appleNew = logic.apple;

  EXPECT_NE(apple.x, appleNew.x);
  EXPECT_NE(apple.y, appleNew.y);
  EXPECT_EQ(logic.getGameInfo().field[appleNew.y][appleNew.x], 1);
}

// Тесты на столкновение
TEST(SnakeLogicTest, CheckCollision) {
  for (int i = 0; i < 100; i++) {
    logic.moveSnake();
  }
  logic.rotateDirection(1);
  logic.moveSnake();
  logic.rotateDirection(2);
  logic.moveSnake();
  logic.rotateDirection(3);
  logic.moveSnake();
  logic.rotateDirection(0);
  logic.moveSnake();
  logic.moveSnake();
  EXPECT_EQ(logic.getGameState(), GAME_OVER);
}
// столкновение сама с собой
TEST(SnakeLogicTest, CheckCollision2) {
  logic.initGame();

  logic.body.clear();
  logic.body.push_back({5, 5});
  logic.body.push_back({5, 6});
  logic.body.push_back({6, 6});
  logic.body.push_back({7, 6});
  logic.body.push_back({7, 5});
  logic.body.push_back({7, 4});
  logic.body.push_back({6, 4});
  logic.body.push_back({5, 4});

  EXPECT_FALSE(logic.checkCollision());
}

// с бортами
TEST(SnakeLogicTest, CheckCollisionWithBorders) {
  logic.getGameState() = GAME_MOVING;
  logic.rotateDirection(1);
  for (size_t i = 0; i < 5; i++) {
    logic.moveSnake();
  }
  EXPECT_EQ(logic.getGameState(), GAME_OVER);
}

// Тест на победу
TEST(SnakeLogicTest, CheckVictory) {
  logic.initGame();
  logic.body = std::vector<s21::Coordinates>(200);
  logic.checkVictory();
  EXPECT_EQ(logic.getGameState(), GAME_WON);
}

// Тест на уровень и скорость
TEST(SnakeLogicTest, UpdateLevel) {
  logic.gameInfo_.score = 4;
  logic.updateLevel();
  EXPECT_EQ(logic.gameInfo_.level, 1);
  EXPECT_EQ(logic.gameInfo_.speed, 1);

  logic.gameInfo_.score = 5;
  logic.updateLevel();
  EXPECT_EQ(logic.gameInfo_.level, 2);
  EXPECT_EQ(logic.gameInfo_.speed, 2);

  logic.gameInfo_.score = 10;
  logic.updateLevel();
  EXPECT_EQ(logic.gameInfo_.level, 3);
  EXPECT_EQ(logic.gameInfo_.speed, 3);

  logic.gameInfo_.score = 55;
  logic.updateLevel();
  EXPECT_EQ(logic.gameInfo_.level, 10);
  EXPECT_EQ(logic.gameInfo_.speed, 10);
}

// сохр рекорда
TEST(SnakeLogicTest, SaveGameScore) {
  logic.initGame();

  logic.gameInfo_.score = logic.gameInfo_.high_score + 1;

  logic.saveGameScore();

  EXPECT_EQ(logic.gameInfo_.high_score, logic.gameInfo_.score);

  std::ifstream file("snake_score.txt");
  int savedScore;
  file >> savedScore;
  EXPECT_EQ(savedScore, logic.gameInfo_.score);
  file.close();
}

// тест на сеттер
TEST(SnakeLogicTest, SetGameState) {
  logic.setGameState(GameState_t::GAME_PAUSED);
  EXPECT_EQ(logic.getGameState(), GameState_t::GAME_PAUSED);

  logic.setGameState(GameState_t::GAME_START);
  EXPECT_EQ(logic.getGameState(), GameState_t::GAME_START);

  logic.setGameState(GameState_t::GAME_OVER);
  EXPECT_EQ(logic.getGameState(), GameState_t::GAME_OVER);
}

// чтение
TEST(SnakeLogicTest, ReadGameScore) {
  {
    std::ofstream testFile("snake_score.txt");
    testFile << 42;
    testFile.close();
  }

  int score = logic.readGameScore();

  EXPECT_EQ(score, 42);
  EXPECT_EQ(logic.getGameInfo().high_score, 42);

  std::remove("snake_score.txt");

  std::remove("snake_score.txt");

  score = logic.readGameScore();

  EXPECT_EQ(score, 0);
  EXPECT_EQ(logic.getGameInfo().high_score,
            42);  // high_score не должен измениться
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
