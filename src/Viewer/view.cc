#include "view.h"

#include <QtWidgets>

#include "ui_view.h"

/**
 * @brief Конструктор класса View.
 *
 * Этот конструктор инициализирует объект класса View и выполняет различные
 * настройки и соединения событий для элементов пользовательского интерфейса.
 *
 * @param parent Родительский виджет (QWidget).
 */
s21::View::View(QWidget *parent) : QMainWindow(parent), ui(new Ui::View) {
  // Инициализация пользовательского интерфейса
  ui->setupUi(this);

  // Создание объекта настроек
  set = new QSettings("launch_settings.init", QSettings::IniFormat);

  // Установка соединений для передачи данных и событий
  connect(ui->widget, &Paint::send_info, this, &View::receiveInfo);
  connect(this, &View::signal_data, ui->widget,
          &Paint::on_transformButton_clicked2);
  connect(this, &View::signal_settings, ui->widget,
          &Paint::on_applySettingsButton_clicked);

  // Восстановление сохраненных настроек из файла настроек
  ui->projectionBox->setCurrentText(set->value("projection").toString());
  ui->lineBox->setCurrentText(set->value("lineType").toString());
  ui->lineColorBox->setCurrentText(set->value("lineColor").toString());
  ui->lineWidthBox->setValue(set->value("lineWidth").toInt());
  ui->vertexColorBox->setCurrentText(set->value("vertexColor").toString());
  ui->vertexDisplayBox->setCurrentText(set->value("vertexDisplay").toString());
  ui->vertexSizeBox->setValue(set->value("vertexSize").toInt());
  ui->backgroundColorBox->setCurrentText(
      set->value("backgroundColor").toString());
}

/**
 * @brief Деструктор класса View.
 *
 * Этот деструктор освобождает ресурсы, выделенные для объекта класса View,
 * включая пользовательский интерфейс и другие ресурсы, которые могли быть
 * выделены в конструкторе.
 */
s21::View::~View() {
  // Освобождение пользовательского интерфейса
  delete ui;
}

/**
 * @brief Обновляет информацию о модели на пользовательском интерфейсе.
 *
 * Эта функция принимает количество вершин, количество полигонов и имя файла
 * и обновляет соответствующие поля на пользовательском интерфейсе.
 *
 * @param verticesCount Количество вершин модели.
 * @param polygonsCount Количество полигонов модели.
 * @param fileName Имя файла модели.
 */
void s21::View::receiveInfo(int verticesCount, int polygonsCount,
                            QString fileName) noexcept {
  // Установка текста на пользовательском интерфейсе
  ui->vertices->setText(QString::number(verticesCount));
  ui->polygons->setText(QString::number(polygonsCount));
  ui->fileName->setText(fileName);

  // Обновление пользовательского интерфейса
  update();
}

/**
 * @brief Вызывается при нажатии на кнопку "Преобразовать модель".
 *
 * Эта функция собирает данные для аффинных преобразований из элементов
 * пользовательского интерфейса (в данном случае, значения из элементов
 * управления с двойными числами) и передает их в виде двумерного массива
 * для выполнения преобразования модели.
 */
void s21::View::on_transformButton_clicked() noexcept {
  // Создаем двумерный массив с данными для аффинных преобразований
  double transform_data[3][3] = {
      {ui->doubleSpinBox->value(), ui->doubleSpinBox_2->value(),
       ui->doubleSpinBox_3->value()},
      {ui->doubleSpinBox_4->value(), ui->doubleSpinBox_5->value(),
       ui->doubleSpinBox_6->value()},
      {ui->doubleSpinBox_7->value(), 0, 0}};

  // Генерируем сигнал с данными для аффинных преобразований
  emit signal_data(transform_data);
}

/**
 * @brief Вызывается при нажатии на кнопку "Внести изменения".
 *
 * Эта функция собирает настройки интерфейса, заданные пользователем, из
 * элементов пользовательского интерфейса (таких как выпадающие списки и
 * ползунки), и передает их в виде QMap<QString, QString> для обновления
 * настроек интерфейса.
 *
 * @see signal_settings
 */
void s21::View::on_applySettings_clicked() noexcept {
  // Создаем QMap для хранения настроек интерфейса
  QMap<QString, QString> map;
  map["projection"] = ui->projectionBox->currentText();
  map["line_type"] = ui->lineBox->currentText();
  map["line_color"] = ui->lineColorBox->currentText();
  map["line_width"] = QString::number(ui->lineWidthBox->value());
  map["vertex_color"] = ui->vertexColorBox->currentText();
  map["vertex_display"] = ui->vertexDisplayBox->currentText();
  map["vertex_size"] = QString::number(ui->vertexSizeBox->value());
  map["background_color"] = ui->backgroundColorBox->currentText();

  // Генерируем сигнал с настройками интерфейса для обновления
  emit signal_settings(map);
}

