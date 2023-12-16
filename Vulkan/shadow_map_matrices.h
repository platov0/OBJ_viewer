#ifndef SHADOW_MAP_MATRICES
#define SHADOW_MAP_MATRICES
#include "glm/gtc/matrix_transform.hpp"
class ShadowMapMatrices
{
public:
	void CreateMatrices(float ortho_matrix_aspect_ration, float ortho_matrix_near, float ortho_matrix_far, float scale_factor, float rotate_x, float rotate_y, float rotate_z, float offset_x, float offset_y, float offset_z, float translate_x, float translate_y, float translate_z, float light_pos_x, float light_pos_y, float light_pos_z, float light_view_x, float light_view_y, float light_view_z)
	{
		CreateShadowVector(light_pos_x, light_pos_y, light_pos_z, light_view_x, light_view_y, light_view_z);
		CreateLookAtMatrix(light_pos_x, light_pos_y, light_pos_z, light_view_x, light_view_y, light_view_z);
		CreateCenteringMatrix(offset_x, offset_y, offset_z);
		CreateScaleMatrix(scale_factor);
		CreateRotateMatrix(rotate_x, rotate_y, rotate_z);
		CreateTranslateMatrix(translate_x, translate_y, translate_z);
		CreateOrthoMatrix(ortho_matrix_aspect_ration, ortho_matrix_near, ortho_matrix_far);
		CreateShadowMatrix();
	}
private:
	void CreateShadowVector(float light_pos_x, float light_pos_y, float light_pos_z, float light_view_x, float light_view_y, float light_view_z)
	{
		shadow_vector = { light_view_x - light_pos_x,light_view_y - light_pos_y ,light_view_z - light_pos_z };
		shadow_vector = glm::normalize(shadow_vector);
	}
	void CreateLookAtMatrix(float pos_x, float pos_y, float pos_z, float view_x, float view_y, float view_z)
	{
		matrix_set.look_at = glm::lookAt(glm::vec3(pos_x, pos_y, pos_z), glm::vec3(view_x, view_y, view_z), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	void CreateCenteringMatrix(float translate_x, float translate_y, float translate_z)
	{
		matrix_set.centering = glm::translate(glm::mat4(1.0f), glm::vec3(translate_x, translate_y, translate_z));
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
	void CreateOrthoMatrix(float aspect_ration, float near, float far)
	{
		// warning
		float bottom = -(2 / aspect_ration);
		float top = (2 / aspect_ration);

		matrix_set.project = glm::ortho(-2.0f, 2.0f, bottom, top, near, far);

		matrix_set.project[2][2] = 1.0f / (-10.0f);
		matrix_set.project[3][2] = 0;
		matrix_set.project[1][1] *= -1;
	}
	void CreateShadowMatrix()
	{
		shadow_matrix = matrix_set.project * matrix_set.look_at * matrix_set.translate * matrix_set.rotate * matrix_set.centering * matrix_set.scale;
	}
private:
	struct MatrixSet
	{
		glm::mat4 scale{ 1.0f };
		glm::mat4 centering{ 1.0f };
		glm::mat4 rotate{ 1.0f };
		glm::mat4 translate{ 1.0f };
		glm::mat4 look_at{ 1.0f };
		glm::mat4 project{ 1.0f };
	};
public:
	MatrixSet matrix_set;
	glm::vec3 shadow_vector;
	glm::mat4 shadow_matrix;
};
#endif // !SHADOW_MAP_MATRICES

