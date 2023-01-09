/*! \file SharpeningInputPanPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SHARPENINGINPUTPANPART_H_
#define SHARPENINGINPUTPANPART_H_

#include "SelectionPart.h"
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/SuriObject.h"
#include "suri/RasterElement.h"

namespace suri {

DECLARE_EVENT_OBJECT(SharpeningInputPanPartEvent);

class SharpeningInputPanPart : public suri::Part,
                               public suri::ProcessAtributeProvider,
                               public suri::InputBandListener {
public:
   FRIEND_EVENT_OBJECT(SharpeningInputPanPartEvent);

   /**
    * Ctor.
    */
   SharpeningInputPanPart(RasterElement* pElement, bool Enable = true, bool Modified = false);

   /**
    * Dtor.
    */
   virtual ~SharpeningInputPanPart();

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

   /**
    * Crea la ventana de la parte.
    */
   virtual bool CreateToolWindow();

   /**
    * Retorna el icono de la herramienta.
    */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

   /**
    * Metodo a implementar por la clase que recibe las notificaciones
    * para recibir la lista de bandas seleccionadas.
    */
   virtual void OnBandSelected(std::vector<BandInfo> BandIxsP);

private:
   EVENT_OBJECT_PROPERTY(SharpeningInputPanPartEvent);

   RasterElement* pElement_;
   BandInfo band_;
   bool changed_;
   int panix_;

   /**
    * Inicializa el textbox.
    */
   void InitializeTextBox();

   /**
    * Devuelve el elemento de alta resolucion.
    */
   RasterElement* GetHiResRasterElement(const std::vector<Element*>& Elements);
};

}  // namespace suri

#endif  // SHARPENINGINPUTPANPART_H_
