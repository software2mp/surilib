/*! \file MetadataPropertiesPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <map>
#include <string>
#include <cstdio>

#include "MetadataPropertiesPart.h"

#include "RasterDatasource.h"
#include "suri/Image.h"
#include "resources.h"

#include "wx/colour.h"
#include "wx/listctrl.h"

#define KEY_COLUMN      0
#define VALUE_COLUMN    1

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(MetadataPropertiesPartEvent, MetadataPropertiesPart)
   IMPLEMENT_EVENT_CALLBACK(OnSaveButtonClicked, OnSaveButtonClicked(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

MetadataPropertiesPart::MetadataPropertiesPart(RasterDatasource* pDatasource) :
      Part(true, false), NEW_EVENT_OBJECT(MetadataPropertiesPartEvent),
      pDatasource_(pDatasource) {
   windowTitle_ = _(caption_METADATA_PROPERTIES);
}

MetadataPropertiesPart::~MetadataPropertiesPart() {

}

/**
 * Indica si el Part tiene cambios para salvar.
 */
bool MetadataPropertiesPart::HasChanged() {
   return false;
}

/**
 * Salva los cambios realizados en el Part.
 */
bool MetadataPropertiesPart::CommitChanges() {
   return true;
}

/**
 * Restaura los valores originales del Part.
 */
bool MetadataPropertiesPart::RollbackChanges() {
   return true;
}

/**
 * Inicializa el part.
 */
void MetadataPropertiesPart::SetInitialValues() {

}

/**
 * Actualiza el estado de la parte.
 */
void MetadataPropertiesPart::Update() {

}

/**
 * Crea la ventana de la parte.
 */
bool MetadataPropertiesPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_METADATA_PROPERTIES_PART"));

   wxListCtrl* plist = XRCCTRL(*pToolWindow_, wxT("ID_METADATA_PROPERTIES_LIST"), wxListView);

   // Columna clave
   plist->InsertColumn(KEY_COLUMN, wxEmptyString, wxLIST_FORMAT_LEFT, 192);

   // Columna valor
   plist->InsertColumn(VALUE_COLUMN, wxEmptyString, wxLIST_FORMAT_LEFT, 340);

   // Cargo la lista
   InitializeList();

   GET_CONTROL(*pToolWindow_,
         wxT("ID_METADATA_PROPERTIES_SAVE_BUTTON"), wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(MetadataPropertiesPartEvent::OnSaveButtonClicked), NULL,
         pEventHandler_);

   return true;
}

/**
 * Retorna el icono de la herramienta.
 */
void MetadataPropertiesPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_RASTER_METADATA, ToolBitmap);
}

/**
 * Handler para el evento de click del boton guardar.
 */
void MetadataPropertiesPart::OnSaveButtonClicked(wxCommandEvent &Event) {
   Element* pelement = pDatasource_ ? pDatasource_->GetElement() : NULL;
   if (pelement == NULL)
      return;

   std::string filename;
   wxFileDialog dlg(NULL, _(caption_METADATA_FILE), wxT(""), wxT(""),
                    _("Archivos de texto (*.txt)|*.txt"),
                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
   if (dlg.ShowModal() != wxID_OK) return;

   filename = dlg.GetPath();

   FILE* pf = fopen(filename.c_str(), "w+");
   if (!pf) return;

   Image* pimage = Image::Open(pelement->GetUrl().mb_str());

   std::vector < std::string > domains;
   domains.push_back(""); // default
   domains.push_back("IMAGE_STRUCTURE");
   domains.push_back("SUBDATASETS");
   domains.push_back("GEOLOCATION");
   domains.push_back("RPC");

   std::map<std::string, std::map<std::string, std::string> > md = pimage->GetMetadata(
         domains);

   std::map<std::string, std::map<std::string, std::string> >::iterator it;
   for (it = md.begin(); it != md.end(); ++it) {
      // Agrego dominio
      fprintf(pf, "%s:\n", it->first.empty() ? "DEFAULT" : it->first.c_str());

      // Agrego items del dominio
      std::map<std::string, std::string>::iterator itkv;
      for (itkv = it->second.begin(); itkv != it->second.end(); ++itkv) {
         fprintf(pf, "   %s=%s\n", itkv->first.c_str(), itkv->second.c_str());
      }

      // Separador de dominios
      fprintf(pf, "\n");
   }

   fclose(pf);
}

/**
 * Inicializa la lista.
 */
void MetadataPropertiesPart::InitializeList() {
   Element* pelement = pDatasource_ ? pDatasource_->GetElement() : NULL;
   if (pelement) {
      wxListCtrl* plist = XRCCTRL(*pToolWindow_, wxT("ID_METADATA_PROPERTIES_LIST"),
                                  wxListView);

      Image* pimage = Image::Open(pelement->GetUrl().mb_str());

      std::vector<std::string> domains;
      domains.push_back(""); // default
      domains.push_back("IMAGE_STRUCTURE");
      domains.push_back("SUBDATASETS");
      domains.push_back("GEOLOCATION");
      domains.push_back("RPC");

      std::map<std::string, std::map<std::string, std::string> > md =
            pimage ? pimage->GetMetadata(domains) : std::map < std::string, std::map<
            std::string, std::string> >();

      int ix = 0;
      std::map<std::string, std::map<std::string, std::string> >::iterator it;
      for (it = md.begin(); it != md.end(); ++it) {
         // Agrego dominio
         plist->InsertItem(ix, wxEmptyString, 0);
         plist->SetItem(ix, KEY_COLUMN, it->first.empty() ? "DEFAULT" : it->first);
         plist->SetItemBackgroundColour(ix, *wxLIGHT_GREY);

         // Agrego items del dominio
         std::map<std::string, std::string>::iterator itkv;
         for (itkv = it->second.begin(); itkv != it->second.end(); ++itkv) {
            plist->InsertItem(++ix, wxEmptyString, 0);
            plist->SetItem(ix, KEY_COLUMN, itkv->first);
            plist->SetItem(ix, VALUE_COLUMN, itkv->second);
         }

         // Separador de dominios
         plist->InsertItem(++ix, wxEmptyString, 0);
         ++ix;
      }
   }
}

}  // namespace suri
