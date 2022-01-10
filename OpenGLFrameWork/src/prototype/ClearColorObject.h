#pragma once

#include "BaseObject.h"

namespace Prototype
{
	class ClearColorObject : public BaseObject
	{
	public:
		ClearColorObject();
		virtual ~ClearColorObject();

		void onUpdate(float deltatime) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		float m_Color[4];
	};
}