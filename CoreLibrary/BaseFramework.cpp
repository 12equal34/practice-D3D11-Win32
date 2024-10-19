#include "pch.h"
#include "BaseFramework.h"

BaseFramework::BaseFramework()
	: Width(640)
	, Height(480)
{
}

BaseFramework::~BaseFramework()
{
}

void BaseFramework::Init()
{
}

void BaseFramework::Reset()
{
}

void BaseFramework::RunFrame(float DeltaSeconds)
{
}

bool BaseFramework::ShouldReset() const
{
	return false;
}