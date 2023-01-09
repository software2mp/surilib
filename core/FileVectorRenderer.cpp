/*! \file FileVectorRenderer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "FileVectorRenderer.h"

#include <string>

#include "suri/Canvas.h"
#include "suri/Element.h"
#include "suri/World.h"

AUTO_REGISTER_RENDERER(suri::FileVectorRenderer);

namespace suri {

/**
 * Ctor.
 */
FileVectorRenderer::FileVectorRenderer() {

}

/**
 * Dtor.
 */
FileVectorRenderer::~FileVectorRenderer() {

}

/**
 * Crea un renderizador a partir de un pElement.
 *
 * @param[in] pElement Elemento a renderizar.
 * @param[in] pPreviousRenderer Renderizador anterior.
 */
Renderer* FileVectorRenderer::Create(Element* pElement, Renderer* pPreviousRenderer) const {
   FileVectorRenderer* pfvrenderer = new FileVectorRenderer;
   return pfvrenderer;
}

/**
 * Retorna el nombre del nodo que maneja el renderizador.
 * (Nombre del renderizador == al nombre del nodo)
 *
 * @return Nombre del nodo manejado por la clase derivada.
 */
std::string FileVectorRenderer::CreatedNode() const {
   return LAYERS_EXPO_NODE;
}

/** Renderiza el elemento dado un World en un Canvas */
bool FileVectorRenderer::Render(const World* pWorldWindow, Canvas* pCanvas, Mask* pMask) {
   if (pCanvas != NULL)
      pCanvas->Flush(pMask);
   return true;
}

/**
 * Obtiene el "bounding box" del elemento renderizado.
 *
 * \pre el subset debe ser seteado antes de llamar a esta funcion con un
 *      valor invalido.
 * \post Se refleja el subset de mundo que renderiza.
 * \post No se modifican los parametros en caso de falla o subset nulo
 * @param[in] pWorld Mundo sobre el cual se debe devolver el BB
 * @param[out] Ulx ULx de mundo renderizado.
 * @param[out] Uly ULy de mundo renderizado.
 * @param[out] Lrx LRx de mundo renderizado.
 * @param[out] Lry LRy de mundo renderizado.
 */
void FileVectorRenderer::GetBoundingBox(const World* pWorld, double&Ulx, double& Uly,
                                        double& Lrx, double& Lry) {
   // NO SE IMPLEMENTA
}

/**
 * Permite obtener los parametros de entrada que requiere el renderizador,
 * es decir, los datos que espera de pPreviousRenderer. En caso de que no
 * tenga algun requerimiento, el parametro tomara los siguientes valores:
 *
 * @code
 *  SizeX = -1
 *  SizeY = -1
 *  BandCount = -1
 *  DataType = DataInfo<void>::Name
 * @endcode
 *
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void FileVectorRenderer::GetInputParameters(int& SizeX, int& SizeY, int& BandCount,
                                            std::string& DataType) const {
   // NO SE IMPLEMENTA
}

/**
 * Permite obtener los parametros de salida del renderizador.
 *
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void FileVectorRenderer::GetOutputParameters(int& SizeX, int& SizeY, int& BandCount,
                                             std::string& DataType) const {
   // NO SE IMPLEMENTA
}

/**
 * Actualizacion de parametros para representar un elemento.
 *
 * @param[in] pElement Elemento a analizar por cambios.
 */
void FileVectorRenderer::Update(Element *pElement) {
   // NO SE IMPLEMENTA
}

}  // namespace suri
