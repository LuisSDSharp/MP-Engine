#include "..\Includes\Transform.h"

Transform::Transform ()
{}

Transform::~Transform ()
{}

void Transform::SetRotation ( const Matrix3 & rotation )
{
	for ( unsigned short x = 0; x < 3; x++ )
	{
		for ( unsigned short y = 0; y < 3; y++ )
		{
			m_rotation [x] [y] = rotation [x] [y];
		}
	}
}

void Transform::SetTranslation ( const Vector3 & translation )
{
	m_translation.x = translation.x;
	m_translation.y = translation.y;
	m_translation.z = translation.z;
}

void Transform::UpdateMatrix ()
{
	m_matrix [0] [0] = m_rotation [0] [0] * m_scale;
	m_matrix [0] [1] = m_rotation [0] [1];
	m_matrix [0] [2] = m_rotation [0] [2];
	m_matrix [0] [3] = 0;

	m_matrix [1] [0] = m_rotation [1] [0];
	m_matrix [1] [1] = m_rotation [1] [1] * m_scale;
	m_matrix [1] [2] = m_rotation [1] [2];
	m_matrix [1] [3] = 0;

	m_matrix [2] [0] = m_rotation [2] [0];
	m_matrix [2] [1] = m_rotation [2] [1];
	m_matrix [2] [2] = m_rotation [2] [2] * m_scale;
	m_matrix [2] [3] = 0;

	m_matrix [3] [0] = m_translation.x;
	m_matrix [3] [1] = m_translation.y;
	m_matrix [3] [2] = m_translation.z;
	m_matrix [3] [3] = 1;
}
