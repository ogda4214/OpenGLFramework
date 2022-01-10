#include "BaseObject.h"
#include "imgui/imgui.h"

namespace Prototype
{
	MenuObject::MenuObject(BaseObject*& currentObjectPointer)
		: m_CurrentObject(currentObjectPointer)
	{

	}

	void MenuObject::onImGuiRender()
	{
		for (auto& obj : m_Objects)
		{
			if (ImGui::Button(obj.first.c_str()))
			{
				m_CurrentObject = obj.second();
			}
		}
	}
}