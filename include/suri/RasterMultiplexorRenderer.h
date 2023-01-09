/*! \file RasterMultiplexorRenderer.h */
/**
 * Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef __RASTER_MULTIPLEXOR_RENDERER_H__
#define __RASTER_MULTIPLEXOR_RENDERER_H__

// Includes Suri
#include "Renderer.h"
#include "RasterRenderer.h"
#include "MemoryCanvas.h"
// forwards
class wxXmlNode;

namespace suri {
namespace core {
namespace render {
/** 
 *	Clase que representa un renderizador que funciona como un multiplexor de 1 banda a 3 bandas. 
 * El objetivo de este renderizaor es el de hacer de el solucionar el problema que ocurre
 * con el sistema de renderizacion cuando se configura la capa raster con una sola banda
 **/
class RasterMultiplexorRenderer : public RasterRenderer {

public:
	/** ctor */
	RasterMultiplexorRenderer();
	/** dtor **/
	~RasterMultiplexorRenderer();
	/** genera un nodo XML a partir de los parametros */	
	static wxXmlNode* GetXmlNode(const RasterRenderer::Parameters& RenderParameters);
	/** genera los parametros a partir de un nodo */
	static RasterRenderer::Parameters GetParameters(const wxXmlNode* pNode);

	virtual RasterMultiplexorRenderer* Create(Element* pElement, Renderer* pPreviousRenderer) const;
   /** Nombre del renderizador == al nombre del nodo */
   /**
    *  Retorna el nombre del nodo que maneja el renderizador.
    * @return Nombre del nodo manejado por la clase derivada.
    */
   virtual std::string CreatedNode() const;
// ----------------------- METODOS DE RENDERIZACION -------------------------                       
	/** Renderiza el elemento dado un World en un Canvas */
	virtual bool Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask);
	/** Obtiene el "bounding box" del elemento renderizado */
	virtual void GetBoundingBox(const World *pWorld, double &Ulx, double &Uly, double &Lrx, double &Lry);
	/** Obtiene los parametros de entrada del renderer */
	virtual void GetInputParameters(int &SizeX, int &SizeY, int &BandCount, std::string &DataType) const;
	/** Obtiene los parametros de salida del renderer */
	virtual void GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,	std::string &DataType) const;
	// ----------------------- METODOS DE ACTUALIZACION -------------------------
	/** Actualizacion de parametros para representar un elemento */
	virtual void Update(Element *pElement);
	void AddPreviousData(std::vector<void*>& PreviousData, std::vector<void*>& FinalData,
	                     Canvas* pCanvas);
};

}	/** namespace render **/
}	/** namespace core	**/
}	/** namespace suri **/

#endif
