/*! \file FiltredElementListWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <iostream>
#include <utility>
#include <map>
#include <string>
#include <list>
#include <set>

// Includes Suri
#include "FiltredElementListWidget.h"
#include "suri/VectorElement.h"
#include "suri/LayerList.h"

// Includes Wx
#include <wx/fs_mem.h> // filesystem

// Includes App

// Defines
/** Tamanio (en pixel) para los iconos de la lista */
#define ICON_SIZE 32
// Forwards

/** namespace suri */
namespace suri {

/**
 * Constructor
 * @param[in] pParent a la ventana padre
 * @param[in] pLayerList lista de elementos
 */
FiltredElementListWidget::FiltredElementListWidget(
      wxWindow *pParent, suri::LayerList *pLayerList) :
      ElementListWidget(pParent, pLayerList, NULL) {
   fileIndex_ = 0;
}

/**
 * Destructor
 */
FiltredElementListWidget::~FiltredElementListWidget() {
   std::map<Element*, std::string>::iterator it = elementIcons_.begin();
   while (it != elementIcons_.end()) {
      wxMemoryFSHandler::RemoveFile(it->second);
      it++;
   }
}

/**
 * Determina si hay que mostrar el elemento. Retorna true si el tipo
 * esta en la lista o es de tipo vector y GetTypeAsString esta en la lista
 * @param[in] pElement elemento que se quiere mostrar
 * @return true si el tipo del elemento esta en el set
 */
bool FiltredElementListWidget::CanShowElement(Element* pElement) {
   bool returnvalue = false;
   std::set<std::string>::iterator it = elementsTypesShown_.find(
         pElement->GetPropertyType().c_str());
   if (it != elementsTypesShown_.end()) {
      returnvalue = true;
   } else {
      VectorElement* pvector = dynamic_cast<VectorElement*>(pElement);
      if (pvector) {
         std::string aux = pvector->GetTypeAsString().c_str();
         it = elementsTypesShown_.find(pvector->GetTypeAsString().c_str());
         if (it != elementsTypesShown_.end()) {
            returnvalue = true;
         }
      }
   }
   return returnvalue;
}

/**
 * Retorna el dibujo que hay que mostrar junto al elemento. Guarda imagen
 * en memoria
 * @param[in] pElement elemento del que se quiere obtener el icono
 * @return string que representa el icono en memoria asociado al elemento
 */
std::string FiltredElementListWidget::GetElementIcon(Element* pElement) {
   wxString name;
   // Si no existe el elemento en mapa creo el nombre del archivo
   std::map<Element*, std::string>::iterator it = elementIcons_.find(pElement);

   if (it != elementIcons_.end()) {
      name = it->second;
      if (pElement->HasChanged()) {
         wxMemoryFSHandler::RemoveFile(name);
         wxBitmap preview;
         pElement->GetPreview(preview, ICON_SIZE, ICON_SIZE);
         wxMemoryFSHandler::AddFile(name, preview, wxBITMAP_TYPE_PNG);
      }
   } else {
      fileIndex_++;
      name = wxString::Format("Element%d", fileIndex_);
      elementIcons_.insert(std::pair<Element*, std::string>(pElement, name.c_str()));
      wxBitmap preview;
      pElement->GetPreview(preview, ICON_SIZE, ICON_SIZE);
      wxMemoryFSHandler::AddFile(name, preview, wxBITMAP_TYPE_PNG);
   }

   wxString memoryname = wxString("memory:") + name;
   return memoryname.c_str();
}

/**
 * Agrega strings a la lista de tipos de elemento seleccionado.
 * @param[in] TypeAsString nuevo tipo de elemento desplegado
 */
void FiltredElementListWidget::AddElementType(std::string TypeAsString) {
   elementsTypesShown_.insert(TypeAsString);
}

/**
 * Limpia la lista de tipo de elementos desplegados
 */
void FiltredElementListWidget::ClearElementTypes() {
   elementsTypesShown_.clear();
}

/**
 * Retorna los elementos seleccionados
 * @return retorna los elementos activos en la lista
 */
std::list<Element*> FiltredElementListWidget::GetActiveElements() {
   std::list<Element*> element;
   int elemcount = pLayerList_->GetElementCount(!isMenuChecked_);
   for (int i = 0; i < elemcount; i++) {
      Element* pelement = pLayerList_->GetElement(i, !isMenuChecked_);
      if (IsElementActive(pelement)) {
         element.push_back(pelement);
      }
   }
   return element;
}

/**
 * Limpia los elementos seleccionados.
 */
void FiltredElementListWidget::ClearActiveElements() {
   int elemcount = pLayerList_->GetElementCount(!isMenuChecked_);
   for (int i = 0; i < elemcount; i++)
      ActivateElement(pLayerList_->GetElement(i, !isMenuChecked_), false);
}

}
