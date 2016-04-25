#ifndef __GLFW_MANAGER__HPP__
# define __GLFW_MANAGER__HPP__

# include "OpenGL.hpp"
# include "OpenCL.hpp"
# include <iostream>
# include <sys/time.h>

class GLFWManager {
	public:
		GLFWManager (void);
		GLFWManager (const GLFWManager &);
		virtual ~GLFWManager ( void );
		GLFWManager &operator=(const GLFWManager &);
		friend std::ostream &operator<< (std::ostream &, const GLFWManager &);
		void		run(void);
		void		setCursorPos(double xPos, double yPos);
	private:
		void		_initGlfw(void);
		void		_tick(void);

		GLFWwindow	*_window;
		int			_width;
		int			_height;
		OpenGL		*_openGL;
		OpenCL		*_openCL;
		int			_frameBufferWidth;
		int			_frameBufferHeight;
		int			_nbParticles;
		double		_xPos;
		double		_yPos;
		int			_nbFrame;
		int64_t		_time;
};

#endif
