/*! \file PartCollection.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/PartCollection.h"

// Includes standard

// Includes Suri

// Includes Wx
#include "wx/wx.h"

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Ctor */
PartCollection::PartCollection() :
      Part() {
}

/** Ctor */
PartCollection::PartCollection(Widget* pWidget) :
      Part(pWidget) {
}

/** Ctor */
PartCollection::PartCollection(const std::string &XrcId, const std::string &PartName) :
      Part(XrcId, PartName, true, false) {
}

/** Dtor */
PartCollection::~PartCollection() {
}

/**
 * Agrega un part a la coleccion
 * @param[in] pNewPart part que se quiere agregar a la coleccion.
 * @param[in] ParentWidgetId es el id del panel donde se debe agregar la
 * ventana del widget asociado a pNewPart.
 */
void PartCollection::AddPart(Part* pNewPart, const std::string &ParentWidgetId) {
   partCollection_.push_back(std::pair<Part*, std::string>(pNewPart, ParentWidgetId));
}

/**
 * Remueve un part de la coleccion
 * @param[in] pPart Part que se quiere remover de la coleccion.
 */
void PartCollection::RemovePart(Part* pPart) {
   // STUB
}

/**
 * Realiza la difusion de HasChanged a todos los parts que componen la
 * coleccion interna.
 * @return bool que indica OR de los resultados particulares de cada part.
 */
bool PartCollection::HasChanged() {
   bool haschanged = false;

   std::list<std::pair<Part*, std::string> >::iterator it = partCollection_.begin();
   while (it != partCollection_.end() && !haschanged) {
      haschanged = haschanged || it->first->HasChanged();
      it++;
   }

   return haschanged;
}

/**
 * Realiza la difusion del metodo commit a todos los parts que componen la
 * coleccion interna.
 * @return bool que indica AND de los Commit particulares de cada part.
 */
bool PartCollection::CommitChanges() {
   bool success = true;

   std::list<std::pair<Part*, std::string> >::iterator it = partCollection_.begin();
   while (it != partCollection_.end() && success) {
      success = success && it->first->CommitChanges();
      it++;
   }

   return success;
}

/**
 * Realiza la difusion de Rollback a todos los parts que componen la coleccion
 * interna.
 * @return bool que indica AND de los Rollback particulares de cada part.
 */
bool PartCollection::RollbackChanges() {
   bool success = true;

   std::list<std::pair<Part*, std::string> >::iterator it = partCollection_.begin();
   while (it != partCollection_.end() && success) {
      success = success && it->first->RollbackChanges();
      it++;
   }

   return success;
}

/**
 * Realiza la difusion de SetInitialValues a todos los parts que componen la
 * coleccion interna.
 */
void PartCollection::SetInitialValues() {
   std::list<std::pair<Part*, std::string> >::iterator it = partCollection_.begin();
   while (it != partCollection_.end()) {
      it->first->SetInitialValues();
      it++;
   }
}

/**
 * Realiza la difusion de Update a todos los parts que componen la coleccion
 * interna.
 */
void PartCollection::Update() {
   std::list<std::pair<Part*, std::string> >::iterator it = partCollection_.begin();
   while (it != partCollection_.end()) {
      it->first->Update();
      it++;
   }
}

/**
 * Agrega a pWidget los controles de todos los parts en partColection_.
 * @return bool que indica si se pudo configurar la ventana correctamente.
 */
bool PartCollection::ConfigureWidget() {
   bool addcontrolok = true;
   std::list<std::pair<Part*, std::string> >::iterator it;
   for (it = partCollection_.begin(); it != partCollection_.end(); it++) {
      if (!GetWidget()->AddControl(it->first->GetWidget(), it->second)) {
         addcontrolok = false;
      }
   }

   return addcontrolok;
}
}
