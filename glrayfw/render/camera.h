#pragma once

#include <cml/cml.h>

class Camera {
public:
	Camera();

	void position( const cml::vector3f& pos );
	const cml::vector3f& position() const;
	void offsetPosition( const cml::vector3f& offsetPosition );

	void offsetAngle( float hor, float ver );
	float horizontalAngle() const;
	void horizontalAngle( float angle );
	float verticalAngle() const;

	cml::vector3f forward() const;
	cml::vector3f right() const;
	cml::vector3f up() const;

	const cml::matrix44f_c& projection();
	const cml::matrix44f_c& view();
	cml::matrix44f_c viewProjectionMatrix();
	void aspect( float a );

private:
	void computeProjection();
	void computeView();

	// Camera settings
	cml::matrix44f_c m_view, m_projection;
	float m_fieldOfView;
	float m_aspect;
	float m_nearClip;
	float m_farClip;

	// Transform
	cml::vector3f m_position;
	float m_horizontalAngle;
	float m_verticalAngle;
};

