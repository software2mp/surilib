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

#ifndef DEFAULTDATASOURCEMANAGER_H_
#define DEFAULTDATASOURCEMANAGER_H_

// Includes Estandar
#include <list>
#include <string>
#include <vector>

// Includes Suri
#include "suri/DatasourceManagerInterface.h"
#include "suri/DatasourceContainerInterface.h"
#include "suri/DatasourceInterface.h"

// Includes App

// Includes Wx
#include "wx/xml/xml.h"

// Defines

// forwards

namespace suri {

/**
 * Datasource Manager por defecto ofrecido por SuriLib para administrar fuentes de datos
 */
class DefaultDatasourceManager : public suri::DatasourceManagerInterface {
public:
   /** Constructor */
   DefaultDatasourceManager();
   /** Destructor */
   virtual ~DefaultDatasourceManager();
   /** Agrega un contenedorfuente de datos y le setea un valor de id */
   virtual bool AddDatasource(DatasourceInterface* pDatasource);
   virtual bool AddDatasourceWithNotification(DatasourceInterface* pDatasource,
		   const suri::SuriObject::UuidType& ViewcontextId,
		   ViewcontextInterface::ViewcontextType ctype = ViewcontextInterface::V2D);
   /** Quita una fuente de datos del manager */
   virtual bool RemoveObject(const SuriObject::UuidType& Id);
   /** Quita una fuente de datos del manager a partir de un id de Element \deprecated */
   virtual bool RemoveDatasourceByElementId(const SuriObject::UuidType& ElementId);
   /** Cambia el orden */
   virtual bool MoveObject(const SuriObject::UuidType& Id,
                            const SuriObject::UuidType& DestinationId);
   /** Mueve un elemento al final */
   virtual bool MoveObjectToEnd(const SuriObject::UuidType& Id);
   /** Cambia el orden de un datasource dentro de la lista a partir del id de element \deprecated */
   virtual bool ChangeOrderFromElementId(const SuriObject::UuidType& ElementId,
                                         const SuriObject::UuidType& ElementDestinationId);
   /** Mueve al final un datasource dentro de la lista a partir del id de element \deprecated */
   virtual bool MoveToEndFromElementId(const SuriObject::UuidType& ElementId);
   /** Devuelve un puntero a una fuente de datos a partir de su id */
   virtual DatasourceInterface* GetDatasource(const SuriObject::UuidType& Id);
   /** Devuelve un puntero a la fuente de datos a partir del id del elemento asociado \deprecated */
   virtual DatasourceInterface *GetDatasourceByElementId(const SuriObject::UuidType& ElementId);
   /** Actualiza un datasource en funcion del nodo xml que se pase */
   virtual void UpdateDatasourceFromWxmlNode(const SuriObject::UuidType& ElementId,
                                             const wxXmlNode *pNodeElement);
   /** Remueve un elemento al contexto activo a partir de un xml \deprecated */
   virtual bool DeleteContextElement(const suri::Element::UidType& ElementUid);
   /** Remueve una lista de elementos en el contexto activo a partir de un xml \deprecated */
   virtual bool DeleteContextElementList(const std::list<suri::Element::UidType>& ElementUid);
   /** Agrega un Datasource que viene de un xml y no hay que hacer update de la vista \deprecated */
   virtual bool AddDatasourceFromXml(DatasourceInterface *pDatasource);
   /** Cambia el nombre de la fuente de datos */
   virtual bool ChangeDatasourceName(const SuriObject::UuidType& DatasourceId,
                                     const std::string &Name);
   /** Cambia el copyright de una fuente de datos */
   virtual bool ChangeDatasourceCopyRight(const SuriObject::UuidType& DatasourceId,
                                          const std::string &CopyRight);
   /** Cambia la descripcion de una fuente de datos */
   virtual bool ChangeDatasourceDescription(const SuriObject::UuidType& DatasourceId,
                                            const std::string &Description);
   /** Cambia el url de la fuente de datos */
   virtual bool ChangeDatasourceUrl(const SuriObject::UuidType& DatasourceId,
                                    const std::string &Url);
   /** Cambia el icono asociado a la fuente de datos \deprecated */
   virtual bool ChangeDatasourceIcon(const SuriObject::UuidType& DatasourceId,
                                     const std::string &Icon);
#ifdef __UNUSED_CODE__
   /** Devuelve la lista de datos como una layerlist \deprecated */
   virtual LayerList* GetDatasourcesAsLayerList();
#endif
   /** Devuelve una lista con los Ids de la fuente de datos */
   virtual std::vector<SuriObject::UuidType> GetOrderedIds() const;
   /** Elimina todos las fuente de datos que contien */
   virtual void Clear();
   /** Bloquea la fuente de datos que corresponde al id que se pasa por parametro **/
   virtual bool BlockDatasource(const SuriObject::UuidType& DatasourceId);
   /** Desbloquea la fuente de datos que corresponde al id que se pasa por parametro **/
   virtual bool UnblockDatasource(const SuriObject::UuidType& DatasourceId);
   /** Bloquea todas las fuentes de datos **/
   virtual void BlockAllDatasources();
   /** Desbloquea todas las fuentes de datos **/
   virtual void UnblockAllDatasource();
   /** Consulta si una fuente de datos se encuentra bloqueada **/
   virtual bool IsDatasourceBlocked(const SuriObject::UuidType& DatasourceId);
   /** Verifica si puede quitar un objeto del manager */
   virtual bool CanRemoveObject(const SuriObject::UuidType& Id);
   /** Devuelve un vector de orden secuencial de los ids que contiene la lista **/
   virtual std::vector<SuriObject::UuidType> GetIds() const;

private:
   /** Constructor de Copia */
   DefaultDatasourceManager(const DefaultDatasourceManager &);
   DatasourceContainerInterface* pDatasourceList_;    /*! DatasourceContainer que administra */
};

} /** namespace suri */

#endif /* DEFAULTDATASOURCEMANAGER_H_ */
