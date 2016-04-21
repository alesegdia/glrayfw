
#include "camera.h"

Camera::Camera() :
	fieldOfView( cml::rad(60.f) ),
	aspect( 4.f / 3.f ),
	nearClip( 0.01f ),
	farClip( 100.f ),
	position( 0.f, 0.f, -1.f ),
	horizontalAngle( 0.f ),
	verticalAngle( 0.f )
{
	ComputeProjection();
}

void Camera::SetAspect( float a )
{
	aspect = a;
}

void Camera::SetPosition( const cml::vector3f& pos )
{
	position = pos;
}

void Camera::OffsetPosition( const cml::vector3f& offsetPosition )
{
	position += offsetPosition;
}

const cml::vector3f& Camera::GetPosition() const
{
	return position;
}

void Camera::OffsetAngle( float hor, float ver )
{
	horizontalAngle += hor;
	verticalAngle += ver;
}

float Camera::GetHorizontalAngle() const
{
	return horizontalAngle;
}

float Camera::GetVerticalAngle() const
{
	return verticalAngle;
}

void Camera::ComputeProjection()
{
	projection = cml::identity<4>();
	cml::matrix_perspective_xfov_RH( projection, fieldOfView, aspect, nearClip, farClip, cml::ZClip::z_clip_neg_one );
}

const cml::matrix44f_c& Camera::GetProjection()
{
	//ComputeProjection();
	return projection;
}

const cml::matrix44f_c& Camera::GetView()
{
	ComputeView();
	return view;
}

void Camera::ComputeView()
{
	cml::matrix44f_c orientation, translation;
	translation = cml::identity<4>();
	orientation = cml::identity<4>();
	cml::matrix_rotate_about_local_axis( orientation, 0, cml::rad(verticalAngle) );
	cml::matrix_rotate_about_local_axis( orientation, 1, cml::rad(horizontalAngle) );
	cml::matrix_set_translation( translation, position );
	view = orientation * translation;
}

cml::matrix44f_c Camera::Matrix()
{
	ComputeView();
	ComputeProjection();
	return projection * view;
}

cml::vector3f Camera::Forward() const
{
	return cml::vector3f( view(2, 0), -view(1, 2), -view( 2, 2 ) );
}

cml::vector3f Camera::Right() const
{
	return cml::vector3f( view(0, 0), -view(0, 1), -view(0, 2) );
}

cml::vector3f Camera::Up() const
{
	return cml::vector3f(0,0,0);
}

