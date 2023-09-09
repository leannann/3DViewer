#include "affine.h"

/**
 * @brief Сдвигает все вершины модели вдоль оси X на заданное расстояние.
 * @param a Расстояние для сдвига.
 */
void s21::Affine::movingX(double a) noexcept {
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i)
    model.viewer.matrix_of_vertexes.matrix[i][0] += a;
}

/**
 * @brief Сдвигает все вершины модели вдоль оси Y на заданное расстояние.
 * @param a Расстояние для сдвига.
 */
void s21::Affine::movingY(double a) noexcept {
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i)
    model.viewer.matrix_of_vertexes.matrix[i][1] += a;
}

/**
 * @brief Сдвигает все вершины модели вдоль оси Z на заданное расстояние.
 * @param a Расстояние для сдвига.
 */
void s21::Affine::movingZ(double a) noexcept {
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i)
    model.viewer.matrix_of_vertexes.matrix[i][2] += a;
}

/**
 * @brief Поворачивает модель вокруг оси X на заданный угол (в радианах).
 * @param a Угол поворота в радианах.
 */
void s21::Affine::rotationX(double a) noexcept {
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i) {
    double temp_y = model.viewer.matrix_of_vertexes.matrix[i][1];
    double temp_z = model.viewer.matrix_of_vertexes.matrix[i][2];
    model.viewer.matrix_of_vertexes.matrix[i][1] =
        cos(a) * temp_y - sin(a) * temp_z;
    model.viewer.matrix_of_vertexes.matrix[i][2] =
        sin(a) * temp_y + cos(a) * temp_z;
  }
}

/**
 * @brief Поворачивает модель вокруг оси Y на заданный угол (в радианах).
 * @param a Угол поворота в радианах.
 */
void s21::Affine::rotationY(double a) noexcept {
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i) {
    double temp_x = model.viewer.matrix_of_vertexes.matrix[i][0];
    double temp_z = model.viewer.matrix_of_vertexes.matrix[i][2];
    model.viewer.matrix_of_vertexes.matrix[i][0] =
        cos(a) * temp_x + sin(a) * temp_z;
    model.viewer.matrix_of_vertexes.matrix[i][2] =
        -sin(a) * temp_x + cos(a) * temp_z;
  }
}

/**
 * @brief Поворачивает модель вокруг оси Z на заданный угол (в радианах).
 * @param a Угол поворота в радианах.
 */
void s21::Affine::rotationZ(double a) noexcept {
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i) {
    double temp_x = model.viewer.matrix_of_vertexes.matrix[i][0];
    double temp_y = model.viewer.matrix_of_vertexes.matrix[i][1];
    model.viewer.matrix_of_vertexes.matrix[i][0] =
        cos(a) * temp_x - sin(a) * temp_y;
    model.viewer.matrix_of_vertexes.matrix[i][1] =
        sin(a) * temp_x + cos(a) * temp_y;
  }
}

/**
 * @brief Масштабирует модель на заданный коэффициент.
 * @param a Коэффициент масштабирования.
 * @note Если a > 0, модель увеличится, иначе уменьшится.
 */
void s21::Affine::scaling(double a) noexcept {
  if (a > 0)
    for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i)
      for (unsigned int j = 0; j < model.viewer.matrix_of_vertexes.columns; ++j)
        model.viewer.matrix_of_vertexes.matrix[i][j] *= a;
}
