/*! \file WmsEditionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "WmsEditionPart.h"
// Includes Wx
// Defines

namespace suri {
/** Nombre del XRC que representa la ventana. */
const std::string WmsEditionPart::WMS_EDITION_XRC_NAME = wxT(
      "ID_WMS_INSITU_EDITOR_PANEL");
const std::string WmsEditionPart::WMS_CGU_PANEL = wxT("ID_WMS_CGU_PANEL");

/**
 * Constructor
 * @param LibraryId id de la biblioteca de tabla de intervalos
 * @param pManager manager que administra la biblioteca
 * @param Feature layout de features del cgu
 */
WmsEditionPart::WmsEditionPart(
      const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
      UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature) :
            LibraryItemEditorPart(LibraryId, pManager, WMS_EDITION_XRC_NAME, WMS_CGU_PANEL,
                                  LibraryManager::READONLY, Feature) {
}

/** Destructor */
WmsEditionPart::~WmsEditionPart() {
}

/**
 * Metodo auxiliar privado que es necesario implementar en las clases hijas.
 * Este metodo es el encargado de actualizar realmente la informacion en el part
 * Es llamado en el metodo SetActiveItem a modo de Template Method
 **/
void WmsEditionPart::UpdateFields(const LibraryItem* pItem, bool Modifiable) {
   if (pItem) {
      wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_TEXTCTRL_URL"), wxTextCtrl);
      const LibraryItemAttribute* attr = pItem->GetAttribute("URL");
      if (attr) {
         ptext->SetValue(attr->GetValue());
         urlvalue_ = pItem->GetAttribute("URL")->GetValue();
      }
      else
         ptext->SetValue("");
   }
}

/**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
 *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
 *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
 *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
 *   @return vector con atributos
 *   @return NULL en caso de que la informnacion del part no sea valida
 *   **/
std::vector<LibraryItemAttribute*>* WmsEditionPart::CreateAttributesFromFields() {
   std::vector<LibraryItemAttribute*>* pattributes = new std::vector<
            LibraryItemAttribute*>;
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_TEXTCTRL_URL"), wxTextCtrl);
   std::string urlvalue = ptext->GetValue().c_str();
   suri::LibraryItemAttribute* libattr = new suri::LibraryItemAttribute("URL", urlvalue);

   pattributes->push_back(libattr);
   return pattributes;
}

/** Indica si el Part tiene cambios para salvar */
bool WmsEditionPart::HasChanged() {
   bool hasChanged = false;
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_TEXTCTRL_URL"), wxTextCtrl);
   std::string currenturlvalue = ptext->GetValue().c_str();
   if (urlvalue_.compare(currenturlvalue))
      hasChanged = true;
   return hasChanged;
}
} /** namespace suri */
