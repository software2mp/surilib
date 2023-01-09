/*! \file BandInfo.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef BANDINFO_H_
#define BANDINFO_H_

// Includes Estandar
#include <string>

// Includes Suri

#include <suri/Serializable.h>
// Includes Wx
// Defines
// forwards

namespace suri {

// Forwards
class RasterElement;

/**
 * Clase que contiene informacion sobre la banda en la que se esta
 * trabajando. Contiene el elemento al que pertence, la posicion de
 * la banda en el elemento y el nombre de la misma (para mostrar en
 * pantalla)
 */
class BandInfo : public Serializable {
public:
   /**
    *
    */
   explicit BandInfo();
   /**
    * Ctor.
    */
   BandInfo(suri::RasterElement* pElementP, int PositionP, const std::string& NameP,
            int Width, int Height, const std::string& TypeP);
   /**
    * Copy Ctor.
    */
   BandInfo(const BandInfo& bandInfo);
   /**
    * Dtor
    */
   virtual ~BandInfo();

   /**
    * Devuelve el elemento al que pertenece la banda.
    */
   suri::RasterElement* GetElement() const;

   /**
    * Devuelve la posicion de la banda en el elemento.
    */
   int GetPosition() const;

   /**
    * Devuelve el nombre de la banda (junto con el nombre del elemento)
    */
   std::string GetName() const;

   /**
    * Devuelve el ancho de la banda.
    */
   int GetWidth() const;

   /**
    * Devuelve el alto de la banda.
    */
   int GetHeight() const;

   /**
    * Devuelve el tipo de la banda.
    */
   std::string GetType() const;

   /**
    *
    */
   static std::string ExtractType(suri::RasterElement* pElement);

   wxXmlNode* SerializeXml() const;

   static BandInfo* DeserializeXml(wxXmlNode* pRoot);

private:
   suri::RasterElement* pElement_;  // Elemento al que pertenece la banda.
   int position_;  // posicion de la banda en el elemento.
   std::string name_;  // nombre de la banda.
   int width_;
   int height_;
   std::string type_;

};

} /** namespace suri */

#endif /* BANDINFO_H_ */
