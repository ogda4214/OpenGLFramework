#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>

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

	class MenuObject : public BaseObject
	{
	public:
		MenuObject(BaseObject*& currentObjectPointer);

		void onImGuiRender() override;

		template<typename T>
		void RegisterObject(const std::string& name)
		{
			std::cout << "RegisterObject" << name << std::endl;

			m_Objects.push_back(std::make_pair(name, []() { return new T(); }));
		}
	private:
		BaseObject*& m_CurrentObject;
		std::vector<std::pair<std::string, std::function<BaseObject*()>>> m_Objects;
	};
}