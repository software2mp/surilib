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

#include "MarkButton.h"

// Includes standard
// Includes suri
#include "suri/ViewerWidget.h"
#include "suri/LatitudeLongitudeInputWidget.h"
#include "suri/messages.h"
#include "suri/Coordinates.h"
#include "suri/Subset.h"
#include "VectorElementEditor.h"
#include "resources.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] pToolbar toolbar donde se inserta el boton
 * @param[in] pViewer puntero al viewer
 * @param[in] pVectorElementEditor herramienta para la edicion
 */
MarkButton::MarkButton(wxWindow *pToolbar, ViewerWidget *pViewer,
                       VectorElementEditor* pVectorElementEditor) :
      Button(pToolbar, wxT(button_GPS_MARK), wxT(tooltip_MARK)), pViewer_(pViewer),
      pVectorElementEditor_(pVectorElementEditor) {
}

/** dtor */
MarkButton::~MarkButton() {
}

/**
 * Obtiene coordenadas del NavigationWidget y se las agrega al vector en
 * edicion.
 * @param[in] Event evento click sobre el boton
 */
void MarkButton::OnButtonClicked(wxCommandEvent &Event) {
   if (!pViewer_->GetNavigator()->GetWidget()) {
      return;
   }

   Coordinates point;
   if (pViewer_->GetNavigator()->GetWidget()->GetCoordinate(point)) {
      // Transformo coordenadas del NavigationWidget a coordenadas de mundo
      ParameterCollection params;
      params.AddValue<std::string>(TransformationFactory::kParamWktIn, LATLON_SPATIAL_REFERENCE);
      params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                   pViewer_->GetWorld()->GetSpatialReference().c_str());
      TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
      CoordinatesTransformation *pcoordtransform = pfactory->Create(TransformationFactory::kExact,
                                                        params);
      TransformationFactoryBuilder::Release(pfactory);

      if (!pcoordtransform) {
         REPORT_DEBUG("D:Error al crear coordinates transform");
         return;
      }
      pcoordtransform->Transform(point);

      // Si las coordenadas estan fuera de mundo muestro msg
      Subset worldsubset;
      pViewer_->GetWorld()->GetWorld(worldsubset);
      if (!worldsubset.IsInside(point)) {
         SHOW_STATUS(message_POINT_OUTSIDE_IMAGE);
      }
      // Agrego punto a vector
      pVectorElementEditor_->AddPoint(point);
      delete pcoordtransform;
   } else {
      REPORT_DEBUG("D:Coordenadas incorrectas");
   }
}
}  // namespace suri
