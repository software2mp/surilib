/*! \file Choice.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CHOICE_H_
#define CHOICE_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/Widget.h"

// Includes Wx
// Defines
// forwards

namespace suri {
// forwards
class LabelText;

// forwards
DECLARE_EVENT_OBJECT(ChoiceEventHandler);

/*!
 * Clase que representa un control grafico de tipo choice.
 * Permite obtener e indicar la seleccion.
 */
class Choice : public Widget {
   /** Constructor copia **/
   Choice(const Choice&);

public:
   /** Constructor **/
   Choice(wxWindow *pViewerToolbar, const std::string& Tooltip, int Flag);
   /** Destructor **/
   virtual ~Choice();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Obtiene la seleccion activa en el control grafico. */
   std::string GetSelection() const;
   /** Indica la seleccion activa en el control grafico. */
   void SetSelection(const std::string& Selection);
   /** Setea la propiedad enabled_ */
   virtual void SetEnabled(bool Enabled = true);
   /** Verifica si el boton se puede o no habilitar */
   virtual bool IsEnabled() const;
   /** Agrega una opcion al choice **/
   virtual void AddChoiceOption(const std::string& Option);
   /** Metodo que indica si posee un label asociado **/
   bool HasLabel() const;
   /** Metodo que obtiene el label **/
   LabelText* GetLabel() const;

protected:
   EVENT_OBJECT_PROPERTY(ChoiceEventHandler);
   FRIEND_EVENT_OBJECT(ChoiceEventHandler);
   /** Metodo que se llama cuando cambia la seleccion en el control grafico.  */
   virtual void OnSelectionChanged(wxCommandEvent &Event);
   /** Metodo que se llama cuando se ejecuta CreateToolWindow **/
   virtual void OnCreateToolWindow();
   /** Referencia al control grafico **/
   wxChoice* pChoice_;
   /** bool que indica si esta habilitado o no el choice **/
   bool enabled_;
   /** label asociado al control grafico **/
   std::string label_;
};

} /** namespace suri */

#endif /* CHOICE_H_ */
