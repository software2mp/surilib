/*! \file AnotationLayer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ANOTATIONLAYER_H_
#define ANOTATIONLAYER_H_

// Includes Estandar

// Includes Suri
#include "VectorLayer.h"
#include "suri/Element.h"
#include "suri/DatasourceInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * AnotationLayer
 */
class AnotationLayer : public suri::VectorLayer {
public:
   /** Constructor */
   explicit AnotationLayer(DatasourceInterface *pDatasource);
   /** Constructor \deprecated */
   AnotationLayer(DatasourceInterface *pDatasource, Element *pElement);
   /** Destructor */
   virtual ~AnotationLayer();
   /** Obtiene el id del datasource asociado */
   virtual SuriObject::UuidType GetAssociatedDatasourceId() const;
   /** Devuelve el estilo de renderizacion del layer */
   virtual void GetRenderizationStyle(const SuriObject::UuidType &LayerId) const;
   /** Devuelve el elemento \deprecated */
   virtual Element* GetElement();
   /** Compara un Datasource contra un Layer*/
   virtual bool Equals(DatasourceInterface &Datasource);
   /** Compara dos tipos diferentes de Layers */
   virtual bool Equals(LayerInterface &Layer);
   /** Compara un Layer contra un Element */
   virtual bool Equals(Element &Element);

private:
   /** Constructor de Copia */
   AnotationLayer(const AnotationLayer&);
   /** Puntero al datasource */
   DatasourceInterface *pDatasource_;
   /** Puntero al elemento */
   Element *pElement_;
};

} /** namespace suri */

#endif /* ANOTATIONLAYER_H_ */
