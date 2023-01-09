/*! \file ReprojectionProcess.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
