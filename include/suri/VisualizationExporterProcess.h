/*! \file VisualizationExporterProcess.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VISUALIZATIONEXPORTERPROCESS_H_
#define VISUALIZATIONEXPORTERPROCESS_H_

// Includes Suri
#include "ReprojectionProcess.h"

namespace suri {

class MapElement;

/** Exporta un elemento que contenga todas las capas desplegadas en el visualizador */
class VisualizationExporterProcess : public FileExporterProcess {
   /** Constructor copia */
   VisualizationExporterProcess(const VisualizationExporterProcess&);
public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
   /** Constructor */
   VisualizationExporterProcess(RasterElement* pRasterElement,
                                std::vector<Element*> InputElements,
                                const Subset &ViewerSubset,
                                DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~VisualizationExporterProcess();

protected:
   /** Obtiene la lista de elementos que se a renderizar con el proceso */
   virtual LayerList *GetElementList();
   /** Devuelve el Modelo Raster de llegada */
   virtual RasterSpatialModel* GetOutputRasterSpatialModel();
   /** Pemite configurar el RasterElement que se va a renderizar */
   virtual bool ConfigureRaster(RasterElement *pRasterElement);
   /** Retorna el nodo datacast configurado con el tipo de dato de salida */
   wxXmlNode* GetDataCastRenderizationNode();
   /** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso */
   virtual void DoOnProcessAdaptLayerSet();
   /** Retorna el elemento de entrada */
   virtual Element* GetInputElement() const { return pRasterElement_; }
   /** Configura el elemento creado con la herramienta */
   virtual bool ConfigureOutput();
   /** Configura el proceso para que la salida sea el raster configurado */
   virtual bool ConfigureProcess();
   /** Configura el subset espacial */
   virtual World *GetWorld();
   /** Devuelve un elemento de tipo mapa con la misma configuracion del que se encuentra activo */
   MapElement* GetMapElement(Element* pElement);
   /** Elemento de entrada */
   RasterElement* pRasterElement_;
   /** Vector con elementos activos */
   std::vector<Element*> InputElements_;
   /** Parametros de reproyeccion */
   ReprojectionProcess::ReprojectionParameters* pReprojectionParameters_;
};

} /** namespace suri */

#endif /* VISUALIZATIONEXPORTERPROCESS_H_ */
