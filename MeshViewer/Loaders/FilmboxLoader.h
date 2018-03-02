#pragma once

#include "ILoader.h"


namespace MeshViewer {
	namespace Loaders {

		ref class FilmboxLoader : public ILoader
		{
		public:
			// ������������ ����� ILoader
			virtual bool Load(String ^ filename) override;
		};

	}
}