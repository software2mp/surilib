/*! \file Hiddable.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef HIDDABLE_INTERFACE_H_
#define HIDDABLE_INTERFACE_H_

// Includes Estandar

// Includes Suri

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Interfaz que permite manejar objetos ocultables
 */
class HiddableInterface {
public:
   /** Destructor */
   virtual ~HiddableInterface() {}
   /** Oculta o desoculta un objeto */
   virtual void Hide(bool Hide = true)=0;
   /** Verifica si el objeto fue ocultado */
   virtual bool IsHidden() const=0;
};

} /** namespace suri */

#endif /* HIDDABLE_INTERFACE_H_ */
