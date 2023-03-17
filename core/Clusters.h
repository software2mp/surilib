/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
