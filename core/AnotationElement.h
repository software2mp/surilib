/*! \file AnotationElement.h
 *
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ANOTATIONELEMENT_H_
#define ANOTATIONELEMENT_H_

// Includes standard

// Includes Suri
#include "suri/MemoryVectorElement.h"
#include "suri/Coordinates.h"
#include "suri/Part.h"
#include "suri/Element.h"

// Includes Wx

// Includes App

// Defines
// \todo: Mover a xmlnames
// define para el nombre del campo
#define SUR_ANOTATION_FIELD "SURLabel" /*! define para el nombre del campo */
// Nombre para la tabla
#define ANOTATION_TABLE_NAME "anotacion" /*! Nombre para la tabla */
// namespace suri
namespace suri {
/** Clase que representa un elemento Anotacion.
 * Permite:
 *    - obtener su pagina de propiedades.
 *    - setear y obtener el nombre del campo de la base de datos utilizado para
 * guardar el contenido de la anotacion.
 *    - setear y obtener el valor del campo de la base de datos utilizado.
 * \note: hereda de VectorElement debido a que la anotacion es un vector
 * de puntos.
 * \note: el nombre del campo en la base de datos es SURAnotation.
 */
class AnotationElement : public VectorElement {
   /**
    *  ctor. de Copia
    *  @param[in] Anotation Anotation element
    */
   AnotationElement(const AnotationElement &Anotation);

public:
   /**
    *  ctor. default
    */
   AnotationElement();

   /**
    *  Dtor.
    */
   virtual ~AnotationElement();

   /**
    *  Funcion estatica para crear un AnotationElement
    */
   static AnotationElement* Create(const std::string &FileName,
                                   const wxString &Anotation,
                                   const std::string &SpatialReference,
                                   const Coordinates &Position);

   /**
    * Funcion estatica para crear un AnotationElement
    */
   static AnotationElement* Create(const wxString &Filename,
                                   const Option& Metadata = Option());

   /**
    * retorna un string para el icono.
    */
   virtual wxString DoGetIcon() const;

   /**
    *  retorna los detalles del elemento
    */
   virtual wxString GetDetails() const;

   /**
    *  Retorna el tipo de elemento
    *  @return tipo
    */
   virtual wxString GetTypeAsString();

   /**
    *  Obtiene el nombre del campo que contiene el texto de las anotaciones
    */
   wxString GetAnotationFieldName(const int Layer, wxString &FieldName);

   /**
    *  Obtiene el contenido del campo SURAnotation del dbf
    */
   wxString GetAnotationFieldValue();

   /**
    *  Setea el nombre del campo que contiene el texto de las anotaciones
    */
   void AddLabelNode(const int Layer, const wxString &FieldName);

   /**
    * Setea el nombre del campo que contiene el texto de las anotaciones
    */
   void AddLabelNode(const wxString &FieldName);

   /**
    * Devuelve los Part que se van a mostrar en las Propiedades.
    */
   virtual std::vector<Part *> DoGetParts();

   /**
    *  Verifica la presencia del campo de anotacion
    */
   bool HasAnotationField();

   /**
    *  Sobreescrito para mantener sincronizacion entre el nombre y la anotacion.
    *  @param[in] ElementName nombre del elemento
    */
   virtual void SetName(const wxString& ElementName);

private:
   /**
    *  Crea e inicializa un elemento anotacion
    *  @param[in] FileName nombre de archivo
    */
   virtual AnotationElement* Initialize(const std::string &FileName,
                                        const Option& Metadata = Option());

protected:
   /**
    *  Establece el contenido del campo SURAnotation del dbf
    */
   void SetAnotationFieldValue(const wxString &Value);
};
}  // end  namespace suri

#endif /* ANOTATIONELEMENT_H_ */
