/*! \file SharpeningInputRGBPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SHARPENINGINPUTRGBPART_H_
#define SHARPENINGINPUTRGBPART_H_

#include "SelectionPart.h"
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/SuriObject.h"
#include "suri/RasterElement.h"

namespace suri {

DECLARE_EVENT_OBJECT(SharpeningInputRGBPartEvent);

class SharpeningInputRGBPart : public suri::Part,
                               public suri::ProcessAtributeProvider,
                               public suri::InputBandListener {
public:
   FRIEND_EVENT_OBJECT(SharpeningInputRGBPartEvent);

   static const int kRedRadio = 0;
   static const int kGreenRadio = 1;
   static const int kBlueRadio = 2;

   /**
    * Ctor.
    */
   SharpeningInputRGBPart(RasterElement* pElement, bool Enable = true, bool Modified = false);

   /**
    * Dtor.
    */
   virtual ~SharpeningInputRGBPart();

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
    * Handler para el evento de click del boton de la componente roja.
    */
   void OnRedRadioClicked(wxCommandEvent &Event);

   /**
    * Handler para el evento de click del boton de la componente verde.
    */
   void OnGreenRadioClicked(wxCommandEvent &Event);

   /**
    * Handler para el evento de click del boton de la componente azul.
    */
   void OnBlueRadioClicked(wxCommandEvent &Event);

   /**
    * Handler para el evento de click del checkbox para poder ingresar el
    * valor digital no valido.
    */
   void OnEnableNDVCheckboxClicked(wxCommandEvent &Event);

   /**
    * Metodo a implementar por la clase que recibe las notificaciones
    * para recibir la lista de bandas seleccionadas.
    */
   virtual void OnBandSelected(std::vector<BandInfo> BandIxsP);

private:
   EVENT_OBJECT_PROPERTY(SharpeningInputRGBPartEvent);

   RasterElement* pElement_;
   std::vector<BandInfo> outputRGBBands_;
   bool changed_;
   bool validData_;
   int rgbix_;

   /**
    * Inicializa los botones.
    */
   void InitializeRadioButtons();

   /**
    * Inicializa los textboxes.
    */
   void InitializeTextBoxes();

   /**
    * Obtiene el radio button seleccionado.
    */
   int GetSelectedRadioButtonPos();

   /**
    * Seleccion el radio button especificado.
    */
   void SelectRadioButtonByName(const std::string& Name);

   /**
    * Obtiene todos los elementos raster de baja resolucion.
    */
   std::vector<RasterElement*> GetLowResRasterElements(const std::vector<Element*>& Elements);
};

}  // namespace suri

#endif  // SHARPENINGINPUTRGBPART_H_
