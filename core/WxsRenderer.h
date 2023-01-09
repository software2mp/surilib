/*! \file WxsRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXSRENDERER_H_
#define WXSRENDERER_H_

// Includes standard

// Includes suri
#include "RasterRenderer.h"
#include "suri/Subset.h"

// forwards
class wxXmlNode;

/** namespace suri */
namespace suri {

/**
 * Renderer que lee una imagen de tipo wxs.
 * Hereda de RasterRenderer, cada vez que se renderiza actualiza pImage_
 * y modifica RasterRenderer::Parameters::rasterModel_.
 * Para actualizar la imagen usa los datos en wxsParameters_
 */
class WxsRenderer : public RasterRenderer {
   /** Ctor. de Copia. */
   WxsRenderer(const WxsRenderer &RasterRenderer);

public:
   /** ctor */
   WxsRenderer();
   /** dtor */
   virtual ~WxsRenderer();
   // ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** Parametros de WxsRenderer. Hereda de RasterRenderer */
   class Parameters : public RasterRenderer::Parameters {
   public:
      /** Inicializa instancia con Parameters de RasterRenderer */
      void Init(const RasterRenderer::Parameters& RasterParameters);
      /** Operador de comparacino entre dos Parameters */
      bool operator==(const WxsRenderer::Parameters& WxsParameters) const;
      /** Operador de copia entre dos WxsRenderer::Parametros */
      WxsRenderer::Parameters& operator=(const WxsRenderer::Parameters& WxsParameters);

      std::string style_; /*! estilo de visualizacion de la capa */
      std::string format_; /*! formato en que se trae la imagen */
      std::string serviceSRId_; /*! Sistema de referencia en
       formato reconocido por servidor */
      Subset extent_; /*! extent completo de la imagen wms */
   };

   // ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** Genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(const WxsRenderer::Parameters &WxsParameters);
   /** Genera los parametros a partir de un nodo */
   static WxsRenderer::Parameters GetParameters(const wxXmlNode *pNode, Option Metadata);

   // -------------------------- METODO DE CREACION ----------------------------
   /** creador + inicializador */
   virtual WxsRenderer* Create(Element *pElement, Renderer *pPreviousRenderer) const;
   /** Nombre del renderizador == al nombre del nodo */
   virtual std::string CreatedNode() const;

protected:
   /** Actualiza parametros de clase padre */
   void UpdateRasterRendererParameters(WxsRenderer::Parameters Parameters);

   // ----------------------- METODOS DE RENDERIZACION -------------------------
   /** Actualizacion de parametros para representar un elemento */
   bool UpdateImage(const World* pWorldWindow);

   // ----------------------- METODOS DE ACTUALIZACION -------------------------
   /** Actualizacion de parametros para representar un elemento */
   virtual void Update(Element *pElement);

   /** Genera xml que requiere GDAL para abrir una imagen wxs. */
   std::string ConfigureXml(const World* pWorldWindow);

   Parameters wxsParameters_;
};

}

#endif /* WXSRENDERER_H_ */
