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

#ifndef MOSAICPROCESS_H_
#define MOSAICPROCESS_H_

// Includes standard
#include <vector>

// Includes suri
#include "FileExporterProcess.h"
#include "ClassInformation.h"
#include "suri/LayerList.h"
#include "suri/Dimension.h"

/** namespace suri */
namespace suri {
// Forward
class DatasourceManagerInterface;

class MosaicProcess: public FileExporterProcess {

public:
   /** Ctor de copia */
   MosaicProcess(MosaicProcess &);
   /** Constructor */
   MosaicProcess(std::vector<Element*> InputElements, const Subset &WindowSubset,
                        DataViewManager* pDataViewManager = NULL,
                        bool ShowSpectralSelectionPart = false);
   /** Dtor */
   virtual ~MosaicProcess();
   /** Nombre del proceso **/
   static const std::string kProcessName;

   LayerList* GetElementList();

   virtual World *GetOutputWorld();

   bool  ConfigureProcess();

   bool RunProcess();
protected:
// ------ Metodos que permiten configurar el proceso ------

   /** Pemite configurar el RasterElement que se va a renderizar */
   virtual bool ConfigureRaster(RasterElement *pRasterElement);

   /** Configura el elemento creado con la herramienta */
   virtual bool ConfigureOutput();

   /** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso**/
   virtual void DoOnProcessAdaptLayerSet();
private:
   /**
    * Obtiene el tamaño de pixel mayor y escala las imagenes para que todas tengan el mismo tamaño
    * de pixel.
    */
   void SetElementsPixelSize();
   Element* GetNewElementWithPixelValue(RasterElement* pElement, double PixelValue);
   void IsValidEdge(std::vector<void*>& Data,
                                   bool& North, bool& South, bool& East, bool& West);
   /** Devuelve el valor de pixeles de cada banda del extent del elemento indicado  **/
   std::vector<void*> GetPixelValue(const Subset& Extent, Element* pElement);

   /** Devuelve la Dimension de la interseccion **/
   Dimension GetIntersectionDimension();

   /**
    *  Aplica feathering de oeste a este de la imagen, solo aplica si el pixel es valido ,
    *  si es invalido pasa al siguiente
    **/
   void ApplyFeatheringFromWestToEast(std::vector<void*>& DataOn,
                                       const std::vector<void*>& DataUnder);
   /**
    *  Aplica feathering de este a oeste de la imagen, solo aplica si el pixel es valido ,
    *  si es invalido pasa al siguiente
    **/
   void ApplyFeatheringFromEastToWest(std::vector<void*>& DataOn,
                                       const std::vector<void*>& DataUnder);
   /**
    *  Aplica feathering de norte a sur de la imagen, solo aplica si el pixel es valido ,
    *  si es invalido pasa al siguiente
    **/
   void ApplyFeatheringFromNorthToSouth(std::vector<void*>& DataOn,
                                        const std::vector<void*>& DataUnder);
   /**
    *  Aplica feathering de sur a norte de la imagen, solo aplica si el pixel es valido ,
    *  si es invalido pasa al siguiente
    **/
   void ApplyFeatheringFromSouthToNorth(std::vector<void*>& DataOn,
                                        const std::vector<void*>& DataUnder);

   /** Si DataOn tiene pixeles no validos los reemplaza por los pixeles de DataUnder **/
   void RepleaceNoDataArea(std::vector<void*>& DataOn,
                           const std::vector<void*>& DataUnder);

   /* *
    * Devuelve la distancia de pixel en "x" y en "y" , si la distancia seteada en el part
    * es mayor que el ancho y el alto de la interseccion, se reemplazan por el ancho y el alto
    * respectivamente.
    * */
   void GetPixelDistance(int& DistanceX, int& DistanceY);

   /**
    * Dado los pesos de cada pixel y los datos de las imagenes, calcula el nuevo valor para el pixel
    * de la imagen en la posicion dada.
    **/
   void SetNewPixelValue(void* DataOn, void * DataUnder, double PonderationOn,
                                              double PonderationUnder, int Position);

   /** Eje X true Eje Y False **/
   void ApplyFeathering(std::vector<void*> DataFinal, const std::vector<void*>& DataUnder,
                                                const std::vector<void*> & DataOn,
                                                const Coordinates& Begin, const Coordinates& End,
                                                bool Axis, double Factor);

   RasterSpatialModel* GetOutputRasterSpatialModel();

   void SetSubsetOutput();

   std::vector<void*> GetFeatheringData();

   /** Devuelve el subset con el area de solapamiento de imagenes ( por ahora solo entre 2) **/
   Subset GetIntersection(RasterElement* On, RasterElement* Under);

   /**Setea el tipo de dato del raster de salida, como el mayor del tipo de dato de entrada**/
   void SetDataSize();

   /* *
    * Devuelve false, si todas las bandas tienen en la posicion Position el valor no valido, true
    * en caso contrario
    * */
   bool IsValidData(const std::vector<void*>& Bands, int Position);

   /**
    * Por alguna razon cuando se hacen transformnaciones de coordenadas se pierden pixeles
    * por lo que guardo la dimension mayor para leer la misma cantidad de pixeles de todos
    * los rasters
    **/
   void SetBiggerIntersectionSubset();

   /** Arregla el subset para que siempre se lea la misma cantidad de pixeles**/
   void FixSubset(Subset& ActualSub);

   /**
    * Luaego de aplicar el feathering principal  se generan lineas en los extremos, este metodo
    * se encarga de aplicar feathering a esas lineas.
    **/
   void ApplySecondaryFeathering(std::vector<void*>&DataOn, const Subset& SubOn,
                                 const Subset& SubUnder);

   int GetDataSize(const Subset& Extent, Element* pElement);

   /** Informacion de las clasesen la imagen de entrada **/
   std::vector<ClassInformation> classInformation_;

   /** Temporal hasta que los procesos usen datasources **/

   DatasourceManagerInterface* pDatasourceManager_;

   unsigned int dataSize_;

   int pixelDistance_;

   /** en coordenadas de mundo **/
   Subset intersection_;

   double noDataValueOn_;
   /** el color para llenar las zonas de no datos **/

   unsigned char noDataColour_;
   double pixelSize_;
   int difx_;

   int dify_;

   Subset biggerIntersectionSubset_;

};
}

#endif /* MOSAICPROCESS_H_ */
