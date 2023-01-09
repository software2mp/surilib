/*! \file DualViewerSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DualViewerSelectionPart_H_
#define DualViewerSelectionPart_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"

// Includes Wx

// Includes App

// Defines

// Namespace suri
namespace suri {
/**
 * Esta clase genera un part que permite la seleccion de dos viewers
 *
 */
class DualViewerSelectionPart : public Part {
public:
   DualViewerSelectionPart();
   virtual ~DualViewerSelectionPart();

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
private:
   static const std::string DUAL_VIEWER_SELECTION_XRC_NAME; /*! Id panel */
protected:
};
}

#endif /* DualViewerSelectionPart_H_ */
