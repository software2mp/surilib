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

#ifndef REPROJECTIONPROCESS_H_
#define REPROJECTIONPROCESS_H_

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "suri/FileExporterProcess.h"
#include "suri/SuriObject.h"
#include "suri/LibraryManager.h"
#include "suri/CoordinatesTransformInterface.h"
#include "suri/Element.h"
#include "suri/Subset.h"
#include "suri/CoordinatesTransformation.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase que representa un proceso de reproyeccion.
 */
class ReprojectionProcess : public FileExporterProcess {
   /** ctor copia */
   ReprojectionProcess(const ReprojectionProcess&);

public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
   ReprojectionProcess(Element* pInputElement, const Subset& ViewportSubset,
                       const SuriObject::UuidType& LibraryId,
                       DataViewManager* pDataViewManager,
                       bool AskGcpFile = false, bool Reproject = true);
   virtual ~ReprojectionProcess();
   /** Actualiza el estado de los parts que administra la clase */
   virtual void DataUpdate();

   class ReprojectionParameters {
   public:
      World* pWorldIn_;
      std::string srWktIn_;
      std::string srWktOut_;
      std::string rasterModelIn_;
      std::string algorithm_;
      RasterSpatialModel* pRasterSpatialModelIn_;
      RasterSpatialModel* pRasterSpatialModelOut_;
      int algorithmOrder_;
      std::string gcpListFileName_;
      int width_;
      int height_;
      CoordinatesTransformation* pTranform_;
      ReprojectionParameters() :
            pWorldIn_(NULL), pRasterSpatialModelIn_(NULL),
            pRasterSpatialModelOut_(NULL),
            pTranform_(NULL) {
      }
     ~ReprojectionParameters() {
        delete pTranform_;
        RasterSpatialModel::Destroy(pRasterSpatialModelIn_);
        RasterSpatialModel::Destroy(pRasterSpatialModelOut_);
     }
   };
   /** Claves para atributos de configuracion del proceso */
   static const std::string ReprojectionParamsKeyAttr;
   static const std::string ReprojectionKeyAtttr;
   static const std::string AskGcpFileAttr;
   static const std::string ShowResizePartAttr;

protected:
   /** Metodo que se llama cuando se configura el ProcessAdaptLayer 
    *  del proceso**/
   virtual void DoOnProcessAdaptLayerSet();
   /** Devuelve el Sistema de Referencia de llegada. **/
   virtual std::string GetOutputSpatialReference();
   /**  Devuelve el subset de llegada.  **/
   virtual Subset GetOutputSubset();
   /** Devuelve el Modelo Raster de llegada */
   virtual RasterSpatialModel* GetOutputRasterSpatialModel();
   /** Configura el subset espacial */
   virtual World *GetWorld();
   /** Pemite configurar el RasterElement que se va a renderizar */
   virtual bool ConfigureRaster(RasterElement *pRasterElement);
   /** Permite configurar la salida del proceso antes de correrlo */
   virtual bool ConfigureProcess();
   /** Una vez ejecutado el proceso hace visible la salida */
   virtual bool ConfigureOutput();
   ReprojectionParameters* pReprojectionParameters_;
   /** booleano que indica si se debe iniciar el part de seleccion de SRout **/
   bool reprojec_;
   bool askGcpFile_;
   bool showResizePart_;
   SuriObject::UuidType libraryId_;
};

}  /** namespace suri */

#endif  /* REPROJECTIONPROCESS_H_ */