/**
 * @brief Вызывается при нажатии на кнопку "Сохранить изображение".
 *
 * Эта функция открывает диалоговое окно для выбора места и формата сохранения
 * изображения модели. После выбора места и формата, функция сохраняет
 * изображение модели с помощью метода `grabFramebuffer()` виджета и сохраняет
 * его в выбранное место с указанным форматом.
 *
 * @note Изображение сохраняется с максимальным качеством (качество 100).
 */
void s21::View::on_saveImage_clicked() noexcept {
  // Открываем диалоговое окно для выбора места и формата сохранения
  QString save = QFileDialog::getSaveFileName(this, NULL, NULL,
                                              "JPEG (*.jpeg) ;; BMP (*.bmp)");

  // Проверяем, было ли выбрано место сохранения
  if (!save.isNull()) {
    // Сохраняем изображение модели в выбранное место с указанным форматом
    ui->widget->grabFramebuffer().save(save, NULL, 100);
  }
}

/**
 * @brief Вызывается при нажатии на кнопку "Запись GIF".
 *
 * Эта функция инициализирует процесс записи GIF-анимации. Она создает объекты
 * QGifImage для записи и QImage для временного хранения кадров. Также
 * устанавливается таймер, который будет вызывать функцию сохранения каждые 50
 * миллисекунд. Запись начинается после нажатия кнопки "Запись GIF".
 *
 * @note GIF-анимация будет записана с заданным интервалом в 50 миллисекунд
 * между кадрами. Вызывается слот save() при каждом таймере.
 */
void s21::View::on_recordScreencast_clicked() {
  // Инициализируем объекты для записи GIF
  gif = new QGifImage;
  frame = new QImage;

  // Создаем и настраиваем таймер для записи каждые 50 миллисекунд
  timer = new QTimer(this);
  count = 0;
  connect(timer, SIGNAL(timeout()), this, SLOT(save()));
  timer->start(50);
}

/**
 * @brief Сохраняет кадры и создает GIF-анимацию.
 *
 * Эта функция вызывается таймером каждые 50 миллисекунд в процессе записи
 * GIF-анимации. Она увеличивает счетчик кадров, захватывает текущий кадр из
 * виджета `ui->widget`, добавляет его в GIF-анимацию и проверяет, достигло ли
 * количество сохраненных кадров максимального значения (50). Если это так, то
 * запись GIF-анимации останавливается и пользователю предлагается выбрать место
 * для сохранения анимации.
 *
 * @note GIF-анимация сохраняется с расширением .gif.
 */
void s21::View::save() noexcept {
  count++;  // Увеличиваем счетчик кадров

  // Захватываем текущий кадр из виджета и добавляем его в GIF-анимацию
  *frame = ui->widget->grabFramebuffer();
  gif->addFrame(*frame);

  if (count == 50) {  // Если записано 50 кадров
    timer->stop();    // Останавливаем таймер записи
    QString save = NULL;

    // Пользователю предлагается выбрать место для сохранения GIF-анимации
    save = QFileDialog::getSaveFileName(this, NULL, NULL, "GIF (*.gif)");
    if (!save.isNull()) gif->save(save);  // Сохраняем анимацию
  }
}

/**
 * @brief Конструктор класса Paint.
 *
 * @param parent Родительский виджет (по умолчанию - nullptr).
 */
s21::Paint::Paint(QWidget *parent) : QOpenGLWidget{parent} {
  axis_check = 0;
  set = new QSettings("launch_settings.init", QSettings::IniFormat);
}

/**
 * @brief Обработчик события нажатия на кнопку "Выбрать файл".
 *
 * Эта функция вызывается при нажатии на кнопку "Выбрать файл" и открывает
 * диалоговое окно для выбора файла. Затем она передает путь к выбранному файлу
 * через контроллер модели для загрузки данных о модели, центрирует модель и
 * отправляет информацию о количестве вершин и полигонов модели в сигнал.
 */
