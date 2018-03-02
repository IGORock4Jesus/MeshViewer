#pragma once

using namespace Platform;

namespace MeshViewer
{
	interface class ILoader
	{
		bool Load(String^ filename);
	};
}