#pragma once
class Entity
{
private:
//	int _ID;
//	static int _NextValidID;
//	void SetID(int val);
public:
	Entity(int id)
	{
//		SetID(id);
	}
	virtual ~Entity() {}

	virtual void Update(int deltaTime) = 0;
//	int ID() const { return _ID; }
};

