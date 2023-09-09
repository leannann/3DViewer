/*!
\file
\brief Заголовочный файл с объявлениям класса Affine который представляет
собой модель для выполнения aффинных преобразований.
*/

#ifndef CPP4_3DVIEWER_V2_VIEWER_AFFINE_H_
#define CPP4_3DVIEWER_V2_VIEWER_AFFINE_H_

#include "model.h"

/**
 * @brief Пространство имён s21 содержит класс Аffine, View,
 * Controller, Model, Paint.
 */

namespace s21 {
/**
 * @brief Класс для аффинных преобразований трехмерных моделей.
 */
class Affine {
 public:
  /**
   * @brief Применяет аффинное преобразование к модели.
   * @param transform_data Матрица аффинного преобразования размером 3x3.
   *                      Первый столбец - сдвиги по осям X, Y и Z.
   *                      Второй столбец - углы поворота вокруг осей X, Y и Z (в
   * радианах). Третий столбец - коэффициент масштабирования.
   * @note Функция последовательно применяет сдвиги, повороты и масштабирование
   * к модели.
   */
  inline void affineTransform(double transform_data[3][3]) noexcept {
    movingX(transform_data[0][0]);
    movingY(transform_data[0][1]);
    movingZ(transform_data[0][2]);
    rotationX(transform_data[1][0]);
    rotationY(transform_data[1][1]);
    rotationZ(transform_data[1][2]);
    scaling(transform_data[2][0]);
  }

  /**
   * @brief Масштабирует модель на заданный коэффициент.
   * @param a Коэффициент масштабирования.
   * @note Если a > 0, модель увеличится, иначе уменьшится.
   */
  void scaling(double a) noexcept;

  /**
   * @brief Сдвигает все вершины модели вдоль оси X на заданное расстояние.
   * @param a Расстояние для сдвига.
   */
  void movingX(double a) noexcept;

  /**
   * @brief Сдвигает все вершины модели вдоль оси Y на заданное расстояние.
   * @param a Расстояние для сдвига.
   */
  void movingY(double a) noexcept;

  /**
   * @brief Сдвигает все вершины модели вдоль оси Z на заданное расстояние.
   * @param a Расстояние для сдвига.
   */
  void movingZ(double a) noexcept;

  /**
   * @brief Поворачивает модель вокруг оси X на заданный угол (в радианах).
   * @param a Угол поворота в радианах.
   */
  void rotationX(double a) noexcept;

  /**
   * @brief Поворачивает модель вокруг оси Y на заданный угол (в радианах).
   * @param a Угол поворота в радианах.
   */
  void rotationY(double a) noexcept;

  /**
   * @brief Поворачивает модель вокруг оси Z на заданный угол (в радианах).
   * @param a Угол поворота в радианах.
   */
  void rotationZ(double a) noexcept;

 private:
  Model& model = Model::getInstance(); /**< Ссылка на экземпляр класса Model. */
};
}  // namespace s21
#endif  // CPP4_3DVIEWER_V2_VIEWER_AFFINE_H_
