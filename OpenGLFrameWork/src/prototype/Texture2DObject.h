#pragma once

#include "BaseObject.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>

namespace Prototype
{
	class Texture2DObject : public BaseObject
	{
	public:
		Texture2DObject();
		virtual ~Texture2DObject();

		void onUpdate(float deltatime) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_View;
		glm::mat4 m_Proj;
		glm::vec4 m_ColorA;
		glm::vec4 m_ColorB;
		glm::vec3 m_TranslationA;
		glm::vec3 m_TranslationB;
	};
}