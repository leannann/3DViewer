#include "model.h"

/**
 * @brief Устанавливает модель в центре виджета и масштабирует её.
 *
 * Функция считает минимальные и максимальные значения координат вершин модели
 * и масштабирует модель так, чтобы она была центрирована и охватывала виджет.
 * Это позволяет отобразить модель в центре виджета с правильным масштабом.
 *
 * @note Функция предполагает, что матрица вершин модели уже содержит данные.
 * @note Функция не изменяет значения min и max координат.
 */
void s21::Model::setInCenter() const noexcept {
  // Вычисляем масштаб для центрирования модели и охвата виджета
  double zoom = (1.5 - (1.5 * (-1))) / fmax(fmax((viewer.maxX - viewer.minX),
                                                 (viewer.maxY - viewer.minY)),
                                            (viewer.maxZ - viewer.minZ));

  // Вычисляем центр модели
  double center_x = viewer.minX + (viewer.maxX - viewer.minX) / 2.0;
  double center_y = viewer.minY + (viewer.maxY - viewer.minY) / 2.0;
  double center_z = viewer.minZ + (viewer.maxZ - viewer.minZ) / 2.0;

  // Применяем масштаб и центрируем модель
  for (unsigned int i = 1; i <= viewer.count_of_vertexes; i++) {
    viewer.matrix_of_vertexes.matrix[i][0] =
        (viewer.matrix_of_vertexes.matrix[i][0] - center_x) * zoom;
    viewer.matrix_of_vertexes.matrix[i][1] =
        (viewer.matrix_of_vertexes.matrix[i][1] - center_y) * zoom;
    viewer.matrix_of_vertexes.matrix[i][2] =
        (viewer.matrix_of_vertexes.matrix[i][2] - center_z) * zoom;
  }
}

/**
 * @brief Инициализирует переменные структур класса Model.
 *
 * Эта функция устанавливает начальные значения переменных в классе Model
 * перед началом работы с моделью. Она обнуляет счетчики полигонов и вершин,
 * а также устанавливает указатели на матрицу вершин и массив полигонов в
 * nullptr. Координаты минимума и максимума по осям X, Y и Z устанавливаются в
 * исключительные значения, чтобы в будущем их можно было корректно обновить при
 * анализе модели.
 */
void s21::Model::initialize() noexcept {
  viewer.count_of_polygons = 0;
  viewer.count_of_vertexes = 0;
  viewer.matrix_of_vertexes.columns = 0;
  viewer.matrix_of_vertexes.rows = 0;
  viewer.matrix_of_vertexes.matrix = nullptr;
  viewer.array_of_polygon = nullptr;
  viewer.minX = DBL_MAX;
  viewer.minY = DBL_MAX;
  viewer.minZ = DBL_MAX;
  viewer.maxX = DBL_MIN;
  viewer.maxY = DBL_MIN;
  viewer.maxZ = DBL_MIN;
}

/**
 * @brief Первичное считывание модели из файла .obj для подсчета вершин и
 * полигонов.
 *
 * Функция открывает файл модели .obj и проходит по его строкам, подсчитывая
 * количество вершин и полигонов. Это первичное считывание модели, которое
 * позволяет узнать количество элементов для дальнейшего выделения памяти и
 * считывания полной модели.
 *
 * @param file_name Путь к файлу .obj модели для считывания.
 *
 * @note Функция не загружает полную модель, а только считает количество вершин
 * и полигонов. Для полной загрузки и анализа модели используйте функцию
 * secondReadParser.
 */
void s21::Model::firstReadParser(const char *file_name) noexcept {
  std::ifstream f(file_name);
  std::string line;
  if (f.is_open())
    while (std::getline(f, line)) {
      if (line.compare(0, 2, "v ") == 0) viewer.count_of_vertexes++;
      if (line.compare(0, 2, "f ") == 0) viewer.count_of_polygons++;
    }

  f.close();
}

/**
 * @brief Вторичное считывание модели из файла .obj для полной загрузки данных
 * модели.
 *
 * Функция открывает файл модели .obj и проходит по его строкам, считывая данные
 * о вершинах и полигонах модели. Это вторичное считывание модели, которое
 * выполняется после подсчета количества вершин и полигонов в модели функцией
 * firstReadParser. Функция считывает координаты вершин, их нормали и связи для
 * полигонов.
 *
 * @param file_name Путь к файлу .obj модели для считывания данных.
 *
 * @note Функция выполняет полную загрузку данных модели из файла .obj.
 * Для последующей работы с моделью используйте эту функцию после вызова
 * firstReadParser.
 */
void s21::Model::secondReadParser(const char *file_name) noexcept {
  std::ifstream f(file_name);
  std::string line;
  if (f.is_open()) {
    int i = 1, j = 1;

    while (std::getline(f, line)) {
      if (line.compare(0, 2, "v ") == 0) {
        char ch;
        sscanf(line.c_str(), "%c %lf %lf %lf", &ch,
               &viewer.matrix_of_vertexes.matrix[i][0],
               &viewer.matrix_of_vertexes.matrix[i][1],
               &viewer.matrix_of_vertexes.matrix[i][2]);
        minMax(i);
        i++;
      }

      if (line.compare(0, 2, "f ") == 0) {
        countVertexesForPolygon(const_cast<char *>(line.c_str()), j);
        vertexesForPolygonMemoryAllocation(j);
        parserVertexesForPolygon(const_cast<char *>(line.c_str()), j);
        j++;
      }
    }
  }
  f.close();
}

