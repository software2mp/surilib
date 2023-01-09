// <COPYRIGHT>

#ifndef WARPTRANSFORM_H_
#define WARPTRANSFORM_H_

// Includes standard

// Includes Suri
#include "suri/Coordinates.h"
// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/**
 * Una clase que representa una funcion de transformacion (x,y)=>(x',y').
 * Se espera que los coeficientes de la funcion puedan ser calculados a
 * partir de una serie de puntos de control, o puedan ser cargados manualmente
 * para obtener la transformacion de una serie de puntos automaticamente.
 */
class WarpTransform {
public:
   WarpTransform();
   virtual ~WarpTransform();

   /**
    *Permite determinar si se pudo determinar la funcion de transformacion.
    *@return bool indicando el exito de la determinacion.
    */
   virtual bool IsOk() = 0;
   /**
    *Permite limpiar los coeficientes y demas parametros de la funcion de
    *transformacion
    */
   virtual void Clear() = 0;
   /**
    * Mapea la lista de coordenadas de entrada a la de salida, aplicando
    * la funcion de transformacion calculada previamente
    * @return bool indicando el exito de la aplicacion de la funcion de
    * transformacion.
    * @param[in] Origin Lista de cordenadas origen.
    * @param[out] Destination Lista de coordenadas de destino
    */
   virtual bool ApplyTransform(const CoordinateListType &Origin,
                               CoordinateListType &Destination) = 0;
   /**
    * Mapea la coordenada de entrada a la coordenada de salida,
    * aplicando la funcion de transformacion calculada previamente
    * @return bool indicando el exito de la aplicacion de la funcion de
    * transformacion
    * @param[in] Origin: Coordenada de origen
    * @param[out] Destination: Coordenada de destino
    */
   virtual bool ApplyTransform(const Coordinates &Origin, Coordinates &Destination) = 0;
};
}
#endif /* WARPTRANSFORM_H_ */

