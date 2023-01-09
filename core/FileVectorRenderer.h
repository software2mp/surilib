/*! \file FileVectorRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILEVECTORRENDERER_H_
#define FILEVECTORRENDERER_H_

#include <string>

#include "suri/Renderer.h"

namespace suri {

class Canvas;
class Element;
class Mask;
class Renderer;
class World;

class FileVectorRenderer : public Renderer {
public:
   /**
    * Ctor.
    */
   FileVectorRenderer();

   /**
    * Dtor.
    */
   virtual ~FileVectorRenderer();

   /**
    * Crea un renderizador a partir de un pElement.
    */
   virtual Renderer* Create(Element* pElement,
                            Renderer* pPreviousRenderer = NULL) const;

   /**
    * Retorna el nombre del nodo que maneja el renderizador.
    * (Nombre del renderizador == al nombre del nodo)
    */
   virtual std::string CreatedNode() const;

   /**
    * Renderiza el elemento dado un World en un Canvas.
    */
   virtual bool Render(const World* pWorldWindow, Canvas* pCanvas, Mask* pMask);

   /**
    * Obtiene el "bounding box" del elemento renderizado.
    */
   virtual void GetBoundingBox(const World* pWorld, double& Ulx, double& Uly,
                               double& Lrx, double& Lry);

   /**
    * Permite obtener los parametros de entrada que requiere el renderizador,
    * es decir, los datos que espera de pPreviousRenderer.
    */
   virtual void GetInputParameters(int& SizeX, int& SizeY, int& BandCount,
                                   std::string& DataType) const;

   /**
    * Permite obtener los parametros de salida del renderizador.
    */
   virtual void GetOutputParameters(int& SizeX, int& SizeY, int& BandCount,
                                    std::string& DataType) const;

   /**
    * Actualizacion de parametros para representar un elemento.
    */
   virtual void Update(Element* pElement);
};

} // namespace suri

#endif  // FILEVECTORRENDERER_H_
