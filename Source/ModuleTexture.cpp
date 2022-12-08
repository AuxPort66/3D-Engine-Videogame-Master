#include "ModuleTexture.h"
#include "Application.h"
#include <fstream>
using namespace DirectX;

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr)) {
		printf("ERROR Init TEXTURE");
	}


	return true;
}

update_status ModuleTexture::Update()
{
	return UPDATE_CONTINUE;
}


bool ModuleTexture::Load(TextureData &result)
{
	std::wstring wPath = std::wstring(result.fileName.begin(), result.fileName.end());

	std::string extension = result.fileName.substr(result.fileName.size() - 4, 4);

	TexMetadata metaData;
	ScratchImage* img = new DirectX::ScratchImage;
	imageTexture = new DirectX::ScratchImage;

	HRESULT hr;

	if (extension == ".dds") {

		hr = LoadFromDDSFile(wPath.c_str(), DirectX::DDS_FLAGS_NONE, &metaData, *img);
		if (FAILED(hr)) {

			return 0;
		}
	}
	else if (extension == ".tga") {

		hr = LoadFromTGAFile(wPath.c_str(), &metaData, *img);
		if (FAILED(hr)) {

			return 0;
		}
	}
	else {

		hr = LoadFromWICFile(wPath.c_str(), WIC_FLAGS_NONE, &metaData, *img);
		if (FAILED(hr)) {

			return 0;
		}
	}

	FlipRotate(img->GetImages(), img->GetImageCount(), img->GetMetadata(), TEX_FR_FLIP_VERTICAL, *imageTexture);

	GLuint texture = result.texture;
	if(result.texture == NULL) glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, result.options.wrapSFunction);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, result.options.wrapTFunction);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, result.options.minFunction);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, result.options.magFunction);

	GLenum internalFormat;
	GLenum format;
	GLenum type;

	switch (imageTexture->GetMetadata().format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
		result.format = "GL_RGBA";
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_BGRA;
		result.format = "GL_BGRA";
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		internalFormat = GL_RGB8;
		format = GL_BGR;
		result.format = "GL_BGR";
		type = GL_UNSIGNED_BYTE;
		break;
	default:
		assert(false && "Unsupported format");
		return 0;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imageTexture->GetMetadata().width, imageTexture->GetMetadata().height, 0, format, type, imageTexture->GetPixels());
	if(result.options.MipMap) glGenerateMipmap(GL_TEXTURE_2D);

	result.texture = texture;
	result.metadata = imageTexture->GetMetadata();

	return 1;
}

DirectX::TexMetadata ModuleTexture::GetMetaData()
{
	return imageTexture->GetMetadata();
}

bool ModuleTexture::CleanUp()
{
	return true;
}
