/*! \file LayerInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LAYERINTERFACE_H_
#define LAYERINTERFACE_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "HiddableInterface.h"
#include "SuriObject.h"
#include "Element.h"
#include "DatasourceInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

// forwards
class ViewcontextInterface;

/**
 * Define la interfaz de las capas
 */
class LayerInterface : public suri::SuriObject, public suri::HiddableInterface {
public:
   /** Constructor **/
   LayerInterface() : blocked_(false) {}
   /** Destructor */
   virtual ~LayerInterface() {}
   /** Obtiene Datasource asociada al Layer. Trazabilidad entre Datasource-Layer */
   virtual SuriObject::UuidType GetAssociatedDatasourceId() const=0;
   /** Devuelve el estilo de renderizacion del layer */
   virtual void GetRenderizationStyle(const SuriObject::UuidType &LayerId) const=0;
   /** Devuelve el elemento \deprecated */
   virtual Element* GetElement()=0;
   /** Compara un Datasource contra un Layer */
   virtual bool Equals(DatasourceInterface &Datasource)=0;
   /** Compara dos tipos diferentes de Layers */
   virtual bool Equals(LayerInterface &Layer)=0;
   /** Compara un Layer contra un Element */
   virtual bool Equals(Element &Element)=0;
   /** Oculta el Layer */
   virtual void Hide(bool Hide = true)=0;
   /** Verifica si esta oculto el Layer */
   virtual bool IsHidden() const=0;
   /** Setea el nombre de la capa **/
   virtual void SetName(const std::string& Name)=0;
   /** Setea el titulo de la capa **/
   virtual void SetTitle(const std::string& Title)=0;
   /** Configura el viewcontext al cual pertenece **/
   virtual void SetViewcontext(ViewcontextInterface* pViewcontext)=0;
   /** Obtiene la instancia de fuente de datos asociada a la capa **/
   virtual DatasourceInterface* FetchAssociatedDatasource()=0;
   /** Indica si la capa se encuentra bloqueada **/
   bool IsBlocked() const {
      return blocked_;
   }
   /** Bloquea la capa **/
   bool Block() {
      if (blocked_)
         return false;
      blocked_ = true;
      return true;
   }
   /** Bloquea la capa **/
   bool Unblock() {
      if (!blocked_)
         return false;
      blocked_ = false;
      return true;
   }

private:
   /** bool que indica si la capa se encuentra bloqueada **/
   bool blocked_;
};

} /** namespace suri */

#endif /* LAYERINTERFACE_H_ */
