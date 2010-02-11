/* Copyright (c) 2007 Scott Lembcke
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
//#include <math.h>

#include "chipmunk.h"
#include "constraints/util.h"

static void
preStep(cpPinJoint *joint, cpFloat dt, cpFloat dt_inv)
{
	cpBody *a = joint->constraint.a;
	cpBody *b = joint->constraint.b;
	
	joint->r1 = cpvrotate(joint->anchr1, a->rot);
	joint->r2 = cpvrotate(joint->anchr2, b->rot);
	
	cpVect delta = cpvsub(cpvadd(b->p, joint->r2), cpvadd(a->p, joint->r1));
	cpFloat dist = cpvlength(delta);
	joint->n = cpvmult(delta, 1.0f/(dist ? dist : (cpFloat)INFINITY));
	
	// calculate mass normal
	joint->nMass = 1.0f/k_scalar(a, b, joint->r1, joint->r2, joint->n);
	
	// calculate bias velocity
	cpFloat maxBias = joint->constraint.maxBias;
	joint->bias = cpfclamp(-joint->constraint.biasCoef*dt_inv*(dist - joint->dist), -maxBias, maxBias);
	
	// compute max impulse
	joint->jnMax = J_MAX(joint, dt);
	
	// apply accumulated impulse
	cpVect j = cpvmult(joint->n, joint->jnAcc);
	apply_impulses(a, b, joint->r1, joint->r2, j);
}

static void
applyImpulse(cpPinJoint *joint)
{
	cpBody *a = joint->constraint.a;
	cpBody *b = joint->constraint.b;
	cpVect n = joint->n;

	// compute relative velocity
	cpFloat vrn = normal_relative_velocity(a, b, joint->r1, joint->r2, n);
	
	// compute normal impulse
	cpFloat jn = (joint->bias - vrn)*joint->nMass;
	cpFloat jnOld = joint->jnAcc;
	joint->jnAcc = cpfclamp(jnOld + jn, -joint->jnMax, joint->jnMax);
	jn = joint->jnAcc - jnOld;
	
	// apply impulse
	apply_impulses(a, b, joint->r1, joint->r2, cpvmult(n, jn));
}

static cpFloat
getImpulse(cpPinJoint *joint)
{
	return cpfabs(joint->jnAcc);
}

static const cpConstraintClass klass = {
	(cpConstraintPreStepFunction)preStep,
	(cpConstraintApplyImpulseFunction)applyImpulse,
	(cpConstraintGetImpulseFunction)getImpulse,
};
CP_DefineClassGetter(cpPinJoint);


cpPinJoint *
cpPinJointAlloc(void)
{
	return (cpPinJoint *)cpmalloc(sizeof(cpPinJoint));
}

cpPinJoint *
cpPinJointInit(cpPinJoint *joint, cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2)
{
	cpConstraintInit((cpConstraint *)joint, &klass, a, b);
	
	joint->anchr1 = anchr1;
	joint->anchr2 = anchr2;
	
	cpVect p1 = cpvadd(a->p, cpvrotate(anchr1, a->rot));
	cpVect p2 = cpvadd(b->p, cpvrotate(anchr2, b->rot));
	joint->dist = cpvlength(cpvsub(p2, p1));

	joint->jnAcc = 0.0f;
	
	return joint;
}

cpConstraint *
cpPinJointNew(cpBody *a, cpBody *b, cpVect anchr1, cpVect anchr2)
{
	return (cpConstraint *)cpPinJointInit(cpPinJointAlloc(), a, b, anchr1, anchr2);
}
