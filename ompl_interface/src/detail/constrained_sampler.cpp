/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2011, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/* Author: Ioan Sucan */

#include "ompl_interface/detail/constrained_sampler.h"

ompl_interface::ConstrainedSampler::ConstrainedSampler(const PlanningGroup *pg, const kinematic_constraints::ConstraintSamplerPtr &cs) :
    ompl::base::StateSampler(pg->getKMStateSpace().getOMPLSpace().get()),
    planning_group_(pg), default_(space_->allocDefaultStateSampler()), constraint_sampler_(cs)
{
}

bool ompl_interface::ConstrainedSampler::sampleC(ompl::base::State *state)
{
    std::vector<double> values;
    if (constraint_sampler_->sample(values, planning_group_->getStartState(), planning_group_->getMaximumSamplingAttempts()))
    {
        planning_group_->getKMStateSpace().copyToOMPLState(state, values);
        return true;
    }
    return false;
}

void ompl_interface::ConstrainedSampler::sampleUniform(ompl::base::State *state)
{
    if (!sampleC(state))
        default_->sampleUniform(state);
}

void ompl_interface::ConstrainedSampler::sampleUniformNear(ompl::base::State *state, const ompl::base::State *near, const double distance)
{
    if (!sampleC(state))
        default_->sampleUniformNear(state, near, distance);
}

void ompl_interface::ConstrainedSampler::sampleGaussian(ompl::base::State *state, const ompl::base::State *mean, const double stdDev)
{
    if (!sampleC(state))
        default_->sampleGaussian(state, mean, stdDev);
}
