/*! \file XmlPropertiesPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
