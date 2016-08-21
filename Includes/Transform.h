#ifndef TRANSFORM_H_
#define TRANSFORM_H_

struct Vector3
{
	float x;
	float y;
	float z;
};

typedef float Matrix3 [3] [3];
typedef float Matrix4 [4] [4];

class Transform
{
	Matrix3				m_rotation;
	Vector3				m_translation;
	float				m_scale;
	Matrix4				m_matrix;

public:
	Transform ();
	virtual ~Transform ();

	void				SetRotation ( const Matrix3& rotation );
	void				SetTranslation ( const Vector3& translation );
	void				SetScale ( const float scale ) { m_scale = scale; }
	
	const Matrix3&		GetRotation () const { return m_rotation; }
	const Vector3&		GetTranslation () const { return m_translation; }
	const float			GetScale () const { return m_scale; }
	const Matrix4&		GetMatrix () const { return m_matrix; }

	void				UpdateMatrix ();
};

#endif