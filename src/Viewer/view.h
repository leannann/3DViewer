/*!
\file
\brief Заголовочный файл с объявлениям класса view который отвечает за
визуализацию модели,
 * управление настройками интерфейса и создание GIF-анимации и класс Paint для
отображения 3D-модели и управления ей.
*/

#ifndef CPP4_3DVIEWER_V2_VIEWER_VIEW_H_
#define CPP4_3DVIEWER_V2_VIEWER_VIEW_H_

#include <QImage>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QSettings>

#include "controller.h"
#include "model.h"
#include "qgifimage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

namespace s21 {
/**
 * @brief Класс представления модели в приложении.
 *
 * Этот класс является главным окном приложения и отвечает за визуализацию
 * модели, управление настройками интерфейса и создание GIF-анимации.
 */
class View : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор класса View.
   *
   * @param parent Родительский виджет (по умолчанию - nullptr).
   */
  View(QWidget *parent = nullptr);

  /**
   * @brief Деструктор класса View.
   */
  ~View();

 signals:
  /**
   * @brief Сигнал для передачи настроек интерфейса.
   *
   * Этот сигнал используется для передачи настроек интерфейса, выбранных
   * пользователем, другим частям приложения.
   *
   * @param map Карта настроек интерфейса в формате "параметр - значение".
   */
  void signal_settings(QMap<QString, QString> map);

  /**
   * @brief Сигнал для передачи данных для аффинных преобразований.
   *
   * Этот сигнал используется для передачи матрицы данных для аффинных
   * преобразований, выбранных пользователем, другим частям приложения.
   *
   * @param transform_data Матрица данных для аффинных преобразований размером
   * 3x3.
   */
  void signal_data(double transform_data[3][3]);

 public slots:
  /**
   * @brief Слот для получения информации о модели.
   *
   * Этот слот вызывается для отображения информации о модели, такой как
   * количество вершин, количество полигонов и имя файла модели.
   *
   * @param verticesСount Количество вершин в модели.
   * @param polygonsСount Количество полигонов в модели.
   * @param fileName Имя файла модели.
   */
  void receiveInfo(int verticesСount, int polygonsСount,
                   QString fileName) noexcept;

 private slots:
  /**
   * @brief Слот для обработки нажатия кнопки "Преобразовать".
   *
   * Этот слот вызывается при нажатии кнопки "Преобразовать модель" и собирает
   * данные для аффинных преобразований в двумерный массив.
   */
  void on_transformButton_clicked() noexcept;

  /**
   * @brief Слот для обработки нажатия кнопки "Применить настройки".
   *
   * Этот слот вызывается при нажатии кнопки "Применить настройки" и обновляет
   * интерфейс в соответствии с выбранными пользователем настройками.
   */
  void on_applySettings_clicked() noexcept;

  /**
   * @brief Слот для обработки нажатия кнопки "Сохранить изображение".
   *
   * Этот слот вызывается при нажатии кнопки "Сохранить изображение" и сохраняет
   * изображение модели в форматах JPEG или BMP, в зависимости от выбора
   * пользователя.
   */
  void on_saveImage_clicked() noexcept;

  /**
   * @brief Слот для обработки нажатия кнопки "Записать GIF".
   *
   * Этот слот вызывается при нажатии кнопки "Записать GIF" и начинает процесс
   * создания GIF-анимации изображения модели.
   */
  void on_recordScreencast_clicked();

  /**
   * @brief Слот для сохранения кадров и создания GIF-анимации.
   *
   * Этот слот вызывается таймером каждые 50 миллисекунд в процессе записи
   * GIF-анимации. Он увеличивает счетчик кадров, захватывает текущий кадр из
   * виджета и добавляет его в GIF-анимацию. По достижении максимального
   * количества сохраненных кадров (50), запись GIF-анимации останавливается и
   * пользователю предлагается выбрать место для сохранения.
   */
  void save() noexcept;

 private:
  Ui::View *ui; /**< Указатель на интерфейс главного окна. */
  QSettings *set; /**< Указатель на настройки приложения. */
  QGifImage *gif; /**< Указатель на GIF-анимацию. */
  QImage *frame;  /**< Указатель на кадр анимации. */
  QTimer *timer; /**< Указатель на таймер записи GIF-анимации. */
  int count; /**< Счетчик сохраненных кадров GIF-анимации. */
};

