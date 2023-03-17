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

#ifndef VISUALIZATIONEXPORTERPROCESSPARTPROVIDER_H_
#define VISUALIZATIONEXPORTERPROCESSPARTPROVIDER_H_

// Includes Suri
#include "FileExporterProcessPartProvider.h"
#include "ReprojectionProcess.h"

namespace suri {

/**
 * Clase encargada de proveer los parts que configuran un proceso que exporta
 * el resultado a un archivo
 */
class VisualizationExporterProcessPartProvider : public FileExporterProcessPartProvider {
   /** Constructor copia */
   VisualizationExporterProcessPartProvider(const VisualizationExporterProcessPartProvider&);
   /** Constructor */
   explicit VisualizationExporterProcessPartProvider(ProcessAdaptLayer* pPal);

public:
   /**
    * Metodo para la creacion de un Part provider
    * @return instancia del creador correspondiente
    * @return NULL en caso contrario
    */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);
   /** Destructor */
   virtual ~VisualizationExporterProcessPartProvider();
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);

private:
   /** Parametros de reproyeccion */
   ReprojectionProcess::ReprojectionParameters* pReprojectionParameters_;
};

} /** namespace suri */

#endif /* VISUALIZATIONEXPORTERPROCESSPARTPROVIDER_H_ */
