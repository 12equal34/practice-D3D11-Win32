#pragma once
#include <DirectXMath.h>

struct ConstantBufferNeverChanges {
    alignas(16) DirectX::XMFLOAT3 dirToDirectionalLight;
    DirectX::XMFLOAT4 directionalLightColor;
};

struct ConstantBufferChangeOnResize {
    DirectX::XMFLOAT4X4 projection;
};

struct ConstantBufferChangesEveryFrame {
    DirectX::XMFLOAT4X4 view;
};

struct ConstantBufferGerstnerWaveParameters {
    float waveNumberX;
    float waveNumberZ;
    float amplitude;
    float phase;
};

struct ConstantBufferChangesEveryPrim {
    DirectX::XMFLOAT4X4 worldMatrix;
    DirectX::XMFLOAT4   meshColor;
    DirectX::XMFLOAT4   diffuseColor;
    DirectX::XMFLOAT4   specularColor;
    // float               specularPower;
};

struct ConstantBufferHemisphericAmbientLight {
    DirectX::XMFLOAT4 ambientLightBaseColor;
    DirectX::XMFLOAT4 ambientLightColorRange;
};