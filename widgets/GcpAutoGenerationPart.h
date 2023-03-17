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

#ifndef GCPAUTOGENERATIONPART_H_
#define GCPAUTOGENERATIONPART_H_

// Includes Estandar
// Includes Suri
#include "suri/GcpAutoGenerationProcess.h"
#include "Viewer2DSelectorPart.h"
// Includes Wx
// Defines
// forwards

class wxCommandEvent;
class wxSpinCtrl;
class wxSlider;
class wxFilePickerCtrl;

namespace suri {

DECLARE_EVENT_OBJECT(GcpAutoGenerationPartEvent);

class GcpAutoGenerationPart : public Viewer2DSelectorPart {
   /** Ctor. de Copia. */
   GcpAutoGenerationPart(const GcpAutoGenerationPart &GcpAutoGenerationPart);
   /** Captura eventos de los spincontrol **/
   FRIEND_EVENT_OBJECT(GcpAutoGenerationPartEvent);
public:
   /**
    * Ctor
    */
   GcpAutoGenerationPart(bool Enable = true, bool Modified = false);
   /**
    * Destructor
    */
   ~GcpAutoGenerationPart();
   /** Inicializa el part **/
   void Initialize();
   /**
    * Salva los cambios realizados en el Part.
    */
   virtual bool CommitChanges();
   /**
    * Devuelve si la parte tiene datos validos
    */
   virtual bool HasValidData();
   
   /**
    * Handler para el evento de cambio de seleccion de choice base.
    * Actualiza las bandas disponibles para GCPs candidatos en el autocorregistro.
    */
   virtual void OnChBaseSelectionChanged(wxCommandEvent& Event);
   /**
    * Handler para el evento de cambio de seleccion de choice warp.
    * Actualiza las bands disponibles para imagen warp en el autocorregistro.
    */
   virtual void OnChWarpSelectionChanged(wxCommandEvent& Event);
   
   /** Captura el cambio en el numero de banda de la imagen base a usar para corregistrar */
   void OnBaseBandValueChanged(wxCommandEvent &Event);
   
   /** Captura el cambio en el numero de banda de la imagen base a usar para corregistrar */
   void OnWarpBandValueChanged(wxCommandEvent &Event);
   
   /** Captura el cambio en el archivo de salida de gcps */
   void OnGcpFileChanged(wxCommandEvent &Event);
   
   /** Captura el cambio en el nivel de agresividad del algoritmo */
   void OnAggressionLevelChanged(wxCommandEvent &Event);
   /**
    * Configura el numero maximo de bandas de cada viewer.
    */
   void SetViewerBands(const std::map<SuriObject::UuidType, int>& ViewerBands);

   /** Indica el valor maximo que puede tomar el spinner de banda base **/
   void SetBaseMaxBand(int maxBand);
   /** Indica el valor maximo que puede tomar el spinner de banda warp **/
   void SetWarpMaxBand(int maxBand);
   /**
    * Devuelve los parametros configurados para el proceso de autocorregistro
    */
   GcpAutoGenerationProcess::Parameters GetParameters() const;
   /** Conecta eventos de la UI **/
   void ConnectEvents();
private:
   /** Retorna el spin control correspondiente al nombre **/
   wxSpinCtrl* GetSpinCtrl(const char* ControlName);
   /** Retorna el filePicker correspondiente al archivo de gcps **/
   wxFilePickerCtrl* GetFilePicker();
   /** Retorna el slider de agresividad del algoritmo **/
   wxSlider* GetSlider();
   int selectedBaseBand_;
   int selectedWarpBand_;
   int aggressionLevel_;
   std::string gcpFilePath_;
   static const char* kBaseBandCtrlName;
   static const char* kWarpBandCtrlName;
   static const char* kGcpFilePickerCtrlName;
   static const char* kAgressionSpinCtrlName;
   EVENT_OBJECT_PROPERTY(GcpAutoGenerationPartEvent);
   std::map<SuriObject::UuidType, int> viewerBands_;
};   
} /** namespace suri */

#endif /* GCPAUTOGENERATIONPART_H_ */
