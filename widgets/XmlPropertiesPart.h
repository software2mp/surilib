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

#ifndef XMLPROPERTIESPART_H_
#define XMLPROPERTIESPART_H_

// Includes de wx
#include "wx/xrc/xmlres.h"
#include "suri/Element.h"
#include "suri/Part.h"

/** namespace suri */
namespace suri {

// forwards suri
class DatasourceInterface;
class LayerInterface;

/** Declara la clase XmlPropertiesPartEvent */
DECLARE_EVENT_OBJECT(XmlPropertiesPartEvent);

/** Part con un wxTextCtrl para mostrar el contenido de los xml */
/**
 * Part que solo aparece en modo debug y permite editar el
 * nodo de los elementos directamente.
 * \note extiende Part para guardar los cambios en Xml
 */
class XmlPropertiesPart : public Part {
   /** Ctor. de Copia. */
   XmlPropertiesPart(const XmlPropertiesPart &XmlPropertiesPart);

public:
   /** Declara friend la clase de evento XmlPropertiesPartEvent */
   FRIEND_EVENT_OBJECT(XmlPropertiesPartEvent);
   /** Ctor.  \deprecated */
   explicit XmlPropertiesPart(Element *pElement);
   /** Ctor.  */
   XmlPropertiesPart(DatasourceInterface* pDatasource, LayerInterface* pLayer);
   /** Dtor. */
   virtual ~XmlPropertiesPart();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Inicializa el valor del text con el Xml del elemento */
   virtual void SetInitialValues();
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Evento UIUpdate */
   void OnUIUpdate(wxUpdateUIEvent &Event);
   /** Obtiene el icono del part */
// virtual void GetWindowIcon(wxBitmap& ToolBitmap,int X=0,int Y=0)const;
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
private:
   Element *pElement_; /*! puntero al Elemento. */
   wxTextCtrl *pTextXml_; /*! text para mostrar el xml. */
   /** Declara la propiedad pEventHandler_ del tipo XmlPropertiesPartEvent* */
   EVENT_OBJECT_PROPERTY(XmlPropertiesPartEvent);
   DatasourceInterface* pDatasource_;
   LayerInterface* pLayer_;
   /** Reemplaza el nodo renderizacion **/
   bool ReplaceElementRenderizationNodes(Element *pElementFrom, Element* pElementDest);
};
}  // namespace suri

#endif /*XMLPROPERTIESPART_H_*/
