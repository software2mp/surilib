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

// Includes Estandar
#include <vector>
#include <string>
#include <memory>

// Includes Suri
#include "TransformationModelParametersPart.h"
#include "resources.h"
#include "suri/LibraryItemAttribute.h"
#include "LibraryItemEditorNotificator.h"
#include "suri/ParserResult.h"
#include "SpatialReference.h"
#include "suri/TextFileGcpLoader.h"
#include "suri/CoordinatesTransformation.h"

// Includes Wx
#include "wx/filepicker.h"

// Defines
#define PRINCIPAL_PANEL_ID "ID_TRANSFORMATION_MODEL_PARAMETERS_PANEL"
#define CGU_PANEL_ID "ID_SR_LIBRARY_PANEL"
#define WARP_PANEL_ID "ID_WARP_PARAMETERS_PANEL"
#define ID_FILECTRL "ID_FILECTRL"
#define DELTA 10

// forwards

namespace suri {

TransformationModelParametersPart::TransformationModelParametersPart(
      const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
      ReprojectionProcess::ReprojectionParameters *pReprojectionParameters,
      const int TransformationFlags) :
      LibraryItemEditorPart(
            LibraryId, pManager, PRINCIPAL_PANEL_ID, CGU_PANEL_ID, true,
            UniversalGraphicalComponentPartInterface::ExtendedRoWithFFAndNoLabel,
            LibraryManager::READONLY),
      pParametersPart_(new WarpParametersPart("", TransformationFlags)),
      pItemSelected_(NULL), flags_(TransformationFlags), order_(-1) {
   pReprojectionParameters_ = pReprojectionParameters;
   windowTitle_ = _(caption_TRANSFORMATION_TOOL);
}

TransformationModelParametersPart::~TransformationModelParametersPart() {
}

/** Retorna el icono de la herramienta */
void TransformationModelParametersPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_REPROJECTION, ToolBitmap);
}

/**
 *
 * @return
 */
std::vector<LibraryItemAttribute*>*
TransformationModelParametersPart::CreateAttributesFromFields() {
   return NULL;
}

/**
 *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
 *  Este metodo es el que realmente tienen que implementar las clases hijas para
 * lograr la especializacion a la hora de iniciar el part (metodo que se llama
 * en CreateToolWindow, a modo de Template Method).
 **/
bool TransformationModelParametersPart::ConfigureFeatures() {
   AddControl(pParametersPart_->GetWidget(), wxT(WARP_PANEL_ID));
   if (!pReprojectionParameters_->srWktOut_.empty()) {
      std::string primaryattribute = SpatialReference::GetAuthorityId(
            pReprojectionParameters_->srWktOut_);
      pNotificator_->NotifyItemSelectionFromEditor(primaryattribute);
   }
   if (flags_ & GcpFile)  {
      pCgu_->Hide();
   }
   return true;
}

/**
 * Metodo auxiliar privado que es necesario implementar en las clases hijas.
 * Este metodo es el encargado de actualizar realmente la informacion en el part
 * Es llamado en el metodo SetActiveItem a modo de Template Method
 **/
void TransformationModelParametersPart::UpdateFields(const LibraryItem* pItem, bool Modifiable) {
}

/**
 * Metodo auxiliar privado que agrega comportamiento adicional al commit de la
 * clase base. Este comportamiento adicional es necesario cuando
 * el commit involucra objetos adicionales a los items de biblioteca
 * del editor (ej. reproyeccion, en donde se necesita actualizar los params
 * de reproyeccion ademas de leer el item de biblioteca)
 * @return true en caso de realizar correctamente el commit "interno"
 * @return false en caso de que ocurra algun error durante el commit
 */
bool TransformationModelParametersPart::OnCommitChanges() {
   if (!UpdateCoordinateTransform())
         return false;

   // Aplico los cambios
   if (!pParametersPart_->CommitChanges())
      return false;

   if (pReprojectionParameters_->pTranform_ == NULL) {
      SHOW_ERROR(wxT("Valores Ingresados Incorrectos"));
      return false;
   }
   return true;
}

/**
 * Metodo auxiliar privado que agrega comportamiento adicional al rollback de la
 * clase base. Este comportamiento adicional es necesario cuando
 * el rollback involucra objetos adicionales a los items de biblioteca
 * del editor (ej. parts compuesto en donde se necesita llamar a los rollbacks de
 * los mismos)
 * @return true en caso de realizar correctamente el rollback "interno"
 * @return false en caso de que ocurra algun error durante el rollback
 */
bool TransformationModelParametersPart::OnRollback() {
   if (pParametersPart_->RollbackChanges()) {
      UpdateCoordinateTransform();
      UpdateReprojectionParameters();
   }
   modified_ = false;
   return true;
}

