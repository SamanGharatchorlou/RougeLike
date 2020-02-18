#include "pch.h"
#include "Renderer.h"

Renderer* Renderer::Get()
{
	static Renderer sInstance;
	return &sInstance;
}