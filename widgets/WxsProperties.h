/*! \file WxsProperties.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXSPROPERTIES_H_
#define WXSPROPERTIES_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"
#include "WxsRenderer.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

class WxsElement;

/** Permite modificar las propiedades de una fuente wxs. */
/**
 * Las propiedades incluyen formato de archivo, sistema de referencia y
 * bounding box.
 */
class WxsProperties : public Part {
public:
   /** Ctor */
   WxsProperties(bool Enable = true, bool Modified = false);
   /** Ctor */
   WxsProperties(WxsElement* pWxsElement, bool Enable = true, bool Modified = false);
   /** Dtor */
   virtual ~WxsProperties();

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

#endif /* WXSPROPERTIES_H_ */
