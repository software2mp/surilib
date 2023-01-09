/*! \file ClassifiedRasterDatasourceValidator.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "ClassifiedRasterDatasourceValidator.h"
#include "RasterDatasource.h"
#include "ClassInformation.h"
#include "ClassifiedRasterDatasourceManipulator.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
ClassifiedRasterDatasourceValidator::ClassifiedRasterDatasourceValidator() {
}

/**
 * Destructor
 */
ClassifiedRasterDatasourceValidator::~ClassifiedRasterDatasourceValidator() {
}

/**
 * Valida el contenido xml de una fuente de datos
 * Se condisera como xml valido un xml con los siguientes nodos:
 * \verbatim
 * <elemento ..>
 *   ...
 *   <clasificacion>
 *       <clase indice="0">
 *           <nombre> nombre </nombre>
 *           <color> valor_hexa </color>
 *           <recinto> id</recinto>
 *       </clase> ....
 *   </clasificacion>
 *   ...
 * </elemento>
 * \endverbatim
 * Para la validacion solo se tiene el nodo nombre para buscar una clase "sin clase"
 * @param[in] pDatasource referencia a la fuente de datos que se le quiere validar el xml
 * @return true en caso de que contenga al menos una clase "sin clase"
 * @return false en otro caso
 */
bool ClassifiedRasterDatasourceValidator::IsXmlValid(DatasourceInterface* pDatasource) const {
   ClassifiedRasterDatasourceManipulator manipulator;
   std::vector<ClassInformation> classvector = manipulator.GetClassInformationList(pDatasource);
   std::vector<ClassInformation>::const_iterator it = classvector.begin();
   bool exitsnoclass = false;
   for (; it != classvector.end() && !exitsnoclass; ++it) {
      exitsnoclass = ((*it).GetIndex() == ClassInformation::NoClassIndex);
   }
   return exitsnoclass;
}

/**
 * Verifica si un tipo es entero
 * @param[in] Type tipo de dato a verificar
 * @return true si el tipo es entero
 * @return false si el tipo no es entero
 */
bool ClassifiedRasterDatasourceValidator::IsIntegerType(const std::string& Type) const {
   if (Type.compare(DataInfo<unsigned char>::Name) == 0 || Type.compare(DataInfo<short>::Name) == 0
         || Type.compare(DataInfo<unsigned short>::Name) == 0
         || Type.compare(DataInfo<int>::Name) == 0
         || Type.compare(DataInfo<unsigned int>::Name) == 0) {
      return true;
   }
   return false;
}

/**
 * Efectua la validacion del objeto en cuestion y devuelve verdadero si es valido
 * Se considera como una fuente de datos clasificada a una fuente de datos que tiene:
 *       la fuente de datos es de tipo raster
 *      como minimo dos clases
 *      tiene que existir la clase no clase
 *      1 sola banda
 *      el tipo de dato de la banda tiene que ser entero
 * @param[in] pDatasource fuente de datos a validar
 * @return true si la fuente de datos es valida como una clasificada
 * @return false si la fuente de datos no es valida como clasificada
 */
bool ClassifiedRasterDatasourceValidator::IsValid(DatasourceInterface* pDatasource) const {
   RasterDatasource* pvd = dynamic_cast<RasterDatasource*>(pDatasource);
   if (pvd == NULL)
      return false;
   if (pvd->GetBandCount() != 1)
      return false;
   if (!IsIntegerType(pvd->GetDataType()))
      return false;
   bool xmlvalid = IsXmlValid(pDatasource);
   return xmlvalid;
}

} /** namespace suri */
