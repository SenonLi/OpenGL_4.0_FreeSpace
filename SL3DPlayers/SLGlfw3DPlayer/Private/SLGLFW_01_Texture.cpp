#include "../stdafx.h"
#include "SLGLFW_01_Texture.h"

#include "SLDigitalImageProcess.h"               // sldip:  For Basic LibreImage Process

//#include "SLImageParam.h"

SLGLFW_01_Texture::SLGLFW_01_Texture()
{
	m_WindowName = "Sen UnitTest_Visual Test Texture ";

	// Load Image to CPU memory
	LoadTexture();
	m_WidgetWidth = m_LibreImage.GetWidth();
	m_WidgetHeight = m_LibreImage.GetHeight();
}

void SLGLFW_01_Texture::InitPaintContext()
{
	// Always initial Renderers before Paintables!!
	// Initial texture2D renderer
	m_Texture2DRenderer = std::make_unique<slgeom::SLTexture2D_Renderer>();

	// One statement below is all we need to upload LibreImage and prepare the textureID that is embeded inside m_LibreImagePaintable
	m_Texture2DRenderer->UploadLinearImageToGPU(m_LibreImagePaintable);

	SLOutputDebugString(_T(" SLGLFW_01_Texture::InitPaintContext() \n\n"));
}

void SLGLFW_01_Texture::PaintGL()
{
	// One statement below is all we need to Paint LibreImage 
	m_Texture2DRenderer->PaintTexture2D(m_LibreImagePaintable.GetTextureID());
}


void SLGLFW_01_Texture::FinalizeGL()
{
	// Reset Renderers: will handle all textureID destroy automatically
	m_Texture2DRenderer.reset();
}

void SLGLFW_01_Texture::PostPaintProcess()
{
	// Process Images
	sldip::SLLibreImage gayscaledImage;
	sldip::GetGrascaledImage(m_LibreImage, gayscaledImage);
	sldip::SaveToImageFile(gayscaledImage, m_SaveFolderPath, L"GrayScaled.png", sldip::SLImageFileType::IMAGE_PNG);

	sldip::HistorgramEqualization(gayscaledImage);

	sldip::SaveToImageFile(gayscaledImage, m_SaveFolderPath, L"HistogramEqualization.png", sldip::SLImageFileType::IMAGE_PNG);
}


void SLGLFW_01_Texture::LoadTexture()
{
	//sldip::LoadImageParam(m_LibreImage, m_ImagePath);
	bool isLoadNon8bit = true;
	sldip::LoadImageParam(m_LibreImage, m_ImagePath, isLoadNon8bit);

	sldip::SaveToImageFile(m_LibreImage, m_SaveFolderPath, _T("Origin.png"), sldip::SLImageFileType::IMAGE_PNG);

	// Use Loaded LibreImage to Initial SLLibreImagePaintable
	m_LibreImagePaintable = slgeom::SLLibreImagePaintable(m_LibreImage);
}
