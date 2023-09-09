/*!
\file
\brief Заголовочный файл с объявлениям класса Controller для управления
моделью и преобразованиями над ней.
*/

#ifndef CPP4_3DVIEWER_V2_VIEWER_CONTROLLER_H_
#define CPP4_3DVIEWER_V2_VIEWER_CONTROLLER_H_

#include "affine.h"
#include "model.h"

namespace s21 {

/**
 * @brief Класс контроллера для управления моделью и преобразованиями.
 *
 * Этот класс представляет собой контроллер, который обеспечивает управление
 * моделью и выполнение преобразований над ней, таких как масштабирование и
 * установка в центр виджета.
 */
class Controller {
 public:
  /**
   * @brief Установка модели в центр виджета.
   *
   * Этот метод вызывает метод setInCenter() модели для установки модели в центр
   * виджета.
   */
  inline void setInCenter() const noexcept { model.setInCenter(); }

  /**
   * @brief Загрузка и обработка модели из файла .obj.
   *
   * Этот метод вызывает метод coreParser() модели для загрузки и обработки
   * модели из файла формата .obj.
   *
   * @param file_name Путь к файлу .obj, содержащему модель.
   */
  inline void coreParser(const char *file_name) noexcept {
    model.coreParser(file_name);
  }

  /**
   * @brief Выполнение аффинных преобразований над моделью.
   *
   * Этот метод вызывает метод affineTransform() объекта `Affine` для выполнения
   * аффинных преобразований над моделью.
   *
   * @param transform_data Матрица 3x3 с данными преобразования.
   */
  inline void s21_affine_transform(double transform_data[3][3]) noexcept {
    affine.affineTransform(transform_data);
  }

  /**
   * @brief Масштабирование модели.
   *
   * Этот метод вызывает метод scaling() объекта `Affine` для масштабирования
   * модели.
   *
   * @param scaleFactor Фактор масштабирования.
   */
  inline void s21_scaling(float scaleFactor) noexcept {
    affine.scaling(scaleFactor);
  }

 private:
  Model &model = Model::getInstance();  ///< Ссылка на экземпляр модели.
  Affine
      affine;  ///< Объект для выполнения аффинных преобразований над моделью.
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_VIEWER_CONTROLLER_H_
