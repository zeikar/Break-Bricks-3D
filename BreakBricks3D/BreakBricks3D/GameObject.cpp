#include "GameObject.h"
#include "Graphics.h"
#include "BMPImage.h"
#include <iostream>

GameObject::GameObject()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

GameObject::~GameObject()
{
}

void GameObject::readOBJ(const char* fileName, const char * textureFileName)
{
	surface.readObj(fileName, true, true);


	// 텍스처 있을 때
	if (textureFileName != nullptr)
	{
		// 직접 uv 계산
		surface.vertex_uv_.init(surface.vertex_positions_.num_elements_);
		for (int i = 0; i < surface.vertex_positions_.num_elements_; i++)
		{
			glm::vec2 ray_xy(surface.vertex_positions_[i].x_, surface.vertex_positions_[i].z_);
			glm::vec2 x_axis(1.0f, 0.0f);
			glm::vec3 ray(-surface.vertex_positions_[i].x_, -surface.vertex_positions_[i].y_, -surface.vertex_positions_[i].z_);
			glm::vec3 normal_ray = glm::normalize(ray);
			float theta = acos(dot(glm::normalize(ray_xy), x_axis));

			if (ray_xy.y < 0.0) theta *= -1;

			//surface.vertex_uv_[i].x_ = atan2(normal_ray.z, normal_ray.y) / (2 * 3.141592f) + 0.5f;
			surface.vertex_uv_[i].x_ = theta / 3.141592f + 0.5f ;
			//surface.vertex_uv_[i].y_ = 0.5f - asin(normal_ray.x) / 3.141592f;
			surface.vertex_uv_[i].y_ = surface.vertex_positions_[i].y_ + 0.5f;


			//std::cout << surface.vertex_positions_[i].x_ << ' ' << surface.vertex_positions_[i].y_ << 
				//' ' << surface.vertex_positions_[i].z_ << std::endl;

			//std::cout << surface.vertex_uv_[i].x_ << ' ' << surface.vertex_uv_[i].y_ << "\n\n";
		}



		gl_obj.initPhongSurfaceWithTexture(surface);
		setTexture(textureFileName);
	}
	else
	{
		gl_obj.initPhongSurface(surface);
	}

	//setMaterial();
}

void GameObject::setMaterial(int matType)
{
	gl_obj.mat_.setMaterial(matType);
}

const GL2_Material& GameObject::getMaterial()
{
	return gl_obj.mat_;
}

void GameObject::setTexture(const char * textureFileName)
{
	GLuint textures;
	glGenTextures(1, &textures);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures);

	// bmp image에서 이미지 읽어옴
	int texture_width, texture_height;
	unsigned char *rgb_array = nullptr;
	readBMP24(textureFileName, &texture_width, &texture_height, &rgb_array);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_array);

	// do not forget these options!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexParameter.xhtml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	hasTexture = true;
}

void GameObject::render()
{
	if (hasTexture)
	{
		// uniform to send texture image data
		{
			GLint loc = glGetUniformLocation(Graphics::getInstance().getGL_World().shaders_.program_id_, "my_texture");
			if (loc != -1)
			{
				// 0 is the index of our first texture
				glUniform1i(loc, 0);
			}
		}
		glUniform1f(glGetUniformLocation(Graphics::getInstance().getGL_World().shaders_.program_id_, "has_texture"), 1);
	}
	else
	{
		glUniform1f(glGetUniformLocation(Graphics::getInstance().getGL_World().shaders_.program_id_, "has_texture"), -1);
	}

	//gl_obj.drawPhongSurface();
	gl_obj.applyLighting(Graphics::getInstance().getLight());

	if (hasTexture)
	{
		gl_obj.drawTextureWithShader(Graphics::getInstance().getGL_World().shaders_);
	}
	else
	{
		gl_obj.drawWithShader(Graphics::getInstance().getGL_World().shaders_);
	}
}

void GameObject::setPosition(const glm::vec3 & pos)
{
	translate(pos - position);
}

void GameObject::translate(const glm::vec3& direction)
{
	position += direction;

	surface.translate(TV(direction));

	updateShader();
}

glm::vec3 & GameObject::getScale()
{
	return scale;
}

void GameObject::setScale(const glm::vec3 & s)
{
	scale.x *= s.x;
	scale.y *= s.y;
	scale.z *= s.z;

	surface.scale(s.x, s.y, s.z, true);

	updateShader();
}

void GameObject::updateShader()
{
	// don't need to update if there is no change

	if (hasTexture)
	{
		gl_obj.updatePhongSurfaceWithTexture(surface);
	}
	else
	{
		gl_obj.updatePhongSurface(surface);
	}
}

glm::vec3 & GameObject::getPosition()
{	
	return position;
}
