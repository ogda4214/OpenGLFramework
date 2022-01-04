#pragma once

#include "BaseObject.h"

namespace Prototype
{
	class ClearColorObject : public BaseObject
	{
	public:
		ClearColorObject();
		virtual ~ClearColorObject();

		virtual void onUpdate(float deltatime) override;
		virtual void onRender() override;
		virtual void onImGuiRender() override;
	private:
		float m_Color[4];
	};
}