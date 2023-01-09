/*! \file EnclosureManipulator.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ENCLOSUREMANIPULATOR_H_
#define ENCLOSUREMANIPULATOR_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/DatasourceManipulatorInterface.h"
#include "EnclosureInformation.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Manipulador de VectorDatasources validados como Recintos (Enclosure)
 */
class EnclosureManipulator : public suri::DatasourceManipulatorInterface {
public:
   /** Constructor */
   explicit EnclosureManipulator(bool TrainArea = true);
   /** Destructor */
   virtual ~EnclosureManipulator();
   /** Crea una nueva fuente de datos que cumple con los requisitos de validacion de Enclosure */
   virtual DatasourceInterface* Create(const std::string& FileName,
                                       const Option& Option) const;
   /** Efectua la logica de agregado para un Datasource validado */
   virtual bool AddNature(DatasourceInterface* pDatasource) const;
   /** Efectua la logica de limpieza para un Datasource validado */
   virtual bool CleanNature(DatasourceInterface* pDatasource) const;
   /** Efectua la logica de copia de un Datasource a otro */
   virtual bool CopyNature(DatasourceInterface* pSourceDatasource,
                    DatasourceInterface* pDestinationDatasource) const;
   /** Devuelve la informacion de recinto de la fuente de datos*/
   EnclosureInformation GetEnclosureInformation(
                        DatasourceInterface* pDatasource) const;
   /** Devuelve la informacion de recinto de la fuente de datos*/
   EnclosureInformation GetEnclosureInformation(
                        Element* pElement) const;

	/** Metodo que agrega la configuracion como area de clasificacion a un elemento **/                        
   bool AddNature(Element* pElement) const;
   // Definiciones para nombres de optiones
   static const std::string SpatialReferenceOptionName;
   static const std::string DriverOptionName;

private:
   /** Metodo auxiliar para crear los campos para un recito */
   bool CreateEnclosureFields(EnclosureInformation &EnclosureInformation,
                                    DatasourceInterface* pDatasource) const;
   /** Metodo auxiliar para crear los campos para un recito */
   bool CreateEnclosureFields(EnclosureInformation &EnclosureInformation,
                              Element* pElement) const;
   /** Metodo auxiliar para crear los nodos xml para un recito \deprecated */
   bool CreateEnclosureXmlNodes(EnclosureInformation &EnclosureInformation,
                                Element* pElement) const;
   /** Metodo auxiliar para la creacion del contenido del nodo consulta del xml del recinto */
   std::string CreateQuery(const std::string& LayerName, const std::string& FieldName,
                           const std::string& Condition = " IN(0)") const;
   /** Obtiene el nombre de la capa activa de la fuente de datos */
   std::string GetActiveLayerName(const std::string DatasourceUrl,
                                  int LayerIndex) const;
   bool trainArea_;
};

} /** namespace suri */

#endif /* ENCLOSUREMANIPULATOR_H_ */
