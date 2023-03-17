/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#ifndef LAYERLIST_H_
#define LAYERLIST_H_

// Include standard
#include <deque>
#include <map>

// Include suri
#include "Element.h"
#include "Model.h"
#include "XmlElement.h"
#include "NoDataValue.h"

/** namespace suri */
namespace suri {

/** forwards */
class ActivationLogic;

/** Model "Interfaz" con una lista de elementos de contexto */
/**
 * Contiene una lista de elementos. Los elementos pueden
 * selecionarce, activarse y moverse de posicion
 *    - Herada de XmlElement para poder guardar sus propiedades
 *      en el xml de descripcion
 *    - Hereda de Model para notificar a los views sus cambios.
 *      (por ej se agrego elemento)
 */
class LayerList : public Model, public XmlElement, public NoDataValue {
private:
   /** Ctor. de Copia. */
   LayerList(const LayerList &LayerList);

public:
   /** Guarda si un elemento esta selecionado y a que grupo pertenece */
   class ElementData {
   public:
      /** Ctor. */
      ElementData() :
            selected_(false), group_(0) {
      }
      bool selected_; /*! Estado de seleccion del elemento */
      int group_; /*! Grupo al que pertenece el elemento */
   };
   enum {
      AllGroups = 0
   };
   /** Tipo de dato para los elementos seleccionados */
   typedef std::map<Element*, ElementData> ElementSelectionType;
   /** Tipo que contiene la lista de elementos */
   typedef std::deque<Element*> ElementListType;
   /** Constructor */
   LayerList();
   /** Destructos */
   virtual ~LayerList();
   /** Inserta un elemento */
   virtual bool AddElement(Element* pElement, int Group = AllGroups, bool Notify = true);
   /** Borra un elemento */
   virtual bool DelElement(int Index, bool IgnoreHidden = true);
   /** Borra un elemento */
   virtual ElementListType::iterator
   DelElement(ElementListType::iterator &ElementIterator);
   /** Borra los elementos desde First hasta uno antes de Last */
   virtual size_t DelElement(ElementListType::iterator &First,
                             ElementListType::iterator &Last);
   /** Borra un elemento */
   virtual bool DelElement(Element *pElement);
   /** elimina un elemento a partir del Uid del Elemento */
   virtual bool DelElement(const suri::Element::UidType &Uid);
   /** Template para eliminar elementos de la lista. */
   template<class IteratorType>
   size_t DelElement(const IteratorType &ItBegin, const IteratorType &ItEnd);
   /** Permite saber si se puede o no mover un elemento a otra posicion */
   virtual bool CanMoveElement(Element* pElementFrom, Element* pElementTo);
   /** Cambia de posicion un elemento */
   virtual bool MoveElement(Element* pElementFrom, Element* pElementTo);
   /** Selecciona o deselecciona un elemento */
   virtual void SelectElement(Element* pElement, bool Status = true);
   /** Selecciona o deselecciona un elemento */
   virtual void SelectElement(int Index, bool Status = true, bool IgnoreHidden = true);
   /** Selecciona o deselecciona un elemento a partir de su Uid */
   virtual void SelectElement(const suri::Element::UidType &Uid, bool Status = true);
   /** activa o desactiva un elemento */
   virtual void ActivateElement(Element* pElement, bool Status = true);
   /** activa o desactiva un elemento */
   virtual void ActivateElement(int Index, bool Status = true,
                                bool IgnoreHidden = true);
   /** activa o desactiva un Elemento a partir del Uid */
   virtual void ActivateElement(const suri::Element::UidType &Uid, bool Status = true);
   /** Cambia el estado de activacion del elemento */
   virtual void ChangeActiveElement(Element* pElement);
   /** Cambia el estado de activacion del elemento */
   virtual void ChangeActiveElement(int Index, bool IgnoreHidden = true);
   /** Cambia el estado de activacion del Elemento a partir del Uid */
   virtual void ChangeActiveElement(const suri::Element::UidType &Uid);
   /** Retorna la lista de elementos */
   virtual ElementListType::iterator GetElementIterator(const bool &Begin = true);
   /** Retorna el elemento deseado */
   virtual Element* GetElement(int Index, bool IgnoreHidden = true) const;
   /** Retorna el elemento deseado */
   virtual Element* GetElement(int Index, int Group, bool IgnoreHidden = true) const;
   /** Retorna el elemento con el mismo puntero */
   virtual Element* GetElement(void *pElement) const;
   /** retorna el Elemento a partir de su Uid */
   virtual Element* GetElement(const suri::Element::UidType &Uid);
   /** Retorna el elemento deseado */
   virtual ElementData* GetElementData(int Index, bool IgnoreHidden = true);
   /** Retorna el elemento deseado */
   virtual ElementData* GetElementData(int Index, int Group, bool IgnoreHidden = true);
   /** Retorna el elemento con el mismo puntero */
   virtual ElementData* GetElementData(void *pElement);
   /** devuelve la estructura ElementData del Elemento a partir de su Uid */
   virtual ElementData* GetElementData(const suri::Element::UidType &Uid);
   /** Retorna la cantidad de elementos */
   virtual int GetElementCount(bool IgnoreHidden = true) const;
   /** Chequea que los nombre de los elementos cargados no se repitan */
   virtual bool CheckFreeName( wxString nameCheck, int Group );
   /** Retorna la cantidad de elementos */
   virtual int GetElementCount(int Group, bool IgnoreHidden = true) const;
   /** Retorna la cantidad de elementos seleccionados */
   virtual int GetSelectedCount(bool IgnoreHidden = true);
   /** Retorna la cantidad de elementos seleccionados en el grupo */
   virtual int GetSelectedCount(int Group, bool IgnoreHidden = true);
   /** devuelve el Uid del 1er Elemento seleccionado */
   virtual const suri::Element::UidType GetSelectedElementUid(int Group,
                                                              bool IgnoreHidden = true);
   /** devuelve una lista de Uids de Elementos seleccionados */
   virtual const std::list<suri::Element::UidType> GetSelectedElementsUid(
         int Group, bool IgnoreHidden = true);
   /** Retorna la cantidad de elementos activos */
   virtual int GetActiveCount(bool IgnoreHidden = true);
   /** Retorna la cantidad de elementos activos */
   virtual int GetActiveCount(int Group, bool IgnoreHidden = true);
   /** Retorna la cantidad de elementos editados */
   virtual int GetEditedCount() const;
   /** Permite asignar una logica de activacion de elementos */
   virtual void SetActivationLogic(ActivationLogic *pLogic);
   /** Retorna un xml con los nodos elemento. */
   virtual wxXmlNode *GetProperties();
   /** Retorna el elemento activo del grupo seleccionado */
   virtual Element* GetActiveElement(int Index, int Group,
                                     bool IgnoreHidden = true) const;
   virtual std::vector<Element*> GetRenderizationOrderList();
   /*desactiva todos los elementos*/
   virtual void DisableAll();

private:
   std::vector<Element*> renderizationOrder_;/* Orden en que se van a renderizar los elementos*/
   ElementListType elementList_; /*! Elementos del contexto */
   ElementSelectionType elementData_; /*! Indices de los elementos seleccionados */
   ActivationLogic *pLogic_; /*! Objeto que determina como se activan los elementos */
};

/**
 * Eliminar elementos de la lista. Primero desactiva los elementos a eliminar
 * y bloquea las actualizaciones sobre los mismos, para luego eliminarlos.
 * @param[in] ItBegin iterador al 1er elemento a eliminar.
 * @param[in] ItEnd iterador al ultimo elemento a eliminar.
 * @return la cantidad de elementos eliminados
 * \todo(05/01/2010 - Alejandro): habria que modificar el metodo DelElement(it, it)
 * para que utilice este template.
 */
template<class IteratorType>
size_t LayerList::DelElement(const IteratorType &ItBegin, const IteratorType &ItEnd) {
   std::list<Element *> elementlist;
   BlockViewerUpdate();

   for (IteratorType it = ItBegin; it != ItEnd; it++) {
      Element *pelement = GetElement((*it));
      if (pelement) {
         pelement->BlockViewerUpdate();
         ActivateElement(pelement, false);
         elementlist.push_back(pelement);
      }
   }

   size_t count = 0;
   std::list<Element *>::iterator it = elementlist.begin();
   for (; it != elementlist.end(); it++)
      if (DelElement((*it))) {
         count++;
      }

   UnblockViewerUpdate();
   SetChanged();
   SendViewerUpdate();

   return count;
};

}

#endif /*LAYERLIST_H_*/
