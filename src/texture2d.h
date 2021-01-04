#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#define GLEW_STATIC
#include "GL/glew.h"
#include <string>
using std::string;

class Texture2d
{
public:
	Texture2d();
	virtual ~Texture2d();

	bool loadTexture(const string& filename, bool generateMipMaps = true);
	void bind(GLuint texUnit = 0);
	void unbind(GLuint texUnit = 0);

private:
	Texture2d(const Texture2d& rhs) {}
	Texture2d& operator = (const Texture2d& rhs) {}
	GLuint mTexture;
};


#endif //TEXTURE2D_H
