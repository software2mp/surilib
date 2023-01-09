/*! \file EnhancementSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ENHANCEMENTSELECTIONPART_H_
#define ENHANCEMENTSELECTIONPART_H_

// Includes standar
#include <map>
#include <list>

// Includes suri
#include "suri/Part.h"
#include "suri/RasterElement.h"
#include "LutRenderer.h"
#include "suri/StatisticsFunctions.h"
#include "Enhancement.h"
#include "suri/LayerList.h"
#include "LutSelectionPart.h"
#include "SRDStatistics.h"
#include "SRDHistogram.h"

/** namespace suri */
namespace suri {

// Forwards
class RasterLayer;

/** Clase que permite seleccionar un realce, crearlo y aplicarlo a una imagen. */
/**
 * Part que recibe un elemento raster y permite seleccionar de una lista, un
 * realce para aplicar a la imagen.
 * Obtiene las estadisticas de la imagen, y utiliza la clase realce para generar
 * una lut segun el realce seleccionado.
 * Agrega la lut generada al elemento, y permite des/activarla.
 * Permite activar la lut que la imagen trae por defecto, pero si se selecciona
 * otro realce la llut original se destruye y no es posible reconstruirla.
 */
class EnhancementSelectionPart : public LutSelectionPart {
public:
   /** Ctor. legacy*/
   explicit EnhancementSelectionPart(RasterElement * pElement);

   /** Ctor. */
   explicit EnhancementSelectionPart(RasterLayer * pElement);

   /** Dtor. */
   virtual ~EnhancementSelectionPart();

   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();

protected:
   /** Obtiene la lista de realces */
   virtual std::list<std::string> GetLutNames();

   /** Crea un nodo Lut a partir del tipo de realce recibido */
   virtual bool CreateLutNode(const std::string &LutName, bool Active,
                              wxXmlNode* &pNewLutNode);

   /** Genera una Lut modificando la que se pasa por parametro. */
   virtual bool CreateLutNode(const wxXmlNode* pLutNode, bool Active,
                              wxXmlNode* &pNewLutNode);

   /** Busca si el nodo coincide con una lut soportada y retorna el nombre. */
   virtual bool GetElementLutName(const wxXmlNode* pLutNode, std::string &LutName);

   /** Verifica si el nodo lut es de tipo correcto. */
   virtual bool IsValidLutType(const wxXmlNode* pLutNode);

   /** Retorna el estado de activacion del elemento */
   virtual bool IsActive(const wxXmlNode* pLutNode);

   /** Retorna el estado de activacion del elemento */
   virtual std::string GetNodeName();

   /** Obtiene las estadisticas del elemento */
   Statistics GetStatistics(Statistics::StatisticsFlag RequiredStats);

   /** */
   void GetRasterStats(suri::raster::data::StatisticsBase*& pStatistics,
                       suri::raster::data::HistogramBase*& pHistogram);

   /** Devuelve un mundo configurado */
   void ConfigureWorld(World *pWorld);

   /** Devuelve una lista con los elementos a renderizar */
   void ConfigureList(LayerList *pList);

   /** Devuelve el elemento a renderizar */
   RasterElement * CreateElement();

private:
   /**
    * Devuelve el valor de dato invalido si esta definido.
    */
   void RetrieveNoDataValue(bool& HasNoDataValue, double& NoDataValue);
};
}

#endif /* ENHANCEMENTSELECTIONPART_H_ */