void s21::Paint::on_SelectFileButton_clicked() noexcept {
  QString f_name;

  QString filename = QFileDialog::getOpenFileName(this, "Выберите файл");

  QByteArray ba = filename.toLocal8Bit();
  const char *filename_c = ba.data();
  controller.coreParser(filename_c);
  controller.setInCenter();
  f_name = filename.split('/').last();

  emit send_info(model.viewer.count_of_vertexes, model.viewer.count_of_polygons,
                 f_name);
  update();
}

/**
 * @brief Обработчик события нажатия на кнопку "Показать/скрыть оси".
 *
 * Эта функция вызывается при нажатии на кнопку "Показать/скрыть оси". Она
 * переключает режим отображения осей модели, а именно, позволяет показать или
 * скрыть оси координат в сцене. Для этого она инкрементирует или декрементирует
 * значение переменной `axis_check` в зависимости от текущего состояния, а затем
 * обновляет сцену для отображения изменений.
 */
void s21::Paint::on_checkAxes_clicked() noexcept {
  if (axis_check != 0)
    axis_check += 1;
  else
    axis_check -= 1;
  update();
}
/**
 * @brief Функция отрисовки 3D-объекта в контексте OpenGL.
 *
 * Эта функция выполняет отрисовку 3D-объекта в контексте OpenGL. Она использует
 * данные, хранящиеся в классе Model, а также настройки интерфейса для
 * управления отображением объекта. Сначала функция устанавливает цвет фона
 * сцены в соответствии с выбранным цветом из настроек. Затем она настраивает
 * проекцию в зависимости от выбранного типа проекции (центральная или
 * ортографическая). После этого функция выполняет отрисовку объекта, вращение
 * и масштабирование согласно текущим параметрам. Если установлен режим
 * отображения осей, функция также отрисовывает оси координат.
 */
void s21::Paint::paintGL() {
  background_color = set->value("backgroundColor").toString();
  if (background_color == "Red")
    glClearColor(1, 0.0, 0.0, 0.0);
  else if (background_color == "Green")
    glClearColor(0, 1, 0.0, 0.0);
  else if (background_color == "Blue")
    glClearColor(0, 0.0, 1, 0.0);
  else if (background_color == "Yellow")
    glClearColor(1, 1, 0.0, 0);
  else if (background_color == "Pink")
    glClearColor(1, 0, 1, 0);
  else if (background_color == "White")
    glClearColor(1, 1, 1, 1);
  else
    glClearColor(0, 0.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  projection_type = set->value("projection").toString();
  if (projection_type == "Central")
    glFrustum(-2, +2, -2, +2, 5.0, 15.0);
  else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);

  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -7.0);
  glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
  glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
  glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
  drawLines();
  drawPoints();
  vertex_display = set->value("vertexDisplay").toString();

  if (axis_check != 0) drawAxis();
}

/**
 * @brief Функция отрисовки линий объекта.
 *
 * Эта функция отвечает за отрисовку линий объекта с использованием данных о
 * цвете, типе линии, толщине линии и координатах вершин из объекта Model.
 * Сначала функция устанавливает цвет линий в соответствии с выбранным цветом
 * из настроек интерфейса. Затем она определяет тип линии (сплошная или пунктир)
 * и устанавливает соответствующие параметры OpenGL. Толщина линии также берется
 * из настроек. Далее функция итерируется по полигонам объекта и отрисовывает
 * линии для каждого полигона, соединяя его вершины.
 */
void s21::Paint::drawLines() noexcept {
  line_color = set->value("lineColor").toString();
  if (line_color == "Red")
    glColor3d(1, 0, 0);
  else if (line_color == "Green")
    glColor3d(0, 1, 0);
  else if (line_color == "Blue")
    glColor3d(0, 0, 1);
  else
    glColor3d(1, 1, 1);
  line_type = set->value("lineType").toString();
  if (line_type == "Dashed") {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0X00FF);
  } else
    glDisable(GL_LINE_STIPPLE);
  line_width = set->value("lineWidth").toInt();
  glLineWidth(line_width);
  for (int i = 1; i < model.viewer.count_of_polygons + 1; i++) {
    for (int j = 0;
         j < model.viewer.array_of_polygon[i].numbers_of_vertexes_for_polygon;
         j++) {
      glBegin(GL_LINE_LOOP);
      glVertex3d(model.viewer.matrix_of_vertexes
                     .matrix[model.viewer.array_of_polygon[i].vertexes[j]][0],
                 model.viewer.matrix_of_vertexes
                     .matrix[model.viewer.array_of_polygon[i].vertexes[j]][1],
                 model.viewer.matrix_of_vertexes
                     .matrix[model.viewer.array_of_polygon[i].vertexes[j]][2]);
    }
    glEnd();
  }
}

