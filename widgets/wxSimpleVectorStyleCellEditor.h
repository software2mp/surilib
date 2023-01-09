/*! \file wxSimpleVectorStyleCellEditor.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXSIMPLEVECTORSTYLECELLEDITOR_H_
#define WXSIMPLEVECTORSTYLECELLEDITOR_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "wxCellEditorWidgetInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
namespace ui {
   class SimpleVectorStylePart;
}

/** Widget para edicion de estilos simples dentro de una tabla **/
class wxSimpleVectorStyleCellEditor : public suri::wxCellEditorWidgetInterface {
public:
   wxSimpleVectorStyleCellEditor();
   virtual ~wxSimpleVectorStyleCellEditor();
   /** Metodo virtual que inicia la edicion del atributo de la celda **/
   virtual bool BeginEdition();
   /** Obtiene el valor resultante de la edicion de la grilla */
   virtual std::string GetValue();
   /** Configura el valor a editar **/
   virtual void SetValue(const std::string& Value);

protected:
   std::string currentValue_;
   ui::SimpleVectorStylePart* pStyleEditor_;
};

} /** namespace suri */

#endif /* WXSIMPLEVECTORSTYLECELLEDITOR_H_ */
