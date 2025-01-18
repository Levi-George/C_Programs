#include "Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{

	type = texType;

	//Texture acquisition
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load("pop_cat.png", &widthImg, &heightImg, &numColCh, 0);

	//generate an opengl texture
	glGenTextures(1, &ID);

	//put the texture into an opengl slot
	glActiveTexture(slot);

	//bind the texture to a texture unit
	glBindTexture(type, ID);

	//establish the texture behavior
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//setup how the texture repeats if it is required
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//If we want to use clamp border, we need a color array of GLUINTs and a TexParameter call
	//float flatColor[] = { 0.0f, 0.1f, 0.2f, 0.3f };
	//glTexParameteri(type, GL_TEXTURE_BORDER_COLOR, flatColor);

	//assign image to an opengl texture object
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);

	//generate mipmap
	glGenerateMipmap(type);

	//free image and then unbind texture
	stbi_image_free(bytes);
	glBindTexture(type, 0);
}


void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	//get tex0 var from shader prog
	GLuint tex0Uni = glGetUniformLocation(shader.ID, "tex0");

	//activate shader to allow modification of tex0
	shader.Activate();

	//set uniform value
	glUniform1i(tex0Uni, unit);


}

void Texture::Bind()
{
	//bind the texture
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	//unbind the texture by binding 0
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	//delete the texture
	glDeleteTextures(1, &ID);
}