/**
 * @brief Функция отрисовки точек объекта.
 *
 * Эта функция отвечает за отрисовку точек объекта с использованием данных о
 * цвете, размере, типе отображения точек и координатах вершин из объекта Model.
 * Сначала функция устанавливает цвет точек в соответствии с выбранным цветом
 * из настроек интерфейса. Затем она определяет размер и тип отображения точек
 * (квадратные или сглаженные) и устанавливает соответствующие параметры OpenGL.
 * Далее функция итерируется по полигонам объекта и отрисовывает точки для
 * каждой вершины полигона.
 */
void s21::Paint::drawPoints() noexcept {
  vertex_color = set->value("vertexColor").toString();
  if (vertex_color == "Red")
    glColor3d(1, 0, 0);
  else if (vertex_color == "Green")
    glColor3d(0, 1, 0);
  else if (vertex_color == "Blue")
    glColor3d(0, 0, 1);
  else
    glColor3d(1, 1, 1);

  glEnable(GL_BLEND);
  if (vertex_display == "Square")
    glDisable(GL_POINT_SMOOTH);
  else
    glEnable(GL_POINT_SMOOTH);
  vertex_size = set->value("vertexSize").toInt();
  glEnable(GL_PROGRAM_POINT_SIZE);
  glPointSize(vertex_size);
  for (int i = 1; i < model.viewer.count_of_polygons + 1; i++) {
    for (int j = 0;
         j < model.viewer.array_of_polygon[i].numbers_of_vertexes_for_polygon;
         j++) {
      glBegin(GL_POINTS);
      glVertex3d(model.viewer.matrix_of_vertexes
                     .matrix[model.viewer.array_of_polygon[i].vertexes[j]][0],
                 model.viewer.matrix_of_vertexes
                     .matrix[model.viewer.array_of_polygon[i].vertexes[j]][1],
                 model.viewer.matrix_of_vertexes
                     .matrix[model.viewer.array_of_polygon[i].vertexes[j]][2]);
    }
    glEnd();
  }
}

/**
 * @brief Функция отрисовки координатных осей.
 *
 * Эта функция отвечает за отрисовку координатных осей объекта в трехмерном
 * пространстве. Она использует цвет (розовый) для отображения осей и
 * использует функции OpenGL для рисования линий, представляющих оси X, Y и Z.
 */
void s21::Paint::drawAxis() noexcept {
  glColor3d(1, 0, 1);
  glBegin(GL_LINE_LOOP);
  glVertex3d(2, 0, 0);
  glVertex3d(-2, 0, 0);
  glEnd();
  glBegin(GL_LINE_LOOP);
  glVertex3d(0, 2, 0);
  glVertex3d(0, -2, 0);
  glEnd();
  glBegin(GL_LINE_LOOP);
  glVertex3d(0, 0, 2);
  glVertex3d(0, 0, -2);
  glEnd();
}

/**
 * @brief Нормализация угла в диапазоне от 0 до 360 градусов.
 *
 * Эта функция принимает угол (в шестнадцатеричных градусах) и нормализует его
 * в диапазоне от 0 до 360 * 16 (или 5760) градусов. Это нужно для корректного
 * отображения углов поворота объекта.
 *
 * @param[in, out] angle Угол, который будет нормализован в указанном диапазоне.
 */
static void qNormalizeAngle(int &angle) noexcept {
  while (angle < 0) angle += 360 * 16;
  while (angle > 360 * 16) angle -= 360 * 16;
}

/**
 * @brief Установка угла поворота по оси X.
 *
 * Эта функция устанавливает угол поворота объекта вокруг оси X. Если угол
 * отличается от текущего угла (xRot), то происходит обновление отображения.
 *
 * @param[in] angle Угол поворота по оси X (в шестнадцатеричных градусах).
 */
void s21::Paint::setXRotation(int angle) noexcept {
  qNormalizeAngle(angle);
  if (angle != xRot) {
    xRot = angle;
    update();
  }
}

/**
 * @brief Установка угла поворота по оси Y.
 *
 * Эта функция устанавливает угол поворота объекта вокруг оси Y. Если угол
 * отличается от текущего угла (yRot), то происходит обновление отображения.
 *
 * @param[in] angle Угол поворота по оси Y (в шестнадцатеричных градусах).
 */
void s21::Paint::setYRotation(int angle) noexcept {
  qNormalizeAngle(angle);
  if (angle != yRot) {
    yRot = angle;
    update();
  }
}

