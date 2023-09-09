/*!
\file
\brief Заголовочный файл с объявлениям класса Model который представляет
собой модель для хранение и обработки данных 3D объекта.
*/

#ifndef CPP4_3DVIEWER_V2_VIEWER_MODEL_H_
#define CPP4_3DVIEWER_V2_VIEWER_MODEL_H_

#include <cfloat>
#include <cmath>
#include <cstring>
#include <fstream>

namespace s21 {

/**
 * @brief Класс модели 3D объекта.
 *
 * Этот класс представляет собой модель 3D объекта, который может быть загружен
 * из файла формата .obj. Он содержит структуры и методы для хранения и
 * обработки данных модели, такие как вершины, полигоны и т. д.
 */
class Model {
 public:
  /**
   * @brief Структура, представляющая вершины полигона.
   */
  struct Facets {
    unsigned int *vertexes;  ///< Массив индексов вершин полигона.
    unsigned int
        numbers_of_vertexes_for_polygon;  ///< Количество вершин в полигоне.
  };

  /**
   * @brief Структура, представляющая матрицу вершин модели.
   */
  struct MatrixStruct {
    double **matrix;  ///< Двумерный массив вершин модели.
    unsigned int rows;  ///< Количество строк (вершин) в матрице.
    unsigned int columns;  ///< Количество столбцов (координат) в матрице.
  };

  /**
   * @brief Структура, содержащая данные модели.
   */
  struct Data {
    unsigned int count_of_vertexes;  ///< Количество вершин модели.
    unsigned int count_of_polygons;  ///< Количество полигонов модели.
    MatrixStruct matrix_of_vertexes;  ///< Матрица вершин модели.
    Facets *array_of_polygon;  ///< Массив полигонов модели.
    double minX, minY, minZ;  ///< Минимальные координаты модели.
    double maxX, maxY, maxZ;  ///< Максимальные координаты модели.
  };

  Data viewer;  ///< Данные модели.

  /**
   * @brief Получение единственного экземпляра модели.
   *
   * Этот метод используется для получения единственного экземпляра модели с
   * использованием шаблона Singleton.
   *
   * @return Ссылка на единственный экземпляр модели.
   */
  inline static Model &getInstance() {
    static Model instance;
    return instance;
  }

  /**
   * @brief Основной метод для загрузки и обработки модели.
   *
   * Этот метод загружает модель из файла формата .obj и выполняет все
   * необходимые операции для её обработки, включая инициализацию, аллокацию
   * памяти и парсинг данных.
   *
   * @param file_name Путь к файлу .obj, содержащему модель.
   */
  inline void coreParser(const char *file_name) noexcept {
    initialize();
    firstReadParser(file_name);
    createMatrixOfVertexes();
    polygonMemoryAllocation();
    secondReadParser(file_name);
  }

  /**
   * @brief Установка модели в центр виджета.
   *
   * Этот метод выполняет установку модели в центр виджета путем масштабирования
   * и перемещения вершин модели так, чтобы она находилась в центре виджета.
   */
  void setInCenter() const noexcept;

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
  void releaseResources();

 protected:
  Model() = default;
  Model(const Model &other) = delete;
  Model(Model &&other) = delete;
  void operator=(const Model &other) = delete;
  ~Model() = default;

 private:
  /**
   * @brief Выделение памяти для массива полигонов.
   *
   * Этот метод выделяет память для массива полигонов модели.
   */
  inline void polygonMemoryAllocation() {
    viewer.array_of_polygon = new Facets[viewer.count_of_polygons + 1];
  }

  /**
   * @brief Выделение памяти для вершин полигона.
   *
   * Этот метод выделяет память для массива вершин полигона.
   *
   * @param j Индекс полигона, для которого выделяется память.
   */
  inline void vertexesForPolygonMemoryAllocation(int j) {
    viewer.array_of_polygon[j].vertexes =
        new unsigned int[viewer.array_of_polygon[j]
                             .numbers_of_vertexes_for_polygon];
  }

  /**
   * @brief Вычисление минимальных и максимальных координат модели.
   *
   * Этот метод вычисляет минимальные и максимальные координаты модели для
   * определения центра модели и масштабирования.
   *
   * @param i Индекс вершины модели.
   */
  inline void minMax(int i) noexcept {
    viewer.maxX = fmax(viewer.matrix_of_vertexes.matrix[i][0], viewer.maxX);
    viewer.maxY = fmax(viewer.matrix_of_vertexes.matrix[i][1], viewer.maxY);
    viewer.maxZ = fmax(viewer.matrix_of_vertexes.matrix[i][2], viewer.maxZ);
    viewer.minX = fmin(viewer.matrix_of_vertexes.matrix[i][0], viewer.minX);
    viewer.minY = fmin(viewer.matrix_of_vertexes.matrix[i][1], viewer.minY);
    viewer.minZ = fmin(viewer.matrix_of_vertexes.matrix[i][2], viewer.minZ);
  }

  void countVertexesForPolygon(char *str, int j);
  void firstReadParser(const char *file_name) noexcept;
  void secondReadParser(const char *file_name) noexcept;

  /**
   * @brief Создание матрицы вершин модели.
   *
   * Этот метод создает двумерную матрицу для хранения вершин модели.
   * Количество строк и столбцов определяется количеством вершин в модели.
   */
  void createMatrixOfVertexes();

  /**
   * @brief Парсинг данных полигона.
   *
   * Этот метод выполняет парсинг данных о вершинах полигона из строки.
   *
   * @param str Строка данных о полигоне.
   * @param j Индекс полигона.
   */
  void parserVertexesForPolygon(char *str, int j);

  /**
   * @brief Инициализация модели.
   *
   * Этот метод выполняет инициализацию модели, сбрасывая все счетчики и
   * освобождая выделенную память.
   */
  void initialize() noexcept;
};

}  // namespace s21
#endif  // CPP4_3DVIEWER_V2_VIEWER_MODEL_H_
