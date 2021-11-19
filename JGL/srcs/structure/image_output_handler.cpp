#include "jgl.h"

namespace jgl
{

	Image_output::Image_output(jgl::Vector2Uint p_size)
	{
		_size = p_size;

		glGenFramebuffers(1, &_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);

		glBindTexture(GL_TEXTURE_2D, _id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _id, 0);

		_drawBuffers[0] = GL_COLOR_ATTACHMENT0;

		glDrawBuffers(1, _drawBuffers);

		desassociate();
	}

	Image_output::Image_output(jgl::Uint p_width, jgl::Uint p_heigth) : Image_output(jgl::Vector2Uint(p_width, p_heigth))
	{

	}

	void Image_output::associate()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
		glViewport(0u, 0u, _size.x, _size.y);
	}

	void Image_output::desassociate()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		jgl::Application::active_application()->viewport()->use();
	}

	jgl::Image* Image_output::save()
	{
		GLuint output_texture;

		glGenTextures(1, &output_texture);
		glBindTexture(GL_TEXTURE_2D, output_texture);

		glBindTexture(GL_TEXTURE_2D, output_texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, output_texture, 0);
		glViewport(0u, 0u, _size.x, _size.y);

		jgl::Image tmp_image(_id);

		tmp_image.draw(0, jgl::Vector2Int(_size.x, _size.y), jgl::Vector2(0, 0), jgl::Vector2(1, 1), 0, 1.0f);

		desassociate();

		return (new jgl::Image(output_texture));
	}
}