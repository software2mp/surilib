/*! \file DatasourceManagerInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DATASOURCEMANAGERINTERFACE_H_
#define DATASOURCEMANAGERINTERFACE_H_

// Includes Estandar
#include <list>
#include <string>
#include <vector>

// Includes Suri
#include "suri/SuriObject.h"
#include "suri/Subject.h"
#include "suri/Element.h"
#include "DatasourceInterface.h"
#include "suri/SuriObjectManagerInterface.h"
#include "suri/DatasourceContainerInterface.h"
#include "suri/ViewcontextInterface.h"

// Includes App

// Includes Wx
#include "wx/xml/xml.h"

// Defines


namespace suri {

// forwards
class LayerList;

/**
 * Interfaz que brinda la funcionabilidad a implementar para administrar las fuentes de datos
 */
class DatasourceManagerInterface : public SuriObjectManagerInterface {
public:
   /** Constructor */
   DatasourceManagerInterface() {}
   /** Destructor */
   virtual ~DatasourceManagerInterface() {}
   /** Agrega un contenedorfuente de datos y le setea un valor de id */
   virtual bool AddDatasource(DatasourceInterface* pDatasource)=0;
   /** Agrega un contenedorfuente de datos y le setea un valor de id */
   virtual bool AddDatasourceWithNotification(DatasourceInterface* pDatasource,
			const suri::SuriObject::UuidType& ViewcontextId,
			ViewcontextInterface::ViewcontextType ctype = ViewcontextInterface::V2D)=0;
   /** Elimina una fuente de datos a partir de un ElementId \deprecated */
   virtual bool RemoveDatasourceByElementId(const SuriObject::UuidType& ElementId)=0;
   /** Cambia el orden de un datasource dentro de la lista a partir del id de element \deprecated */
   virtual bool ChangeOrderFromElementId(const SuriObject::UuidType& ElementId,
                                         const SuriObject::UuidType& ElementDestinationId)=0;
   /** Mueve al final un datasource dentro de la lista a partir del id de element \deprecated */
   virtual bool MoveToEndFromElementId(const SuriObject::UuidType& ElementId)=0;
   /** Devuelve un puntero a una fuente de datos a partir de su id */
   virtual DatasourceInterface* GetDatasource(const SuriObject::UuidType& Id)=0;
   /** Devuelve un puntero a la fuente de datos a partir del id del elemento asociado \deprecated */
   virtual DatasourceInterface *GetDatasourceByElementId(const SuriObject::UuidType& ElementId)=0;
   /** Actualiza un datasource en funcion del nodo xml que se pase */
   virtual void UpdateDatasourceFromWxmlNode(const SuriObject::UuidType& ElementId,
                                             const wxXmlNode *pNodeElement)=0;
   /** Remueve un elemento al contexto activo a partir de un xml \deprecated */
   virtual bool DeleteContextElement(const suri::Element::UidType& ElementUid)=0;
   /** Remueve una lista de elementos en el contexto activo a partir de un xml \deprecated */
   virtual bool DeleteContextElementList(const std::list<suri::Element::UidType>& ElementUid)=0;
   /** Agrega un Datasource que viene de un xml y no hay que hacer update de la vista \deprecated */
   virtual bool AddDatasourceFromXml(DatasourceInterface *pDatasource)=0;
   /** Cambia el nombre de una fuente de datos */
   virtual bool ChangeDatasourceName(const SuriObject::UuidType& DatasourceId,
                                     const std::string &Name)=0;
   /** Cambia el copyright de una fuente de datos */
   virtual bool ChangeDatasourceCopyRight(const SuriObject::UuidType& DatasourceId,
                                          const std::string &CopyRight)=0;
   /** Cambia la descripcion de una fuente de datos */
   virtual bool ChangeDatasourceDescription(const SuriObject::UuidType& DatasourceId,
                                            const std::string &Description)=0;
   /** Cambia el url de la fuente de datos */
   virtual bool ChangeDatasourceUrl(const SuriObject::UuidType& DatasourceId,
                                    const std::string &Url)=0;
   /** Cambia el icono asociado a la fuente de datos \deprecated */
   virtual bool ChangeDatasourceIcon(const SuriObject::UuidType& DatasourceId,
                                     const std::string &Icon)=0;
   /** Elimina todos las fuente de datos que contien */
   virtual void Clear()=0;
   /** Bloquea la fuente de datos que corresponde al id que se pasa por parametro **/
   virtual bool BlockDatasource(const SuriObject::UuidType& DatasourceId)=0;
   /** Desbloquea la fuente de datos que corresponde al id que se pasa por parametro **/
   virtual bool UnblockDatasource(const SuriObject::UuidType& DatasourceId)=0;
   /** Bloquea todas las fuentes de datos **/
   virtual void BlockAllDatasources()=0;
   /** Desbloquea todas las fuentes de datos **/
   virtual void UnblockAllDatasource()=0;
   /** Consulta si una fuente de datos se encuentra bloqueada **/
   virtual bool IsDatasourceBlocked(const SuriObject::UuidType& DatasourceId)=0;
   /** Verifica si puede quitar un objeto del manager */
   virtual bool CanRemoveObject(const SuriObject::UuidType& Id) = 0;
   /** Devuelve un vector de orden secuencial de los ids que contiene la lista **/
   virtual std::vector<SuriObject::UuidType> GetIds() const=0;

private:
   /** Constructor de Copia */
   DatasourceManagerInterface(const DatasourceManagerInterface&);
};

} /* namespace suri */

#endif /* DATASOURCEMANAGERINTERFACE_H_ */
