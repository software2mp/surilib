/** \file SingleViewerSelectionPart.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SINGLEVIEWERSELECTIONPART_H_
#define SINGLEVIEWERSELECTIONPART_H_

#include <vector>
#include <string>
#include <map>

#include "suri/Part.h"
#include "suri/SuriObject.h"

namespace suri {

DECLARE_EVENT_OBJECT(SingleViewerSelectionPartEvent);

/**
 * Esta clase genera un part que permite la seleccion de un viewer.
 */
class SingleViewerSelectionPart : public Part {
public:
   FRIEND_EVENT_OBJECT(SingleViewerSelectionPartEvent);

   /**
    * Ctor
    */
   SingleViewerSelectionPart();

   /**
    * Dtor
    */
   virtual ~SingleViewerSelectionPart();

   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();

   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();

   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();

   /** Inicializa el part */
   virtual void SetInitialValues();

   /** Actualiza el estado del Part */
   virtual void Update();

   /** Modifica el nombre del viewer */
   virtual bool ConfigureWidget();

   /** Configura los visualizadores disponibles. */
   void SetCurrentViewers(const std::map<suri::SuriObject::UuidType, std::string>& CurrentViewers);

   /** Handler para el evento de cambio de seleccion de choice. */
   void OnChViewerChanged(wxCommandEvent &Event);

   /** Devuelve el visualizador seleccionado */
   SuriObject::UuidType GetSelectedViewer();

private:
   EVENT_OBJECT_PROPERTY(SingleViewerSelectionPartEvent);

   static const std::string SINGLE_VIEWER_SELECTION_XRC_NAME; /* < Id Panel */
   static const int kInitialPos = 0;
   static const std::string kViewer;
   SuriObject::UuidType selectedViewerId_;

   std::map<suri::SuriObject::UuidType, std::string> currentViewers_;

   /** Inicializa el part realizando todas las configuraciones necesarias. */
   void InitializePart();

   /** Inicializa el combo de seleccion de visualizadores */
   void InitializeChoice();

};
}  // namespace suri

#endif /* SINGLEVIEWERSELECTIONPART_H_ */
