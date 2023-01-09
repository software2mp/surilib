/*! \file ViewerSynchronizationPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWERSYNCHRONIZATIONPART_H_
#define VIEWERSYNCHRONIZATIONPART_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/ViewportLinkManager.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

/** Part que permite sincronizar visualizadores */
/**
 * Se pueden elegir los visualizadores a sincronizar, el tipo de
 * sincronizacion(geografica, proyectada, imagen) y si solo se sincroniza
 * el centro o el bounding box completo.
 */
class ViewerSynchronizationPart : public Part {
public:
   /** Ctor */
   ViewerSynchronizationPart(ViewportManagerInterface *pViewportManager,
                             ViewcontextManagerInterface *pViewcontextManager,
                             ViewportLinkManager *pViewportLink, bool Enable = true,
                             bool Modified = false);
   /** Dtor */
   virtual ~ViewerSynchronizationPart();

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
   /** Modifica el nombre del viewer. */
   virtual bool ConfigureWidget();
   /** Devuelve si la parte tiene datos validos */
   virtual bool HasValidData();

private:
   ViewportManagerInterface *pViewportManager_;
   ViewcontextManagerInterface *pViewcontextManager_;
   ViewportLinkManager* pViewportLink_;
};

}

#endif /* VIEWERSYNCHRONIZATIONPART_H_ */
