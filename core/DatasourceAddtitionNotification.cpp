/*! \file DatasourceAddtitionNotification.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar

// Includes Suri
#include "DatasourceAddtitionNotification.h"
#include "suri/ObserverInterface.h"
#include "DefaultViewcontext.h"
#include "suri/LayerFactory.h"
#include "suri/LayerInterface.h"
#include "suri/DatasourceInterface.h"
#include "suri/TerrainElement.h"
#include "SpatialReference.h"
#include "suri/LibraryManager.h"
#include "suri/LibraryManagerFactory.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 * @param[in] pDatasource referencia a la fuente de datos que se agrego
 * @param[in] ViewcontextId identificador del contexto
 * @param[in] Update indica si el observer tiene que actualizar luego de agregar la capa.
 *
 */
DatasourceAddtitionNotification::DatasourceAddtitionNotification(
      DatasourceInterface* pDatasource, const suri::SuriObject::UuidType& ViewcontextId,
      bool Update): pDatasource_(pDatasource), update_(Update), viewcontextId_(ViewcontextId),
      applyOk_(false) {}

/**
 * Destructor
 */
DatasourceAddtitionNotification::~DatasourceAddtitionNotification() {}

/**
 * Metodo que ejecuta una accion sobre el observador a ser notificado cuando
 * la capa es compatible y el id es el mismo que se configuro en la notificacion
 * @param[in] pObserver Observador que escucha la notificacion
 * @return true en caso de haber notificado correctamente, false en otro caso
 */
bool DatasourceAddtitionNotification::ApplyNotification(ObserverInterface* pObserver) {
	// Es necesario realizar el casteo hace un viewcontext, ya que es necesario
	// aplicar la notificacion sobre una instancia de viewcontext y no sobre cualquier
	// obeserver
	ViewcontextInterface* pviewcontext = dynamic_cast<ViewcontextInterface*>(pObserver);
	LibraryManager* pmanager = dynamic_cast<LibraryManager*>(pObserver);
	LayerInterface* player = NULL;
	// Solo se aplica la notificacion a clases de tipo Viewcontext
	if (!pviewcontext && !pmanager)
	   return true;
   if (pviewcontext) {
      player = LayerFactory::CreateLayer(pDatasource_);
      bool sameviewcontext = (pviewcontext->GetViewcontextId().compare(viewcontextId_)
            == 0) || viewcontextId_.compare(SuriObject::NullUuid) == 0;
      if (player && pviewcontext->IsCompatibleLayerType(player) && sameviewcontext) {
         applyOk_ = pviewcontext->AddLayer(player, update_);
      } else {
         if (pviewcontext->GetViewcontextId().compare(viewcontextId_) == 0)
            applyOk_ = false;
         delete player;
      }
   } else {
      std::string srwkt = pDatasource_->GetSpatialReference();
      std::string authid = SpatialReference::GetAuthorityId(srwkt);
      LM_ID_TYPE rnumber = pmanager->RegisterClient(LibraryManager::ALL);
      LibraryManager::AccessKey key = pmanager->GenerateAccessKey(rnumber);
      const Library* plib = pmanager->GetLibraryByCode(
            LibraryManagerFactory::SpatialReferenceLibraryCode, key);
      pmanager->AddItemToFavorites(plib->GetId(), authid);
   }
	return applyOk_;
}

/** Devuelve el resultado de aplicar la notificacion **/
bool DatasourceAddtitionNotification::GetApplyResult() const {
   return applyOk_;
}
} /** namespace suri */
