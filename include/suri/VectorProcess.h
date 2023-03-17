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

#ifndef VECTORPROCESS_H_
#define VECTORPROCESS_H_

// Includes Estandar
#include <vector>

// Includes Suri
#include "ProcessInterface.h"
#include "VectorRenderizationController.h"
#include "DatasourceInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class Part;

/*!
 * Clase que representa un proceso sobre capas vectoriales.
 */
class VectorProcess : public suri::ProcessInterface {
   /** ctor copia **/
   VectorProcess(const VectorProcess&);

public:
   /** constructor **/
   VectorProcess();
   /** destructor **/
   virtual ~VectorProcess();
#ifdef __UNUSED_CODE__
   /** Agrega las partes. Sacar este metodo cuando se desacoplen los procesos */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);
#endif
   // ------ Metodos que administran la ejecucion del proceso ------
   /** Corre el proceso y genera la salida usando los metodos de configuracion */
   virtual bool RunProcess();
   /** Permite configurar la salida del proceso antes de correrlo */
   virtual bool ConfigureProcess();
   /** Una vez ejecutado el proceso hace visible la salida */
   virtual bool ConfigureOutput();

protected:
   /** Controlador para la renderizacion de capas vectoriales **/
   VectorRenderizationController* pController_;
   /** Obtiene la lista de elementos a renderizar en el proceso **/
   virtual DatasourceContainerInterface* GetRenderizationList()=0;
   /** Configura el subset espacial */
   virtual World* GetWorld()=0;
   /** Exporta los metadatos de una capa a un archivo */
   virtual bool ExportMetadataToFile(DatasourceInterface* pDatasource, std::string Filename);
};

} /** namespace suri */

#endif /* VECTORPROCESS_H_ */
