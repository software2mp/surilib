/*! \file LayerList.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <cmath>
#include <algorithm>
#include <list>
#include <vector>

// Includes suri
#include "suri/LayerList.h"
#include "suri/Configuration.h"
#include "SingleElementActivationLogic.h"

/** namespace suri */
namespace suri {

/**
 * Constructor
 */
LayerList::LayerList() : XmlElement(wxT("lista")) {  // No se traduce, elemento de XML
   // Logica de seleccion por default (1 solo elemento activo posible)
   pLogic_ = new SingleElementActivationLogic();
}

/**
 * Destructor
 */
LayerList::~LayerList() {
   // Elimino la logica antes de eliminar los elementos porque intenta limpiar
   // el viewer que ya fue destruido.
   delete pLogic_;
   pLogic_ = NULL;
   // Elimino todos los viewers
   BlockViewerUpdate();
   DelElement(elementList_.begin(), elementList_.end());
   UnblockViewerUpdate();
   Model::Clean();
}



std::vector<Element*> LayerList::GetRenderizationOrderList() {
   if (Configuration::GetParameter("app_short_name", "") == wxT("SoPI"))
      return renderizationOrder_;
   else {
      std::vector<Element*> aux;
      std::vector<Element*>::const_iterator it = renderizationOrder_.begin();
      for (; it != renderizationOrder_.end(); ++it) {
         ElementData data = elementData_[*it];
         if (data.group_ != 6)
            aux.push_back(*it);
      }
      for (it = renderizationOrder_.begin(); it != renderizationOrder_.end(); ++it) {
         ElementData data = elementData_[*it];
         if (data.group_ == 6) aux.push_back(*it);
      }
      return aux;
   }
}



/** Inserta un elemento */
/**
 * agrega el elemento con el grupo que le corresponde
 * Busca en elementList secuencialmente hasta que el numero de grupo del
 * elemento sea mayor que Group. Luego agrega el elemento en esta posicion y
 * agrega a elementData_ el par elemento-ElementData.
 * Finalmente hace update.
 * @param[in] pElement elemento que se quiere agregar
 * @param[in] Group grupo al cual se agrega el elemento
 * \post El elemento se agrego al LayerList antes del primer elemento con grupo mayor
 * \attention pElement sera responsabilidad de la LayerList y no debe hacerce delete del elemento fuera de la clase
 */
bool LayerList::AddElement(Element* pElement, int Group, bool Notify) {
   bool retval = false;

   // Verifico que no me hayan pasado un elemento nulo
   if (pElement) {
      LayerList::ElementListType::iterator it;
      for (it = elementList_.begin(); it != elementList_.end(); ++it) {
         ElementData *pedata = GetElementData(*it);
         if (std::abs(static_cast<double>(pedata->group_)) > std::abs(static_cast<double>(Group))) {
            break;
         }
      }

      elementList_.insert(it, pElement);
      renderizationOrder_.push_back(pElement);
      ElementData edata;

      edata.group_ = Group;
      elementData_.insert(std::make_pair(pElement, edata));

      if (Group == -1)
         MoveElement(pElement, renderizationOrder_[0]);

      if (Notify) {
         SetChanged();
         SendViewerUpdate();
      }
      retval = true;
   }

   return retval;
}

/**
 *  Saca un elemento del listado
 *  @param[in] Index indice del elemento que se quiere eliminar
 *  @param[in] IgnoreHidden flag que indica si hay que ignorar elementos ocultos
 *  @return true si pudo eliminar el elemento, false en otro caso
 */
bool LayerList::DelElement(int Index, bool IgnoreHidden) {
   return DelElement(GetElement(Index, IgnoreHidden));
}

/**
 *  Saca un elemento del listado en funcion del iterador. Tambien
 * elimina el nodo correspondiente en elementData_
 * @param[in] ElementIterator iterador que apunta al elemento que se quiere eliminar
 * @return iterador al elemento siguiente al elemento eliminado. O un iterador al
 *          final de la lista si no encontro el elemento a eliminar.
 */
LayerList::ElementListType::iterator LayerList::DelElement(
      ElementListType::iterator &ElementIterator) {
   // es para que no se produzca SIGSEGV cuando desreferencia el iterador.
   if (ElementIterator == elementList_.end()) {
      return elementList_.end();
   }

   Element *pelement = *ElementIterator;

   // bloqueo sin problema ya que el elemento va a ser eliminado
   pelement->BlockViewerUpdate();
   ActivateElement(pelement, false);

   ElementListType::iterator elit = elementList_.erase(ElementIterator);
   std::vector<Element*>::iterator it =
         find(renderizationOrder_.begin(), renderizationOrder_.end(), pelement);
   renderizationOrder_.erase(it);
   elementData_.erase(pelement);

   SetChanged();
   SendViewerUpdate();

   Element::Destroy(pelement);

   // Si no lo destruyo (y torno NULL) quiere decir que no fue
   // creado con la factoria, entonces lo destruyo con delete.
   // Si lo destruyo, volvio NULL y no afecta el delete.

   return elit;
}

/**
 * Borra los elementos desde First hasta uno antes de Last
 * \pre deben ser iteradores de la lista y encontrarse en orden
 * @param First iterador de comienzo
 * @param Last iterados de fin (no incluido)
 * @return cantidad de elementos eliminados
 */
size_t LayerList::DelElement(ElementListType::iterator &First, ElementListType::iterator &Last) {
   ElementListType::iterator it = elementList_.begin();
   // Verifico que sean iteradores correctos y que esten en orden
   bool foundfirst = false, foundlast = false;
   while (it != elementList_.end()) {
      if (it == First && !foundlast) {
         foundfirst = true;
      }
      if (it == Last) {
         foundlast = true;
      }
      it++;
   }
   // Si el ultimo es end
   foundlast |= Last == elementList_.end();
   size_t count = 0;
   it = First;
   Element *plastelement = Last == elementList_.end() ? NULL : *Last;
   // Si los encontro en orden
   if (foundfirst && foundlast) {
      BlockViewerUpdate();
      // Recorro desactivando para que el delete no mande un update forzado
      while (it != elementList_.end() && (*it) != plastelement) {
         (*it)->BlockViewerUpdate();
         (*it)->Activate(false);
         it++;
      }
      it = First;
      // Recorro borrando hasta el final o que me encuentro con el ultimo
      // elemento (que no se deberia borrar)
      while (it != elementList_.end() && (*it) != plastelement) {
         it = DelElement(it);
         count++;
      }
      UnblockViewerUpdate();
   }
   SetChanged();
   SendViewerUpdate();
   return count;
}

/**
 * Borra un elemento
 * Busca en la lista el elemento con un iterador y llama a
 * DelElement(ElementListType::iterator)
 * @param[in] pElement elemento que se quiere eliminar
 * @return true si pudo eliminar el elemento, falso si no pudo o el elemento es nulo
 */
bool LayerList::DelElement(Element *pElement) {
   bool retval = false;

   // Verifico que el elemento que me hayan pasado no sea nulo
   if (pElement) {
      ElementListType::iterator it;
      for (it = elementList_.begin(); it != elementList_.end(); it++) {
         if (pElement == *it) {
            // Se podria verificar la eliminacion, buscando el Elemento en la lista.
            // No se hace xq por cada caso de exito recorro 1 vez la lista.
            DelElement(it);
            retval = true;
            break;
         }
      }
   }
   return retval;
}

/**
 * elimina un elemento a partir del Uid del Elemento. Obtiene un puntero al
 * elemento invocando a GetElement(Uid) y luego invoca a DelElement con dicho
 * puntero.
 * @param[in] Uid Id del Elemento.
 * @return true en caso de haber eliminado con exito el Elemento.
 * @return false en caso de no haber podido eliminar el Elemento.
 */
bool LayerList::DelElement(const suri::Element::UidType &Uid) {
   return DelElement(GetElement(Uid));
}

/** Permite saber si se puede o no mover un elemento a otra posicion */
/**
 * Controla que el grupo del elemento anterior y el grupo del elemento
 * siguiente concuerden con el grupo del elemento que se quiere mover(<= y >= respectivamente)
 * @param[in] pElementFrom elemento que se quiere mover
 * @param[in] pElementTo elemento aterior a la posicion donde quiere moverse
 * @return bool que indica si se puede mover
 */
bool LayerList::CanMoveElement(Element* pElementFrom, Element* pElementTo) {
   // si no hay elementdata => no existen en la lista
   if (!GetElementData(pElementFrom)
         || (pElementTo != NULL && !GetElementData(pElementTo))) {
      return false;
   }
   // busca el anterior y siguiente
   Element *pprevelem = pElementTo ? pElementTo : elementList_.back();
   Element *pnextelem = pElementTo;
   ElementListType::iterator it = elementList_.begin();
   while (it != elementList_.end() && *it != pElementTo) {
      pprevelem = *it;
      it++;
   }
   // identifica los grupos anterior y siguiente
   int prevgroup = GetElementData(pprevelem)->group_, nextgroup =
         pnextelem ? GetElementData(pnextelem)->group_ : prevgroup;
   int fromgroup = GetElementData(pElementFrom)->group_;
   if (!(fromgroup >= prevgroup && fromgroup <= nextgroup)) {
      return false;
   }
   return true;
}

/**
 * Objetivo: Mueve el elemento pElementFrom a la posicion del elemento
 * pElementTo y desplaza el resto de elementos una posicion hacia abajo.
 * Si pElementTo es NULL => mueve pElementFrom a la ultima posicion.
 * @param[in]	pElementFrom: elemento desde a dragear
 * @param[in]	pElementTo: elemento que se encuentra en la posicion hasta.
 */
bool LayerList::MoveElement(Element* pElementFrom, Element* pElementTo) {
#ifdef __UNUSED_CODE__
   if (!CanMoveElement(pElementFrom, pElementTo)) {
      return false;
   }
#endif
   ElementListType::iterator iter = elementList_.begin();
   while (iter != elementList_.end() && *iter != pElementFrom)
      iter++;

   std::vector<Element*>::iterator it = renderizationOrder_.begin();
   while (it != renderizationOrder_.end() && *it != pElementFrom) {
      it++;
   }

   elementList_.erase(iter);
   renderizationOrder_.erase(it);

   if (pElementTo) {
      it = renderizationOrder_.begin();
      iter = elementList_.begin();

      while (iter != elementList_.end() && *iter != pElementTo)
         iter++;

      while (it != renderizationOrder_.end() && *it != pElementTo)
         it++;

      elementList_.insert(iter, pElementFrom);
      renderizationOrder_.insert(it, pElementFrom);
   } else {
      elementList_.push_back(pElementFrom);
      renderizationOrder_.push_back(pElementFrom);
   }
   SetChanged();
   SendViewerUpdate();
   return true;
}

/**
 * activa o desactiva un elemento
 * @param[in] Index indice del elemento que se quiere des / activar
 * @param[in] Status estado para des / activar
 * @param[in] IgnoreHidden indica si hay que ignorar los elementos ocultos
 */
void LayerList::SelectElement(int Index, bool Status, bool IgnoreHidden) {
   SelectElement(GetElement(Index, IgnoreHidden), Status);
}

/**
 * activa o desactiva un elemento
 * @param[in] pElement elemento que se quiere des / activar
 * @param[in] Status estado para des / activar
 */
void LayerList::SelectElement(Element* pElement, bool Status) {
   if (!pElement) {
      return;
   }

   LayerList::ElementSelectionType::iterator it = elementData_.find(pElement);

   // si no cambio el estado salgo
   if (it == elementData_.end() || (*it).second.selected_ == Status) {
      return;
   }
   (*it).second.selected_ = Status;
   SetChanged();
   SendViewerUpdate();
}

/**
 * Selecciona o deselecciona un elemento a partir de su Uid. Obtiene un puntero
 * al Elemento invocando a GetElement(Uid) y luego con dicho puntero invoca a
 * SelectElement.
 * @param[in]	Uid Id del Elemento.
 * @param[in]	Status Estado del Elemento.
 */
void LayerList::SelectElement(const suri::Element::UidType &Uid, bool Status) {
   SelectElement(GetElement(Uid), Status);
}

/** activa o desactiva un elemento */
void LayerList::ActivateElement(int Index, bool Status, bool IgnoreHidden) {
   ActivateElement(GetElement(Index, IgnoreHidden), Status);
}

/**
 *  Funcion principal de activacion / desactivacion de elementos, la logica debe
 * pasar por aca. Se llama a pLogic_ para que active el elemento.
 * @param[in] pElement elemento que se quiere des/activar
 * @param[in] Status estado para des / activar
 */
void LayerList::ActivateElement(Element* pElement, bool Status) {
   Element *pelement = GetElement(pElement);
   if ((pelement) && (pelement->IsActive() != Status)) {
      if (pLogic_) {
         pLogic_->SetActivation(pelement, elementList_, elementData_, Status);
         // Envia una modificacion de la lista
         SetChanged();
         SendViewerUpdate();
      } else {
         pelement->ActivateGIS(Status);
      }
   }
}

/**
 * Activa o desactiva un elemento a partir de su Uid. Obtiene un puntero
 * al Elemento invocando a GetElement(Uid) y luego con dicho puntero invoca a
 * ActivateElement.
 * @param[in]	Uid Id del Elemento.
 * @param[in]	Status Estado del Elemento.
 */
void LayerList::ActivateElement(const suri::Element::UidType &Uid, bool Status) {
   ActivateElement(GetElement(Uid), Status);
}

/**
 * Cambia el estado de activacion del elemento
 * @param[in] Index indice del elemento
 * @param[in] IgnoreHidden indica si hay que ignorar elementos ocultos
 */
void LayerList::ChangeActiveElement(int Index, bool IgnoreHidden) {
   ChangeActiveElement(GetElement(Index, IgnoreHidden));
}

/**
 * Cambia el estado de activacion del elemento
 * @param[in] pElement elemento que se quiere des/activar
 */
void LayerList::ChangeActiveElement(Element* pElement) {
   Element *pelement = GetElement(pElement);
   if (pelement) {
      ActivateElement(pelement, !pelement->IsActive());
   }
}

/**
 * Cambia el estado de Activacion de un elemento a partir de su Uid.
 * Obtiene un puntero al Elemento invocando a GetElement(Uid) y luego con dicho
 * puntero invoca a ChangeActiveElement.
 * @param[in]	Uid Id del Elemento.
 */
void LayerList::ChangeActiveElement(const suri::Element::UidType &Uid) {
   ChangeActiveElement(GetElement(Uid));
}

/**
 * Retorna Iteradores al principio o fin de la lista
 * Provee un iterator a la lista de elementos. Sirve para que otros
 * objetos puedan navegar la lista, pero no cambiar nada.
 * @param[in] Begin indica si se busca el inicio de la lista, falso da end()
 * @return Iterator al inicio o fin de la lista
 */
LayerList::ElementListType::iterator LayerList::GetElementIterator(const bool &Begin) {
   if (Begin) {
      return elementList_.begin();
   } else {
      return elementList_.end();
   }
}

/**
 *  Permite acceder en forma directa a un elemento dado su indice.
 * \post
 *    -# Si IgnoreHidden=True busca elemento en posicion Index considerando solo
 *        elementos visibles
 *    -# Si IgnoreHidden=False busca entre todos los elementos (no visibles
 *       inclusive)
 * @param[in] Index indice del elemento deseado
 * @param[in] IgnoreHidden si es true, el indice es con respecto a los elementos
 *             visibles
 * @return puntero al elemento
 */
Element* LayerList::GetElement(int Index, bool IgnoreHidden) const {
   return GetElement(Index, AllGroups, IgnoreHidden);
}

/**
 * Retorna el elemento deseado
 * Busca el elemento en la lista, si ignoreHidden=false busca entre todos caso
 * contrario busca solo en el grupo indicado. Si grupo=AllGroups buscara en
 * grupos mayores a AllGroups.
 * Esto significa que la posicion Index apuntara a distintos elementos segun
 * los valores de los otros parametros
 * \post
 *    -# Si IgnoreHidden=False busca entre todos los elementos (no visibles inclusive)
 *    -# Si IgnoreHidden=False y grupo=AllGroups busca considerando solo elementos visibles
 *		-# Si IgnoreHidden=False y grupo=NroGrupo busca entre los elementos de ese grupo
 * @param[in] Index indice del elemento en la lista
 * @param[in] Group grupo al que pertenece el elemento
 * @param[in] IgnoreHidden indica si hay que eliminar los elementos ocultos
 * @return elemento que esta en posicion Index dentro del conjunto buscado
 * \attention el elemento retornado es responsabilidad de la LayerList(no eliminar externamente).
 */
Element* LayerList::GetElement(int Index, int Group, bool IgnoreHidden) const {
   if (Index < 0) {
      return NULL;
   }
   ElementListType::const_iterator it = elementList_.begin();
   while (it != elementList_.end()) {
      ElementData *pedata = const_cast<LayerList*>(this)->GetElementData(*it);
      if (pedata
            && (((Group == pedata->group_ && Group != AllGroups)
                  || (pedata->group_ >= AllGroups && Group == AllGroups))
                  || !IgnoreHidden)) {
         if (Index == 0) {
            return *it;
         }

         Index--;
      }
      it++;
   }
   return NULL;
}

/**
 * Retorna el elemento con el mismo puntero
 * @param[in] pElement elemento buscado
 * @return puntero al elemento en la lista, o NULL si no se encontro el elemento
 * \attention el elemento retornado es responsabilidad de la LayerList(no eliminar externamente).
 */
Element* LayerList::GetElement(void *pElement) const {
   LayerList::ElementListType::const_iterator it = elementList_.begin();
   for (; it != elementList_.end(); ++it)
      if (pElement == (*it)) {
         return (*it);
      }
   return NULL;
}

/**
 * Devuelve un puntero al Elemento cuyo Id sea el que recibe por parametro.
 * @param[in] Uid Id del Elemento.
 * @return puntero al Elemento en caso de haberlo encontrado.
 * @return NULL en caso de que el Elemento no exista.
 */
Element* LayerList::GetElement(const suri::Element::UidType &Uid) {
   LayerList::ElementListType::const_iterator it = elementList_.begin();
   for (; it != elementList_.end(); it++)
      if (Uid == (*it)->GetUid()) {
         return (*it);
      }
   return NULL;
}

/**
 * Retorna el elemento deseado
 * @param[in] Index indice del elemento
 * @param[in] IgnoreHidden indica si hay que ignorar los elementos ocultos
 * @return ElementData del elemento
 * \attention el elemento retornado es responsabilidad de la LayerList(no eliminar externamente).
 */
LayerList::ElementData* LayerList::GetElementData(int Index, bool IgnoreHidden) {
   return GetElementData(Index, AllGroups, IgnoreHidden);
}

/**
 * Retorna el elemento con el mismo puntero
 * @param[in] pElement elemento del que se quiere obtener el ElementData
 * @return ElementData del elemento o NULL si el elemento es nulo o no se encuentra
 *           en la lista
 * \attention el elemento retornado es responsabilidad de la LayerList(no eliminar externamente).
 */
LayerList::ElementData* LayerList::GetElementData(void *pElement) {
   if (!pElement) {
      return NULL;
   }

   LayerList::ElementListType::const_iterator it = elementList_.begin();
   for (; it != elementList_.end(); ++it)
      if (pElement == (*it)) {
         return &((*elementData_.find(*it)).second);
      }
   return NULL;
}

/**
 * Retorna el elemento deseado
 * @param[in] Index indice del elemento
 * @param[in] Group grupo al que pertenece el elemento
 * @param[in] IgnoreHidden indica si hay que ignorar los elementos ocultos.
 * @return ElementData del elemento
 * \attention el elemento retornado es responsabilidad de la LayerList(no eliminar externamente).
 */
LayerList::ElementData* LayerList::GetElementData(int Index, int Group,
                                                  bool IgnoreHidden) {
   return &((*elementData_.find(GetElement(Index, Group, IgnoreHidden))).second);
}

/**
 * Devuelve la estructura ElementData del Elemento a partir de su Uid. Obtiene
 * un puntero al Elemento invocando a GetElement(Uid), y luego con dicho puntero
 * invoca a GetElementData.
 * @param[in]	Uid Id del Elemento.
 * @return puntero a la estructura ElementData en caso de que el Elemento exista
 * en la lista.
 * @return NULL en caso de que el Elemento no exista en la lista.
 */
LayerList::ElementData* LayerList::GetElementData(const suri::Element::UidType &Uid) {
   return GetElementData(GetElement(Uid));
}

/**
 *  Retorna la cantidad de elementos de la lista
 *  @param[in] IgnoreHidden indica si hay que ignorar los elementos ocultos
 *  @return cantidad de elementos de la lista
 */
int LayerList::GetElementCount(bool IgnoreHidden) const {
   return GetElementCount(AllGroups, IgnoreHidden);
}

/**
 * Retorna la cantidad de elementos
 * El valor que se retorna depende de los parametros:
 * Si IgnoreHidden=false retorna la cantidad de elementos total. Caso contrario:
 * \post
 *    -# Si Group=nro grupo retorna la cantidad de elementos en ese grupo.
 *    -# Si Group=AllGroups retorna la cantidad de elementos no escondidos.
 * @param[in] Group indica el / los grupos de los cuales se quieren contar los elementos
 * @param[in] IgnoreHidden indica si hay que ignorar los elementos ocultos
 * @return cantidad de elementos del /los grupo indicado
 */
int LayerList::GetElementCount(int Group, bool IgnoreHidden) const {
   int count = 0;
   for (ElementListType::const_iterator it = elementList_.begin();
         it != elementList_.end(); it++) {
      Element *pelement = *it;
      ElementData *pedata = const_cast<LayerList*>(this)->GetElementData(pelement);
      if (pedata
            && (((Group == pedata->group_ && Group != AllGroups)
                  || (pedata->group_ >= AllGroups && Group == AllGroups))
                  || !IgnoreHidden)) {
         count++;
      }
   }
   return count;
}

/** Devuelve TRUE si esta libre el nombre consultado */
bool LayerList::CheckFreeName( wxString nameCheck, int Group ) {
   for (ElementListType::const_iterator it = elementList_.begin();
         it != elementList_.end(); it++) {
      Element *pelement = *it;
      ElementData *pedata = const_cast<LayerList*>(this)->GetElementData(pelement);
      if (pedata
            && (((Group == pedata->group_ && Group != AllGroups)
                  || (pedata->group_ >= AllGroups && Group == AllGroups)) )) {

         if ( nameCheck.CmpNoCase(pelement->GetName()) == 0 )
         return false; // El nombre existe.
      }
   }
   return true; // El nombre no existe.
}

/**
 * Retorna la cantidad de elementos seleccionados
 * @param[in] IgnoreHidden indica si hay que ignorar los elementos ocultos
 * @return cantidad todal de elementos de la lista
 */
int LayerList::GetSelectedCount(bool IgnoreHidden) {
   return GetSelectedCount(AllGroups, IgnoreHidden);
}

/**
 * Retorna la cantidad de elementos seleccionados en el grupo
 * @param[in] Group Grupo en el cual quiero contar los elementos, AllGroups para todos
 * @param[in] IgnoreHidden ignora los elementos ocultos (group_ < AllGroups)
 * @return cantidad de elementos seleccionados en el grupo
 */
int LayerList::GetSelectedCount(int Group, bool IgnoreHidden) {
   int count = 0;
   LayerList::ElementListType::const_iterator it = elementList_.begin();
   for (; it != elementList_.end(); ++it) {
      ElementData *pedata = GetElementData(*it);
      if (pedata->selected_
            && (((Group == pedata->group_ && Group != AllGroups)
                  || (pedata->group_ >= AllGroups && Group == AllGroups))
                  || !IgnoreHidden)) {
         ++count;
      }
   }
   return count;
}

/**
 * devuelve el Uid del 1er Elemento seleccionado. Obtiene lista de Uid de
 * Elementos seleccionados y devuelve el 1ro de la lista.
 * @param[in]	Group
 * @param[in]	IgnoreHidden
 * @return Uid del 1er Elemento seleccionado en caso de que exista por lo menos
 * 1 Elemento seleccionado
 * @return Element::uidNull_ en caso de que no exista 1 Elemento seleccionado.
 */
const suri::Element::UidType LayerList::GetSelectedElementUid(int Group,
                                                              bool IgnoreHidden) {
   std::list<suri::Element::UidType> uidlist = GetSelectedElementsUid(Group,
                                                                      IgnoreHidden);
   if (!uidlist.empty()) {
      return uidlist.front();
   }

   return suri::Element::uidNull_;
}

/**
 * devuelve lista de Uid de Elementos seleccionados.
 * @param[in]	Group
 * @param[in]	IgnoreHidden
 * @return lista de Uid de Elementos seleccionados en caso de que haya Elementos
 * seleccionados.
 * @return lista vacia en caso de que no haya Elementos seleccionados.
 */
const std::list<suri::Element::UidType> LayerList::GetSelectedElementsUid(
      int Group, bool IgnoreHidden) {
   std::list<suri::Element::UidType> uidlist;

   LayerList::ElementListType::const_iterator it = elementList_.begin();
   for (; it != elementList_.end(); ++it) {
      ElementData *pedata = GetElementData(*it);
      if (pedata->selected_
            && (((Group == pedata->group_ && Group != AllGroups)
                  || (pedata->group_ >= AllGroups && Group == AllGroups))
                  || !IgnoreHidden)) {
         uidlist.push_back((*it)->GetUid());
      }
   }

   return uidlist;
}

/**
 * Retorna la cantidad de elementos activos
 * @param[in] IgnoreHidden indica si hay que ignorar los elementos ocultos
 * @return cantidad de elementos activos
 */
int LayerList::GetActiveCount(bool IgnoreHidden) {
   return GetActiveCount(AllGroups, IgnoreHidden);
}

/**
 * Retorna la cantidad de elementos activos
 * @param[in] Group grupo del cual se quieren contar los elementos activos
 * @param[in] IgnoreHidden indica si hay que ignorar los elementos ocultos
 * @return cantidad de elementos activos del grupo
 */
int LayerList::GetActiveCount(int Group, bool IgnoreHidden) {
   int count = 0;
   LayerList::ElementListType::const_iterator it = elementList_.begin();
   for (; it != elementList_.end(); ++it) {
      ElementData *pedata = GetElementData(*it);
      if ((*it)->IsActive()
            && (((Group == pedata->group_ && Group != AllGroups)
                  || (pedata->group_ >= AllGroups && Group == AllGroups))
                  || !IgnoreHidden)) {
         ++count;
      }
   }
   return count;
}

/**
 * Calcula la cantidad de elementos en edicion
 * @return cantidad de elementos en edicion
 */
int LayerList::GetEditedCount() const {
   int count = 0;
   LayerList::ElementListType::const_iterator it = elementList_.begin();
   for (; it != elementList_.end(); ++it) {
      if ((*it)->IsEdited()) {
         ++count;
      }
   }
   return count;
}

/**
 * Permite asignar una logica de activacion de elementos
 * @param[in] pLogic logica de activacion de los elementos
 */
void LayerList::SetActivationLogic(ActivationLogic *pLogic) {
   delete pLogic_;
   pLogic_ = pLogic;
}

/**
 * Retorna un xml con los nodos elemento.
 * @return Devuelve un xml con los nodos elemento de la lista.
 * \attention el wxXmlNode retornado(y su eliminacion) es responsabilidad del invocante
 */
wxXmlNode *LayerList::GetProperties() {
   wxXmlNode *proot = new wxXmlNode(*pXmlProperties_);
   ElementListType::iterator it = elementList_.begin();
   while (it != elementList_.end()) {
      if (GetElementData((*it))->group_ >= AllGroups) {
         wxXmlNode *pnode = proot->GetChildren();
         while (pnode && pnode->GetNext())
            pnode = pnode->GetNext();
#if (wxMAJOR_VERSION == 2) && (wxMINOR_VERSION == 8) && (wxRELEASE_NUMBER <8)
         wxXmlNode *pnewnode = (*it)->GetProperties();
         pnewnode->SetParent(pnode ? pnode->GetParent() : proot);
         pnewnode->SetNext(pnode ? pnode->GetNext() : NULL);
         if ( pnode ) {
            pnode->SetNext(pnewnode);
         }
#else
         proot->InsertChildAfter((*it)->GetProperties(), pnode);
#endif
      }
      it++;
   }
   return proot;
}

/**
 * Obtiene el elemento activo de la lista.
 * @param[in] Index indice del elemento, dentro del conjunto y contando solo los
 * elementos activos del conjunto.
 * @param[in] Group grupo de elementos
 * @param[in] IgnoreHidden ignora los elementos ocultos.
 * @return elemento activo del grupo seleccionado, en la posicion indicada.
 * @return NULL si no encuentra el elemento
 */
Element* LayerList::GetActiveElement(int Index, int Group, bool IgnoreHidden) const {
   if (Index < 0) {
      return NULL;
   }
   ElementListType::const_iterator it = elementList_.begin();
   while (it != elementList_.end()) {
      ElementData *pedata = const_cast<LayerList*>(this)->GetElementData(*it);
      if ((*it)->IsActive() && pedata
            && (((Group == pedata->group_ && Group != AllGroups)
                  || (pedata->group_ >= AllGroups && Group == AllGroups))
                  || !IgnoreHidden)) {
         if (Index == 0) {
            return *it;
         }

         Index--;
      }
      it++;
   }
   return NULL;
}

void LayerList::DisableAll(){
   ElementListType::const_iterator it = elementList_.begin();
     while (it != elementList_.end()) {
        (*it)->Activate(false);
        it++;
     }
}
}  // namespace suri
