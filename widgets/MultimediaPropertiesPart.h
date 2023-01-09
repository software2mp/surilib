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
