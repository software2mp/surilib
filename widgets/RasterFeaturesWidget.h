/*! \file RasterFeaturesWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERFEATURESWIDGET_H_
#define RASTERFEATURESWIDGET_H_

#include "RasterOffsetConfigurationWidget.h"
#include "suri/ParameterCollection.h"
#include "suri/Widget.h"

namespace suri {

DECLARE_EVENT_OBJECT(RasterFeaturesWidgetEvent);

class RasterFeaturesWidget : public suri::Widget {
public:
   FRIEND_EVENT_OBJECT(RasterFeaturesWidgetEvent);

   static const std::string kFeatureBandHeaderOffset;
   static const std::string kFeatureBandTailOffset;
   static const std::string kFeatureBandCount;
   static const std::string kFeatureDataType;
   static const std::string kFeatureFileHeaderOffset;
   static const std::string kFeatureFileTailOffset;
   static const std::string kFeatureFormat;
   static const std::string kFeatureLineHeaderOffset;
   static const std::string kFeatureLineTailOffset;
   static const std::string kFeatureMux;

   /**
    * Ctor.
    */
   RasterFeaturesWidget();

   /**
    * Dtor.
    */
   ~RasterFeaturesWidget();

   /**
    * Se crea la ventana de herramienta en las clases derivadas.
    */
   virtual bool CreateToolWindow();

   /**
    * Establece los parametros de entrada y salida.
    */
   void SetIOParams(ParameterCollection* pParams);

   /**
    * Indica si hubo cambios en el control.
    */
   void SetChangedFlag(bool* pChanged);

private:
   EVENT_OBJECT_PROPERTY(RasterFeaturesWidgetEvent);

   bool* pChanged_;
   RasterOffsetConfigurationWidget::RasterOffsetInfo offsetInfo_;
   ParameterCollection* pIOParams_;

   /**
    * Manejador para el evento click del boton offset.
    */
   void OnOffsetClicked(wxCommandEvent& Event);

   /**
    * Manejador para el evento de cambio de opcion en alguno
    * de los choices.
    */
   void OnOptionsChanged(wxCommandEvent& Event);

   /**
    * Inicializa los controles con los datos disponibles.
    */
   void InitializeValues();

   /**
    * Actualiza los valores de los parametros de salida.
    */
   void SavaValues();
};

}  // namespace suri

#endif  // RASTERFEATURESWIDGET_H_
