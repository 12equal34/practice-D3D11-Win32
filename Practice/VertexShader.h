#pragma once
#include "Bindable.h"

namespace Hardware::DX
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(Renderer& renderer, std::wstring_view wsv);
		void Bind(Renderer& renderer) noexcept override;

		ID3D10Blob* GetBlob() noexcept;
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
		Microsoft::WRL::ComPtr<ID3D10Blob> m_pBlob;
	};
}
