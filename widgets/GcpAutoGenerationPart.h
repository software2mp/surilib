/*! \file GcpAutoGenerationPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
