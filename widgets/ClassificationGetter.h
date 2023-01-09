/*! \file ClassificationGetter.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CLASSIFICATIONGETTER_H_
#define CLASSIFICATIONGETTER_H_

// Includes Estandar
// Includes Suri
#include "ClassInformation.h"
#include "suri/RasterElement.h"

// Includes Wx
// Defines
// forwards

namespace suri {

class ClassificationGetter {
   /** ctor copia **/
   ClassificationGetter(const ClassificationGetter&);
public:
   /** ctor **/
   ClassificationGetter() {}
   /** dtor **/
   virtual ~ClassificationGetter() {}
   /** Retorna la cantidad de clases */
   virtual int GetClassCount()=0;
   /** Retorna las propiedades de la clase en la posicion indicada. Las Posiciones van de 0 a */
// GetClassCount()-1.
   virtual bool GetClassProperties(std::vector<ClassInformation> &Properties)=0;
   /** Retorna el nodo que se debe agregar al elemento que se renderiza */
   virtual wxXmlNode* GetClassificationRendererNode(RasterElement* pRasterElement)=0;
   /** Metodo que indica la seleccion de bandas aciva para la clasificacion **/
   virtual void SetSelectedBands(const std::vector<int>& SelectedBands)=0;

   /** Metodo que sirve para verificar si la clasificacion es valida */
   virtual bool IsValid() { return true; }
};

} /** namespace suri */

#endif /* CLASSIFICATIONGETTER_H_ */
