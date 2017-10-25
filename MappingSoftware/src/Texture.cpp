#include <Texture.h>

Texture::Texture(GLint i_WrapMode_In, GLint i_FilterMode_In, std::string s_Name_In, int i_RGBSoilFormat, GLint i_RGBOpenGLFormat)
{
	//Generate texture for this rectangle
	glGenTextures(1, &ui_Texture);
	glBindTexture(GL_TEXTURE_2D, ui_Texture);

	// Set texture wrapping mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, i_WrapMode_In);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, i_WrapMode_In);
	// Set filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, i_FilterMode_In);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, i_FilterMode_In);

	// Load the texture
	int i_Width, i_Height;
	std::string path = "assets/textures/" + s_Name_In;
	unsigned char* uc_Image = SOIL_load_image(path.c_str(), &i_Width, &i_Height, 0, i_RGBSoilFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, i_RGBOpenGLFormat, i_Width, i_Height, 0, i_RGBOpenGLFormat, GL_UNSIGNED_BYTE, uc_Image);
	SOIL_free_image_data(uc_Image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
}

GLuint* Texture::get_Texture()
{
	return &ui_Texture;
}