#ifndef MODEL_PARSER_H
#define MODEL_PARSER_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "model/errors.h"

namespace s21 {
const int kMaxVertices = 1500000;
const int kMaxFaces = 5000000;

struct Coordinate {
  float x{0.0f}, y{0.0f}, z{0.0f};
  bool IsValid() const {
    return !std::isnan(x) && !std::isnan(y) && !std::isnan(z);
  }
};
struct TextureCoordinate {
  float u{0.0f}, v{0.0f};
  bool IsValid() const {
    return !std::isnan(u) && !std::isnan(v) && u >= -0.01f && u <= 1.01f &&
           v >= -0.01f && v <= 1.01f;
  }
};
struct Face {
  Coordinate *position[3];
  TextureCoordinate *texture[3];
  Coordinate *normal;
};
struct Counter {
  int v = 0, vt = 0, vn = 0, f = 0;
};

/**
 * @class WireframeObject
 * @brief A class representing a 3D wireframe object loaded from an OBJ file
 *
 * WireframeObject handles the parsing and storage of 3D model data including:
 * - Vertices (3D coordinates)
 * - Texture coordinates
 * - Normal vectors
 * - Faces (triangles defined by vertex indices)
 *
 * The class provides functionality to:
 * - Load OBJ files
 * - Store geometric and texture data
 * - Manage object identification
 * - Access model components (vertices, faces, etc.)
 */
class WireframeObject {
 public:
  // Rule of three
  WireframeObject(const std::string file_path);
  ~WireframeObject();
  WireframeObject(const WireframeObject &other);
  WireframeObject &operator=(const WireframeObject &other);

  // Member functions
  void SetName(const std::string &new_name) { name_ = new_name; }
  std::string GetName() const { return name_; }
  int GetId() const { return id_; }
  static void ResetIdCounter() { next_id_ = 0; }
  const auto GetVertices() const { return vertices_; }
  const auto GetFaces() const { return faces_; }

  void AssignName(const std::string file_path) noexcept;

 protected:
  ErrorCode AllocateMemory(std::ifstream &file);
  void GetValues(std::ifstream &file) noexcept;

  // helper functions
  using InputFunction = bool (WireframeObject::*)(std::istringstream &);
  using ParseFunction = void (WireframeObject::*)(std::istringstream &);
  bool CheckVertex(std::istringstream &iss);
  bool CheckTexture(std::istringstream &iss);
  bool CheckNormal(std::istringstream &iss);
  bool CheckFace(std::istringstream &iss);
  void ParseVertex(std::istringstream &iss);
  void ParseTextureCoordinate(std::istringstream &iss);
  void ParseNormal(std::istringstream &iss);
  void ParseFace(std::istringstream &iss);
  bool ValidateCounters() const;

 protected:
  static int next_id_;
  std::string name_;
  std::vector<Coordinate> vertices_;
  std::vector<Face> faces_;
  std::vector<TextureCoordinate> textures_;
  std::vector<Coordinate> normals_;
  int id_ = -1;
  Counter count_;
};  // class WireframeObject
}  // namespace s21

#endif  // MODEL_PARSER_H