/**
 * @brief Парсинг строковых данных полигона для получения вершин полигона.
 *
 * Функция разбирает строку данных полигона, извлекая индексы вершин, связанных
 * с этим полигоном. Полученные индексы вершин сохраняются в массиве структуры,
 * представляющей полигон. Эта функция используется при вторичном считывании
 * модели из файла .obj.
 *
 * @param str Строковое представление полигона для разбора.
 * @param j Индекс полигона в массиве полигонов модели.
 *
 * @note Функция предназначена для внутреннего использования и обычно вызывается
 * внутри функции secondReadParser.
 */
void s21::Model::parserVertexesForPolygon(char *str, int j) {
  char *tmp = new char[strlen(str) + 1];

  if (tmp) {
    strncpy(tmp, str, strlen(str) + 1);
    char *sep_str = strtok(tmp, " ");
    int e = 0;
    unsigned int vertex = 0;

    while (sep_str != NULL) {
      if ((vertex = atof(sep_str)) != 0) {
        viewer.array_of_polygon[j].vertexes[e] = vertex;
        e++;
      }
      sep_str = strtok(NULL, " ");
    }
    delete[] tmp;
  }
}

/**
 * @brief Подсчет количества вершин в полигоне.
 *
 * Функция анализирует строковое представление полигона и определяет
 * количество вершин, связанных с этим полигоном. Полученное количество
 * сохраняется в соответствующем поле структуры, представляющей полигон.
 * Эта функция используется при вторичном считывании модели из файла .obj.
 *
 * @param str Строковое представление полигона для анализа.
 * @param j Индекс полигона в массиве полигонов модели.
 *
 * @note Функция предназначена для внутреннего использования и обычно вызывается
 * внутри функции secondReadParser.
 */
void s21::Model::countVertexesForPolygon(char *str, int j) {
  char *tmp = new char[strlen(str) + 1];

  if (tmp) {
    strncpy(tmp, str, strlen(str) + 1);
    char *sep_str = strtok(tmp, " ");
    unsigned int count = 0;

    while (sep_str != NULL) {
      count++;
      sep_str = strtok(NULL, " ");
    }

    viewer.array_of_polygon[j].numbers_of_vertexes_for_polygon = count - 1;
    delete[] tmp;
  }
}

/**
 * @brief Создание матрицы вершин модели.
 *
 * Функция создает двумерный массив, представляющий матрицу вершин модели.
 * Эта матрица будет использоваться для хранения координат вершин модели.
 * Количество строк в матрице равно количеству вершин модели плюс одна, а
 * количество столбцов всегда равно 3, так как каждая вершина имеет три
 * координаты (x, y, z).
 *
 * @note Матрица вершин будет использоваться для внутреннего хранения данных
 * модели. Для добавления вершин в матрицу и работы с ней используются другие
 * функции.
 */
void s21::Model::createMatrixOfVertexes() {
  viewer.matrix_of_vertexes.columns = 3;
  viewer.matrix_of_vertexes.rows = viewer.count_of_vertexes + 1;
  viewer.matrix_of_vertexes.matrix =
      new double *[viewer.matrix_of_vertexes.rows];

  if (viewer.matrix_of_vertexes.matrix)
    for (unsigned int i = 0; i < viewer.matrix_of_vertexes.rows; i++) {
      viewer.matrix_of_vertexes.matrix[i] =
          new double[viewer.matrix_of_vertexes.columns];
    }
}

/**
 * @brief Освобождение ресурсов, связанных с моделью.
 *
 * Функция освобождает ресурсы, которые были выделены для хранения данных
 * модели. В частности, она удаляет выделенную память для матрицы вершин и
 * массива полигонов. Также сбрасывает счетчики вершин и полигонов модели.
 *
 * @note После вызова этой функции модель будет находиться в
 * неинициализированном состоянии. Чтобы использовать модель снова, необходимо
 * повторно инициализировать её с помощью функции initialize().
 */
void s21::Model::releaseResources() {
  // Удаление матрицы вершин
  for (unsigned int i = 0; i < viewer.matrix_of_vertexes.rows; i++)
    delete[] viewer.matrix_of_vertexes.matrix[i];

  delete[] viewer.matrix_of_vertexes.matrix;
  viewer.matrix_of_vertexes.columns = 0;
  viewer.matrix_of_vertexes.rows = 0;

  // Удаление массива полигонов
  for (unsigned int i = 1; i < viewer.count_of_polygons + 1; i++)
    delete[] viewer.array_of_polygon[i].vertexes;

  delete[] viewer.array_of_polygon;

  // Сброс счетчиков
  viewer.count_of_vertexes = 0;
  viewer.count_of_polygons = 0;
}
