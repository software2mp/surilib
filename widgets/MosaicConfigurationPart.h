/*! \file MosaicConfigurationPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MOSAICCONFIGURATIONPART_H_
#define MOSAICCONFIGURATIONPART_H_

// Includes standard
#include <string>
#include <vector>

// Includes Suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/Element.h"
// Includes Wx
// Includes wx
#include "wx/listctrl.h"
// Includes App

/** namespace suri */
namespace suri {
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(MosaicConfigurationPartEvent);

class MosaicConfigurationPart : public Part, public ProcessAtributeProvider {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(MosaicConfigurationPartEvent);
   /** Ctor. */
   MosaicConfigurationPart(std::vector<Element*> Selected);
   /** Dtror. */
   virtual ~MosaicConfigurationPart();
   /** Crea la ventana de la parte */
   bool CreateToolWindow();
   /** Devuelve el estado del Part. */
   bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   bool CommitChanges();
   /** Restaura los valores originales del Part. */
   bool RollbackChanges();
   /** Carga los valores iniciales */
   void SetInitialValues();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Evento UIUpdate */
   void OnUiUpdate(wxUpdateUIEvent &Event);
   /** Verifica que tenga datos validos */
   bool HasValidData();
   /** Metodo que se ejecuta con el boton para subir imagenes */
   void OnUpButton(wxCommandEvent &Event);
   /** Metodo que se ejecuta con el boton para bajar imagenes */
   void OnDownButton(wxCommandEvent &Event);
   /** Metodo que se ejecuta cuando se le da doble click a una imagen de la lista **/
   void OnElementActivate(wxCommandEvent &Event);
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
protected:
private:
   void SetElementsOrder();
   void ActiveElement(const wxString& Name);
   int GetPixelDistance();
   unsigned char GetNoDataColour();
   wxListCtrl *imagelist_;
   std::map<wxString, bool> activeElements_;
   std::vector<Element*> selected_;
   EVENT_OBJECT_PROPERTY(MosaicConfigurationPartEvent); /*! Objeto para conectar */
   int pixelDistance_;
   unsigned char noDataColour_;
};
}  // namespace suri
#endif /* MOSAICCONFIGURATIONPART_H_ */
