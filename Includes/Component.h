#ifndef ICOMPONENT_H_
#define ICOMPONENT_H_

class GameObject;

class Component
{
private:
	GameObject*				m_pOwner;

protected:
	unsigned short			m_id;

public:
	explicit Component(GameObject* pOwner) : m_pOwner(pOwner) {}
	virtual ~Component() {}

	virtual void			Initialize() = 0;

	GameObject*				GetOwner() { return m_pOwner; }

	unsigned short			GetID () { return m_id; }
};

#endif