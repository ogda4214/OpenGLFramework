#include "ClearColorObject.h"

#include "Renderer.h"
#include "imgui/imgui.h"

Prototype::ClearColorObject::ClearColorObject() :
	m_Color{ 0.0f,0.0f,0.0f,1.0f }
{

}

Prototype::ClearColorObject::~ClearColorObject()
{
}

void Prototype::ClearColorObject::onUpdate(float deltatime)
{
}

void Prototype::ClearColorObject::onRender()
{
	GLCall(glClearColor(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));
	GLCall(glClear(GL_COLOR_BUFFER_BIT))
}

void Prototype::ClearColorObject::onImGuiRender()
{
	ImGui::ColorEdit4("ClearColor", (float*)&m_Color);
}
