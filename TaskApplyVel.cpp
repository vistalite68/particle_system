#include "TaskApplyVel.hpp"
#include <sstream>
#include <iomanip>

TaskApplyVel::TaskApplyVel (cl_context context, cl_device_id device, cl_int nbParticles)
{
	this->_context = context;
	this->_device = device;
	this->_nbParticle = nbParticles;
}

void		TaskApplyVel::createKernel()
{
	this->_createKernel("kernel/applyVel.cl", "applyVel");
}
std::string convert(float value)
{
	std::stringstream ss;
	ss << std::setprecision(std::numeric_limits<float>::digits10+1);
	ss << value;
	return ss.str();
}

void		TaskApplyVel::setNbParticles(int nbParticles)
{
	this->_defineOptions.insert(t_DefineOption("NB_PARTICLES", std::to_string(nbParticles)));
}

void		TaskApplyVel::setGravityDefine(double massPoint, double massParticles, double propConst)
{
	this->_defineOptions.insert(t_DefineOption("MASS_POINT", std::to_string(massPoint)));
	this->_defineOptions.insert(t_DefineOption("MASS_PARTICLES", std::to_string(massParticles)));
	this->_defineOptions.insert(t_DefineOption("G_CONST", convert(propConst)));
	this->_defineOptions.insert(t_DefineOption("DIVIDEND", std::to_string(static_cast<float>(massPoint * massParticles * propConst))));
}
