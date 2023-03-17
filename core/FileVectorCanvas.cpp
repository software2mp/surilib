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

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/FileVectorCanvas.h"
#include "suri/Vector.h"
#include "suri/VectorElement.h"
#include "suri/DatasourceContainerInterface.h"
#include "suri/DatasourceInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
FileVectorCanvas::FileVectorCanvas(DatasourceContainerInterface* pVectorList,
                                   Subset& OutputSubset,
                                   const std::string &SpatialReference,
                                   const std::string& FileName) :
      pVectorList_(pVectorList), subset_(OutputSubset), srsOut_(SpatialReference),
      filename_(FileName) {
}

/** Destructor **/
FileVectorCanvas::~FileVectorCanvas() {
}

/** Inicializa el canvas para que sea compatible con el que se le pasa */
bool FileVectorCanvas::InitializeAs(const Canvas *pCanvas) {
   return false;
}

/** Metodo de clonacion */
Canvas *FileVectorCanvas::Clone() const {
   return NULL;
}

/** Lectura del canvas */
void FileVectorCanvas::Read(std::vector<int> &BandIndex, std::vector<void*> &OutputData) const {
}

/** Lectura hacia otro canvas (in-place) */
void FileVectorCanvas::Read(Canvas *pTarget, const Mask *pMask) const {
}

/** Escritura al canvas
 * @param[in] BandIndex indice de bandas donde se escriben los datos
 * @param[in] InputData vector de datos que deben escribirse en las bandas locales
 *                 indicadas por BandIndex.
 * @param[in] pMask Mascara de los datos
 */
void FileVectorCanvas::Write(std::vector<int> &BandIndex, std::vector<void*> &InputData,
                             const Mask *pMask) {
}

/** Escritura desde otro canvas (in-place) */
void FileVectorCanvas::Write(const Canvas *pSource, const Mask *pMask) {
}

/** Canvas vectorial */
wxDC *FileVectorCanvas::GetDC() {
   return NULL;
}

/** Escribe los datos */
void FileVectorCanvas::Flush(const Mask *pMask) {
   /** vector **/
   if (filename_.empty())
      return;
   // Por ahora solo se toma el primer elemento de la layerlist y se renderiza a disco
   DatasourceContainerInterface::UuidListType ids = pVectorList_->GetDatasourceList();
   DatasourceInterface* pfirstelement = pVectorList_->GetDatasource(*ids.begin());
   Vector* porigvector = pfirstelement->GetDatasourceAsVector();
   if (!porigvector)
      return;
   // TODO (Gabriel - TCK #3623): Tener en cuenta que si se sobreescribe un archivo
   // debe pisarse la capa
   Vector* pdestvector = Vector::Open(filename_, Vector::ReadWrite);
   porigvector->Copy(pdestvector, subset_, srsOut_);
   Vector::Close(pdestvector);

   // porigvector se libera en el destructor de FilteredDatasource.
   // Vector::Close(porigvector);
}

/** Limpia los datos del canvas */
void FileVectorCanvas::Clear() {
}

/** Retorna la cantidad de bandas */
int FileVectorCanvas::GetBandCount() const {
   return -1;
}

/** Permite modificar la cantidad de bandas (en algunos casos) */
void FileVectorCanvas::SetBandCount(int BandCount, bool SaveBands) {
}

/** Nombre del tipo de dato */
std::string FileVectorCanvas::GetDataType() const {
   return "";
}

/** Tamanio del dato en bytes */
int FileVectorCanvas::GetDataSize() const {
   return -1;
}

/** Nombre del tipo de dato */
void FileVectorCanvas::SetDataType(const std::string& DataType) {
}

/** no tiene utilidad para canvas vectorial **/
void FileVectorCanvas::GetInternalData(std::vector<int> &BandIndex,
                                       std::vector<void*> &OutputData) {
}
} /** namespace suri */
