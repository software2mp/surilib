/*! \file MarkButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
