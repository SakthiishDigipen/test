#pragma once
#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <Precompiled/pch.h>
#include "ECSComponent.h"


namespace BE
{

    struct AllocatedImage {
        VkImage _image;
        VmaAllocation _allocation;
    };

    struct Texture {
        AllocatedImage image;
        VkImageView imageView;
    };

    struct AllocatedBuffer {
        VkBuffer _buffer;
        VmaAllocation _allocation;
    };

    struct Material {
        VkDescriptorSet textureSet{ VK_NULL_HANDLE };
        VkPipeline pipeline;
        VkPipelineLayout pipelineLayout;
    };
    //Mesh
    struct VertexInputDescription {
        std::vector<VkVertexInputBindingDescription> bindings;
        std::vector<VkVertexInputAttributeDescription> attributes;

        VkPipelineVertexInputStateCreateFlags flags = 0;
    };
    //Mesh
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color;
        glm::vec2 uv;
        static VertexInputDescription get_vertex_description();
    };
    //Mesh
    struct Mesh {
        std::vector<Vertex> _vertices;

        AllocatedBuffer _vertexBuffer;

        bool load_from_obj(const char* filename);
    };
    //MeshPushConstants
    struct MeshPushConstants {
        glm::vec4 data;
        glm::mat4 render_matrix;
    };

    class RenderObject : public IECSComponent
    {
    public:
        Mesh* mesh;

        Material* material;

        glm::mat4 transformMatrix;

    };

    //default array of renderable objects
    std::vector<BE::RenderObject> _renderables;
    //TO-DO random error? 
    std::unordered_map<std::string, Material> _materials;
    std::unordered_map<std::string, Mesh> _meshes;
    std::unordered_map<std::string, Texture> _loadedTextures;
}


#endif