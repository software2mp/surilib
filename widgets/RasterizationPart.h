/*! \file RasterizationPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */




#ifndef RASTERIZATIONPART_H_
#define RASTERIZATIONPART_H_

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
DECLARE_EVENT_OBJECT(RasterizationPartEvent);

class RasterizationPart : public Part, public ProcessAtributeProvider {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(RasterizationPartEvent);
   /** Ctor. */
   RasterizationPart(std::vector<Element*> Selected);
   /** Dtror. */
   virtual ~RasterizationPart();
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
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
protected:
private:
   void ConfigureDataTypeChoice();
   std::vector<std::string> dataTypeNames_;
   std::string attributeSelection_;
   std::string typeSelection_;
   wxChoice* pDNChoice_;
   wxChoice* pDataTypeChoice_;
   EVENT_OBJECT_PROPERTY(RasterizationPartEvent); /*! Objeto para conectar */
};
}  // namespace suri
#endif /* RASTERIZATIONPART_H_ */
