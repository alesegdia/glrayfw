
#include "camera.h"

Camera::Camera() :
    m_fieldOfView( cml::rad(90.f) ),
    m_aspect( 16.f / 9.f ),
	m_nearClip( 0.01f ),
	m_farClip( 100.f ),
    m_position( 0.f, 0.f, -1.f ),
	m_horizontalAngle( 0.f ),
	m_verticalAngle( 0.f )
{
	computeProjection();
}

void Camera::aspect( float a )
{
	m_aspect = a;
}

void Camera::position( const cml::vector3f& pos )
{
	m_position = pos;
}

const cml::vector3f& Camera::position() const
{
	return m_position;
}

void Camera::offsetAngle( float hor, float ver )
{
	m_horizontalAngle += hor;
	m_verticalAngle += ver;
}

float Camera::horizontalAngle() const
{
	return m_horizontalAngle;
}

float Camera::verticalAngle() const
{
	return m_verticalAngle;
}

void Camera::horizontalAngle(float angle)
{
	m_horizontalAngle = angle;
}

void Camera::computeProjection()
{
	m_projection = cml::identity<4>();
    float k = 500000;
    //cml::matrix_perspective_xfov_RH( m_projection, m_fieldOfView, m_aspect, m_nearClip, m_farClip, cml::ZClip::z_clip_neg_one );
    cml::matrix_perspective_RH(m_projection, 1280.f / k, 720.f / k, 0.001f, 10000.f, cml::ZClip::z_clip_zero);
}

const cml::matrix44f_c& Camera::projection()
{
	return m_projection;
}

const cml::matrix44f_c& Camera::view()
{
	computeView();
	return m_view;
}

void Camera::computeView()
{
    cml::matrix44f_c orientation, translation, scale;
	translation = cml::identity<4>();
    orientation = cml::identity<4>();
    scale = cml::identity<4>();
    cml::matrix_rotate_about_local_axis( orientation, 0, cml::rad(m_verticalAngle) );
	cml::matrix_rotate_about_local_axis( orientation, 1, cml::rad(m_horizontalAngle) );
	cml::matrix_set_translation( translation, m_position );
	m_view = orientation * translation;
}

cml::matrix44f_c Camera::viewProjectionMatrix()
{
	computeView();
	computeProjection();
	return m_projection * m_view;
}

cml::vector3f Camera::forward() const
{
	return cml::vector3f( m_view(2, 0), -m_view(1, 2), -m_view( 2, 2 ) );
}

cml::vector3f Camera::right() const
{
	return cml::vector3f( m_view(0, 0), -m_view(0, 1), -m_view(0, 2) );
}

cml::vector3f Camera::up() const
{
	return cml::vector3f(0,0,0);
}

