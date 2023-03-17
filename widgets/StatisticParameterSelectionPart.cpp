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

// Includes standard
#include <string>
#include <sstream>
#include <map>

// Includes suri
#include "suri/StatisticsExporterProcess.h"
#include "StatisticParameterSelectionPart.h"
#include "wxmacros.h"
#include "suri/RasterElement.h"
#include "suri/RasterSpatialModel.h"
#include "suri/AuxiliaryFunctions.h"
#include "SpatialReference.h"
#include "suri/DatasourceManagerInterface.h"
#include "ClassifiedRasterDatasourceValidator.h"
#include "ClassifiedRasterDatasourceManipulator.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"

namespace suri {

/** Nombre del XRC que representa la ventana. */
const std::string StatisticParameterSelectionPart::STATISTIC_PARAMETER_SELECTION_XRC_NAME =
      wxT("ID_STATISTIC_PARAMETER_SELECTION_PANEL");

/**
 * Ctor. Carga el XRC de la pantalla
 * @param[in] pDatasourceManager administrador de fuente de datos de la aplicacion
 * @param[in] DatasourceId id de la fuente de datos
 * @param[in] EnableBasicStats bool que indica si se desea activar los parametros basicos
 */
StatisticParameterSelectionPart::StatisticParameterSelectionPart(
                                 DatasourceManagerInterface* pDatasourceManager,
                                 SuriObject::UuidType DatasourceId,
						 					bool EnableBasicParams) :
      Part(STATISTIC_PARAMETER_SELECTION_XRC_NAME, "Estadisticas"), 
		enableBasicParams_(EnableBasicParams){
   showAsArea_ = false;
   basicStatistics_ = false;
   histogram_ = false;
   crossBandStatistics_ = false;

   pDatasourceManager_ = pDatasourceManager;
   datasourceId_ = DatasourceId;
}

/** Dtor */
StatisticParameterSelectionPart::~StatisticParameterSelectionPart() {
}

/**
 * Indica si el Part tiene cambios para salvar.
 * @return bool que indica si hay cambios
 */
bool StatisticParameterSelectionPart::HasChanged() {
   return !(ReadParametersFromWidget()
                 && showAsArea_ == widgetShowAsArea_
                 && histogram_ == widgetHistogram_
                 && crossBandStatistics_ == widgetCrossBandStatistics_
                 && basicStatistics_ == widgetBasicStatistics_);
}

/**
 * Salva los cambios realizados en el Part.
 * @return bool que indica si pudo guardar los cambios
 */
bool StatisticParameterSelectionPart::CommitChanges() {
   if (!ReadParametersFromWidget()) return false;

   showAsArea_ = widgetShowAsArea_;
   basicStatistics_ = widgetBasicStatistics_;
   crossBandStatistics_ = widgetCrossBandStatistics_;
   histogram_ = widgetHistogram_;
   if (pPal_) {
      std::map<std::string, std::string> options = GetOutputOptions();
      pPal_->AddAttribute<std::map<std::string, std::string> >(
            StatisticsExporterProcess::StadisticsOutParamsKeyAttr, options);
      Statistics::StatisticsFlag flags = GetSelectedStatistics();
      pPal_->AddAttribute<Statistics::StatisticsFlag>(
            StatisticsExporterProcess::SelectedStadisticsKeyAttr, flags);
   }
   return true;
}

/**
 * Restaura los valores originales del Part.
 * @return bool que indica si pudo deshacer los cambios.
 */
bool StatisticParameterSelectionPart::RollbackChanges() {
   widgetShowAsArea_ = showAsArea_;
   widgetBasicStatistics_ = basicStatistics_;
   widgetCrossBandStatistics_ = crossBandStatistics_;
   widgetHistogram_ = histogram_;
   if (pPal_) {
      std::map<std::string, std::string> options = GetOutputOptions();
      pPal_->AddAttribute<std::map<std::string, std::string> >(
            StatisticsExporterProcess::StadisticsOutParamsKeyAttr, options);
      Statistics::StatisticsFlag flags = GetSelectedStatistics();
      pPal_->AddAttribute<Statistics::StatisticsFlag>(
            StatisticsExporterProcess::SelectedStadisticsKeyAttr, flags);
   }
   return UpdateParametersToWidget();
}

/** Inicializa el part */
void StatisticParameterSelectionPart::SetInitialValues() {
   DatasourceInterface* pdatasource = pDatasourceManager_->GetDatasource(datasourceId_);
   RasterElement* prasterelement = pdatasource ? dynamic_cast<RasterElement*>(
                                                      pdatasource->GetElement()) : NULL;
   if (prasterelement) {
      // Si el raster tiene una sola banda no permito generar estadisticas entre
      // bandas.
      if (prasterelement->GetBandCount() <= 1)
         GET_CONTROL(*GetWidget()->GetWindow(), wxT("ID_CROSSBAND_PARAMS_CHECK"),
            wxCheckBox)->Enable(false);

      // Si el raster no esta proyectado no permito mostrar resultados como areas
      if (!SpatialReference::IsProjected(prasterelement->GetSpatialReference().c_str()))
         GET_CONTROL(*GetWidget()->GetWindow(), wxT("ID_EXPRESS_AS_AREA_CHECK"),
            wxCheckBox)->Enable(false);
		if (!enableBasicParams_) {
			GET_CONTROL(*GetWidget()->GetWindow(), wxT("ID_BASIC_PARAMS_CHECK"),
         wxCheckBox)->Enable(false);
		}
   }
   RollbackChanges();
}

/**
 * Devuelve si la parte contiene datos validos.
 * @return bool que indica si los datos son validos
 */
bool StatisticParameterSelectionPart::HasValidData() {
   if (!ReadParametersFromWidget())
      return false;
   return true;
}

/** Actualiza el estado de la parte */
void StatisticParameterSelectionPart::Update() {
}

/**
 * Retorna las estadisticas seleccionadas por el usuario
 * @return estadisticas seleccionadas por el usuario
 */
Statistics::StatisticsFlag StatisticParameterSelectionPart::GetSelectedStatistics() {
   Statistics::StatisticsFlag statistics = Statistics::None;

   ClassifiedRasterDatasourceValidator classifiedrastervalidator;
   DatasourceInterface* pdatasource = pDatasourceManager_->GetDatasource(datasourceId_);
   if (basicStatistics_ && !classifiedrastervalidator.IsValid(pdatasource))
      statistics = Statistics::Mean | Statistics::Variance |
                                 Statistics::MaxValue | Statistics::MinValue;
   if (crossBandStatistics_)
      statistics = statistics | Statistics::CovarianceMatrix
                                       | Statistics::CorrelationMatrix;
   if (histogram_)
      statistics = statistics | Statistics::Histogram;

   return statistics;
}

/** 
 * Retorna las opciones de salida que se pasan a TextFormtaConverter 
 * todo Requiere refactor. Ver TCK #841
 * @return opciones de salida que se pasan a TextFormtaConverter 
 */
std::map<std::string, std::string> StatisticParameterSelectionPart::GetOutputOptions() {
   std::map<std::string, std::string> options;

   DatasourceInterface* pdatasource = pDatasourceManager_->GetDatasource(datasourceId_);
   RasterElement* prasterelement = pdatasource ? dynamic_cast<RasterElement*>(
                                                      pdatasource->GetElement()) : NULL;
   if (prasterelement) {
      options["datatype"] = prasterelement->GetRasterProperties().DataType;
      if (showAsArea_) {
         RasterSpatialModel::Parameters smparameters =
               RasterSpatialModel::WktToParameters(prasterelement->GetRasterModel());

         options["unit"] = SpatialReference::GetProjCoordinateSystemUnits(
                                    prasterelement->GetSpatialReference().c_str());
         options["pixel_area"] = DoubleToString(
               smparameters.pixelSizes_[0] * smparameters.pixelSizes_[0]);
      }

      ClassifiedRasterDatasourceValidator classifiedrastervalidator;
      if (classifiedrastervalidator.IsValid(pdatasource)) {
         options["bins_0_255"] = "true";
         if (basicStatistics_)
            options["show_mode"] = "true";

         ClassifiedRasterDatasourceManipulator classifiedrastermanipulator;
         std::vector<ClassInformation> classes = classifiedrastermanipulator.
                                            GetClassInformationList(pdatasource);
         std::stringstream ss;
         std::vector<ClassInformation>::iterator it = classes.begin();
         for (; it != classes.end(); ++it) {
            ss << it->GetIndex() << "=";
            ss << it->GetName() << ";";
         }
         options["bin_names"] = ss.str();

         options["histogram_bin_column_name"] = html_CLASS_NAME;
      }
   }
   return options;
}

/**
 * Actualiza los atributos widget con datos en control grafico
 * @return bool que indica si pudo leer los parametros
 */
bool StatisticParameterSelectionPart::ReadParametersFromWidget() {
   widgetBasicStatistics_ = USE_CONTROL(*GetWidget()->GetWindow(),
         wxT("ID_BASIC_PARAMS_CHECK"),
         wxCheckBox, GetValue(), false);
   widgetCrossBandStatistics_ = USE_CONTROL(*GetWidget()->GetWindow(),
         wxT("ID_CROSSBAND_PARAMS_CHECK"),
         wxCheckBox, GetValue(), false);
   widgetHistogram_ = USE_CONTROL(*GetWidget()->GetWindow(),
         wxT("ID_HISTOGRAM_CHECK"),
         wxCheckBox, GetValue(), false);

   widgetShowAsArea_ = USE_CONTROL(*GetWidget()->GetWindow(),
         wxT("ID_EXPRESS_AS_AREA_CHECK"),
         wxCheckBox, GetValue(), false);

   return true;
}

/**
 * Actualiza el control grafico con datos en los atributos widget
 * @return bool que indica si pudo actualizar la ventana
 */
bool StatisticParameterSelectionPart::UpdateParametersToWidget() {
   GET_CONTROL(*GetWidget()->GetWindow(), wxT("ID_EXPRESS_AS_AREA_CHECK"),
         wxCheckBox)->SetValue(widgetShowAsArea_);

   GET_CONTROL(*GetWidget()->GetWindow(), wxT("ID_BASIC_PARAMS_CHECK"),
         wxCheckBox)->SetValue(widgetBasicStatistics_);

   GET_CONTROL(*GetWidget()->GetWindow(), wxT("ID_CROSSBAND_PARAMS_CHECK"),
         wxCheckBox)->SetValue(widgetCrossBandStatistics_);

   GET_CONTROL(*GetWidget()->GetWindow(), wxT("ID_HISTOGRAM_CHECK"),
         wxCheckBox)->SetValue(widgetHistogram_);

   return true;
}
}  // end namespace suri
