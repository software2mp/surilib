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

#ifndef FILEVECTORCANVAS_H_
#define FILEVECTORCANVAS_H_


// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "Canvas.h"
#include "Subset.h"
#include "suri/DatasourceContainerInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase encargada de volcar el resultado de una
 * operacion vectorial a un archivo de tipo vectorial. (ej. shapefile)
 */
class FileVectorCanvas : public suri::Canvas {
public:
   /** ctor **/
   explicit FileVectorCanvas(DatasourceContainerInterface* pVectorList,
                    Subset& OutputSubset, const std::string &SpatialReference, 
                     const std::string& FileName);
   /** dtor **/
   virtual ~FileVectorCanvas();
   /** Inicializa el canvas para que sea compatible con el que se le pasa */
   virtual bool InitializeAs(const Canvas *pCanvas);
   /** Metodo de clonacion */
   virtual Canvas *Clone() const;
   /** Lectura del canvas */
   void Read(std::vector<int> &BandIndex, std::vector<void*> &OutputData) const;
   /** Lectura hacia otro canvas (in-place) */
   void Read(Canvas *pTarget, const Mask *pMask = NULL) const;
   /** Escritura al canvas
    * @param[in] BandIndex indice de bandas donde se escriben los datos
    * @param[in] InputData vector de datos que deben escribirse en las bandas locales
    *                 indicadas por BandIndex.
    * @param[in] pMask Mascara de los datos
    */
   virtual void Write(std::vector<int> &BandIndex, std::vector<void*> &InputData,
                      const Mask *pMask = NULL);
   /** Escritura desde otro canvas (in-place) */
   void Write(const Canvas *pSource, const Mask *pMask = NULL);
   /** Canvas vectorial */
   virtual wxDC *GetDC();
   /** Escribe los datos */
   virtual void Flush(const Mask *pMask);
   /** Limpia los datos del canvas */
   virtual void Clear();
   /** Retorna la cantidad de bandas */
   virtual int GetBandCount() const;
   /** Permite modificar la cantidad de bandas (en algunos casos) */
   virtual void SetBandCount(int BandCount, bool SaveBands = false);
   /** Nombre del tipo de dato */
   virtual std::string GetDataType() const;
   /** Tamanio del dato en bytes */
   virtual int GetDataSize() const;
   /** Nombre del tipo de dato */
   virtual void SetDataType(const std::string& DataType);

private:
   /** no tiene utilidad para canvas vectorial **/
   virtual void GetInternalData(std::vector<int> &BandIndex,
                                std::vector<void*> &OutputData);
   /** Lista de vectores **/
   DatasourceContainerInterface* pVectorList_;
   /** Subset de llegada**/
   Subset subset_;
   /** Sistema de referencia de salida **/
   std::string srsOut_;
   /** filename del archivo a renderizar **/
   std::string filename_;
};

} /** namespace suri */

#endif /* FILEVECTORCANVAS_H_ */
