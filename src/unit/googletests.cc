#include <gtest/gtest.h>

#include "../Viewer/affine.h"
#include "../Viewer/model.h"

TEST(ParserTest, Test1) {
  s21::Model &model = s21::Model::getInstance();
  const char *file_name = "obj_models/Wolf_obj.obj";
  model.coreParser(file_name);
  unsigned int expect_count_of_vertexes = 1690;
  unsigned int expect_count_of_polygons = 1566;
  ASSERT_EQ(expect_count_of_polygons, model.viewer.count_of_polygons);
  ASSERT_EQ(expect_count_of_vertexes, model.viewer.count_of_vertexes);
  model.releaseResources();
}

TEST(ParserTest, Test2) {
  s21::Model &model = s21::Model::getInstance();
  const char *file_name = "obj_models/cube.obj";
  model.coreParser(file_name);
  unsigned int expect_count_of_vertexes = 8;
  unsigned int expect_count_of_polygons = 12;
  ASSERT_EQ(expect_count_of_polygons, model.viewer.count_of_polygons);
  ASSERT_EQ(expect_count_of_vertexes, model.viewer.count_of_vertexes);
  model.releaseResources();
}

TEST(ParserTest, Test3) {
  s21::Model &model = s21::Model::getInstance();
  const char *file_name = "obj_models/Spider.obj";
  model.coreParser(file_name);
  unsigned int expect_count_of_vertexes = 1760;
  unsigned int expect_count_of_polygons = 1464;
  ASSERT_EQ(expect_count_of_polygons, model.viewer.count_of_polygons);
  ASSERT_EQ(expect_count_of_vertexes, model.viewer.count_of_vertexes);
  model.releaseResources();
}

TEST(ParserTest, Test4) {
  s21::Model &model = s21::Model::getInstance();
  const char *file_name = "obj_models/Beetle.obj";
  model.coreParser(file_name);
  unsigned int expect_count_of_vertexes = 150000;
  unsigned int expect_count_of_polygons = 300000;
  ASSERT_EQ(expect_count_of_polygons, model.viewer.count_of_polygons);
  ASSERT_EQ(expect_count_of_vertexes, model.viewer.count_of_vertexes);
  model.releaseResources();
}

TEST(ParserTest, Test5) {
  s21::Model &model = s21::Model::getInstance();
  const char *file_name = "obj_models/smaug.obj";
  model.coreParser(file_name);
  unsigned int expect_count_of_vertexes = 5901;
  unsigned int expect_count_of_polygons = 6222;
  ASSERT_EQ(expect_count_of_polygons, model.viewer.count_of_polygons);
  ASSERT_EQ(expect_count_of_vertexes, model.viewer.count_of_vertexes);
  model.releaseResources();
}

TEST(ParserTest, Test6) {
  s21::Model &model = s21::Model::getInstance();
  const char *file_name = "obj_models/torso.obj";
  model.coreParser(file_name);
  model.setInCenter();
  unsigned int expect_count_of_vertexes = 50002;
  unsigned int expect_count_of_polygons = 100000;
  ASSERT_EQ(expect_count_of_polygons, model.viewer.count_of_polygons);
  ASSERT_EQ(expect_count_of_vertexes, model.viewer.count_of_vertexes);
  model.releaseResources();
}

TEST(ParserTest, Test7) {
  s21::Model &model = s21::Model::getInstance();
  const char *file_name = "obj_models/no_File.obj";
  model.coreParser(file_name);
  unsigned int expect_count_of_vertexes = 0;
  unsigned int expect_count_of_polygons = 0;
  ASSERT_EQ(expect_count_of_polygons, model.viewer.count_of_polygons);
  ASSERT_EQ(expect_count_of_vertexes, model.viewer.count_of_vertexes);
  model.releaseResources();
}

TEST(AffineTest, AffineTransform) {
  s21::Model &model = s21::Model::getInstance();
  const char *file_name = "obj_models/no_File.obj";
  model.coreParser(file_name);
  double transform_data[3][3] = {
      {2.0, 1.0, 3.0}, {0.5, 1.0, -0.2}, {1.5, 0.8, 2.0}};
  s21::Affine affine;
  affine.affineTransform(transform_data);
  model.releaseResources();
}

// Тест на смещение по X
TEST(MovingTest, MovingX) {
  s21::Model &model = s21::Model::getInstance();
  const char *file_name = "obj_models/cube.obj";
  model.coreParser(file_name);
  s21::Affine affine;
  affine.movingX(2);
  double expect_array[8] = {3.000000, 3.000000, 1.000000, 1.000000,
                            3.000000, 2.999999, 1.000000, 1.000000};
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i) {
    ASSERT_NEAR(model.viewer.matrix_of_vertexes.matrix[i][0],
                expect_array[i - 1], 1e-6);
  }
  model.releaseResources();
}

// Тест на смещение по Y
TEST(MovingTest, MovingY) {
  s21::Model &model = s21::Model::getInstance();
  const char *file_name = "obj_models/cube.obj";
  model.coreParser(file_name);
  s21::Affine affine;
  affine.movingY(4.5);
  double expect_array[8] = {3.500000, 3.500000, 3.500000, 3.500000,
                            5.500000, 5.500000, 5.500000, 5.500000};
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i) {
    ASSERT_NEAR(model.viewer.matrix_of_vertexes.matrix[i][1],
                expect_array[i - 1], 1e-6);
  }
  model.releaseResources();
}

