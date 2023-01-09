/*! \file ClassificationPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CLASSIFICATIONPART_H_
#define CLASSIFICATIONPART_H_

// Includes standard
#include <vector>
// Includes Suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/RasterElement.h"
#include "suri/VectorStyle.h"
#include "ClassInformation.h"
#include "ClassificationGetter.h"

// Includea wx
#include "wx/wx.h"
// Defined
// forward

/** namespace suri */
namespace suri {
/** Interfaz para los Part que obtienen parametros para una clasificacion */
/**
 * Interfaz que usa el ClassificationExporterWidget para generar el renderizaror
 * encargado de clasificar.
 * Tambien lo utiliza para obtener las clases y colores generados.
 */
class ClassificationPart : public Part, public ProcessAtributeProvider, public ClassificationGetter {
public:
    /** ctor */
   ClassificationPart() {}
   /** dtor */
   virtual ~ClassificationPart() {}
   /** Retorna la cantidad de clases */
   virtual int GetClassCount()=0;
   /** Retorna las propiedades de la clase en la posicion indicada. Las Posiciones
    * van de 0 a GetClassCount()-1.**/
   virtual bool GetClassProperties(std::vector<ClassInformation> &Properties)=0;
   /** Retorna el nodo que se debe agregar al elemento que se renderiza */
   virtual wxXmlNode* GetClassificationRendererNode(RasterElement* pRasterElement)=0;
   /** Metodo que indica la seleccion de bandas aciva para la clasificacion **/
   virtual void SetSelectedBands(const std::vector<int>& SelectedBands) {
      // se implementa vacio para no forzar esto mismo en las clases hijas que no requieran
      // hacer uso de este metodo
   }
};
}

#endif /* CLASSIFICATIONPART_H_ */
