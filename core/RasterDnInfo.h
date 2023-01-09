/** \file RasterDnInfo.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERDNINFO_H_
#define RASTERDNINFO_H_

#include <vector>
#include <string>

namespace suri {

class DatasourceInterface;
class MemoryCanvas;
class World;
class Element;
class CoordinatesTransformation;

/** Clase que obtiene informacion de las bandas de una capa raster **/
class RasterDnInfo {
   /** ctor copia **/
   RasterDnInfo(const RasterDnInfo&);

public:
   /** Ctor. */
   RasterDnInfo();
   /** Dtor. */
   ~RasterDnInfo();
   /**
    * Configura el pixel (de la combinacion pixel/linea).
    */
   void SetPixel(double Pixel);

   /**
    * Configura la linea (de la combinacion pixel/linea).
    */
   void SetLine(double Line);

   /**
    * Configura el origen de datos para para el cual se van a calcular los DN.
    */
   void SetDatasource(DatasourceInterface* Datasource);

   /**
    * Configura el mundo a ser utilizado.
    */
   void SetWorld(World* pWorld);

   /**
    * Configura el canvas a ser utilizado.
    */
   void SetCanvas(MemoryCanvas* pCanvas);

   /**
    * Configura el elemento a ser utilizado.
    */
   void SetElement(Element* pElement);

   /**
    * Devuelve un vecto que contiene para cada banda cual es el DN para el pixel/linea.
    */
   std::vector<std::string> GetBandsDnInfo();

private:
   double pixel_;
   double line_;
   DatasourceInterface* datasource_;
   World* pWorld_;
   MemoryCanvas* pCanvas_;
   Element* pElement_;
   /** Transformacion activa **/
   CoordinatesTransformation* pCt_;
   /** Obtiene un canvas para el elemento raster.*/
   MemoryCanvas* GetCanvas();
   /** Obtiene el elemento raster. */
   Element* GetElement();
   /** Metodo auxiliar que actualiza la transformacion **/
   void UpdateTransform();
};
} // namespace suri
#endif  // RASTERDNINFO_H_
