/*! \file MapToolSetupPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef __MAP_TOOL_SETUP_PART_H__
#define __MAP_TOOL_SETUP_PART_H__

#include "suri/Part.h"
#include "suri/DataViewManager.h"
namespace suri {

/** Clase encargada de administrar la configuracion general de la aplicacion **/
class MapToolSetupPart : public Part {
   /** constructor copia **/
   explicit MapToolSetupPart(const MapToolSetupPart&);

public:
   /** Ctor. */
   explicit MapToolSetupPart(DataViewManager* pDataViewManager = NULL);
   /** Destructor. */
   virtual ~MapToolSetupPart();
   /** Crea el part (el interior de la ventana)*/
   virtual bool CreateToolWindow();
   /** Asigna el icono para el part.*/
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Inicializa los controles*/
   virtual void SetInitialValues();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();

   virtual void Update() {
   }

private:
   DataViewManager* pDataViewManager_;
   wxString nativeFontInfo_;
   std::string titleText_;
   int titleTextStyle_;
   int titleTextSize_;
   int titleFamily_;
   std::string TitleColour_;
   std::string legendBorderColour_;
   std::string legendBackgroundColour_;
   std::string northPath_;
};

} /* namespace suri */

#endif   // __MAP_TOOL_SETUP_PART_H__
