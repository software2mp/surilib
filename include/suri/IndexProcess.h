/*! \file IndexExporterWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef INDEXEXPORTERWIDGET_H_
#define INDEXEXPORTERWIDGET_H_

// Includes standar
#include <string>
#include <vector>

// Includes suri
#include "suri/FileExporterProcess.h"
#include "suri/RasterElement.h"

/** namespace suri */
namespace suri {
// Forward
class DataViewManager;
/** Clase que crea una herramienta para el calculo de indices. */
/**
 * Esta clase recibe un elemento y un subset. A las partes que agrega la clase
 * base (seleccion de subset espacial, y nombre y formato de archivo) agrega una
 * parte para seleccion de indices y bandas.
 * Con los datos obtenidos de estas partes configura el elemento para aplicar
 * la herramienta de indice.
 * Calcula el espacio requerido por el elemento creado.
 * FileExporterWidget normalmente incluye un Part de seleccion espectral, como
 * esta funcionalidad no es requerida en FileExporterWidget se elimina
 * impelmentando CreateSpectralSectionPart y dejandolo vacio.
 * En el futuro se podria separar la logica de archivo de la de seleccion
 * espectral para que esto no sea necesario.
 */
class IndexProcess : public FileExporterProcess {
   /** Ctor. de copia */
   IndexProcess(const IndexProcess &IndexExporterWidget);

public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
   /** Constructor */
   IndexProcess(Element *pInputElement, const Subset &ViewerSubset,
                DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~IndexProcess();
   /** Clave para atributo de seleccion de indice **/
   static const std::string IndexKeyAttr;
   /** Clave para el atributo de ecuaciones */
   static const std::string EquationKey;

protected:
   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
    **/
   std::vector<wxXmlNode*> DoGetXmlParameters() const;
   // ------ Metodos que permiten configurar el proceso raster ------
   /** Permite configurar la salida del proceso antes de correrlo */
   virtual bool ConfigureRaster(RasterElement *pRasterElement);
   /** Otiene el tamano del objeto a salvar */
   virtual wxLongLong GetNeededSpace();

   // ------ Metodos que permiten redefinir bandas de entrada ------
   /** Retorna las bandas selecciondas por el usuario */
   virtual void GetSelectedRasterBands(std::vector<int>& SelectedBands);
   /** Indica si las bandas se configuraron correctamente */
   virtual bool HasValidSpectralData();

private:
   /** Crea el nodo renderizacion con el indice */
   wxXmlNode* GetIndexRenderizationNode();
};

}  // namespace suri

#endif /* INDEXEXPORTERWIDGET_H_ */
