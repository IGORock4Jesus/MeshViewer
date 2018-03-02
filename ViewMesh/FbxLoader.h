#pragma once

#include "Mesh.h"


class FbxLoader
{
	Microsoft::WRL::ComPtr<IDirect3DDevice9> device;

	FbxManager* manager{ nullptr };
	FbxIOSettings* ios{ nullptr };
	FbxImporter* importer{ nullptr };
	FbxScene* scene{ nullptr };

	FbxNode* FindNode(FbxNode* node, const string& name);
	FbxNode* FindNode(const string& name);

public:
	FbxLoader(ComPtr<IDirect3DDevice9> device, const string&  filename);
	~FbxLoader();

	shared_ptr<Mesh> FindMesh(const string& name);
};