/** Metodo auxiliar que a partir de un item de biblioteca genera la
 *  transformacion de coordenadas correspondiente
 *  @param[in] Item que posee el SRout para generar la transformacion de coordenadas
 *  @return NULL en caso de que no se pueda crear la transformacion de coordenadas
 *  @return CoordinateTransformInterface* puntero a la instancia creada
 */
CoordinatesTransformation*
TransformationModelParametersPart::CreateCoordinateTransformFromSrs(
          const std::string &SpatialReference, const std::string &Algorithm, int Order) {
   // Configuro los parametros
   suri::ParameterCollection params;
   params.AddValue<std::string>(Algorithm, Algorithm);
   params.AddValue<int>(suri::TransformationFactory::kParamOrder, Order);
   params.AddValue<std::string>(suri::TransformationFactory::kParamWktIn,
                                pReprojectionParameters_->srWktIn_);
   params.AddValue<std::string>(suri::TransformationFactory::kParamWktOut, SpatialReference);
   if (Algorithm == suri::TransformationFactory::kPolynomial) {
      std::vector<GroundControlPoint> vgcps;
      CreateGcpListUsingExact(params, vgcps);
      params.AddValue<std::vector<GroundControlPoint> >
                     (suri::TransformationFactory::kParamGcps, vgcps);
   }

   suri::TransformationFactory* pfactory = suri::TransformationFactoryBuilder::Build();
   suri::CoordinatesTransformation* ptransformation = pfactory->Create(Algorithm, params);
   TransformationFactoryBuilder::Release(pfactory);
   return ptransformation;
}

/** Metodo auxiliar que a partir de un listado de puntos de control genera la
 *  transformacion de coordenadas correspondiente
 *  @param[in] Item que posee el SRout para generar la transformacion de coordenadas
 *  @return NULL en caso de que no se pueda crear la transformacion de coordenadas
 *  @return CoordinateTransformInterface* puntero a la instancia creada
 */
CoordinatesTransformation*
TransformationModelParametersPart::CreateCoordinateTransformFromGcp(
                       GcpList* pGcpList, const std::string &Algorithm, int Order) {
   suri::ParameterCollection params;
   params.AddValue<std::string>(Algorithm, Algorithm);
   params.AddValue<int>(suri::TransformationFactory::kParamOrder, Order);
   params.AddValue<std::vector<GroundControlPoint> >
   (suri::TransformationFactory::kParamGcps, pGcpList->CreateAdaptedGcps());

   TransformationFactory* pfactory = suri::TransformationFactoryBuilder::Build();
   CoordinatesTransformation* ptransformation = pfactory->Create(Algorithm, params);
   TransformationFactoryBuilder::Release(pfactory);
   return ptransformation;
}

/**
 * Actualiza la transformacion en funcion de los parametros configurados
 * en el part de seleccion de gcps y algoritmo
 */
bool TransformationModelParametersPart::UpdateCoordinateTransform() {
   // Si no se modifico ningun parametro no actualizo
   bool gcpfile = flags_ & GcpFile;
   if (!gcpfile && pItemSelected_ && GetActiveItem()
         && pItemSelected_->GetId().compare(GetActiveItem()->GetId()) == 0
         && pReprojectionParameters_->pTranform_ != NULL) return true;

   if (gcpfile && lastPathChoiced_ == pParametersPart_->GetFileName()
         && lastOrderSelected_ == pParametersPart_->GetPolynomialOrderSelected()
         && pReprojectionParameters_->pTranform_ != NULL) return true;

   if (!gcpfile && GetActiveItem() != NULL && GetActiveItem()->GetAttribute("WKT") != NULL) {
           std::string wkt = GetActiveItem()->GetAttribute("WKT")->GetValue();
           pPal_->AddAttribute<std::string>(ProcessAdaptLayer::OutputSRSKeyAttr, wkt);
   }
   if (algorithm_.compare(pParametersPart_->GetAlgorithmSelected()) == 0
         && order_ == pParametersPart_->GetPolynomialOrderSelected())
      return pReprojectionParameters_->pTranform_ != NULL;

   algorithm_ = pParametersPart_->GetAlgorithmSelected();
   order_ = pParametersPart_->GetPolynomialOrderSelected();

   // Acutalizo CoordinateTransform
   delete pReprojectionParameters_->pTranform_;
   pReprojectionParameters_->pTranform_ = NULL;

   if (!gcpfile && GetActiveItem() && GetActiveItem()->GetAttribute("WKT")) {
      pReprojectionParameters_->pTranform_ = CreateCoordinateTransformFromSrs(
            GetActiveItem()->GetAttribute("WKT")->GetValue(), algorithm_, order_);
      pItemSelected_ = GetActiveItem();
   }

   if (gcpfile) {
      if (pParametersPart_->HasValidData()) {
         GcpList& gcplist = pParametersPart_->GetGcpList();
         pReprojectionParameters_->pTranform_ = CreateCoordinateTransformFromGcp(
               &gcplist, algorithm_, order_);

         pReprojectionParameters_->algorithm_ = algorithm_;
         pReprojectionParameters_->algorithmOrder_ = order_;
         pReprojectionParameters_->gcpListFileName_ = pParametersPart_->GetFileName();
         pReprojectionParameters_->srWktOut_ = gcplist.GetSpatialReferenceDestination();

         lastOrderSelected_ = order_;
         lastPathChoiced_ = pParametersPart_->GetFileName();

         pPal_->AddAttribute<std::string>(ProcessAdaptLayer::GcpListFileNameKeyAttr,
                                          lastPathChoiced_);
      }
   }

   // Envio una notificacion de que cambio la transformacion (SRS)
   // Ver RasterOutputSizePart
   // Notify(NULL);

   return pReprojectionParameters_->pTranform_ != NULL;
}

