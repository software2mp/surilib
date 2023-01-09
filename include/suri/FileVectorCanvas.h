/*! \file FileVectorCanvas.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
