#pragma once

#include "ILoader.h"


namespace MeshViewer {
	namespace Loaders {

		ref class FilmboxLoader : public ILoader
		{
		public:
			// Унаследовано через ILoader
			virtual bool Load(String ^ filename) override;
		};

	}
}