/**
 * @brief Класс для отображения 3D-модели и управления ей.
 *
 * Класс Paint наследуется от QOpenGLWidget и предоставляет функциональность для
 * отображения 3D-модели и управления ей, включая вращение, масштабирование и
 * изменение настроек интерфейса.
 */
class Paint : public QOpenGLWidget {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор класса Paint.
   *
   * @param[in] parent Родительский виджет (по умолчанию - nullptr).
   */
  Paint(QWidget *parent = nullptr);

  /**
   * @brief Деструктор класса Paint.
   *
   * Освобождает ресурсы модели при уничтожении объекта.
   */
  ~Paint() { model.releaseResources(); }

  /**
   * @brief Отрисовать линии объекта.
   */
  void drawLines() noexcept;

  /**
   * @brief Отрисовать точки объекта.
   */
  void drawPoints() noexcept;

  /**
   * @brief Отрисовать оси координат.
   */
  void drawAxis() noexcept;

 public slots:
  /**
   * @brief Установить вращение по оси X.
   *
   * @param[in] angle Угол вращения.
   */
  void setXRotation(int angle) noexcept;

  /**
   * @brief Установить вращение по оси Y.
   *
   * @param[in] angle Угол вращения.
   */
  void setYRotation(int angle) noexcept;

  /**
   * @brief Установить вращение по оси Z.
   *
   * @param[in] angle Угол вращения.
   */
  void setZRotation(int angle) noexcept;

  /**
   * @brief Обработчик нажатия кнопки преобразования модели.
   *
   * @param[in] transform_data Матрица преобразования 3x3.
   */
  void on_transformButton_clicked2(double transform_data[3][3]) noexcept;

  /**
   * @brief Обработчик нажатия кнопки выбора файла модели.
   */
  void on_SelectFileButton_clicked() noexcept;

  /**
   * @brief Обработчик нажатия кнопки проверки отображения осей.
   */
  void on_checkAxes_clicked() noexcept;

  /**
   * @brief Обработчик нажатия кнопки применения настроек интерфейса.
   *
   * @param[in] map Карта настроек с ключами и значениями.
   */
  void on_applySettingsButton_clicked(QMap<QString, QString> map) noexcept;

 signals:
  /**
   * @brief Сигнал, отправляемый для отображения информации о модели.
   *
   * @param[in] vertices_count Количество вершин в модели.
   * @param[in] polygons_count Количество полигонов в модели.
   * @param[in] f_name Имя файла модели.
   */
  void send_info(int vertices_count, int polygons_count, QString f_name);

 protected:
  /**
   * @brief Переопределенная функция отрисовки сцены.
   */
  void paintGL() override;

  /**
   * @brief Переопределенная функция обработки нажатия клавиши мыши.
   *
   * @param[in] event Событие нажатия клавиши мыши.
   */
  void mousePressEvent(QMouseEvent *event) override;

  /**
   * @brief Переопределенная функция обработки движения мыши.
   *
   * @param[in] event Событие движения мыши.
   */
  void mouseMoveEvent(QMouseEvent *event) override;

  /**
   * @brief Переопределенная функция обработки вращения колесика мыши.
   *
   * @param[in] event Событие вращения колесика мыши.
   */
  void wheelEvent(QWheelEvent *event) override;

  /**
   * @brief Масштабировать модель.
   *
   * @param[in] scaleFactor Фактор масштабирования.
   */
  void scaleModel(float scaleFactor) noexcept;

 private:
  s21::Model &model =
      s21::Model::getInstance(); /**< Ссылка на объект модели. */
  s21::Controller controller; /**< Объект контроллера. */
  QString projection_type;    /**< Тип проекции. */
  QString line_type;          /**< Тип линии. */
  QString line_color;         /**< Цвет линии. */
  int line_width;             /**< Ширина линии. */
  QString vertex_color;       /**< Цвет вершин. */
  QString vertex_display; /**< Способ отображения вершин. */
  int vertex_size;        /**< Размер вершин. */
  int axis_check; /**< Переключатель отображения осей. */
  QString background_color; /**< Цвет фона. */
  QSettings *set;           /**< Настройки интерфейса. */
  int xRot;                 /**< Угол вращения по оси X. */
  int yRot;                 /**< Угол вращения по оси Y. */
  int zRot;                 /**< Угол вращения по оси Z. */
  QPoint lastPos;           /**< Последняя позиция мыши. */
};
}  // namespace s21
#endif  // CPP4_3DVIEWER_V2_VIEWER_VIEW_H_
