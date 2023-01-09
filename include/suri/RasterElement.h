/*! \file RasterElement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERELEMENT_H_
#define RASTERELEMENT_H_

// Includes Estandar
#include <map>
#include <string>

// Includes suri
#include "suri/Element.h"
#include "Part.h"

/** namespace suri */
namespace suri {
// forwards
class Image;
class World;

/** Element que representa una imagen */
/**
 *  Este elemento representa una imagen raster.
 * \note especializa Element para representar Raster en LayerList, guardar
 *       sus datos en xml e informar cambios a las Vistas registradas.
 */
class RasterElement : public Element {
   /** Ctor. de Copia. */
   RasterElement(const RasterElement &RasterElement);

public:
   /** Contiene un las propiedades de raster */
   typedef struct {
      std::string DataType; /*! tipo de dato "C" como string */
   } RasterPropertiesType;

   /** Constructor */
   RasterElement();
   /** Destructor */
   virtual ~RasterElement();
   /** retorna un string para el icono */
   virtual wxString DoGetIcon() const;
   /** crea una previsualizacion del elemento */
   virtual void GetPreview(wxBitmap &Preview, int Width = 0, int Height = 0) const;
   /** retorna los detalles del elemento */
   virtual wxString GetDetails() const;

   /** Funcion estatica para crear un RasterElement */
   static RasterElement* Create(const std::string &FileName,
                                const Option& Metadata = Option());
   /** Devuelve una copia del raster **/
   RasterElement* Clone() const;
   /** seteo la matriz del modelo de raster */
   virtual bool SetRasterModel(const std::string &Wkt);
   /** Obtengo la matriz del modelo de raster */
   virtual std::string GetRasterModel() const;
   /** Obtengo el tipo de dato de la imagen */
   virtual RasterPropertiesType GetRasterProperties();
   /** Cargo las propiedades de archivo */
   virtual void SetRasterProperties(const RasterPropertiesType &Properties);
   /** Devuelve los Part que se van a mostrar en las Propiedades. */
   virtual std::vector<Part *> DoGetParts(
         DataViewManager* pDataViewManager = NULL, LayerInterface *pLayer = NULL,
         DatasourceInterface *pDatasource = NULL);
   /** busca en el nodo del elemento, el tipo de dato que se usa para guardar el raster */
   virtual wxString GetTypeAsString();
   /** devuelve la descripcion del tipo de formato */
   virtual wxString GetFormatDescription();
   /** Obtiene el tamano del raster guardado en el nodo */
   virtual void GetRasterSize(int &Width, int &Height);
   /** Devuelve el extent del raster guardado en el nodo. */
   virtual void GetElementExtent(Subset &WorldExtent);
   /** Carga las propiedades de las bandas */
   virtual void InitializeBandProperties(Image *pImage);
   /** Carga la cantidad de bandas del raster */
   virtual void SetBandCount(int BandsCount);
   /**
    * Establece la cantidad de bandas sin modificar el contenido.
    * Solo actualiza la propiedad del elemento "bandas"
    */
   virtual void SetBandCountEx(int BandCount);
   /** Carga el nombre de una banda */
   virtual bool SetBandName(const std::string &BandName, int BandNumber);
   /** Devuelve la cantidad de bandas del raster */
   virtual int GetBandCount() const;
   /** Devuelve el nombre de una banda */
   virtual bool GetBandName(std::string &BandName, int BandNumber) const;
   /** Devuelve las resolucion del pixel */
   virtual double GetPixelRes() const;

   /** Guarda el tamano del raster en el nodo */
   virtual void SetRasterSize(int Width, int Height);

   /**
    * Devuelve el valor de validez de la banda.
    */
   virtual bool GetBandValidity(int& BandValidity, int BandNumber) const;

   /**
    * Establece la validez de una banda.
    */
   virtual bool SetBandValidity(int BandValidity, int BandNumber);

   /**
    * Borra la banda especificada (del XML)
    */
   virtual bool DeleteBand(int BandNumber);


   /**
    * Configura un mundo nuevo para el renderizador.
    */
   World* CreateWorld();

   /** Guarda el extent del raster en el nodo. */
   virtual void SetElementExtent(const Subset &WorldExtent);

protected:

   /** Devuelve vector con Parts especificos de la visualizacion elemento. */
   virtual std::vector<suri::Part*> DoGetVisualizationParts(
         DataViewManager* pDataViewManager = NULL, LayerInterface *pLayer = NULL,
         DatasourceInterface *pDatasource = NULL);

private:
   /** Inicializa RasterElement con archivo en FileName */
   virtual RasterElement* Initialize(const std::string &FileName,
                                     const Option& Metadata = Option());
};
}

#endif /*RASTERELEMENT_H_*/
