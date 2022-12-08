#include "ModuleTexture.h"
#include "Application.h"
#include "ModuleEngine.h"
#include <fstream>

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
	App->engine->log->Debug(("Star loading texture " + std::string(result.fileName) + "\n").c_str());

	std::wstring wPath = std::wstring(result.fileName.begin(), result.fileName.end());

	std::string extension = result.fileName.substr(result.fileName.size() - 4, 4);

	DirectX::TexMetadata metaData;
	DirectX::ScratchImage* img = new DirectX::ScratchImage;
	imageTexture = new DirectX::ScratchImage;

	HRESULT hr;

	if (extension == ".dds") {

		hr = LoadFromDDSFile(wPath.c_str(), DirectX::DDS_FLAGS_NONE, &metaData, *img);
		if (FAILED(hr)) {

			App->engine->log->Error("Error DDS loading failed\n");
			return 0;
		}
	}
	else if (extension == ".tga") {

		hr = LoadFromTGAFile(wPath.c_str(), &metaData, *img);
		if (FAILED(hr)) {

			App->engine->log->Error("Error TGA loading failed\n");
			return 0;
		}
	}
	else {

		hr = LoadFromWICFile(wPath.c_str(), DirectX::WIC_FLAGS_NONE, &metaData, *img);
		if (FAILED(hr)) {

			App->engine->log->Error("Error WIC loading failed\n");
			return 0;
		}
	}

	FlipRotate(img->GetImages(), img->GetImageCount(), img->GetMetadata(), DirectX::TEX_FR_FLIP_VERTICAL, *imageTexture);

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
		App->engine->log->Error("Error Unsupported format\n");
		assert(false && "Unsupported format");
		return 0;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imageTexture->GetMetadata().width, imageTexture->GetMetadata().height, 0, format, type, imageTexture->GetPixels());
	if(result.options.MipMap) glGenerateMipmap(GL_TEXTURE_2D);

	result.texture = texture;
	result.metadata = imageTexture->GetMetadata();

	App->engine->log->Debug(("Texture " + std::string(result.fileName) + " loaded\n").c_str());

	return 1;
}

DirectX::TexMetadata ModuleTexture::GetMetaData()
{
	return imageTexture->GetMetadata();
}

bool ModuleTexture::CleanUp()
{
	delete imageTexture;
	return true;
}
