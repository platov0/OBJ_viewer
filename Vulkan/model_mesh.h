#ifndef MODEL_MESH
#define MODEL_MESH
#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <vector>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/normal.hpp>
#include "tiny_obj_loader.h"
class ModelMesh
{
public:
	void CreateMesh(std::string path)
	{
		LoadMesh(path);
		CalculateBoundaryBox();
		CenteringMesh();
	}
private:
	void LoadMesh(std::string path)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.data()))
		{
			throw std::runtime_error(warn + err);
		}
		// assembling the meshes
		for (auto& shape : shapes)
		{
			for (auto& index : shape.mesh.indices)
			{
				vertices.emplace_back();

				vertices.back().position.x = attrib.vertices[(index.vertex_index * 3) + 0];
				vertices.back().position.y = attrib.vertices[(index.vertex_index * 3) + 1];
				vertices.back().position.z = attrib.vertices[(index.vertex_index * 3) + 2];

				if (index.normal_index != -1)
				{
					vertices.back().normal.x = attrib.normals[(index.normal_index * 3) + 0];
					vertices.back().normal.y = attrib.normals[(index.normal_index * 3) + 1];
					vertices.back().normal.z = attrib.normals[(index.normal_index * 3) + 2];
				}
			}
		}
		// calculating the normals
		for (int i = 0; i < vertices.size() / 3; i++)
		{
			if (!vertices[3 * i].normal.x)
			{
				vertices[(3 * i) + 0].normal = glm::triangleNormal(vertices[(3 * i) + 0].position, vertices[(3 * i) + 1].position, vertices[(3 * i) + 2].position);
				vertices[(3 * i) + 1].normal = glm::triangleNormal(vertices[(3 * i) + 0].position, vertices[(3 * i) + 1].position, vertices[(3 * i) + 2].position);
				vertices[(3 * i) + 2].normal = glm::triangleNormal(vertices[(3 * i) + 0].position, vertices[(3 * i) + 1].position, vertices[(3 * i) + 2].position);
			}
		}
		// fill indices
		for (int i = 0; i < vertices.size(); i++)
		{
			indices.push_back(i);
		}
	}
	void CalculateBoundaryBox()
	{
		//defining the boundaries of the box
		boudary_box.x_min = vertices[0].position.x;
		boudary_box.x_max = vertices[0].position.x;
		boudary_box.y_min = vertices[0].position.y;
		boudary_box.y_max = vertices[0].position.y;
		boudary_box.z_min = vertices[0].position.z;
		boudary_box.z_max = vertices[0].position.z;
		for (Vertex& vertex : vertices)
		{
			//x
			if (vertex.position.x < boudary_box.x_min)
			{
				boudary_box.x_min = vertex.position.x;
			}
			if (vertex.position.x > boudary_box.x_max)
			{
				boudary_box.x_max = vertex.position.x;
			}
			//y
			if (vertex.position.y < boudary_box.y_min)
			{
				boudary_box.y_min = vertex.position.y;
			}
			if (vertex.position.y > boudary_box.y_max)
			{
				boudary_box.y_max = vertex.position.y;
			}
			//z
			if (vertex.position.z < boudary_box.z_min)
			{
				boudary_box.z_min = vertex.position.z;
			}
			if (vertex.position.z > boudary_box.z_max)
			{
				boudary_box.z_max = vertex.position.z;
			}
		}
	}
	void CenteringMesh()
	{
		float delta_x, delta_y, delta_z;

		delta_x = boudary_box.x_max - boudary_box.x_min;
		delta_y = boudary_box.y_max - boudary_box.y_min;
		delta_z = boudary_box.z_max - boudary_box.z_min;

		basic_transform.scale = 1 / (std::max(delta_x, std::max(delta_y, delta_z)));

		float x_center, y_center, z_center;

		x_center = ((boudary_box.x_max - boudary_box.x_min) / 2) + boudary_box.x_min;
		y_center = ((boudary_box.y_max - boudary_box.y_min) / 2) + boudary_box.y_min;
		z_center = ((boudary_box.z_max - boudary_box.z_min) / 2) + boudary_box.z_min;

		basic_transform.x_offset = -x_center * basic_transform.scale;
		basic_transform.y_offset = -y_center * basic_transform.scale;
		basic_transform.z_offset = -z_center * basic_transform.scale;
	}
private:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
	};
	struct BoundaryBox
	{
		float x_min;
		float x_max;
		float y_min;
		float y_max;
		float z_min;
		float z_max;
	};
	struct BasicTransform
	{
		float scale;
		float x_offset;
		float y_offset;
		float z_offset;
	};
public:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	BoundaryBox boudary_box{};
	BasicTransform basic_transform{};
};
#endif
