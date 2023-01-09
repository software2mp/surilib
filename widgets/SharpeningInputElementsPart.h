/*! \file SharpeningInputElementsPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SHARPENINGINPUTELEMENTSPART_H_
#define SHARPENINGINPUTELEMENTSPART_H_

#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"

namespace suri {

DECLARE_EVENT_OBJECT(SharpeningInputElementsPartEvent);

class SharpeningInputElementsPart : public Part, public ProcessAtributeProvider {
public:
   FRIEND_EVENT_OBJECT(SharpeningInputElementsPartEvent);

   /**
    * Ctor.
    */
   SharpeningInputElementsPart(bool Enable = true, bool Modified = false);

   /**
    * Dtor.
    */
   virtual ~SharpeningInputElementsPart();

   /**
    * Indica si el Part tiene cambios para salvar.
    */
   virtual bool HasChanged();

   /**
    * Salva los cambios realizados en el Part.
    */
   virtual bool CommitChanges();

   /**
    * Restaura los valores originales del Part.
    */
   virtual bool RollbackChanges();

   /**
    * Inicializa el part.
    */
   virtual void SetInitialValues();

   /**
    * Devuelve si la parte tiene datos validos.
    */
   virtual bool HasValidData();

   /**
    * Actualiza el estado de la parte.
    */
   virtual void Update();

private:
   EVENT_OBJECT_PROPERTY(SharpeningInputElementsPartEvent);

   bool changed_;
   int rgbix_;
   int panix_;

   /**
    * Inicializa los combos.
    */
   void InitializeChoices();

   /**
    * Carga los combos.
    */
   void InitializeChoicesContent();

   /**
    * Handler para el evento de seleccion de item del choice
    * de seleccion del elemento para composicion RGB.
    */
   void OnRGBChoiceSelected(wxCommandEvent &Event);

   /**
    * Handler para el evento de seleccion de item del choice
    * de seleccion del elemento para composicion RGB.
    */
   void OnPanChoiceSelected(wxCommandEvent &Event);
};

}  // namespace suri

#endif  // SHARPENINGINPUTELEMENTSPART_H_
