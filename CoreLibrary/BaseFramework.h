#pragma once

#include <string>

class BaseFramework
{
public:
	BaseFramework();
	~BaseFramework();
public:
	virtual void Init();
	virtual void RunFrame(float DeltaSeconds);
	virtual void Reset();
	virtual bool ShouldReset() const;
public:
	FORCEINLINE std::wstring GetTitle() const { return FrameworkTitle; }
	FORCEINLINE void SetTitle(const std::wstring& NewTitle) { FrameworkTitle = NewTitle; }

	FORCEINLINE int  GetWidth() const { return Width; }
	FORCEINLINE void SetWidth(int NewWidth) { Width = NewWidth; }

	FORCEINLINE int  GetHeight() const { return Height; }
	FORCEINLINE void SetHeight(int NewHeight) { Height = NewHeight; }
protected:
	std::wstring FrameworkTitle;
	int Width;
	int Height;
};