/**
 * @brief Установка угла поворота по оси Z.
 *
 * Эта функция устанавливает угол поворота объекта вокруг оси Z. Если угол
 * отличается от текущего угла (zRot), то происходит обновление отображения.
 *
 * @param[in] angle Угол поворота по оси Z (в шестнадцатеричных градусах).
 */
void s21::Paint::setZRotation(int angle) noexcept {
  qNormalizeAngle(angle);
  if (angle != zRot) {
    zRot = angle;
    update();
  }
}

/**
 * @brief Обработчик события нажатия кнопки "Преобразовать модель".
 *
 * Эта функция вызывается при нажатии на кнопку "Преобразовать модель" и
 * передает данные о преобразовании в класс Affine через контроллер. После
 * выполнения преобразований она обновляет изображение модели.
 *
 * @param[in] transform_data Двумерный массив, представляющий данные для
 * аффинного преобразования.
 * @see s21::Controller::s21_affine_transform
 */
void s21::Paint::on_transformButton_clicked2(
    double transform_data[3][3]) noexcept {
  controller.s21_affine_transform(transform_data);
  update();
}

/**
 * @brief Обработчик нажатия кнопки мыши.
 *
 * Функция вызывается при нажатии кнопки мыши и сохраняет текущее положение
 * курсора.
 *
 * @param[in] event Объект события мыши.
 */
void s21::Paint::mousePressEvent(QMouseEvent *event) { lastPos = event->pos(); }

/**
 * @brief Обработчик перемещения мыши.
 *
 * Функция вызывается при перемещении мыши и обрабатывает движение курсора для
 * изменения угловой ориентации модели. Если нажата левая кнопка мыши,
 * изменяются углы xRot и yRot, если правая кнопка мыши - xRot и zRot.
 *
 * @param[in] event Объект события мыши.
 */
void s21::Paint::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->position().x() - lastPos.x();
  int dy = event->position().y() - lastPos.y();

  if (event->buttons() & Qt::LeftButton) {
    setXRotation(xRot + 8 * dy);
    setYRotation(yRot + 8 * dx);
  } else if (event->buttons() & Qt::RightButton) {
    setXRotation(xRot + 8 * dy);
    setZRotation(zRot + 8 * dx);
  }

  lastPos = event->pos();
}

/**
 * @brief Обработчик события прокрутки колеса мыши.
 *
 * Функция вызывается при прокрутке колеса мыши и изменяет масштаб модели
 * в зависимости от направления прокрутки.
 *
 * @param[in] event Объект события колеса мыши.
 */
void s21::Paint::wheelEvent(QWheelEvent *event) {
  int delta = event->angleDelta().y();
  if (delta > 0)
    // Увеличиваем масштаб
    scaleModel(1.1);
  else if (delta < 0)
    // Уменьшаем масштаб
    scaleModel(0.9);

  update();
}

/**
 * @brief Изменение масштаба модели.
 *
 * Функция изменяет масштаб модели на основе переданного коэффициента
 * масштабирования.
 *
 * @param[in] scaleFactor Коэффициент масштабирования.
 * @see s21::Controller::s21_scaling
 */
void s21::Paint::scaleModel(float scaleFactor) noexcept {
  controller.s21_scaling(scaleFactor);
  update();
}

/**
 * @brief Обработчик нажатия кнопки применения настроек интерфейса.
 *
 * Функция применяет измененные пользователем настройки интерфейса и сохраняет
 * их. Также функция обновляет изображение модели с учетом новых настроек.
 *
 * @param[in] map Карта настроек с ключами и значениями.
 */
void s21::Paint::on_applySettingsButton_clicked(
    QMap<QString, QString> map) noexcept {
  // Применение настроек
  projection_type = map["projection"];
  line_type = map["line_type"];
  line_color = map["line_color"];
  line_width = map["line_width"].toInt();
  vertex_color = map["vertex_color"];
  vertex_size = map["vertex_size"].toInt();
  vertex_display = map["vertex_display"];
  background_color = map["background_color"];

  // Сохранение настроек в файл
  set->setValue("projection", map["projection"]);
  set->setValue("lineType", map["line_type"]);
  set->setValue("lineColor", map["line_color"]);
  set->setValue("lineWidth", map["line_width"]);
  set->setValue("vertexColor", map["vertex_color"]);
  set->setValue("vertexSize", map["vertex_size"]);
  set->setValue("vertexDisplay", map["vertex_display"]);
  set->setValue("backgroundColor", map["background_color"]);

  // Обновление изображения
  update();
}
