#include <GL/glut.h>
#include <cmath>

struct SF3dVector
{
	GLfloat x,y,z;
};

struct SF2dVector
{
	GLfloat x,y;
};

SF3dVector F3dVector(GLfloat x, GLfloat y, GLfloat z);

struct Camera
{
	SF3dVector Position;
	SF3dVector ViewDir;
	SF3dVector RightVector;	
	SF3dVector UpVector;

	GLfloat RotatedX, RotatedY, RotatedZ;	
	
	Camera();
	void Render() const;

	void Move(SF3dVector Direction);
	void RotateX(GLfloat Angle);
	void RotateY(GLfloat Angle);
	void RotateZ(GLfloat Angle);

	void MoveForward(GLfloat Distance);
	void MoveUpward(GLfloat Distance);
	void StrafeRight(GLfloat Distance);
};

