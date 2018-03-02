#include "pch.h"
#include "FilmboxLoader.h"

using namespace Windows::UI::Popups;


namespace MeshViewer {
	namespace Loaders {
		
		bool FilmboxLoader::Load(String ^ filename)
		{
			auto fn = filename->Data();

			FbxManager* manager{ FbxManager::Create() };
			if (!manager) {
				MessageDialog^ box = ref new MessageDialog("The FBX Manager cannot be created.");
				box->ShowAsync();
				return false;
			}




			return false;
		}

	}
}