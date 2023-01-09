/*! \file FilterExporterWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILTEREXPORTERWIDGET_H_
#define FILTEREXPORTERWIDGET_H_

// Includes standard
#include <vector>

// Includes suri
#include "FileExporterProcess.h"
#include "ClassInformation.h"

/** namespace suri */
namespace suri {
// Forward
class DatasourceManagerInterface;

/** Flag para que se muestre filtro pasa bajos */
#define NO_FILTER                 0x00000000
/** Flag para que se muestre filtro pasa bajos */
#define KERNEL_FILTER                 0x0000007
/** Flag para que se muestre filtro pasa altos */
#define POSTCLASSIFICATION_FILTER              0x00000018

/** Clase para aplicar herramienta de filtros */
/**
 *	Esta clase hereda de FileExporterWidget, y agrega una parte para la seleccion
 *	del tipo y tamanio del filtro.
 *	Crea una lista de elementos con el elemento a renderizar, al cual le configura
 *	un nodo renderizacion con la combinacion de bandas, un nodo filtro
 *	de convolucion con el filtro seleccionado, y en caso de que exista un nodo
 *	de algebra de bandas con las operaciones que requiere el filtro (solo para
 *	el filtro de deteccion de bordes).
 */
class FilterProcess : public FileExporterProcess {
public:
   /** Tipos de filtro de la herramienta */
   typedef enum {
      LowPassFilter = 0, HighPassFilter = 1, EdgeDetectionFilter = 2,
      MinFilter = 3, MaxFilter = 4, LibraryFilter = 5
   } FilterType;
   static const char* FilterTypeString[];

   /** Ctor de copia */
   FilterProcess(FilterProcess &);
   /** Constructor */
   FilterProcess(Element *pInputElement, const Subset &WindowSubset,
                        int Filters,
                        DataViewManager* pDataViewManager = NULL,
                        bool ShowSpectralSelectionPart = true);
   /** Dtor */
   virtual ~FilterProcess();
   /** Nombre del proceso **/
   static const std::string kProcessName;
   //TODO Hacer una clase derivada?
   /** Nombre del proceso hijo **/
   static const std::string kChildProcessName;
   /** Clave para el atributo de nodo xml de filtro **/
   static const std::string FilterNodeKey;
   /** Clave para el atributo para el tipo de filtro **/
   static const std::string FilterTypeKey;
   /** Clave para el atributo de nodo xml de ecuaciones */
   static const std::string EquationNodeKey;
   /** Clave para el atributo para flags de filtros */
   static const std::string FilterFlagsKey;

protected:
// ------ Metodos que permiten configurar el proceso ------
   /** Pemite configurar el RasterElement que se va a renderizar */
   virtual bool ConfigureRaster(RasterElement *pRasterElement);
   /** Configura el elemento creado con la herramienta */
   virtual bool ConfigureOutput();

// ------ Metodos que leen el part de seleccion de filtro ------
   /** Configura las opciones del elemento */
   void SetFilterOptions(RasterElement *pRaster);
   /** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso**/
   virtual void DoOnProcessAdaptLayerSet();
   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
    **/
   std::vector<wxXmlNode*> DoGetXmlParameters() const;

private:
   int flags_; /*! Tipo de filtro que se quiere correr */

   std::vector<ClassInformation> classInformation_;   /*! Informacion de las */
                                           /** clasesen la imagen de entrada */
   /** Temporal hasta que los procesos usen datasources */
   DatasourceManagerInterface* pDatasourceManager_;

   /** Verifica si el nodo contiene una ecuacion definida. */
   bool HasEquation(const wxXmlNode* pNode) const;
};
}

#endif /* FILTEREXPORTERWIDGET_H_ */
