#ifndef LOAD_MODEL_HPP
#define LOAD_MODEL_HPP

#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include "tiny_obj_loader.h"

struct Vertex
{
    alignas(16) glm::vec3 pos;
    alignas(16) glm::vec3 color;
    alignas(16) glm::vec3 normal;
    alignas(8) glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 4>
    getAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 4>
            attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, normal);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    bool operator==(const Vertex &other) const
    {
        return pos == other.pos && color == other.color &&
               texCoord == other.texCoord;
    }
};

namespace std
{
template <> struct hash<Vertex>
{
    size_t operator()(Vertex const &vertex) const
    {
        return ((hash<glm::vec3>()(vertex.pos) ^
                 (hash<glm::vec3>()(vertex.color) << 1)) >>
                1) ^
               (hash<glm::vec2>()(vertex.texCoord) << 1);
    }
};
} // namespace std

struct Shape
{
    uint32_t indexCount;
    uint32_t firstIndex;

    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkBuffer> vertUniformBuffers;
    std::vector<VkDeviceMemory> vertUniformBuffersMemory;
    std::vector<void *> vertUniformBuffersMapped;

    std::vector<VkBuffer> fragUniformBuffers;
    std::vector<VkDeviceMemory> fragUniformBuffersMemory;
    std::vector<void *> fragUniformBuffersMapped;
};

struct LMRetValue
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Shape> shapes;
};

const std::string _LM_MODEL_PATH = "assets/models/pool_table/POOL TABLE.obj";

// this edits the vertices parameter to center the shape
inline void centerPolygon(Shape shape, std::vector<uint32_t> indices,
                          std::vector<Vertex> &vertices)
{
    glm::vec3 centroid = glm::vec3(0);

    std::unordered_map<uint32_t, uint32_t> uniqueVertices{};

    for (int i = shape.firstIndex; i < shape.firstIndex + shape.indexCount; i++)
    {
        Vertex vertex = vertices[indices[i]];

        if (uniqueVertices.count(indices[i]) > 0)
            continue;

        uniqueVertices[indices[i]] = static_cast<uint32_t>(vertices.size());

        centroid += vertex.pos;
    }

    centroid /= uniqueVertices.size();

    std::cout << "Centroid is: " << glm::to_string(centroid) << std::endl;

    std::cout << "Radius is: "
              << glm::distance(vertices[indices[shape.firstIndex]].pos,
                               centroid)
              << std::endl;

    uniqueVertices.clear();

    for (int i = shape.firstIndex; i < shape.firstIndex + shape.indexCount; i++)
    {
        Vertex *vertex = &vertices[indices[i]];

        if (uniqueVertices.count(indices[i]) > 0)
        {
            continue;
        }
        if (indices[i] == 3380)
        {
        }

        uniqueVertices[indices[i]] = static_cast<uint32_t>(vertices.size());

        vertex->pos -= centroid;
    }
}

inline LMRetValue lmLoadModel()
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Shape> shapes_all;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                          _LM_MODEL_PATH.c_str()))
    {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (size_t shape_index = 0; shape_index < shapes.size(); shape_index++)
    {

        Shape new_shape{};
        new_shape.firstIndex = indices.size();
        for (const auto &index : shapes[shape_index].mesh.indices)
        {
            // std::cout << index.vertex_index;
            Vertex vertex{};

            vertex.pos = {attrib.vertices[3 * index.vertex_index + 0],
                          attrib.vertices[3 * index.vertex_index + 1],
                          attrib.vertices[3 * index.vertex_index + 2]};

            // shift everything to center
            vertex.pos.x += -0.13;

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

            // Check if `normal_index` is zero or positive. negative = no normal
            // data
                vertex.normal = {attrib.normals[3 * index.normal_index + 0],
                              attrib.normals[3 * index.normal_index + 1],
                              attrib.normals[3 * index.normal_index + 2]};

            vertex.color = {1.0f, 1.0f, 1.0f};

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
            new_shape.indexCount++;
        }
        shapes_all.push_back(new_shape);

        // this is for adding all the balls since for some reason
        // they are not separate shapes
        if (shape_index == 10)
        {
            std::cout << "adding balls!" << std::endl;
            shapes_all.pop_back();
            for (int i = 0; i < 16; i++)
            {
                Shape billiard_ball{};
                billiard_ball.firstIndex = new_shape.firstIndex + (672 * i);
                billiard_ball.indexCount = 672;
                shapes_all.push_back(billiard_ball);
            }
        }
    }
    centerPolygon(shapes_all[2], indices, vertices);
    centerPolygon(shapes_all[7], indices, vertices);
    centerPolygon(shapes_all[8], indices, vertices);
    for (size_t i = 10; i < shapes_all.size(); i++){
        centerPolygon(shapes_all[i], indices, vertices);
    }
    // centerPolygon(shapes_all[10], indices, vertices);
    std::cout << "Num Vertices: " << vertices.size() << std::endl;
    LMRetValue ret{};
    ret.vertices = vertices;
    ret.indices = indices;
    ret.shapes = shapes_all;

    return ret;
}
#endif