/** Indica si el Part tiene cambios para salvar */
bool TransformationModelParametersPart::HasChanged() {
   return LibraryItemEditorPart::HasChanged() || pParametersPart_->HasChanged();
}

/**
 * Inicializa el sistema de referencia de salida con el de entrada. Si no
 * encuentra el srs en la biblioteca no hace nada.
 */
void TransformationModelParametersPart::SetInitialValues() {
   std::string principal = SpatialReference::GetAuthorityId(
         pReprojectionParameters_->srWktIn_);
   const Library* plibrary = editorClient_.GetLibrary(libraryId_);
   if (plibrary && plibrary->GetItemByPrincipal(principal))
      pCgu_->SelectItemByPrincipal(principal);
}

/** Salva los cambios realizados en el Part */
bool TransformationModelParametersPart::CommitChanges() {
	bool result = UpdateReprojectionParameters();
	if (result) {
		return LibraryItemEditorPart::CommitChanges();
	} else {
		return false;
	}
}

/** Devuelve si la parte tiene datos validos */
bool TransformationModelParametersPart::HasValidData() {
	return pParametersPart_->HasValidData() && UpdateCoordinateTransform();
}

/**
 * Actualiza la transformacion en funcion de los parametros configurados
 * en el part de seleccion de gcps y algoritmo
 */
bool TransformationModelParametersPart::UpdateReprojectionParameters() {
   if (flags_ & GcpFile) {
      if (!pParametersPart_->HasValidData()) {
         return false;
      }
      GcpList& gcplist = pParametersPart_->GetGcpList();
      pReprojectionParameters_->gcpListFileName_ = pParametersPart_->GetFileName();
      pReprojectionParameters_->srWktOut_ = gcplist.GetSpatialReferenceDestination();
   } else {
      const LibraryItemAttribute* pwktattribute =
            pItemSelected_ ? pItemSelected_->GetAttribute("WKT") : NULL;
      if (!pwktattribute) {
         return false;
      }
      pReprojectionParameters_->srWktOut_ = pwktattribute->GetValue();
   }

   std::string algorithm = pParametersPart_->GetAlgorithmSelected();
   pReprojectionParameters_->algorithm_ = algorithm;

   int order = pParametersPart_->GetPolynomialOrderSelected();
   pReprojectionParameters_->algorithmOrder_ = order;

   pPal_->AddAttribute<std::string>(ProcessAdaptLayer::AlgorithmKeyAttr, algorithm);
   pPal_->AddAttribute<int>(ProcessAdaptLayer::AlgorithmOrderKeyAttr, order);

   // Envio una notificacion de que cambio la transformacion (SRS)
   // Ver RasterOutputSizePart
   // Notify(NULL);
	return true;
}

/**
 * Genera una lista de puntos de control en funcion de una transformacion exacta.
 * @param[in] Params parametros para la configuracion de la transformacion
 * @param[out] List lista de puntos de control sobre la cual se cargan los puntos
 */
void TransformationModelParametersPart::CreateGcpListUsingExact(const ParameterCollection& Params,
                                                                std::vector<GroundControlPoint>& List) {
   List.clear();
   suri::TransformationFactory* pfactory = suri::TransformationFactoryBuilder::Build();
   suri::CoordinatesTransformation* pexact = pfactory->Create(
         suri::TransformationFactory::kExact, Params);

   Subset extent;
   if (pReprojectionParameters_->pWorldIn_ == NULL)
      return;
   pReprojectionParameters_->pWorldIn_->GetWorld(extent);
   std::vector<Coordinates> coords = Subset::ExtractCoordinates(
         extent, DEFAULT_CALCULATION_POINTS, DELTA);
   GcpList gcplist;
   for (int ix = 0, len = coords.size(); ix < len; ++ix) {
      Coordinates src = coords[ix];
      Coordinates dest = coords[ix];
      pexact->Transform(dest);
      List.push_back(GroundControlPoint(src, dest));
   }
   TransformationFactoryBuilder::Release(pfactory);
}
} /** namespace suri */
