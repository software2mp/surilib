/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
