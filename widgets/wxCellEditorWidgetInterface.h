/*! \file wxCellEditorWidgetInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXCELLEDITORWIDGETINTERFACE_H_
#define WXCELLEDITORWIDGETINTERFACE_H_

// Includes Estandar
#include <string>

// Includes Suri

// Includes Wx
// Defines
// forwards

namespace suri {

/** Interfaz que representa el comportamiento esperado de un widget encargado de
 * editar el contenido de una celda. El resultado de la edición se transmitira
 * mediante un string (array de caracteres). **/
class wxCellEditorWidgetInterface {
public:
   wxCellEditorWidgetInterface() {}
   virtual ~wxCellEditorWidgetInterface() {}
   /** Metodo virtual que inicia la edicion del atributo de la celda **/
   virtual bool BeginEdition()=0;
   /** Obtiene el valor resultante de la edicion de la grilla */
   virtual std::string GetValue()=0;
   /** Configura el valor a editar **/
   virtual void SetValue(const std::string& Value)=0;
};

} /** namespace suri */

#endif /* WXCELLEDITORWIDGETINTERFACE_H_ */
