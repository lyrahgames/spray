#ifndef SPRAY_RAY_TRACER_OBJ_LOADER_H_
#define SPRAY_RAY_TRACER_OBJ_LOADER_H_

#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "scene.h"

namespace spray {

class Obj_loader {
 public:
  using vertex_type = Eigen::Vector3f;
  using normal_type = Eigen::Vector3f;
  using uv_type = Eigen::Vector2f;
  using face_point_type = Eigen::Vector3i;
  using face_type = std::vector<face_point_type>;

  Obj_loader() = default;
  Obj_loader(const std::string& file_path) {
    std::fstream file(file_path, std::ios::in);
    if (!file)
      throw std::runtime_error("The file '" + file_path +
                               "' could not be opened! Does it exist?");

    std::string command;
    while (file >> command) {
      std::string line;
      std::getline(file, line);
      std::stringstream arguments{line};

      if (command == "v") {
        vertex_type vertex;
        for (int i = 0; i < 3; ++i) arguments >> vertex[i];
        vertex_data.push_back(vertex);
      } else if (command == "vn") {
        normal_type normal;
        for (int i = 0; i < 3; ++i) arguments >> normal[i];
        normal_data.push_back(normal);
      } else if (command == "vt") {
        uv_type uv;
        for (int i = 0; i < 2; ++i) arguments >> uv[i];
        uv_data.push_back(uv);
      } else if (command == "f") {
        face_type face;
        face_point_type face_point;
        arguments >> face_point[0];
        char separator;
        arguments.get(separator);
        if (separator == '/') {
          separator = arguments.peek();
          if (separator == '/') {  // v//vn mode
            face_point[1] = 0;
            arguments.ignore();  // we already know == '/'
            arguments >> face_point[2];
            face.push_back(face_point);
            while (arguments >> face_point[0]) {
              // arguments.ignore(2);
              arguments.get(separator);  // == '/'
              arguments.get(separator);  // == '/'
              arguments >> face_point[2];
              face.push_back(face_point);
            }
          } else {
            arguments >> face_point[1];
            arguments.get(separator);
            if (separator == '/') {  // v/vt/vn mode
              arguments >> face_point[2];
              face.push_back(face_point);
              while (arguments >> face_point[0]) {
                arguments.get(separator);  // == '/'
                arguments >> face_point[1];
                arguments.get(separator);  // == '/'
                arguments >> face_point[2];
                face.push_back(face_point);
              }
            } else {  // v/vt mode
              face_point[2] = 0;
              face.push_back(face_point);
              while (arguments >> face_point[0]) {
                arguments.get(separator);  // == '/'
                arguments >> face_point[1];
                face.push_back(face_point);
              }
            }
          }
        } else {  // v mode
          face_point[1] = 0;
          face_point[2] = 0;
          face.push_back(face_point);
          while (arguments >> face_point[0]) face.push_back(face_point);
        }

        for (auto& face_point : face) {
          face_point[0] = (face_point[0] < 0)
                              ? (vertex_data.size() + face_point[0] + 1)
                              : (face_point[0]);
          face_point[1] = (face_point[1] < 0)
                              ? (uv_data.size() + face_point[1] + 1)
                              : (face_point[1]);
          face_point[2] = (face_point[2] < 0)
                              ? (normal_data.size() + face_point[2] + 1)
                              : (face_point[2]);
        }

        face_data.push_back(face);
      } else {
        // std::cout << "Ignoring: " << command << " " << line << std::endl;
      }
    }

    // for (const auto& vertex : vertex_data)
    //   std::cout << vertex.transpose() << std::endl;

    // for (const auto& normal : normal_data)
    //   std::cout << normal.transpose() << std::endl;

    // for (const auto& face : face_data) {
    //   for (const auto& face_point : face) {
    //     std::cout << face_point[0] << "/" << face_point[1] << "/"
    //               << face_point[2] << "\t";
    //   }
    //   std::cout << std::endl;
    // }
  }

  Scene operator()() {
    Scene scene;
    scene.vertex_data().resize(vertex_data.size());
    for (std::size_t i = 0; i < vertex_data.size(); ++i) {
      scene.vertex_data()[i].position = vertex_data[i];
    }

    scene.normal_data().resize(normal_data.size());
    for (std::size_t i = 0; i < normal_data.size(); ++i) {
      scene.normal_data()[i] = normal_data[i];
    }

    // scene.vertex_data().reserve(vertex_data.size());
    // std::vector<int> normal_cache;
    // normal_cache.reserve(vertex_data.size());

    scene.primitive_data().reserve(face_data.size());

    for (const auto& face : face_data) {
      for (int i = 2; i < static_cast<int>(face.size()); ++i) {
        Scene::primitive primitive;
        primitive.vertex_id[0] = face[0][0] - 1;
        primitive.vertex_id[1] = face[i - 1][0] - 1;
        primitive.vertex_id[2] = face[i][0] - 1;

        if (face[0][2] == 0) {
          Eigen::Vector3f normal =
              (scene.vertex_data()[primitive.vertex_id[1]].position -
               scene.vertex_data()[primitive.vertex_id[0]].position)
                  .cross(scene.vertex_data()[primitive.vertex_id[2]].position -
                         scene.vertex_data()[primitive.vertex_id[0]].position);
          normal.normalize();

          primitive.normal_id[0] = scene.normal_data().size();
          primitive.normal_id[1] = scene.normal_data().size();
          primitive.normal_id[2] = scene.normal_data().size();

          scene.normal_data().push_back(normal);
        } else {
          primitive.normal_id[0] = face[0][2] - 1;
          primitive.normal_id[1] = face[i - 1][2] - 1;
          primitive.normal_id[2] = face[i][2] - 1;
        }

        // primitive.normal =
        //     (scene.vertex_data()[primitive.vertex_id[1]].position -
        //      scene.vertex_data()[primitive.vertex_id[0]].position)
        //         .cross(scene.vertex_data()[primitive.vertex_id[2]].position -
        //                scene.vertex_data()[primitive.vertex_id[0]].position);
        // primitive.normal.normalize();
        scene.primitive_data().push_back(primitive);
      }
    }

    return scene;
  }

  // private:
  std::vector<vertex_type> vertex_data;
  std::vector<normal_type> normal_data;
  std::vector<uv_type> uv_data;
  std::vector<face_type> face_data;
};

}  // namespace spray

#endif  // SPRAY_RAY_TRACER_OBJ_LOADER_H_