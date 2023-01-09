/*! \file GenericPropertiesPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
