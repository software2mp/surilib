/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

//<COPYRIGHT> a reemplazar a futuro

#ifndef MULTIMEDIAPROPERTIESPART_H_
#define MULTIMEDIAPROPERTIESPART_H_

//Includes standard

//Includes Suri
#include "suri/Part.h"
#include "suri/Element.h"

//Includes Wx

//Includes App
#include "MultimediaElement.h"

//Defines

//Forwards

//-------------------------------MultimediaPropertiesPart--------------------------------
DECLARE_EVENT_OBJECT(MultimediaPropertiesPartEvent);

//! Clase que permite visualizar y editar las propiedades de un elemento Foto.
/**
 * Permite visualizar y editar las propiedades
 * especificas de un elemento del contextos de Fotografia.
 * Permite editar el titulo y la descripcion de la foto que luego se despliega
 * en un html formado por los datos mencionados y la foto en cuestion.
 */
class MultimediaPropertiesPart : public suri::Part {
   //! Ctor. de Copia.
   MultimediaPropertiesPart(const MultimediaPropertiesPart &MultimediaPropertiesPart);

public:
   FRIEND_EVENT_OBJECT(MultimediaPropertiesPartEvent);
   //! Ctor.
   explicit MultimediaPropertiesPart(suri::MultimediaElement *pElement, bool ShowPreviewSelector = true);
   //! Dtor.
   ~MultimediaPropertiesPart();
   //! Se crea la ventana de herramienta en las clases derivadas
   virtual bool CreateToolWindow();
   //! Inicializa los controles
   virtual void SetInitialValues();
   //! Devuelve el estado del Part.
   virtual bool IsEnabled();
   //! Indica si el Part tiene cambios para salvar.
   virtual bool HasChanged();
   //! Salva los cambios realizados en el Part.
   virtual bool CommitChanges();
   //! Restaura los valores originales del Part.
   virtual bool RollbackChanges();
   //! Evento UIUpdate
   virtual void OnUIUpdate(wxUpdateUIEvent &Event);
   //! Obtiene el icono del part
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   //! Actualiza el estado de la parte
   virtual void Update() {
   }
protected:
private:
   suri::MultimediaElement *pElement_;  ///<elemento fotografia.
   wxTextCtrl *pTextTitle_;  ///< text para el titulo
   wxTextCtrl *pTextDescription_;  ///< text para el descricion
   EVENT_OBJECT_PROPERTY(MultimediaPropertiesPartEvent);
   bool showImageSelector_;
};

void InitMultimediaPropertiesXmlResource();

#endif /*MULTIMEDIAPROPERTIESPART_H_*/
