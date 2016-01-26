#pragma once
#include "State.h"
template <class entity_type>

class FSMController
{
private:
	entity_type* _owner;
	State<entity_type>* _currentState;
	State<entity_type>* _previousState;
	State<entity_type>* _globalState;

public:
	FSMController(entity_type* owner) :_owner(owner),
		_currentState(NULL),
		_previousState(NULL),
		_globalState(NULL)
	{}

	void SetCurrentState(State<entity_type>* state) { _currentState = state; }
	void SetGlobalState(State<entity_type>* state) { _globalState = state; }
	void SetPreviousState(State<entity_type>* state) { _previousState = state; }

	void Update() const
	{
		if (_globalState)
			_globalState->Update(_owner);
		if (_currentState)
			_currentState->Update(_owner);
	}

	void ChangeState(State<entity_type>* newState)
	{
		_previousState = _currentState;
		_currentState = newState;
		_currentState->Enter(_owner);
	}

	void ReturnToPreviousState()
	{
		ChangeState(_previousState);
	}

	State<entity_type>* GetCurrentState() { return _currentState; }
	State<entity_type>* GetPreviousState() { return _previousState; }
	State<entity_type>* GetGlobalState() { return _globalState; }

	bool IsInState(const State<entity_type>& state)const
	{
		if (typeid(*_currentState) == typeid(state))
			return true;
		return false;
	}
};