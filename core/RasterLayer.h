/*! \file RasterLayer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERLAYER_H_
#define RASTERLAYER_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/LayerInterface.h"
#include "suri/Element.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/***
 *  RasterLayer modela una capa de tipo raster
 */
class RasterLayer : public LayerInterface {
public:
   /** Constructor \deprecated */
   RasterLayer(DatasourceInterface *pDatasource, Element *pElement);
   /** Constructor*/
   explicit RasterLayer(DatasourceInterface *pDatasource);
   /** Destructor */
   virtual ~RasterLayer();
   /** Obtiene la Datasource asociada a la Layer. Trazabilidad entre Datasource Layer */
   virtual SuriObject::UuidType GetAssociatedDatasourceId() const;
   /** Devuelve el estilo de renderizacion del layer */
   virtual void GetRenderizationStyle(const SuriObject::UuidType &LayerId) const;
   /** Devuelve el elemento \deprecated */
   virtual Element* GetElement();
   /** Devuelve la cantidad de bandas del raster */
   virtual int GetBandCount() const;
   /** Devuelve el nombre de una banda */
   virtual bool GetBandName(std::string &BandName, int BandNumber) const;
   /** Compara un Datasource contra un Layer */
   virtual bool Equals(DatasourceInterface &Datasource);
   /** Compara dos tipos diferentes de Layers */
   virtual bool Equals(LayerInterface &Layer);
   /** Compara un Layer contra un Element \deprecated */
   virtual bool Equals(Element &Element);
   /** Oculta */
   virtual void Hide(bool Hide = true);
   /** Verifica si esta oculto */
   virtual bool IsHidden() const;
   /** Setea el nombre de la capa **/
   virtual void SetName(const std::string& Name);
   /** Setea el titulo de la capa **/
   virtual void SetTitle(const std::string& Title);
   /** Configura el viewcontext al cual pertenece **/
   virtual void SetViewcontext(ViewcontextInterface* pViewcontext);
   /** Obtiene la instancia de fuente de datos asociada a la capa **/
   virtual DatasourceInterface* FetchAssociatedDatasource();
   /** Devuelve el viewcontext */
   virtual ViewcontextInterface* GetViewcontext();

private:
   /** Constructor de copia */
   RasterLayer(const RasterLayer &);
   /** Referencia al elemento que encapsula */
   DatasourceInterface *pDatasource_;
   /** Referencia al elemento \deprecated */
   Element *pElement_;
   /** Variable Ocultacion */
   bool hidden_;
   /** nombre y titulo de la capa **/
   std::string name_, title_;
   /** Viewcontext al que pertenece **/
   ViewcontextInterface* pViewcontext_;
};
} /** namespace suri */

#endif /* RASTERLAYER_H_ */
