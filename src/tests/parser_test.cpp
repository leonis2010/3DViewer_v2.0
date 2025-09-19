#include "model/parser.h"

#include "gtest/gtest.h"
#include "model/errors.h"

class ParserTest : public ::testing::Test {
 protected:
  void SetUp() override { ClearLogFile(); }

  void TearDown() override {}

  void ClearLogFile() {
    std::ofstream clear_log("logs/debug.log", std::ios::trunc);
    clear_log.close();
  }

  std::string GetLastLogMessage() {
    std::ifstream log_file("logs/debug.log");
    std::string last_line;
    if (log_file.is_open()) {
      std::string line;
      while (std::getline(log_file, line)) {
        last_line = line;
      }
      log_file.close();
    }
    return last_line;
  }
};

TEST(ErrorTest, success) { s21::LogError("ErrorTest", s21::success_code); }

TEST(ErrorTest, memory_malfunction) {
  s21::LogError("ErrorTest", s21::memory_error);
}

TEST(ErrorTest, unknown) { s21::LogError("ErrorTest", s21::unknown_error); }

TEST_F(ParserTest, get_id) {
  s21::WireframeObject obj1("samples/simple.obj");
  EXPECT_EQ(obj1.GetId(), 0);
  s21::WireframeObject obj2("samples/simple.obj");
  EXPECT_EQ(obj1.GetId(), 0);
  EXPECT_EQ(obj2.GetId(), 1);
}

TEST_F(ParserTest, non_existent_file) {
  s21::WireframeObject obj("samples/non_existent_file.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_NE(log_string.find("File not found"), std::string::npos);
  EXPECT_EQ(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "");
  EXPECT_EQ(obj.GetVertices().size(), 0);
  EXPECT_EQ(obj.GetFaces().size(), 0);
}

TEST_F(ParserTest, empty_file) {
  s21::WireframeObject obj("samples/empty.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_NE(log_string.find("Invalid file format"), std::string::npos);
  EXPECT_EQ(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "");
  EXPECT_EQ(obj.GetVertices().size(), 0);
  EXPECT_EQ(obj.GetFaces().size(), 0);
}

TEST_F(ParserTest, corrupt_sample_0) {
  s21::WireframeObject obj("samples/corrupt_sample_0.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_NE(log_string.find("Invalid format in line"), std::string::npos);
  EXPECT_EQ(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "");
  EXPECT_EQ(obj.GetVertices().size(), 0);
  EXPECT_EQ(obj.GetFaces().size(), 0);
}

TEST_F(ParserTest, corrupt_sample_1) {
  s21::WireframeObject obj("samples/corrupt_sample_1.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_NE(log_string.find("Invalid format in line"), std::string::npos);
  EXPECT_EQ(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "");
  EXPECT_EQ(obj.GetVertices().size(), 0);
  EXPECT_EQ(obj.GetFaces().size(), 0);
}

TEST_F(ParserTest, corrupt_sample_2) {
  s21::WireframeObject obj("samples/corrupt_sample_2.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_NE(log_string.find("Invalid format in line"), std::string::npos);
  EXPECT_EQ(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "");
  EXPECT_EQ(obj.GetVertices().size(), 0);
  EXPECT_EQ(obj.GetFaces().size(), 0);
}

TEST_F(ParserTest, corrupt_sample_3) {
  s21::WireframeObject obj("samples/corrupt_sample_3.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_NE(log_string.find("Invalid format in line"), std::string::npos);
  EXPECT_EQ(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "");
  EXPECT_EQ(obj.GetVertices().size(), 0);
  EXPECT_EQ(obj.GetFaces().size(), 0);
}

TEST_F(ParserTest, corrupt_sample_4) {
  s21::WireframeObject obj("samples/corrupt_sample_4.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_NE(log_string.find("Invalid"), std::string::npos);
  EXPECT_EQ(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "");
  EXPECT_EQ(obj.GetVertices().size(), 0);
  EXPECT_EQ(obj.GetFaces().size(), 0);
}

TEST_F(ParserTest, valid_sample_0) {
  s21::WireframeObject obj("samples/simple.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_EQ(log_string, "");
  EXPECT_NE(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "simple.obj");
  EXPECT_GT(obj.GetVertices().size(), 0);
  EXPECT_GT(obj.GetFaces().size(), 0);
}

TEST_F(ParserTest, valid_sample_1) {
  s21::WireframeObject obj("samples/simple_v_vt.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_EQ(log_string, "");
  EXPECT_NE(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "simple_v_vt.obj");
  EXPECT_GT(obj.GetVertices().size(), 0);
  EXPECT_GT(obj.GetFaces().size(), 0);
}

TEST_F(ParserTest, valid_sample_2) {
  s21::WireframeObject obj("samples/simple_v.obj");
  std::string log_string = GetLastLogMessage();
  EXPECT_EQ(log_string, "");
  EXPECT_NE(obj.GetId(), -1);
  EXPECT_EQ(obj.GetName(), "simple_v.obj");
  EXPECT_GT(obj.GetVertices().size(), 0);
  EXPECT_GT(obj.GetFaces().size(), 0);
}

TEST_F(ParserTest, copy_constructor) {
  s21::WireframeObject obj1("samples/simple.obj");
  s21::WireframeObject obj2(obj1);
  EXPECT_NE(obj1.GetId(), obj2.GetId());
  EXPECT_EQ(obj1.GetVertices().size(), obj2.GetVertices().size());
}

TEST_F(ParserTest, copy_operator) {
  s21::WireframeObject obj1("samples/simple.obj");
  s21::WireframeObject obj2("samples/simple_v.obj");
  obj2 = obj1;
  EXPECT_NE(obj1.GetId(), obj2.GetId());
  EXPECT_EQ(obj1.GetVertices().size(), obj2.GetVertices().size());
}

TEST_F(ParserTest, assign_name) {
  s21::WireframeObject obj("samples/simple.obj");
  obj.AssignName("Hi");
  EXPECT_EQ(obj.GetName(), "Hi");
}
