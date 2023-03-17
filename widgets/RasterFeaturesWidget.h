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
