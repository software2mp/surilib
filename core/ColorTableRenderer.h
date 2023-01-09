/*! \file ColorTableRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COLORTABLERENDERER_H_
#define COLORTABLERENDERER_H_

// Includes standard
// Includes suri
#include "ColorTable.h"
#include "LutRenderer.h"

/** namespace suri */
namespace suri {
/** Clase para el manejo de tablas de color */
/**
 * A una banda le aplica 3 tablas y forma una imagen color del mismo tamanio
 * pero con tres bandas.
 * Implementa metodos de conversion de y a nodo XML para los parametros.
 * Metodos de conversion entre parametros de LutTable y de ColorTable
 */
class ColorTableRenderer : public LutRenderer {
   /** Ctor. de Copia. */
   ColorTableRenderer(const ColorTableRenderer &ColorTableRenderer);

public:
   /** Ctor */
   ColorTableRenderer();
   /** Dtor */
   virtual ~ColorTableRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** parametros de ColorTalbeRenderer */
   class Parameters {
   public:
      /** Ctor. */
      Parameters() {
      }
      ColorTable colorTable_; /*! Tabla con colores que uso para renderizar */
   };
// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(const Parameters &RenderParameters);
   /** genera los parametros a partir de un nodo */
   static Parameters GetParameters(const wxXmlNode *pNode);
// -------------------------- METODO DE CREACION ----------------------------
   /** Crea un renderizador a partir de un pElement */
   virtual Renderer *Create(Element *pElement, Renderer *pPreviousRenderer) const;
   /** Nombre del renderizador == al nombre del nodo */
   virtual std::string CreatedNode() const;
// ----------------------- METODOS DE ACTUALIZACION -------------------------
   /** Actualizacion de parametros para representar un elemento */
   virtual void Update(Element *pElement);

protected:
   /** Adapta Parameters de ColorTable a Lut */
   static bool ColorTableToLut(ColorTableRenderer::Parameters InParameters,
                               LutRenderer::Parameters &OutParameters);
   /** Adapta Parameters de Lut a ColorTable */
   static bool LutToColorTable(LutRenderer::Parameters &InParameters,
                               ColorTableRenderer::Parameters &OutParameters);
};
}

#endif /* COLORTABLERENDERER_H_ */
