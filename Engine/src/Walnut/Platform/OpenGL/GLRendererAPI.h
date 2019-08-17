#pragma once
#include "Walnut/Graphics/RendererAPI.h"

namespace Walnut
{
	class GLRendererAPI : public RendererAPI
	{
	public:
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;
		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		void EnableDepthTesting() override;

	};
}
