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

#ifndef GENERICPROPERTIESPART_H_
#define GENERICPROPERTIESPART_H_

// Includes suri
#include "suri/Element.h"
#include "suri/Part.h"


/** namespace suri */
namespace suri {

// forwards
class DatasourceManagerInterface;
class DatasourceInterface;

/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(GenericPropertiesPartEvent);

/** Part con controles para las propiedades comunes a los Elementos */
/**
 * Contiene controles para metodos GetName,GetCopyRight,GetUrl,GetSizeAsString,
 * GetDescription del Elemento.
 * Muestra y permite modificar estas propiedades del elemento.
 * \note extiende Part para guardar los cambios en propiedades del generales
 *       de elemento
 */
class GenericPropertiesPart : public Part {
   /** Ctor. de Copia. */
   GenericPropertiesPart(const GenericPropertiesPart &GenericPropertiesPart);

public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(GenericPropertiesPartEvent);

   /** Ctor. \deprecated*/
   GenericPropertiesPart(Element *pElement);

   /** Ctor */
   GenericPropertiesPart(
         DatasourceManagerInterface* pDatasourceManager, DatasourceInterface* pDatasource);
   /** Dtor. */
   virtual ~GenericPropertiesPart();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Inicializa los controles */
   virtual void SetInitialValues();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Evento UIUpdate */
   void OnUIUpdate(wxUpdateUIEvent &Event);
   /** Evento de presion del boton de cambio de icono */
   void OnChangeIconButtonPressed();
   /** Obtiene el icono del part */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
protected:
private:
   Element *pElement_; /*! elemento que se esta configurando */
   wxString selectedIcon_; /*! archivo seleccionado para el icono */
   EVENT_OBJECT_PROPERTY(GenericPropertiesPartEvent); /*! Objeto que conecta */
   /* los eventos. */

private:
   /** */
   DatasourceManagerInterface* pDatasourceManager_;
   /** */
   DatasourceInterface* pDatasource_;
};
}  // namespace

#endif /*GENERICPROPERTIESPART_H_*/
