/*! \file TasseledCapTransformationPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TASSELEDCAPTRANSFORMATIONPART_H_
#define TASSELEDCAPTRANSFORMATIONPART_H_

#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/RasterElement.h"

#include "wx/clntdata.h"

namespace suri {

class SensorClientData : public wxClientData {
public:
   /**
    * Ctor.
    */
   explicit SensorClientData(const std::string& Sensor) : sensor_(Sensor) { }

   /**
    * Dtor.
    */
   virtual ~SensorClientData() { }

   /**
    * Devuelve el nombre del sensor.
    */
   std::string GetSensor() { return sensor_; }

private:
   std::string sensor_;

};

DECLARE_EVENT_OBJECT(TasseledCapTransformationPartEvent);

class TasseledCapTransformationPart : public suri::Part, public suri::ProcessAtributeProvider {
public:
   FRIEND_EVENT_OBJECT(TasseledCapTransformationPartEvent);

   /**
    * Ctor.
    */
   TasseledCapTransformationPart(RasterElement* pElement, bool Enable = true, bool Modified = false);

   /**
    * Dtor.
    */
   virtual ~TasseledCapTransformationPart();

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
    * Implementacion del metodo de creacion de ventana de Widget.
    */
   virtual bool CreateToolWindow();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

private:
   EVENT_OBJECT_PROPERTY(TasseledCapTransformationPartEvent);

   RasterElement* pElement_;
   bool changed_;
   std::vector<SensorClientData*> sensorClientData_;
   std::string selectedSensor_;
   std::vector<int> selectedBands_;
   std::string commitSensor_;
   std::vector<int> commitBands_;

   /**
    * Handler para el evento de cambio de sensor.
    */
   void OnCbSensorChanged(wxCommandEvent &Event);

   /**
    * Handler para el evento de check de la lista.
    */
   void OnChkBandsToggled(wxCommandEvent &Event);

   /**
    * Carga el combo de transformaciones.
    */
   void LoadCbSensor();

   /**
    * Carga selector de bandas de salida.
    */
   void LoadPnlOutputBands(bool CheckAll = true);
};

}  // namespace suri

#endif  // TASSELEDCAPTRANSFORMATIONPART_H_
