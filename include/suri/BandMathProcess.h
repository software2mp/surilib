/** \file BandMathExporterWidget.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef BANDMATHEXPORTERWIDGET_H_
#define BANDMATHEXPORTERWIDGET_H_

// Includes standard
#include <vector>

// Includes suri
#include "suri/FileExporterProcess.h"

namespace suri {

// fordwards
class DataViewManager;
/**
 * Configura nodo en renderizador que genera bandas de salida.
 */
class BandMathProcess : public FileExporterProcess {
public:
   /** Constructor */
   BandMathProcess(Element *pInputElement, const Subset &ViewerSubset,
                   DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~BandMathProcess();
   /** Nombre del proceso **/
   static const std::string kProcessName;
   /** Clave para la obtencion del tipo de dato en que se expresa el algebra de bandas **/
   static const std::string DataTypeKeyAttr;
   /** Clave para el atributo de ecuacion **/
   static const std::string EquationKey;
   /** Clave para el atributo de variables de la ecuacion **/
   static const std::string EquationVariablesKey;

protected:
   // ------ Metodos que permiten configurar el proceso raster ------
   /** Permite configurar la salida del proceso antes de correrlo */
   virtual bool ConfigureRaster(RasterElement *pRasterElement);
   /** Otiene el tamano del objeto a salvar */
   virtual wxLongLong GetNeededSpace();

private:
   /** Retorna el nodo configurado con ecuacion ingresada por el usuario */
   wxXmlNode*  GetBandMathRenderizationNode();
   /** Retorna el nodo datacast configurado con el tipo de dato de salida */
   wxXmlNode*  GetDataCastRenderizationNode();
   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
    **/
   std::vector<wxXmlNode*> DoGetXmlParameters() const;
};
}  // namespace


#endif /* BANDMATHEXPORTERWIDGET_H_ */
