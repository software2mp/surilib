/*! \file Setup3DPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SETUP3DPART_H_
#define SETUP3DPART_H_

#include "suri/Part.h"

namespace suri {

DECLARE_EVENT_OBJECT(Setup3DPartEvent);

/** Clase encargada de administrar la configuracion de las opciones de 3D en la aplicacion **/
class Setup3DPart : public Part {
public:
   /** Declara friend la clase TerrainPropertiesPartEvent */
   FRIEND_EVENT_OBJECT(Setup3DPartEvent);
   /* Ctor. */
   Setup3DPart();
   /* Destructor. */
   virtual ~Setup3DPart();
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
   /** Evento de movimiento de sliders */
   void OnSliderScroll(wxScrollEvent &Event);
   int iquality_;
   int iexageration_;
   int icamera_;
   EVENT_OBJECT_PROPERTY(Setup3DPartEvent); /*! Objeto que conecta */
};

} /* namespace suri */

#endif /* SETUP3DPART_H_ */
