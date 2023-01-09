/*! \file TmsXmlWriterBuilder.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TMSXMLWRITERBUILDER_H_
#define TMSXMLWRITERBUILDER_H_

namespace suri {

class TmsXmlWriter;

class TmsXmlWriterBuilder {
public:
   /**
    * Dtor.
    */
   virtual ~TmsXmlWriterBuilder() { }

   /**
    * Construye la cadena de reponsabilidad con todas las TmsXmlWriter disponibles.
    */
   static TmsXmlWriter* Build();

   /**
    * Libera la memoria reservada por la cadena (metodo utilizado por el codigo cliente).
    */
   static void Release(TmsXmlWriter*& Factory);

private:
   /**
    * Ctor (Esta clase no debe ser instanciable).
    */
   TmsXmlWriterBuilder() { }
};

}  // namespace suri

#endif   // TMSXMLWRITERBUILDER_H_
