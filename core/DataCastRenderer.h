/*! \file DataCastRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef DATACASTRENDERER_H_
#define DATACASTRENDERER_H_

// Includes suri
#include "suri/Renderer.h"

/** Forwards */
class wxXmlNode;

/** namespace suri */
namespace suri {
/** Renderer que castea la informacion del canvas */
/**
 * Este renderizador transformara los datos del canvas al tipo de dato indicado
 * por el nodo xml. Los tipos de dato posibles son los indicados en la clase
 * DataTypes.h.
 * \note hereda de renderer para formar parte del pipeline de renderizacion
 */
class DataCastRenderer : public Renderer {
   /** Ctor. de Copia. */
   DataCastRenderer(const DataCastRenderer &DataCastRenderer);

public:
   /** ctor */
   DataCastRenderer();
   /** dtor */
   virtual ~DataCastRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** parametros de cast */
   class Parameters {
   public:
      /** Tipo de dato para la funcion de transformacion de datos */
      typedef void (*CastFunctionType)(void*, void*, size_t);
      CastFunctionType function_; /*! Funcion de transformacion de datos */
      std::string destinationDataType_; /*! Tipo de dato de salida */
   };
// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(const Parameters &RenderParameters);
   /** genera los parametros a partir de un nodo */
   static Parameters GetParameters(const wxXmlNode *pNode);
// -------------------------- METODO DE CREACION ----------------------------
   /** creador + inicializador */
   virtual DataCastRenderer *Create(Element *pElement,
                                    Renderer *pPreviousRenderer) const;
   /** Nombre del renderizador == al nombre del nodo */
   virtual std::string CreatedNode() const;
// ----------------------- METODOS DE RENDERIZACION -------------------------
   /** Cambia el tipo de dato */
   virtual bool Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask);
   /** Obtiene el "bounding box" del elemento renderizado */
   virtual void GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                               double &Lrx, double &Lry);
   /** Obtiene los parametros de entrada del renderer */
   virtual void GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                   std::string &DataType) const;
   /** Obtiene los parametros de salida del renderer */
   virtual void GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                    std::string &DataType) const;
// ----------------------- METODOS DE ACTUALIZACION -------------------------
   /** Actualizacion de parametros para representar un elemento */
   virtual void Update(Element *pElement);
protected:
private:
   Parameters parametersToChar_;
   Parameters parameters_; /*! Parametros de la instancia */
};
}

#endif /*DATACASTRENDERER_H_*/
