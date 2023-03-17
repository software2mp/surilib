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

#ifndef LIBRARYMANAGERFACTORY_H_
#define LIBRARYMANAGERFACTORY_H_

// Includes standard
#include <string>
// Includes suri
#include "LibraryManager.h"

namespace suri {

/** Clase encargada de generar y configurar el administrador de bibliotecas
 *  de la aplicacion
 */
class LibraryManagerFactory {
public:
   /** Inicializa LibraryManager */
   static LibraryManager* CreateLibraryManager();
   /** Recarga todos los origenes en LibraryManager */
   static void ResetLibraryManager(LibraryManager* pManager);
   /** Codigo de bibliotecas existente **/
   static std::string SpatialReferenceLibraryCode;
   static std::string DatumLibraryCode;
   static std::string EllipsoidLibraryCode;
   static std::string DensitySliceCode;
   static std::string WmsLibraryCode;
   static std::string VectorstyleLibraryCode;
   static std::string SpectralSignLibraryCode;
   static std::string ConvolutionFiltersCode;
   static std::string EquationLibraryCode;
   static std::string IndexLibraryCode;

private:
   /** Agrega un origen de archivo a biblioteca indicada */
   static void AddTextFileOriginToLibrary(
         Library* pLibrary, const std::string &FileName,
         LibraryItemOrigin::OriginCategoryType Category, const std::string& OriginName);
   /** Agrega un origen en memoria a biblioteca indicada */
   static void AddMemoryOriginToLibrary(Library* pLibrary,
                                        LibraryItemOrigin::OriginCategoryType Category,
                                        const std::string& OriginName);

   /**
    * Carga los distintos origenes disponibles a la libreria.
    */
   static void LoadOrigins(Library* pLibrary, const std::string& LibCode);
};
} /* namespace suri */


#endif /* LIBRARYMANAGERFACTORY_H_ */
