#pragma once

namespace Prototype
{
	class BaseObject
	{
	public:
		BaseObject() {};
		virtual ~BaseObject() {};

		virtual void onUpdate(float deltatime) {};
		virtual void onRender() {};
		virtual void onImGuiRender() {};
	};
}