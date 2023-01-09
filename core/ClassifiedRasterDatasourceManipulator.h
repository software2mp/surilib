/*! \file ClassifiedRasterDatasourceManipulator.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CLASSIFIEDRASTERDATASOURCEMANIPULATOR_H_
#define CLASSIFIEDRASTERDATASOURCEMANIPULATOR_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "ClassifiedRasterDatasourceValidator.h"
#include "ClassInformation.h"
#include "suri/DatasourceManipulatorInterface.h"
#include "suri/DatasourceInterface.h"
#include "suri/VectorStyle.h"

// Includes Wx
#include "wx/xml/xml.h"

// Defines

// forwards

namespace suri {

/**
 * Manipulador de RasterDatasources validados como rasters clasificados
 */
class ClassifiedRasterDatasourceManipulator : public suri::DatasourceManipulatorInterface {
public:
   /** Constructor */
   ClassifiedRasterDatasourceManipulator();
   /** Destructor */
   virtual ~ClassifiedRasterDatasourceManipulator();
   /** Crea una nueva fuente de datos que cumple con los requisitos de validacion */
   virtual DatasourceInterface* Create(const std::string& FileName, const Option& Option) const;
   /** Efectua la logica de agregado para un Datasource validado */
   virtual bool AddNature(DatasourceInterface* pDatasource) const;
   /** Efectua la logica de limpieza para un Datasource validado */
   virtual bool CleanNature(DatasourceInterface* pDatasource) const;
   /** Efectua la logica de copia de un Datasource a otro */
   virtual bool CopyNature(DatasourceInterface* pSourceDatasource,
                 DatasourceInterface* pDestinationDatasource) const;
   /** Agrega una clase a la fuente de datos clasificada */
   bool AddClassInformation(ClassInformation* pClassInformation, DatasourceInterface* pDatasource,
                            bool Replace = false) const;
   /** Devuelve un id para la creacion de un nuevo datasource */
   ClassInformation::IndexType GetClassInformationNewId(DatasourceInterface* pDatasource) const;
   /** Obtiene una clase de la fuente de datos clasificada a partir de un indice */
   ClassInformation GetClassInformation(DatasourceInterface* pDatasource,
                                        ClassInformation::IndexType Index) const;
   /** Obtiene la cantidad de clases dentro de una fuente de datos clasificada */
   int GetClassInformationCount(DatasourceInterface* pDatasource) const;
   /** Devuelve una lista con las clases dentro de la fuente de datos */
   std::vector<ClassInformation> GetClassInformationList(DatasourceInterface* pDatasource) const;
   /** Definicion de nombres para la opcion SizeX */
   static const std::string SizeXOptionName;
   /** Definicion de nombres para la opcion SizeY */
   static const std::string SizeYOptionName;
   /** Definicion de nombres para la opcion Driver */
   static const std::string DriverOptionName;

private:
   /** Valor para cantidad de clases invalida */
   static const int InvalidCount;
   /** Validador de Raster Clasificado */
   ClassifiedRasterDatasourceValidator validator_;
   /** Metodo auxiliar para la creacion de los nodos xml correspondientes \deprecated */
   void CreateXmlNodes(DatasourceInterface* pDatasource) const;
   /** Metodo auxiliar para la creacion de una clase a partir del xml */
   ClassInformation CreateClassInformationFromXml(wxXmlNode* pNode,
                                                  ClassInformation::IndexType Index = -1,
                                                  bool ClassFound = true) const;
   /** Agrega los nodos xml de una clase a la fuente de datos clasificada */
   void CreateXmlClassNode(ClassInformation* pClassInformation,
                           DatasourceInterface* pDatasource) const;
   /** Modifica los nodos xml de una clase a la fuente de datos clasificada */
   bool ModifyXmlClassNode(ClassInformation* pClassInformation,
                           DatasourceInterface* pDatasource) const;
   /** Devuelve el nodo xml correspondiente a la clase que se indica por parametro */
   wxXmlNode* GetClassInformationXmlNode(DatasourceInterface* pDatasource,
                                         ClassInformation::IndexType Index) const;
};

} /** namespace suri */

#endif /* CLASSIFIEDRASTERDATASOURCEMANIPULATOR_H_ */
