/*! \file TerrainElement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TERRAINELEMENT_H_
#define TERRAINELEMENT_H_

// Includes Estandar
#include <string>
#include <vector>
#include <map>

// Includes suri
#include "suri/Element.h"
#include "Part.h"

// defines
#define TEXTURE_SEPARATION_TOKEN "|"

/** namespace suri */
namespace suri {

class RasterElement;
class DatasourceInterface;
class LayerInterface;
class DatasourceManagerInterface;

/** Element de Terreno (representa un Terreno y una Textura asociada) */
/**
 *  Este elemento representa un modelo de Terreno. Esta compuesto por una
 *  imagen en formato tiff de una banda y una textura, en formato tiff de
 *  tres bandas asociada. Se crea y agrega a la lista del contexto "3D" al
 *  abrir una imagen en un formato de archivo soportado.
 * \note especializa Element para representar Terrain en LayerList,
 *   guardar sus datos en xml e informar cambios a views
 */
class TerrainElement : public Element {
   /** Ctor. de Copia. */
   TerrainElement(const TerrainElement &TerrainElement);

public:
   /** Constructor */
   TerrainElement();
   /** Destructor */
   virtual ~TerrainElement();
   /** retorna un string para el icono */
   virtual wxString DoGetIcon() const;
   /** retorna los detalles del elemento */
   virtual wxString GetDetails() const;
   /** retorna el factor de terreno */
   virtual int GetTerrainFactor();
   /** establece el factor de terreno */
   virtual void SetTerrainFactor(const int Factor);
   /** retorna el factor de textura */
   virtual int GetTextureFactor();
   /** retorna el factor de textura a partir de un elemento*/
   virtual int GetTextureFactor(RasterElement* pRaster);

   /** establece el factor de textura */
   virtual void SetTextureFactor(const int Factor);
   /** retorna el factor de exageracion */
   virtual int GetExagerationFactor();
   /** establece el factor de exageracion */
   virtual void SetExagerationFactor(const int Factor);
   /** obtiene  el color de fondo desde el xml */
   virtual std::string GetBackgroundColor();
   /** guarda el color de fondo en el xml */
   virtual void SetBackgroundColor(const std::string &BackgroundColour);
   /** retorna el paso de camara */
   virtual int GetCameraStep();
   /** establece el paso de camara */
   virtual void SetCameraStep(const int Step);
   /** retorna la calidad del terreno. */
   virtual int GetQuality();
   /** establece la calidad del terreno. */
   virtual void SetQuality(const int Quality);
   /** Cantidad de elementos de la Tabla Calidad */
   virtual int GetQualityCount();
   /** Funcion estatica para crear un TerrainElement */
   static TerrainElement* Create(const std::string &FileName,
                                 const Option& Metadata = Option());
   /** Devuelve vector con Parts especificos del elemento. */
   virtual std::vector<suri::Part*> DoGetParts(DataViewManager* pDataViewManager = NULL,
                                               LayerInterface *pLayer = NULL,
                                               DatasourceInterface *pDatasource = NULL);
   /** Agrega la textura para el Terreno. */
   bool AddTexture(const std::string &FileName);

   /** Quita la textura para el Terreno. */
   bool RemoveTexture(const std::string& FileName);

   /** Valida que la textura y el terreno sean compatibles */
   virtual bool Validate();

   /**
    * Establece que el elemento debe actualizarse obligatoriamente sin importar
    * otras condiciones que harian que no se actualice.
    */
   void ForceUpdate(bool Force = true) { force_ = Force; }

   /**
    * Indica si se el elemento esta obligado a actualizarse.
    */
   bool ForcedUpdate() { return force_; }

private:
   /** Valida que la textura y el terreno tengan el mismo sist de referencia */
   virtual bool ValidateSpatialReference();
   /** Valida que la textura y el terreno tengan extent superpuesto */
   virtual bool ValidateSubsets();
   /** Devuelve factor de terreno o textura. */
   int GetFactor(const int Size, long Multiplier, const bool IsTerrainFactor);
   /** Devuelve un factor de textura valido para el factor de terreno. */
   int GetValidTextureFactor(const int TerrainFactor, const int TextureFactor);
   /** Retorna el id de la clase */
   virtual std::string DoClassId() {
      return "TerrainElement";
   }
   /** Devuelve vector con Parts especificos de la visualizacion elemento. */
   virtual std::vector<suri::Part*> DoGetVisualizationParts(
         DataViewManager* pDataViewManager = NULL, LayerInterface *pLayer = NULL,
         DatasourceInterface *pDatasource = NULL);

public:
   /** Retorna RasterElement de la textura. */
   RasterElement* GetTexture() const;

private:
   bool force_;  // actualizacion obligatoria

   /** Inicializa el terreno con archivo en FileName */
   virtual TerrainElement* Initialize(const std::string &FileName,
                                      const Option& Metadata = Option());
};

}

#endif /*TERRAINELEMENT_H_*/
