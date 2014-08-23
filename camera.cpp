#include "camera.h"
#include <cmath>

SF3dVector F3dVector(GLfloat x, GLfloat y, GLfloat z)
{
	SF3dVector tmp;
	tmp.x = x;
	tmp.y = y;
	tmp.z = z;
	return tmp;
}

GLfloat GetF3dVectorLength(SF3dVector const &v)
{
	return sqrt(v.x* v.x+ v.y* v.y+ v.z* v.z);
}

SF3dVector Normalize3dVector(SF3dVector v)
{
	SF3dVector res;
	float l= GetF3dVectorLength(v);
	if (l== 0.0f) return F3dVector(0, 0, 0);
	res.x= v.x/ l;
	res.y= v.y/ l;
	res.z= v.z/ l;
	return res;
}

SF3dVector operator+ (SF3dVector v, SF3dVector u)
{
	SF3dVector res;
	res.x= v.x+ u.x;
	res.y= v.y+ u.y;
	res.z= v.z+ u.z;
	return res;
}

SF3dVector operator- (SF3dVector v, SF3dVector u)
{
	SF3dVector res;
	res.x= v.x- u.x;
	res.y= v.y- u.y;
	res.z= v.z- u.z;
	return res;
}


SF3dVector operator* (SF3dVector v, float r)
{
	SF3dVector res;
	res.x= v.x* r;
	res.y= v.y* r;
	res.z= v.z* r;
	return res;
}

SF3dVector CrossProduct (SF3dVector* u, SF3dVector* v)
{
	SF3dVector resVector;
	resVector.x= u->y* v->z- u->z* v->y;
	resVector.y= u->z* v->x- u->x* v->z;
	resVector.z= u->x* v->y- u->y* v->x;

	return resVector;
}
float operator* (SF3dVector v, SF3dVector u)
{
	return v.x*u.x+v.y*u.y+v.z*u.z;
}

Camera::Camera()
: Position(F3dVector(0, 0, 0))
, ViewDir(F3dVector(0, 0, -1))
, RightVector(F3dVector(1, 0, 0))
, UpVector(F3dVector(0, 1, 0))
, RotatedX(0)
, RotatedY(0)
, RotatedZ(0)
{}

void Camera::Move(SF3dVector Direction)
{
	Position = Position + Direction;
}

void Camera::RotateX (GLfloat Angle)
{
	RotatedX += Angle;
	
	//Rotate viewdir around the right vector:
	ViewDir = Normalize3dVector(ViewDir*cos(Angle)
								+ UpVector*sin(Angle));

	//now compute the new UpVector (by cross product)
	UpVector = CrossProduct(&ViewDir, &RightVector)* -1;
}

void Camera::RotateY(GLfloat Angle)
{
	RotatedY += Angle;
	
	//Rotate viewdir around the up vector:
	ViewDir = Normalize3dVector(ViewDir*cos(Angle)
								- RightVector*sin(Angle));

	//now compute the new RightVector (by cross product)
	RightVector = CrossProduct(&ViewDir, &UpVector);
}

void Camera::RotateZ(GLfloat Angle)
{
	RotatedZ += Angle;
	
	//Rotate viewdir around the right vector:
	RightVector = Normalize3dVector(RightVector*cos(Angle)
								+ UpVector*sin(Angle));

	//now compute the new UpVector (by cross product)
	UpVector = CrossProduct(&ViewDir, &RightVector)*-1;
}

void Camera::Render() const
{

	//The point at which the camera looks:
	SF3dVector ViewPoint = Position+ViewDir;

	//as we know the up vector, we can easily use gluLookAt:
	gluLookAt(	Position.x,Position.y,Position.z,
				ViewPoint.x,ViewPoint.y,ViewPoint.z,
				UpVector.x,UpVector.y,UpVector.z);

}

void Camera::MoveForward(GLfloat Distance)
{
	Position= Position+ ViewDir* -Distance;
}

void Camera::StrafeRight(GLfloat Distance)
{
	Position= Position+ RightVector* Distance;
}

void Camera::MoveUpward(GLfloat Distance)
{
	Position= Position+ UpVector* Distance;
}

