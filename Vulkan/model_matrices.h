#ifndef MODEL_MATRICES
#define MODEL_MATRICES
#include "glm/gtc/matrix_transform.hpp"
class ModelMatrices
{
public:
	void CreateMatrices(glm::mat4 shadow_matrix, glm::mat4 centering, glm::vec3 shadow_vector, float scale_factor, float rotate_x, float rotate_y, float rotate_z, float translate_x, float translate_y, float translate_z)
	{
		CreateShadowMatrix(shadow_matrix);
		CreateShadowVector(shadow_vector);
		CreateCenteringMatrix(centering);
		CreateScaleMatrix(scale_factor);
		CreateRotateMatrix(rotate_x, rotate_y, rotate_z);
		CreateTranslateMatrix(translate_x, translate_y, translate_z);
		CreatePerspectiveMatrix();
	}
private:
	void CreateShadowMatrix(glm::mat4 shadow_matrix)
	{
		matrix_set.shadow_matrix = shadow_matrix;
	}
	void CreateShadowVector(glm::vec3 shadow_vector)
	{
		matrix_set.shadow_vector = shadow_vector;
	}
	void CreateCenteringMatrix(glm::mat4 centering)
	{
		matrix_set.centering = centering;
	}
	void CreateScaleMatrix(float scale_factor)
	{
		matrix_set.scale = glm::scale(glm::mat4(1.0f), glm::vec3(scale_factor, scale_factor, scale_factor));
	}
	void CreateRotateMatrix(float rotate_x, float rotate_y, float rotate_z)
	{
		glm::mat4 rotate_m_x = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotate_m_y = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotate_m_z = glm::rotate(glm::mat4(1.0f), glm::radians(rotate_z), glm::vec3(0.0f, 0.0f, 1.0f));

		matrix_set.rotate = rotate_m_z * rotate_m_y * rotate_m_x;
	}
	void CreateTranslateMatrix(float translate_x, float translate_y, float translate_z)
	{
		matrix_set.translate = glm::translate(glm::mat4(1.0f), glm::vec3(translate_x, translate_y, translate_z));
	}
	void CreatePerspectiveMatrix()
	{
		matrix_set.project = glm::perspective(90.0f, 800 / (float)600, 0.1f, 100.0f);
		matrix_set.project[1][1] *= -1;
	}
private:
	struct MatrixSet
	{
		glm::mat4 shadow_matrix{ 1.0f };
		glm::mat4 scale{ 1.0f };
		glm::mat4 centering{ 1.0f };
		glm::mat4 rotate{ 1.0f };
		glm::mat4 translate{ 1.0f };
		glm::mat4 project{ 1.0f };

		glm::vec3 shadow_vector{ 1.0f };
	};
public:
	MatrixSet matrix_set;
};
#endif // !SHADOW_MAP_MATRICES
