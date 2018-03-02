#include "stdafx.h"
#include "FbxLoader.h"


using namespace std;


FbxNode * FbxLoader::FindNode(FbxNode * node, const string & name)
{
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		auto n = node->GetChild(i);
		if (n->GetName() == name)
			return n;

		if (n = FindNode(n, name))
			return n;
	}
	return nullptr;
}

FbxNode * FbxLoader::FindNode(const string & name)
{
	FbxNode *node = scene->GetRootNode();
	if (node) {
		return FindNode(node, name);
	}
	return nullptr;
}

FbxLoader::FbxLoader(ComPtr<IDirect3DDevice9> device, const string&  filename)
	: device(device)
{
	manager = FbxManager::Create();
	ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);

	importer = FbxImporter::Create(manager, "");

	bool status = importer->Initialize(filename.c_str(), -1, ios);
	if (!status) {
		throw exception(importer->GetStatus().GetErrorString());
	}

	scene = FbxScene::Create(manager, "myScene");
	importer->Import(scene);
}


FbxLoader::~FbxLoader()
{
	if (scene) {
		scene->Destroy();
	}
	if (importer) {
		importer->Destroy();
	}
	if (ios) {
		ios->Destroy();
	}
	if (manager) {
		manager->Destroy();
	}
}


shared_ptr<Mesh> FbxLoader::FindMesh(const string& name)
{
	auto node = FindNode(name);
	if (node == nullptr)
		return nullptr;
	auto att = node->GetNodeAttribute();
	if (att == nullptr || att->GetAttributeType() != FbxNodeAttribute::eMesh)
		return nullptr;

	FbxMesh* mesh = (FbxMesh*)att;

	int vertexCount = mesh->GetControlPointsCount();
	FbxVector4* vs = mesh->GetControlPoints();

	vector<D3DXVECTOR3> vertices;

	for (int i = 0; i < vertexCount; i++)
	{
		FbxVector4* vector = vs + i;
		vertices.push_back({ (float)vector->mData[0],(float)vector->mData[1],(float)vector->mData[2] });
	}

	int polygonCount = mesh->GetPolygonCount();
	vector<UINT> indices;

	int vertexIndex = 0;
	for (int i = 0; i < polygonCount; i++)
	{
		int polygonSize = mesh->GetPolygonSize(i);
		if (polygonSize != 3)
			throw exception("Polygons must be have size = 3!");

		for (int j = 0; j < 3; j++)
		{
			int index = mesh->GetPolygonVertex(i, j);
			indices.push_back(index);

			// normals
			int elementNormalCount = mesh->GetElementNormalCount();

			for (int n = 0; n < elementNormalCount; n++)
			{
				FbxGeometryElementNormal* elementNormal = mesh->GetElementNormal(n);

				if (elementNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (elementNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						elementNormal->GetDirectArray().GetAt(vertexIndex);
						break;
					case FbxGeometryElement::eIndexToDirect:

						break;
					default:
						break;
					}
				}
			}

			vertexIndex++;
		}
	}

	int materialCount = node->GetMaterialCount();
	D3DMATERIAL9 d3dmaterail{ 0 };

	for (int i = 0; i < materialCount; i++)
	{
		FbxSurfaceMaterial* material = node->GetMaterial(i);
		if (material->GetClassId().Is(FbxSurfacePhong::ClassId)) {
			auto phong = (FbxSurfacePhong*)material;
			auto diffuse = phong->Diffuse.Get();
			d3dmaterail.Diffuse = { (float)diffuse[0],(float)diffuse[1],(float)diffuse[2] };
		}

	}



	shared_ptr<Mesh> output = make_shared<Mesh>(device, vertices, indices, d3dmaterail);

	return output;
}
