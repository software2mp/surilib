/*! \file WxsVisualizationPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXSVISUALIZATIONPART_H_
#define WXSVISUALIZATIONPART_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"
#include "WxsRenderer.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

class WxsElement;

/** Permite modificar la visualizacion de una fuente wxs. */
/**
 * Se puede modificar el estilo de visualizacion de una fuente wms
 */
class WxsVisualizationPart : public Part {
public:
   /** Ctor */
   WxsVisualizationPart(bool Enable = true, bool Modified = false);
   /** Ctor */
   WxsVisualizationPart(WxsElement* pWxsElement, bool Enable = true, bool Modified =
         false);
   /** Dtor */
   virtual ~WxsVisualizationPart();

   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valor's originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();

   /** Configura los parametros ingresados por usuario */
   void ConfigureParameters(WxsRenderer::Parameters &Parameters);

protected:
   /** Configura los widgetParameters con datos en control */
   bool ReadParametersFromWidget();
   /** Actualiza el control con datos en los widgetParameters */
   bool UpdateParametersToWidget();

   WxsRenderer::Parameters parameters_;  ///< Parametros donde se guardan los
                                         ///< datos en commit
   WxsElement* pWxsElement_;  ///< Si no es NULL se le actualiza el nodo wxs en
   ///< cada commit.
   WxsRenderer::Parameters widgetParameters_;  ///< Parametros con valores
   ///< ingresados en pantalla
};
}

#endif /* WXSVISUALIZATIONPART_H_ */
