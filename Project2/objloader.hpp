#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include <vector>
#include <glm/glm.hpp>
#include <iostream>

bool loadOBJ(
    const std::string& path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals
);

#endif // OBJLOADER_HPP
