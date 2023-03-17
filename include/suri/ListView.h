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

#ifndef LISTVIEW_H_
#define LISTVIEW_H_

// Include suri
#include "CallbackView.h"
#include "LayerList.h"

/** namespace suri */
namespace suri {
/** Obserba la LayerList y llama a metodo Callback */
/**
 * Llama los metodos Callback que recibe en constructor en caso de cambio en
 * la lista
 * Ademas se ser un view permite consultar los cambios en la lista.
 * \note especializa la clase CallbackView para tratar con la LayerList
 */
template<class CallbackClass>
class ListView : public CallbackView<CallbackClass> {
   /** Ctor. de Copia. */
   ListView(const ListView &ListView);

public:
   /** rehago el typedef */
   typedef typename CallbackView<CallbackClass>::pCallbackMethod pCallbackMethod;
   /** ctor */
   ListView(CallbackClass *pObject, pCallbackMethod pUpdateCallbackMethod,
            pCallbackMethod pDeleteCallbackMethod = NULL);
   /** dtor */
   virtual ~ListView();
// ----------------------------- CALLBACKVIEWER -----------------------------
   /** llama al metodo callback registrado */
   virtual void Update(Model *pModel);
// ----------------------------- CONSULTA LISTA -----------------------------
   /** Obtiene la lista */
   LayerList *GetList() const;
   /** Indica si se agrego un elemento */
   bool ElementAdded() const;
   /** Devuelve los elementos agregados */
   LayerList::ElementListType GetNewElements();
   /** Indica si se elimino un elemento */
   bool ElementDeleted() const;
   /** Indica si se movio un elemento */
   bool ElementMoved() const;
protected:
   /** resetea las propiedades */
   void ResetStatus();
private:
   int lastCount_; /*! Cuenta los elementos */
   int lastActiveCount_; /*! Cuenta los elementos activos */
   int lastSelectedCount_; /*! Cuenta los elementos seleccionados */
   bool elementAdded_; /*! Flag que indica que se agrego elemento a la lista */
   bool elementDeleted_; /*! Flag que indica que se borro elemento de la lista */
   bool elementMoved_; /*! Flag que indica que se movio elemento en la lista */
   LayerList::ElementListType elementList_; /*! Lista testigo para ver que */
   /* modificaciones sucedieron */
};

/**
 * Constructor
 * @param[in] pObject objeto que se registra al callback
 * @param[in] pUpdateCallbackMethod metodo que llama cuando se agrega un elemento
 * @param[in] pDeleteCallbackMethod metodo que se llama cuando se elimina un elemento
 */
template<class CallbackClass>
ListView<CallbackClass>::ListView(CallbackClass *pObject,
                                  pCallbackMethod pUpdateCallbackMethod,
                                  pCallbackMethod pDeleteCallbackMethod) :
      CallbackView<CallbackClass>(pObject, pUpdateCallbackMethod,
                                  pDeleteCallbackMethod),
      lastCount_(0), elementAdded_(false), elementDeleted_(false), elementMoved_(false) {
}

/** dtor */
template<class CallbackClass>
ListView<CallbackClass>::~ListView() {
}

// ----------------------------- CALLBACKVIEWER -----------------------------
/**
 * Llama al metodo callback registrado
 * @param[in] pModel model
 * \todo Mejorar deteccion de cambios
 */
template<class CallbackClass>
void ListView<CallbackClass>::Update(Model *pModel) {
   LayerList *plist = GetList();
   /** quiere decir que recibe un update de algo que no conoce */
   if (plist != pModel || !plist->HasChanged()) {
      return;
   }

   // Busco si se modifico el orden de los elementos
   int nextindex = 0;
   elementMoved_ = false;
   LayerList::ElementListType::iterator oldit = elementList_.begin();
   LayerList::ElementListType::iterator newit = GetList()->GetElementIterator();
   int deletedelements = 0;
   for (; oldit != elementList_.end() && !elementMoved_; oldit++) {
      if (*newit != *oldit) {
         // Si no son iguales lo busco en la lista
         int index = 0;
         bool found = false;
         LayerList::ElementListType::iterator auxit = GetList()->GetElementIterator();
         for (; auxit != GetList()->GetElementIterator(false); auxit++, index++)
            if (*auxit == *oldit) {
               found = true;
            }

         if (found) {
            if (nextindex > index) {
               // Si lo encontro y esta antes que el elemento actual se
               // que el elemento se desplazo
               elementMoved_ = true;
            } else {
               // En caso que halla elementos nuevos.
               nextindex = index + 1;
            }

            // Supongo que para la mayoria de los casos el orden se mantiene
            newit = auxit;
            newit++;
         } else {
            deletedelements++;
         }
      } else {
         newit++;
         nextindex++;
      }
   }

   // Verifico si se elimino algun elementos
   elementDeleted_ = false;
   if (deletedelements > 0) {
      elementDeleted_ = true;
   }

   // Verifico si se agregaron elementos
   int newcount = plist->GetElementCount(false);
   int oldcount = elementList_.size();
   elementAdded_ = false;
   if (newcount > oldcount - deletedelements) {
      elementAdded_ = true;
   }

   CallbackView<CallbackClass>::Update(pModel);
   ResetStatus();
}

// ----------------------------- CONSULTA LISTA -----------------------------
/**
 * Obtiene la lista
 * @return Lista de elementos
 */
template<class CallbackClass>
LayerList *ListView<CallbackClass>::GetList() const {
   return View::GetModel<LayerList>();
}

/**
 * Indica si se agrego un elemento
 * @return bool que indica si se agrego un elemento
 */
template<class CallbackClass>
bool ListView<CallbackClass>::ElementAdded() const {
   return elementAdded_;
}

/**
 * Devuelve los elementos agregados
 * @return lista con los elementos agregados
 */
template<class CallbackClass>
LayerList::ElementListType ListView<CallbackClass>::GetNewElements() {
   LayerList::ElementListType newelements;
   LayerList::ElementListType::iterator it = GetList()->GetElementIterator();
   for (; it != GetList()->GetElementIterator(false); it++) {
      LayerList::ElementListType::iterator nit = elementList_.begin();
      bool found = false;
      for (; nit != elementList_.end(); ++nit) {
         if (*nit == *it) {
            found = true;
         }
      }
      if (!found) {
         newelements.push_back(*it);
      }
   }
   return newelements;
}

/**
 * Indica si se elimino un elemento
 * @return bool que indica si se elimino un elemento
 */
template<class CallbackClass>
bool ListView<CallbackClass>::ElementDeleted() const {
   return elementDeleted_;
}

/**
 * Indica si se movio un elemento
 * @return bool que indica si se movio un elemento
 */
template<class CallbackClass>
bool ListView<CallbackClass>::ElementMoved() const {
   return elementMoved_;
}

/** resetea las propiedades */
template<class CallbackClass>
void ListView<CallbackClass>::ResetStatus() {
   elementAdded_ = false;
   elementDeleted_ = false;
   elementMoved_ = false;
   // cargo los elementos de la lista
   elementList_.clear();
   LayerList::ElementListType::iterator it = GetList()->GetElementIterator();
   for (; it != GetList()->GetElementIterator(false); it++)
      elementList_.push_back(*it);
   LayerList *plist = GetList();
   /** quiere decir que recibe un update de algo que no conoce */
   if (!plist) {
      return;
   }
   lastCount_ = plist->GetElementCount(false);
   lastActiveCount_ = plist->GetActiveCount(false);
   lastSelectedCount_ = plist->GetSelectedCount(false);
}
}

#endif /*LISTVIEW_*/

