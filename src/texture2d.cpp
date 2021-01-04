#include "texture2d.h"
#include <iostream>
#include <cassert>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"



Texture2d::Texture2d()
	: mTexture(0)
{ // nothing to add here 
}

Texture2d::~Texture2d()
{
	glDeleteTextures(1, &mTexture);
}

bool Texture2d::loadTexture(const string& filename, bool generateMipMaps)
{
	int width, height, components;

	unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &components, STBI_rgb_alpha);
	if (imageData == NULL)
	{	
		std::cerr << "Error loading texture '" << filename << "'" << std::endl;
		return false;
	}

	// invert image so the shader gets right orientation
	int widthInBytes = width * 4;
	unsigned char* top = NULL;
	unsigned char* bottom = NULL;
	unsigned char temp = 0;
	int halfHeight = height / 2;
	for (int row = 0; row < halfHeight; row++)
	{
		top = imageData + row * widthInBytes;
		bottom = imageData + (height - row - 1) * widthInBytes;
		for (int col = 0; col < widthInBytes; col++)
		{
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		GL_RGBA,		  // input
		width, 
		height, 
		0,				  // border thickness of image.
		GL_RGBA,		  // output
		GL_UNSIGNED_BYTE, // unsigned char*
		imageData);

	if (generateMipMaps)
		glGenerateMipmap(GL_TEXTURE_2D);
	;
	stbi_image_free(imageData);

	glBindTexture(GL_TEXTURE_2D, 0);


	return true;
}

void Texture2d::bind(GLuint texUnit)
{
	assert(texUnit >= 0 && texUnit < 32);

	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

void Texture2d::unbind(GLuint texUnit)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}
