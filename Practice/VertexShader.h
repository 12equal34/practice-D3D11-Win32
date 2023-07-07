#pragma once
#include <string>
#include "IBindable.h"
#include "DXResource.h"

namespace Hardware::DX
{
	class VertexShader : public IBindable
	{
	public:
		VertexShader(std::wstring_view wsv);
		void Bind() noexcept override;

		ID3D10Blob* GetBlob() noexcept;
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
		Microsoft::WRL::ComPtr<ID3D10Blob> m_pBlob;
	};
}
