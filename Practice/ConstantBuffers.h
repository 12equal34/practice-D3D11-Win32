#pragma once
#include <DirectXMath.h>

struct ConstantBufferNeverChanges {
    alignas(16) DirectX::XMFLOAT3 dirToDirectionalLight;
    alignas(16) DirectX::XMFLOAT3 directionalLightColor;
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
    alignas(16) DirectX::XMFLOAT3 meshColor;
    alignas(16) DirectX::XMFLOAT3 diffuseColor;
    alignas(16) DirectX::XMFLOAT3 specularColor;
    float specularExponent;
    float specularIntensity;
};

struct ConstantBufferHemisphericAmbientLight {
    alignas(16) DirectX::XMFLOAT3 ambientLightBaseColor;
    alignas(16) DirectX::XMFLOAT3 ambientLightColorRange;
};
