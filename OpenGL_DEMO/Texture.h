#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<stb/stb_image/stb_image.h>
#include<glad/glad.h>

#include"shaderClass.h"

class Texture
{
public:
	GLuint ID;
	GLenum type;

	//How we load the texture
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);;

	//allows us to write to shader, our texture to implant onto square
	void texUnit(Shader& shader, const char* uniform, GLuint unit);

	//Bind, unbind, and delete
	void Bind();
	void Unbind();
	void Delete();

};

#endif