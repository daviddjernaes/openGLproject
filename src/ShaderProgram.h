#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#define GLEW_STATIC

#include <map>
#include <string>
#include "GL/glew.h"
#include "glm/glm.hpp"
//#include "glm/gtc/type_precision.hpp"


using std::string;


class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	enum ShaderType
	{
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	bool loadShaders(const char* vsFilename, const char* fsFilename);
	void use();

	void setUniform(const GLchar* name, const glm::vec2& v);
	void setUniform(const GLchar* name, const glm::vec3& v);
	void setUniform(const GLchar* name, const glm::vec4& v);
	void setUniform(const GLchar* name, const glm::mat4& m);
	void setUniform(const GLchar* name, const GLfloat& f);
	void setUniform(const GLchar* name, const GLint& v);
	void setUniformSampler(const GLchar* name, const GLint& slot);



	GLuint getProgram()const;

private:

	string fileToString(const string& filename);
	void checkCompileErrors(GLuint shader, ShaderType type);
	GLint getUniformLocation(const GLchar* name);

	GLuint mHandle;  // leading m is used to denote class 'member' -- just a style thing
	std::map<string, GLint> mUniformLocations;
};

#endif // SHADER_PROGRAM_H