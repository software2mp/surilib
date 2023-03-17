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

// Includes Estandar
#include <string>
#include <list>
#include <vector>

// Includes Suri
#include "DefaultDatasourceManager.h"
#include "DefaultDatasourceList.h"
#include "DatasourceOrderChangeNotification.h"
#include "DatasourceAddtitionNotification.h"
#include "ObjectAditionGroupNotification.h"
#include "DatasourceRemovalNotification.h"
#include "SuriObjectRemoveGroupNotification.h"
#include "SuriObjectOrderChangeGroupNotification.h"
#include "DatasourcePropertiesChangedNotification.h"
#include "suri/DatasourceInterface.h"
#include "suri/NotificationInterface.h"
#include "suri/DatasourceContainerInterface.h"
#include "suri/DatasourceBlockedNotification.h"
#include "suri/DatasourceUnblockedNotification.h"
#include "suri/LayerStateChangeNotification.h"

// Includes App

// Includes Wx
#include "wx/xml/xml.h"

// Defines

// forwards

namespace suri {

/** Constructor **/
DefaultDatasourceManager::DefaultDatasourceManager() :
      pDatasourceList_(new DefaultDatasourceList()) {
}

/** Destructor */
DefaultDatasourceManager::~DefaultDatasourceManager() {
   delete pDatasourceList_;
}

/**
 *  Elimina todos las fuente de datos que contien
 */
void DefaultDatasourceManager::Clear() {
   if (pDatasourceList_)
      delete pDatasourceList_;
   pDatasourceList_ = new DefaultDatasourceList();
}

/**
 * Agrega un contenedor de datasource
 * @param[in] pDatasource
 * @return false
 */
bool DefaultDatasourceManager::AddDatasource(DatasourceInterface* pDatasource) {
   if (!pDatasource) {
      return false;
   }
   bool success = true;
   DatasourceInterface* pCurrentDS = pDatasource;
   while (pCurrentDS != NULL && success) {
      DatasourceInterface* pNextDS = pCurrentDS->GetNextDatasource();
      // TODO matiaslafroce: "IsValid" no es un nombre feliz, quizas algo como "skip" es mejor
      // Utilizo esa funcion por si tengo un datasource que no quiero agregar
      // pero no quiero que me rompa el pipeline de Datasources (como los vectores multigeometria)
      if (pCurrentDS->IsValid()) {
         success = pDatasourceList_->AddDatasource(pCurrentDS);
         if (success) {
            DatasourceAddtitionNotification notification(pCurrentDS);
            Notify(&notification);
            ObjectAditionGroupNotification groupnotification(pCurrentDS->GetId());
            Notify(&groupnotification);
            pCurrentDS->SetNextDatasource(NULL);
         } else {
            DatasourceInterface::Destroy(pCurrentDS);
         }
      } else {
         DatasourceInterface::Destroy(pCurrentDS);
      }
      pCurrentDS = pNextDS;
   }
   return success;
}

/**
 * Agrega un contenedor de datasource
 * @param[in] pDatasource
 * @return false
 */
bool DefaultDatasourceManager::AddDatasourceWithNotification(
      DatasourceInterface* pDatasource,
      const suri::SuriObject::UuidType& ViewcontextId,
      ViewcontextInterface::ViewcontextType CType) {
   if (!pDatasource) {
      return false;
   }
   bool success = false;
   if (pDatasource->GetNextDatasource() == NULL) {
      DatasourceInterface* pexistds = pDatasourceList_->GetDatasourceByElementId(
            pDatasource->GetElement()->GetUid());
      SuriObject::UuidType predecessor = SuriObject::NullUuid;
#ifdef __UNUSED_CODE__
      DefaultDatasourceList* pdslist = dynamic_cast<DefaultDatasourceList*>(pDatasourceList_);
      if (pdslist) {
         DefaultDatasourceList::DatasourceList::const_iterator it = pdslist->GetIterator();
         for (; pdslist->HasNextDatasource(it) ; ++it) {
            if ((*it)->GetClassId().compare(pDatasource->GetClassId()) == 0)
               predecessor = (*it)->GetId();
         }
      }
#endif
      if (pDatasourceList_->AddDatasource(pDatasource)
            || pexistds) {
         DatasourceInterface* pdsnotif = pexistds ? pexistds : pDatasource;
         DatasourceAddtitionNotification notification(pdsnotif, ViewcontextId);
         Notify(&notification);
         ObjectAditionGroupNotification groupnotification(pDatasource->GetId(), predecessor, CType);
         Notify(&groupnotification);
         success = notification.GetApplyResult();
      }
   } else {
      DatasourceInterface* pdatasource = pDatasource;

      while (pdatasource != NULL) {
         if (pDatasourceList_->AddDatasource(pdatasource)
               || pDatasourceList_->Exists(pdatasource)) {
            DatasourceAddtitionNotification notification(pdatasource);
            Notify(&notification);
            ObjectAditionGroupNotification groupnotification(
                  pdatasource->GetId());
            Notify(&groupnotification);
            success = true;
         }
         DatasourceInterface* previous = pdatasource;
         pdatasource = pdatasource->GetNextDatasource();
         previous->SetNextDatasource(NULL);
      }
   }
   return success;
}

/**
 * Agrega un Datasource que viene de un xml
 * \deprecated
 * @param[in] pDatasource Datasource a agregar
 * @return false
 */
bool DefaultDatasourceManager::AddDatasourceFromXml(DatasourceInterface* pDatasource) {
   if (!pDatasource) {
      return false;
   }
   if (pDatasourceList_->AddDatasource(pDatasource)) {
      DatasourceAddtitionNotification notification(pDatasource);
      Notify(&notification);
      ObjectAditionGroupNotification groupnotification(pDatasource->GetId());
      Notify(&groupnotification);
   }
   return true;
}

/** Quita una fuente de datos del manager
 *  @param[in] Id Id del datasource a eliminar
 *  @return bool true en caso de poder eliminar el elemento, false en caso contrario
 */
bool DefaultDatasourceManager::RemoveObject(const SuriObject::UuidType& Id) {
   DatasourceInterface* pdatasource = pDatasourceList_->GetDatasource(Id);
   if (pdatasource == NULL) {
      return false;
   }
   if (!CanRemoveObject(Id))
      return false;
   if (pDatasourceList_->RemoveDatasource(Id)) {
      DatasourceRemovalNotification notification(pdatasource);
      Notify(&notification);
      SuriObjectRemoveGroupNotification groupnotification(pdatasource->GetId());
      Notify(&groupnotification);
      DatasourceInterface::Destroy(pdatasource);
   }
   return true;
}

/**
 * Quita una fuente de datos del manager a partir de un Id del Element asociado
 * \deprecated
 * @param[in] ElementId Id del elemento asociado al datasource a remover
 * @return bool true en caso de poder elminar la fuente de datos, false en caso contrario
 */
bool DefaultDatasourceManager::RemoveDatasourceByElementId(const SuriObject::UuidType& ElementId) {
   DatasourceInterface* pdatasource = pDatasourceList_->GetDatasourceByElementId(ElementId);
   if (pdatasource == NULL) {
      return false;
   }
   if (!CanRemoveObject(pdatasource->GetId()))
         return false;
   if (pDatasourceList_->RemoveDatasource(pdatasource->GetId())) {
      DatasourceRemovalNotification notification(pdatasource);
      Notify(&notification);
      SuriObjectRemoveGroupNotification groupnotification(pdatasource->GetId());
      Notify(&groupnotification);
      DatasourceInterface::Destroy(pdatasource);
   }
   return true;
}

/**
 * Cambia el orden de un datasource dentro de la lista a partir del id de Datasource
 * @param[in] Id Id del Datasource que se desea mover dentro de la lista
 * @param[in] DestinationId Id del Datasource que se encuentra en la posicion a insertar
 * @return true en caso de poder mover el orden, false en otro caso
 */
bool DefaultDatasourceManager::MoveObject(const SuriObject::UuidType& Id,
                                  const SuriObject::UuidType& DestinationId) {
   DatasourceInterface* pdatasource = pDatasourceList_->GetDatasource(Id);
   DatasourceInterface* patasourcedestination = pDatasourceList_->GetDatasource(DestinationId);
   if (pdatasource == NULL || patasourcedestination == NULL)
      return false;
   if (!pDatasourceList_->ChangeOrder(Id, DestinationId))
      return false;
   DatasourceOrderChangeNotification notification(pdatasource, patasourcedestination);
   Notify(&notification);
   SuriObjectOrderChangeGroupNotification groupnotification(pdatasource->GetId(),
                                                       patasourcedestination->GetId());
   Notify(&groupnotification);
   return true;
}

/**
 * Mueve un elemento al final
 * @param[in] Id identificador del elemento a mover. Debe existir en manager.
 * @return bool que indica si fue exitoso
 */
bool DefaultDatasourceManager::MoveObjectToEnd(const SuriObject::UuidType& Id) {
   DatasourceInterface* pdatasource = pDatasourceList_->GetDatasource(Id);
   if (!pDatasourceList_->MoveToEnd(pdatasource))
      return false;
   DatasourceOrderChangeNotification notification(pdatasource);
   Notify(&notification);
   SuriObjectOrderChangeGroupNotification groupnotification(pdatasource->GetId());
   Notify(&groupnotification);
   return true;
}

/**
 * Cambia el orden de un datasource dentro de la lista a partir del Id del Element asociado
 * \deprecated
 * @param[in] ElementId Id del elemento que se desea mover dentro de la lista
 * @param[in] ElementDestinationId Id del elemento que se encuentra en la posicion a insertar
 * @return bool true en caso de poder mover el orden, false en otro caso
 */
bool DefaultDatasourceManager::ChangeOrderFromElementId(
      const SuriObject::UuidType& ElementId, const SuriObject::UuidType& ElementDestinationId) {
   DatasourceInterface* pdatasource = pDatasourceList_->GetDatasourceByElementId(ElementId);
   DatasourceInterface* pdatasourcedestination = pDatasourceList_->GetDatasourceByElementId(
         ElementDestinationId);
   if (pdatasource == NULL || pdatasourcedestination == NULL) {
      return false;
   }
   if (!pDatasourceList_->ChangeOrder(pdatasource->GetId(), pdatasourcedestination->GetId()))
      return false;
   DatasourceOrderChangeNotification notification(pdatasource, pdatasourcedestination);
   Notify(&notification);
   SuriObjectOrderChangeGroupNotification groupnotification(pdatasource->GetId(),
                                                   pdatasourcedestination->GetId());
   Notify(&groupnotification);
   return true;
}

/**
 * Mueve una fuente de datos al final de la lista a partir de un Id del Element asociado
 * @param[in] ElementId Id del elemento que se desea mover
 * @return bool true en caso de poder mover al final, false en otro caso
 * \deprecated
 */
bool DefaultDatasourceManager::MoveToEndFromElementId(const SuriObject::UuidType& ElementId) {
   DatasourceInterface* pdatasource = GetDatasourceByElementId(ElementId);
   if (!pDatasourceList_->MoveToEnd(pdatasource))
      return false;
   DatasourceOrderChangeNotification notification(pdatasource);
   Notify(&notification);
   SuriObjectOrderChangeGroupNotification groupnotification(pdatasource->GetId());
   Notify(&groupnotification);
   return true;
}

/**
 * Devuelve un puntero a una fuente de datos a partir de su id
 *  @param[in] Id id de la fuente de datos
 *  @return false
 */
DatasourceInterface* DefaultDatasourceManager::GetDatasource(const SuriObject::UuidType& Id) {
   return pDatasourceList_->GetDatasource(Id);
}

/**
 * Devuelve un puntero a la fuente de datos a partir del id del elemento asociado
 * \deprecated
 */
DatasourceInterface *DefaultDatasourceManager::GetDatasourceByElementId(
      const SuriObject::UuidType& ElementId) {
   return pDatasourceList_->GetDatasourceByElementId(ElementId);
}

/**
 * Actualiza un datasource en funcion del nodo xml que se pase
 * @param[in] ElementId
 * @param[in] pNodeElement
 */
void DefaultDatasourceManager::UpdateDatasourceFromWxmlNode(const SuriObject::UuidType& ElementId,
                                                            const wxXmlNode *pNodeElement) {
   //Element *pelement = pDatasourceList_->GetDatasourceByElementId(ElementId)->GetElement();
   DatasourceInterface* pdatasource = pDatasourceList_->GetDatasourceByElementId(ElementId);
   Element *pelement = NULL;

   if (pdatasource)
      pelement = pdatasource->GetElement();
   else
      return;
   if (pelement)
      pelement->AddNode(NULL, new wxXmlNode(*pNodeElement), true);
}

/**
 * Callback del ElementManager
 * \deprecated
 */
bool DefaultDatasourceManager::DeleteContextElement(const suri::Element::UidType& ElementUid) {
   return RemoveDatasourceByElementId(ElementUid);
}

/**
 * Callback del ElementManager
 * \deprecated
 */
bool DefaultDatasourceManager::DeleteContextElementList(
      const std::list<Element::UidType>& ElementUid) {
   std::list<Element::UidType>::const_iterator it = ElementUid.begin();
   bool ok = true;
   for (; it != ElementUid.end(); ++it) {
      if (!RemoveDatasourceByElementId(*it)) {
         ok = false;
      }
   }
   return ok;
}

/** Cambia el nombre de la fuente de datos
 * @param[in] DatasourceId Id de la fuente de datos
 * @param[in] Name nombre que se desea asignar a la fuente de datos
 * @return false en caso de poder cambiar el nombre de la fuente de datos
 * @return true en caso de poder cambiar el nombre de la fuente de datos
 */
bool DefaultDatasourceManager::ChangeDatasourceName(const SuriObject::UuidType& DatasourceId,
                                                    const std::string& Name) {
   DatasourceInterface* pdatasource = GetDatasource(DatasourceId);
   if (pdatasource->SetName(Name)) {
      DatasourcePropertiesChangedNotification notification(
            pdatasource, Name, DatasourcePropertiesChangedNotification::Name);
      Notify(&notification);
   } else {
      return false;
   }
   return true;
}

/** Cambia el copyright de una fuente de datos
 * @param[in] DatasourceId Id de la fuente de datos
 * @param[in] CopyRight copyright que se desea asignar a la fuente de datos
 * @return false en caso de no poder cambiar el copyright de la fuente de datos
 * @return true en caso de poder cambiar el copyright de la fuente de datos
 */
bool DefaultDatasourceManager::ChangeDatasourceCopyRight(const SuriObject::UuidType& DatasourceId,
                                                         const std::string &CopyRight) {
   DatasourceInterface* pdatasource = GetDatasource(DatasourceId);
   if (pdatasource->SetCopyRight(CopyRight)) {
      DatasourcePropertiesChangedNotification notification(
            pdatasource, CopyRight, DatasourcePropertiesChangedNotification::Copyright);
      Notify(&notification);
   } else {
      return false;
   }
   return true;
}

/** Cambia la descripcion de una fuente de datos
 * @param[in] DatasourceId Id de la fuente de datos
 * @param[in] Description descripcion que se desea asignar a la fuente de datos
 * @return false en caso de no poder cambiar la descripcion de la fuente de datos
 * @return true en caso de poder cambiar la descripcion de la fuente datos
 */
bool DefaultDatasourceManager::ChangeDatasourceDescription(const SuriObject::UuidType& DatasourceId,
                                                           const std::string &Description) {
   DatasourceInterface* pdatasource = GetDatasource(DatasourceId);
   if (pdatasource->SetDescription(Description)) {
      DatasourcePropertiesChangedNotification notification(
            pdatasource, Description, DatasourcePropertiesChangedNotification::Description);
      Notify(&notification);
   } else {
      return false;
   }
   return true;
}

/** Cambia el url de la fuente de datos
 * @param[in] DatasourceId Id de la fuente de datos
 * @param[in] Url url que se desea asignar a la fuente de datos
 * @return false en caso de no poder cambiar la url de la fuente de datos
 * @return true en caso de poder cambiar la url de la fuente datos
 */
bool DefaultDatasourceManager::ChangeDatasourceUrl(const SuriObject::UuidType& DatasourceId,
                                                   const std::string &Url) {
   DatasourceInterface* pdatasource = GetDatasource(DatasourceId);
   if (pdatasource->SetUrl(Url)) {
      DatasourcePropertiesChangedNotification notification(
            pdatasource, Url, DatasourcePropertiesChangedNotification::Url);
      Notify(&notification);
   } else {
      return false;
   }
   return true;
}

/** Cambia el icono asociado a la fuente de datos \deprecated
 * @param[in] DatasourceId Id de la fuente de datos
 * @param[in] Icon icono que se desea asignar a la fuente de datos
 * @return false en caso de no poder cambiar el icono de la fuente de datos
 * @return true en caso de poder cambiar el icono de la fuente datos
 */
bool DefaultDatasourceManager::ChangeDatasourceIcon(const SuriObject::UuidType& DatasourceId,
                                                    const std::string &Icon) {
   DatasourceInterface* pdatasource = GetDatasource(DatasourceId);
   if (pdatasource->SetIcon(Icon)) {
      DatasourcePropertiesChangedNotification notification(
            pdatasource, Icon, DatasourcePropertiesChangedNotification::Icon);
      Notify(&notification);
   } else {
      return false;
   }
   return true;
}

#ifdef __UNUSED_CODE__
/** Devuelve la lista de datos como una layerlist \deprecated**/
LayerList* DefaultDatasourceManager::GetDatasourcesAsLayerList() {
   return pDatasourceList_->GetLayerList();
}
#endif
/**
 * Devuelve una lista con los Ids de la fuente de datos
 * @return vector con ids de la fuente de datos
 */
std::vector<SuriObject::UuidType> DefaultDatasourceManager::GetOrderedIds() const {
   return pDatasourceList_->GetDatasourceIds();
}

/** Bloquea la fuente de datos que corresponde al id que se pasa por parametro **/
bool DefaultDatasourceManager::BlockDatasource(const SuriObject::UuidType& DatasourceId) {
   DatasourceInterface* pdatasource = GetDatasource(DatasourceId);
   if (!pdatasource || pdatasource->IsBlocked()) {
      return false;
   }
   pdatasource->Block();
   DatasourceBlockedNotification notif = DatasourceBlockedNotification(
         pdatasource->GetId());
   Notify(&notif);
   LayerStateChangeNotification changenotif =
         LayerStateChangeNotification();
   Notify(&changenotif);
   return true;
}

/** Desbloquea la fuente de datos que corresponde al id que se pasa por parametro **/
bool DefaultDatasourceManager::UnblockDatasource(
      const SuriObject::UuidType& DatasourceId) {
   DatasourceInterface* pdatasource = GetDatasource(DatasourceId);
   if (!pdatasource || !pdatasource->IsBlocked()) {
      return false;
   }
   pdatasource->Unblock();
   DatasourceUnblockedNotification notif = DatasourceUnblockedNotification(
         pdatasource->GetId());
   Notify(&notif);
   LayerStateChangeNotification changenotif =
         LayerStateChangeNotification();
   Notify(&changenotif);
   return true;
}

/** Bloquea todas las fuentes de datos **/
void DefaultDatasourceManager::BlockAllDatasources() {
   DatasourceContainerInterface::UuidListType list =
         this->pDatasourceList_->GetDatasourceList();
   DatasourceContainerInterface::UuidListType::iterator it = list.begin();
   for (; it != list.end(); ++it) {
      BlockDatasource(*it);
   }
}

/** Desbloquea todas las fuentes de datos **/
void DefaultDatasourceManager::UnblockAllDatasource() {
   DatasourceContainerInterface::UuidListType list =
         this->pDatasourceList_->GetDatasourceList();
   DatasourceContainerInterface::UuidListType::iterator it = list.begin();
   for (; it != list.end(); ++it) {
      UnblockDatasource(*it);
   }
}

/** Consulta si una fuente de datos se encuentra bloqueada **/
bool DefaultDatasourceManager::IsDatasourceBlocked(
      const SuriObject::UuidType& DatasourceId) {
   DatasourceInterface* pdatasource = GetDatasource(DatasourceId);
   if (!pdatasource)
      return false;
   return pdatasource->IsBlocked();
}

/** Devuelve un vector de orden secuencial de los ids que contiene la lista **/
std::vector<SuriObject::UuidType> DefaultDatasourceManager::GetIds() const {
   return pDatasourceList_->GetDatasourceIds();
}

/** Verifica si puede quitar un objeto del manager */
bool DefaultDatasourceManager::CanRemoveObject(const SuriObject::UuidType& Id) {
   return !IsDatasourceBlocked(Id);
}
} /** namespace suri */
