/*! \file ClassificationExporterWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CLASSIFICATIONEXPORTERWIDGET_H_
#define CLASSIFICATIONEXPORTERWIDGET_H_

// Includes standard
#include <string>
#include <vector>
// Includes Suri
#include "suri/FileExporterProcess.h"
#include "ClassInformation.h"

/** namespace suri */
namespace suri {
// foward
class ViewcontextManagerInterface;
class DatasourceManagerInterface;

/** Clase para clasificar usando poligonos */
/**
 * Utiliza una lista con elementos vectoriales de tipo poligono (representan
 * clases) para clasificar un elemento.
 * Agrega un Part para seleccion de parametros del clasificador.
 * Crea un elemento nuevo con el resultado de la clasificacion, que tiene una
 * tabla de color con los colores de las clases.
 */
class ClassificationProcess : public FileExporterProcess {
   /** Ctor. de copia */
   ClassificationProcess(const ClassificationProcess &);

public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
   /** Ctor. */
   ClassificationProcess(LayerList *pLayerList, Element *pInputElement,
                         const Subset &WindowSubset,
                         std::string ClassificationType,
                         DataViewManager* pDataViewManager,
                         bool ShowSpectralPart = false);
   /** Dtor. */
   virtual ~ClassificationProcess();
   /** Clave para atributo de tipo de clasificacion**/
   static const std::string ClassificationType;
   /** Clave para la obtencion de funcion encargada de obtener las propiedades
    *  de las clases y el nodo xml resultantes de la clasificacion*/
   static const std::string ClassGetterAttrKey;

protected:
   /** Metodo que especializa la configuracion del ProcessAdaptLayer para
    *  los procesos especificos
    */
   virtual void DoOnProcessAdaptLayerSet();
   /** Configura el nodo antes de renderizar */
   virtual bool ConfigureRaster(RasterElement *pRasterElement);
   /** Configura el elemento creado con la herramienta */
   virtual bool ConfigureOutput();
   /** Otiene el tamanio del objeto a salvar */
   wxLongLong GetNeededSpace();
   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
   **/
   virtual std::vector<wxXmlNode*> DoGetXmlParameters() const; 

private:
   /*! Layer list donde estan los poligonos con clases */
   LayerList *pLayerList_;
   DatasourceManagerInterface* pDatasourceManager_;
   ViewcontextManagerInterface* pViewcontextManager_;
   std::string classificationType_;
};
}  // suri

#endif /* CLASSIFICATIONEXPORTERWIDGET_H_ */
