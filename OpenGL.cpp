#include "OpenGL.hpp"

OpenGL::OpenGL(void) 
	: _width(640), _height(480), _nbParticles(100000) {
	this->_initOpenGL();
}

OpenGL::OpenGL(int32_t width, int32_t height, int32_t nbParticles, float borderSize) 
	: _width(width), _height(height), _nbParticles(nbParticles), _borderSize(borderSize) {
	this->_initOpenGL();
}

void	OpenGL::setBorderSize(float borderSize)
{
	this->_borderSize = borderSize;
	this->_setTrans();
}

void	OpenGL::_initOpenGL(void) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glViewport(0, 0, this->_width, this->_height);
	glEnable(GL_PROGRAM_POINT_SIZE);
	this->_initShader();
	this->_initBuffer();
	glUseProgram(this->_shader_program);
	this->_setUniformLocation();
	this->_setStaticUniform();
	this->_setTrans();
}

std::string *OpenGL::_getSrc(std::string filename) const {
    std::ifstream ifs(filename);
    
    return new std::string( (std::istreambuf_iterator<char>(ifs) ),
                            (std::istreambuf_iterator<char>()) );
}

void		OpenGL::_initBuffer(void)
{
	GLuint						attrloc;

	glGenVertexArrays(1, &(this->_vao));
	glBindVertexArray(this->_vao);
	glGenBuffers(2, this->_vbo);


	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * this->_nbParticles, NULL, GL_STREAM_DRAW);
	attrloc = glGetAttribLocation(this->_shader_program, "in_Position");
	glVertexAttribPointer(attrloc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrloc);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 1 * this->_nbParticles, NULL, GL_STREAM_DRAW);
	attrloc = glGetAttribLocation(this->_shader_program, "in_Dist");
	glVertexAttribPointer(attrloc, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrloc);
	glBindVertexArray(0);
}

void		OpenGL::draw(void)
{
	this->setDynUniform();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(this->_shader_program);
	glBindVertexArray(this->_vao);
	glDrawArrays(GL_POINTS, 0, this->_nbParticles);
	glFinish();
}

void		OpenGL::_setTrans(void)
{
	glUniform4f(this->_uloc_T, 0.f, 0.f, -this->_borderSize, 0.f);
}

void		OpenGL::_setUniformLocation(void)
{
	this->_uloc_P = glGetUniformLocation(this->_shader_program, "P");
	this->_uloc_R = glGetUniformLocation(this->_shader_program, "R");
	this->_uloc_T = glGetUniformLocation(this->_shader_program, "T");
}

void		OpenGL::_setStaticUniform(void)
{
	GLfloat		*proj_matrix;

	proj_matrix = Matrix::get_projection(0.785f, this->_width / this->_height, 0.1, 600000);
	glUniformMatrix4fv(this->_uloc_P, 1, GL_FALSE, proj_matrix);
	delete proj_matrix;
}

void		OpenGL::setDynUniform(void)
{
	GLfloat		*rot_matrix;
	static float	alpha = 0.0f * M_PI;

	//alpha += 0.005f;
	if (alpha > 2.f * M_PI)
		alpha -= 2.f * M_PI;
	rot_matrix = Matrix::get_rot_m(alpha);
	glUniformMatrix4fv(this->_uloc_R, 1, GL_FALSE, rot_matrix);
	delete rot_matrix;
}

void		OpenGL::_initShader(void)
{
	std::string		*vertSrc;
	std::string		*fragSrc;

	vertSrc = this->_getSrc("shaders/vertex_shader.vert");
	fragSrc = this->_getSrc("shaders/fragment_shader.frag");
	this->_vs = this->_getShader(vertSrc, GL_VERTEX_SHADER);
	this->_fs = this->_getShader(fragSrc, GL_FRAGMENT_SHADER);
	delete vertSrc;
	delete fragSrc;
	if (!(this->_shader_program = glCreateProgram()))
		throw OpenGLException();
	glAttachShader(this->_shader_program, this->_vs);
	glAttachShader(this->_shader_program, this->_fs);
	glLinkProgram(this->_shader_program);
	glDetachShader(this->_shader_program, this->_vs);
	glDetachShader(this->_shader_program, this->_fs);
	glDeleteShader(this->_vs);
	glDeleteShader(this->_fs);
	glUseProgram(this->_shader_program);
}

GLuint		OpenGL::_getShader(const std::string *shaderSrc, GLenum shaderType)
{
	GLuint		shader;
	GLint		shader_ok;
    GLsizei		log_length;
    char		info_log[8192];
    const char	*src;

	src = shaderSrc->c_str();
	shader = glCreateShader(shaderType);
	glShaderSource (shader, 1, &src, NULL);
	glCompileShader (shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
	if (shader_ok != GL_TRUE)
	{
		std::cout << "ERROR: Failed to compile shader" << std::endl;
		glGetShaderInfoLog(shader, 8192, &log_length,info_log);
		std::cout << "ERROR: " << info_log << std::endl;
		glDeleteShader(shader);
		throw OpenGLException();
	}
	return (shader);
}

OpenGL::OpenGL(const OpenGL &src) {
	(void)src;
}

OpenGL::~OpenGL( void ) {
	glDeleteProgram(this->_shader_program);
	glDeleteBuffers(2, this->_vbo);
}

OpenGL &OpenGL::operator=(const OpenGL &src) {
	(void)src;
	return *this;
}

std::ostream &operator<<(std::ostream &stream, const OpenGL &obj) {
	(void)obj;
	return stream;
}

GLuint		OpenGL::getParticlesColorVBO(void)
{
	return this->_vbo[1];
}

GLuint		OpenGL::getParticlesVBO(void)
{
	return this->_vbo[OPENCL_VBO];
}