// Тест на смещение по Z
TEST(MovingTest, MovingZ) {
  s21::Model &model = s21::Model::getInstance();
  const char *file_name = "obj_models/cube.obj";
  model.coreParser(file_name);
  s21::Affine affine;
  affine.movingZ(-3);
  double expect_array[8] = {-4.000000, -2.000000, -2.000000, -4.000000,
                            -3.999999, -1.999999, -2.000000, -4.000000};
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i) {
    ASSERT_NEAR(model.viewer.matrix_of_vertexes.matrix[i][2],
                expect_array[i - 1], 1e-6);
  }
  model.releaseResources();
}

// Тест на вращение по X
TEST(MovingTest, RotationX) {
  s21::Model &model = s21::Model::getInstance();
  const char *file_name = "obj_models/cube.obj";
  model.coreParser(file_name);
  s21::Affine affine;
  affine.rotationX(2);
  double expect_array_y[8] = {1.325444, -0.493151, -0.493151, 1.325444,
                              0.493150, -1.325445, -1.325444, 0.493151};
  double expect_array_z[8] = {-0.493151, -1.325444, -1.325444, -0.493151,
                              1.325444,  0.493150,  0.493151,  1.325444};
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i) {
    ASSERT_NEAR(model.viewer.matrix_of_vertexes.matrix[i][1],
                expect_array_y[i - 1], 1e-6);
  }
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i) {
    ASSERT_NEAR(model.viewer.matrix_of_vertexes.matrix[i][2],
                expect_array_z[i - 1], 1e-6);
  }
  model.releaseResources();
}

// Тест на вращение по Y
TEST(MovingTest, RotationY) {
  s21::Model &model = s21::Model::getInstance();
  const char *file_name = "obj_models/cube.obj";
  model.coreParser(file_name);
  s21::Affine affine;
  affine.rotationY(2);
  double expect_array_y[8] = {-1.325444, 0.493151, 1.325444, -0.493151,
                              -1.325443, 0.493152, 1.325444, -0.493151};
  double expect_array_z[8] = {-0.493151, -1.325444, 0.493151, 1.325444,
                              -0.493151, -1.325444, 0.493151, 1.325444};
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i) {
    ASSERT_NEAR(model.viewer.matrix_of_vertexes.matrix[i][0],
                expect_array_y[i - 1], 1e-6);
  }
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i) {
    ASSERT_NEAR(model.viewer.matrix_of_vertexes.matrix[i][2],
                expect_array_z[i - 1], 1e-6);
  }
  model.releaseResources();
}

// Тест на вращение по Z
TEST(MovingTest, RotationZ) {
  s21::Model &model = s21::Model::getInstance();
  const char *file_name = "obj_models/cube.obj";
  model.coreParser(file_name);
  s21::Affine affine;
  affine.rotationZ(2);
  double expect_array_y[8] = {0.493151,  0.493151,  1.325444,  1.325444,
                              -1.325444, -1.325444, -0.493151, -0.493151};
  double expect_array_z[8] = {1.325444, 1.325444, -0.493151, -0.493151,
                              0.493151, 0.493150, -1.325444, -1.325444};
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i) {
    ASSERT_NEAR(model.viewer.matrix_of_vertexes.matrix[i][0],
                expect_array_y[i - 1], 1e-6);
  }
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i) {
    ASSERT_NEAR(model.viewer.matrix_of_vertexes.matrix[i][1],
                expect_array_z[i - 1], 1e-6);
  }
  model.releaseResources();
}

// Тест масштабирования
TEST(MovingTest, scaling) {
  s21::Model &model = s21::Model::getInstance();
  const char *file_name = "obj_models/cube.obj";
  model.coreParser(file_name);
  s21::Affine affine;
  affine.scaling(2);
  double expect_array_x[8] = {2.000000, 2.000000, -2.000000, -2.000000,
                              2.000000, 1.999998, -2.000000, -2.000000};
  double expect_array_y[8] = {-2.000000, -2.000000, -2.000000, -2.000000,
                              2.000000,  2.000000,  2.000000,  2.000000};
  double expect_array_z[8] = {-2.000000, 2.000000, 2.000000, -2.000000,
                              -1.999998, 2.000002, 2.000000, -2.000000};
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i) {
    ASSERT_NEAR(model.viewer.matrix_of_vertexes.matrix[i][0],
                expect_array_x[i - 1], 1e-6);
  }

  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i) {
    ASSERT_NEAR(model.viewer.matrix_of_vertexes.matrix[i][1],
                expect_array_y[i - 1], 1e-6);
  }
  for (unsigned int i = 1; i < model.viewer.matrix_of_vertexes.rows; ++i) {
    ASSERT_NEAR(model.viewer.matrix_of_vertexes.matrix[i][2],
                expect_array_z[i - 1], 1e-6);
  }
  model.releaseResources();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
