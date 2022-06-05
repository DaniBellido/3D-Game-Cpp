#pragma once
#include "Model.h"

// extending the CModel class
class CDoor : public CModel
{
	private: // new member variables
	CVector openPosition, closePosition;
	
	public: // new member functions
	void SetPositions(CVector open, CVector close);
	void Open();
	void Close();
};
