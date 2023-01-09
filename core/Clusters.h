/** \file ClassContainer.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef CLASSCONTAINER_H_
#define CLASSCONTAINER_H_

// Includes standard
#include <vector>

// Includes suri
#include "suri/StatisticsFunctions.h"
#include "SRDStatistics.h"

namespace suri {

/**
 * Contiene informacion de los clusters que se usan para clasificar. 
 * Se encarga de su persistencia a xml.
 */
class Clusters {
public:
   /** Ctor */
   Clusters();
   /** Dtor */
   ~Clusters();

   /**
    * Informacion de las clases
    */
   struct ClusterData {
      ClusterData() : classId_(-1), pStatistics_(NULL) {}
      int classId_;  /*! Valor de la clase en el raster de salida asociada al cluster. */
      Statistics statistics_;  /*! Estadisticas del area de entrenamiento */
      suri::raster::data::StatisticsBase* pStatistics_;
   };
   /** Agrega una clase a contenedor */
   std::vector<ClusterData>& GetClusterVector();
   bool AddCluster(int AssociatedClassId, const Statistics &Stats);
   /** Agrega una clase a contenedor */
   bool AddCluster(int AssociatedClassId, suri::raster::data::StatisticsBase* pStats);
   /** Elimina una clase del contendor */
   void Clean();
   /** Valida que las clases sean consistentes entre ellas */
   bool Check();

      /** Persiste las clases dentro de pParentNode */
   bool WriteToXml(wxXmlNode* pParentNode) const;
   /** Lee las clases desde pParentNode */
   bool ReadFromXml(const wxXmlNode* &pParentNode);

private:
   std::vector<ClusterData> clusters_;  /*! Clases guardadas en vector */
};

}  // namespace

#endif /* CLASSCONTAINER_H_ */
