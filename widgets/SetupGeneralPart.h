/*! \file SetupGeneralPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SETUPGENERALPART_H_
#define SETUPGENERALPART_H_

#include "suri/Part.h"

namespace suri {

/** Clase encargada de administrar la configuracion general de la aplicacion **/
class SetupGeneralPart : public Part {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(SetupGeneralPartEvent);

   /** Ctor. */
   SetupGeneralPart(bool restrictedmode = false);
   /** Destructor. */
   virtual ~SetupGeneralPart();
   /** Crea el part (el interior de la ventana)*/
   virtual bool CreateToolWindow();
   /** Asigna el icono para el part.  */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Inicializa los controles */
   virtual void SetInitialValues();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Actualiza el estado de la parte */
   virtual void Update();

private:

   wxString language_; // Lenguaje
   wxString workingdirectory_; // Directorio de trabajo.
   wxString visualizationdirectory_; // Directorio donde se guardan las visualizaciones.
   int      zoomfactor_; // Zoom factor.
   wxString coordgeo_; // formato coordenadas geo.
   wxString coordmts_; // formato coordenadas metros.
   bool restrictedmode_; // Deshabilita opciones que estan de más para esa aplición.
};

} /* namespace suri */

#endif /* SETUPGENERALPART_H_ */
