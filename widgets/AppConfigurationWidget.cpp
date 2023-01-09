/*! \file AppConfigurationWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/AppConfigurationWidget.h"
#include "suri/Configuration.h"
#include "suri/Part.h"
#include "suri/DialogWidget.h"

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(AppConfigurationWidgetEvent, AppConfigurationWidget)
         IMPLEMENT_EVENT_CALLBACK(OnButtonOk, OnButtonOk(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT


/* Constructor. */
AppConfigurationWidget::AppConfigurationWidget(
      AppConfigPartsProviderInterface* pappconfigparts, wxString Titulo,
      DataViewManager* pDataviewmanager) :
      NotebookWidget(Titulo), pAppconfigparts_(pappconfigparts), pDataviewmanager_(
            pDataviewmanager), NEW_EVENT_OBJECT(AppConfigurationWidgetEvent) {
   CreateTool();
}

/* Destructor */
AppConfigurationWidget::~AppConfigurationWidget() {
}

/** Commit de los cambios efectuados. */
bool AppConfigurationWidget::ApplyChanges() {
   if (!PartContainerWidget::ApplyChanges()){
      Configuration::DiscardChanges();
      return false;
   }
   return true;
}

/** Inserta los parts necesarios para la configuracion de la aplicacion */
bool AppConfigurationWidget::InsertParts() {
   std::vector<Part *> parts = pAppconfigparts_->GetConfigParts();
   std::vector<Part *>::iterator part = parts.begin();

   for (; part != parts.end(); ++part) {
      if (!InsertPart(*part))
            return false;
   }
   return true;
}
void AppConfigurationWidget::OnButtonOk(wxCommandEvent &Event) {
   Event.Skip(false);
   if (ApplyChanges()) {
      Configuration::Save();
      EndModal(wxID_OK);
   }
}

} // End namespace suri
