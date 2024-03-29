#include "jgl.h"

#include "jgl_drawing_function.h"

namespace jgl
{
	extern jgl::Vector2Int g_viewport_actual_size;

	Image_output::Image_output(jgl::Vector2Uint p_size)
	{
		desassociate();
		_size = p_size;

		glGenFramebuffers(1, &_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _id, 0);

		glGenTextures(1, &_depthBuffer);
		glBindTexture(GL_TEXTURE_2D, _depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _size.x, _size.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);

		desassociate();
	}

	Image_output::Image_output(jgl::Uint p_width, jgl::Uint p_heigth) : Image_output(jgl::Vector2Uint(p_width, p_heigth))
	{

	}

	void Image_output::associate()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
		clear();
	}

	void Image_output::clear()
	{
		glViewport(0, 0, _size.x, _size.y);
		glColorMask(TRUE, TRUE, TRUE, TRUE);
		glClearColor(0, 0, 0, 0);
		g_viewport_actual_size = jgl::Vector2Int(_size.x, _size.y);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Image_output::desassociate()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		jgl::Application::active_application()->viewport()->use();
		jgl::Application::active_application()->reset();
	}

	void Image_output::check_framebuffer_status(jgl::String msg)
	{
		jgl::cout << msg << " - Framebuffer status : ";

		switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
		{
		case (GL_FRAMEBUFFER_UNDEFINED): {
			jgl::cout << "GL_FRAMEBUFFER_UNDEFINED"; break; }
		case (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT): {
			jgl::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break; }
		case (GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT): {
			jgl::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break; }
		case (GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER): {
			jgl::cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break; }
		case (GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER): {
			jgl::cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break; }
		case (GL_FRAMEBUFFER_UNSUPPORTED): {
			jgl::cout << "GL_FRAMEBUFFER_UNSUPPORTED"; break; }
		case (GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE): {
			jgl::cout << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break; }
		case (GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS): {
			jgl::cout << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; break; }
		case (GL_FRAMEBUFFER_COMPLETE): {
			jgl::cout << "GL_FRAMEBUFFER_COMPLETE"; break; }
		}
		jgl::cout << jgl::endl;
	}

	jgl::Image* Image_output::save()
	{
		//Creation of the texture output
		GLuint output_texture;

		glGenTextures(1, &output_texture);
		glBindTexture(GL_TEXTURE_2D, output_texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//Setting du framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, output_texture, 0);

		_drawBuffers[0] = GL_COLOR_ATTACHMENT0;

		glDrawBuffers(1, _drawBuffers);

		//Render
		jgl::Image::draw(_id, jgl::Vector2Int(0, 0), jgl::Vector2Int(_size.x, _size.y), jgl::Vector2(0, 0), jgl::Vector2(1, 1), 0, 1.0f);

		return (new jgl::Image(output_texture));
	}

	void Image_output::print()
	{
		GLsizei nrChannels = 4;
		GLsizei stride = nrChannels * _size.x;
		GLsizei bufferSize = stride * _size.y;
		std::vector<char> buffer(bufferSize);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _id);

		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glGetTextureImage(_id, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferSize, buffer.data());

		jgl::cout << "Image output buffer : " << jgl::endl;
		for (jgl::Size_t y = 0; y < _size.y; y++)
		{
			for (jgl::Size_t x = 0; x < _size.x; x++)
			{
				if (x != 0)
					jgl::cout << " - ";

				for (jgl::Size_t i = 0; i < 4; i++)
				{
					jgl::cout << "[" << jgl::normalize_int(static_cast<unsigned char>(buffer[i + x * 4 + y * _size.x * 4]), ' ', 3) << "]";
				}
			}
			jgl::cout << jgl::endl;
		}
		jgl::cout << jgl::endl;
	}

	void Image_output::save_to_png(jgl::String p_path)
	{
		GLsizei nrChannels = 4;
		GLsizei stride = nrChannels * _size.x;
		GLsizei bufferSize = stride * _size.y;
		std::vector<char> buffer(bufferSize);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _id);

		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glGetTextureImage(_id, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferSize, buffer.data());

		if (p_path.size() > 4 && p_path.substr(p_path.size() - 4, 4) != ".png")
		{
			jgl::save_to_png(p_path, _size, buffer.data());
		}
		else if (p_path.size() > 4 && p_path.substr(p_path.size() - 4, 4) != ".jpg")
		{
			jgl::save_to_jpg(p_path, _size, buffer.data());
		}
		else if (p_path.size() > 4 && p_path.substr(p_path.size() - 4, 4) != ".bmp")
		{
			jgl::save_to_bmp(p_path, _size, buffer.data());
		}
		else if (p_path.size() > 4 && p_path.substr(p_path.size() - 4, 4) != ".tga")
		{
			jgl::save_to_tga(p_path, _size, buffer.data());
		}
		else
		{
			THROW_EXCEPTION(jgl::Error_level::Warning, 1, "Can't save such image");
		}
	}
}