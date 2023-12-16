#include "render.h"
int main()
{
	Render render;
	render.InitWindowApp(1200, 900);
	render.InitScene("C:\\Users\\User\\Desktop\\c++\\deer.obj", "F:\\github\\OBJ-viewer\\Vulkan\\shader resources\\shadow map shaders\\SPIR-V_vertex_shader.spv", "F:\\github\\OBJ-viewer\\Vulkan\\shader resources\\shadow map shaders\\SPIR-V_fragment_shader.spv", "F:\\github\\OBJ-viewer\\Vulkan\\shader resources\\basic shaders\\SPIR-V_vertex_shader.spv", "F:\\github\\OBJ-viewer\\Vulkan\\shader resources\\basic shaders\\SPIR-V_fragment_shader.spv", 1200, 900, 9600, 7200);
	render.InitSyncPrimitives();
	render.Draw(1200, 900, 9600, 7200);
}