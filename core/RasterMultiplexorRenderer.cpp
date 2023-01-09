/*! \file ZoomRenderer.cpp */
/**
 * Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/RasterMultiplexorRenderer.h"
#include "suri/xmlnames.h"
#include "suri/messages.h"
#include "suri/Canvas.h"
#include "suri/Element.h"
#include "MemoryCanvas.h"
#include "suri/DataTypes.h"

namespace suri {

template<typename T>
void SetPixelValue(std::vector<void*> PreviousData, std::vector<void*> FinalData, int size,
                   double NoDataValue) {
   T* pdata = static_cast<T*>(*FinalData.begin());
   for (int x = 0; x < size; x++) {
      if (pdata[x] == NoDataValue) {
         std::vector<void*>::iterator itfinal = FinalData.begin();
         std::vector<void*>::iterator itprev = PreviousData.begin();
         for (; itfinal != FinalData.end(); ++itprev, ++itfinal) {
            T* dfinal = static_cast<T*>((*itfinal));
            T* dprev = static_cast<T*>((*itprev));
            dfinal[x] = dprev[x];
         }
      }
   }
}

typedef void (*CastFunctionType2)(std::vector<void*>, std::vector<void*>, int,
      double);

INITIALIZE_DATATYPE_MAP(CastFunctionType2, SetPixelValue);

AUTO_REGISTER_RENDERER(suri::core::render::RasterMultiplexorRenderer);


namespace core {
namespace render {

/** ctor */
RasterMultiplexorRenderer::RasterMultiplexorRenderer() {
}

/** dtor **/
RasterMultiplexorRenderer::~RasterMultiplexorRenderer() {
}


/** genera un nodo XML a partir de los parametros */
wxXmlNode* RasterMultiplexorRenderer::GetXmlNode(const Parameters& RenderParameters) {
	return NULL;
}
/** genera los parametros a partir de un nodo */
RasterRenderer::Parameters RasterMultiplexorRenderer::GetParameters(const wxXmlNode* pNode) {
	return RasterRenderer::GetParameters(pNode);
}

RasterMultiplexorRenderer* RasterMultiplexorRenderer::Create(Element* pElement, 
																				Renderer* pPreviousRenderer) const {
	wxXmlNode* pnode = pElement->GetNode("");
	if (!pnode)
		return NULL;

	RasterMultiplexorRenderer* prenderer = new RasterMultiplexorRenderer;
	prenderer->pPreviousRenderer_ = pPreviousRenderer;

	prenderer->parameters_ = GetParameters(pnode);
	return prenderer;
}

/** Nombre del renderizador == al nombre del nodo */
/**
 *  Retorna el nombre del nodo que maneja el renderizador.
 * @return Nombre del nodo manejado por la clase derivada.
 */
std::string RasterMultiplexorRenderer::CreatedNode() const {
	return BAND_MULTIPLEXOR_NODE;
}


// ----------------------- METODOS DE RENDERIZACION -------------------------                       
/** Renderiza el elemento dado un World en un Canvas */
bool RasterMultiplexorRenderer::Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask) {
   if (pPreviousRenderer_ == NULL)
		return false;
	if (parameters_.bandCombination_.size() == 3) {
		return pPreviousRenderer_->Render(pWorldWindow, pCanvas, pMask);
	} else if (parameters_.bandCombination_.size() == 1) {
      std::vector<void*> previousData(pCanvas->GetBandCount());
      std::vector<int> previousBands(pCanvas->GetBandCount());
      for (int b = 0; b < pCanvas->GetBandCount(); ++b) {
         previousBands[b] = b;
         previousData[b] = new unsigned char[pCanvas->GetSizeX() * pCanvas->GetSizeY()
               * pCanvas->GetDataSize()];
      }
		MemoryCanvas multiplexCanvas;
		int x, y;
		pCanvas->GetSize(x, y);
		multiplexCanvas.SetSize(x, y);
		multiplexCanvas.SetDataType(pCanvas->GetDataType());
		multiplexCanvas.SetBandCount(3);
		pCanvas->Read(previousBands, previousData);
		pCanvas->Clear();
		pPreviousRenderer_->Render(pWorldWindow, pCanvas, pMask);
		std::vector<int> bands(pCanvas->GetBandCount());
		for (int b = 0; b < pCanvas->GetBandCount(); ++b)
			bands[b] = b;

		std::vector<void*> data(bands.size());
		for (size_t b = 0; b < bands.size(); ++b)
		   data[b] = new unsigned char[x * y * pCanvas->GetDataSize()];
		pCanvas->Read(bands, data);
		/** completo con una copia de la banda **/
		for (size_t b = 0; b < 3; ++b) {
			std::vector<int> band;
			band.push_back(b);
			multiplexCanvas.Write(band, data);
		}
		for (size_t b = 0; b < data.size(); ++b)
		   delete[] (unsigned char*)(data[b]);

		pCanvas->Clear();
		pCanvas->InitializeAs(&multiplexCanvas);
		std::vector<int> muxbands(3);
		for (int b = 0; b < 3; ++b)
			muxbands[b] = b;
		std::vector<void*> muxdata(3);
      for (size_t b = 0; b < muxbands.size(); ++b)
         muxdata[b] = new unsigned char[x * y * pCanvas->GetDataSize()];
		multiplexCanvas.Read(muxbands, muxdata);
		AddPreviousData(previousData, muxdata, pCanvas);
		pCanvas->Write(muxbands, muxdata);
		for (size_t b = 0; b < muxdata.size(); ++b)
		   delete[] static_cast<unsigned char*>(muxdata[b]);
		for (unsigned int p = 0; p < previousBands.size(); p++)
		   delete[] static_cast<unsigned char*>(previousData[p]);
	return true;
	}
	return false;
}

void RasterMultiplexorRenderer::AddPreviousData(std::vector<void*>& PreviousData,
                                                std::vector<void*>& FinalData,
                                                Canvas* pCanvas) {
   if (!parameters_.generateMask_) return;
   double noDataValue = parameters_.noDataValue_;
   int size = pCanvas->GetSizeX() * pCanvas->GetSizeY();
   SetPixelValueTypeMap[pCanvas->GetDataType()](PreviousData, FinalData, size, noDataValue);
}


/** Obtiene el "bounding box" del elemento renderizado */
void RasterMultiplexorRenderer::GetBoundingBox(const World *pWorld, 
															double &Ulx, double &Uly, double &Lrx, double &Lry) {
	if (pPreviousRenderer_)
		pPreviousRenderer_->GetBoundingBox(pWorld, Ulx, Uly, Lrx, Lry);
}

/** Obtiene los parametros de entrada del renderer */
void RasterMultiplexorRenderer::GetInputParameters(int &SizeX, int &SizeY, 
																int &BandCount, std::string &DataType) const {
	if (pPreviousRenderer_)
		pPreviousRenderer_->GetInputParameters(SizeX, SizeY, BandCount, DataType);
}

/** Obtiene los parametros de salida del renderer */
void RasterMultiplexorRenderer::GetOutputParameters(int &SizeX, int &SizeY, 
																	int &BandCount,std::string &DataType) const {	
	if (pPreviousRenderer_)
		pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------
/** Actualizacion de parametros para representar un elemento */
void RasterMultiplexorRenderer::Update(Element *pElement) {
	parameters_ = GetParameters(pElement->GetNode(""));
}

}/** namespace render **/
}/** namespace core**/
}/** namespace suri **/
