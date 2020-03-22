#ifndef _SHADER_H
#define _SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class shader
{
public:
	unsigned int ID;
	shader(){}
	shader &use();
	void compile(const char* vertexsource, const char * fragsource, const char* geometrysource = nullptr);
	void set_float(const char* name,  float value, bool useshader = false);
	void set_int(const char* name,  unsigned int  value, bool useshader = false);
	void set_vec2(const char* name, float x, float y, bool useshader = false);
	void set_vec2(const char* name, const glm::vec2 &value, bool useshader = false);
	void set_vec3(const char* name, float x, float y, float z, bool useshader = false);
	void set_vec3(const char* name, const glm::vec3 &value, bool useshader = false);
	void set_vec3(const char* name, float x, float y, float z, float w, bool useshader = false);
	void set_vec3(const char* name, const glm::vec4 &value, bool useshader = false);
	void set_mat2(const char* name, const glm::mat2 &mat, bool useshader = false);
	void set_mat3(const char* name, const glm::mat3 &mat, bool useshader = false);
	void set_mat4(const char* name, const glm::mat4 &mat, bool useshader = false);
private:
	void check_compile_errors(unsigned int obj, const char * type);
};


#endif