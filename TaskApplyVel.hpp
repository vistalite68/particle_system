#ifndef __TASK_APPLY_VEL__HPP__
# define __TASK_APPLY_VEL__HPP__

# include <iostream>
# include "Task.hpp"

class TaskApplyVel : public Task {
	public:
		TaskApplyVel(cl_context context, cl_device_id device, cl_int nbParticles);
		void		createKernel(void);
		void		setGravityDefine(double massPoint, double massParticles, double propConst);
		void		setNbParticles(int nbParticles);
};

#endif

