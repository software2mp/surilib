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

#include "VectorElementEditor.h"

// Includes suri
#include "suri/ViewerWidget.h"
#include "suri/MemoryVectorElement.h"
#include "suri/World.h"
#include "suri/Configuration.h"
#include "suri/VectorStyleManager.h"
#include "suri/VectorEditor.h"
#include "suri/FileManagementFunctions.h"
#include "suri/messages.h"
#include "suri/ElementManager.h"
#include "resources.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DatasourceInterface.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/DataViewManager.h"
#include "AnotationElement.h"
#include "MeasureDeletionRequestedNotification.h"

// Includes app

// Includes OGR
#include "ogrsf_frmts.h"

// Forwards

/** namespace suri */
namespace suri {
/**
 * Contador que permite asignar un nombre distinto a cada vector que
 * se crea en memoria, asi se evita que dos contextos editen el mismo vector.
 * \ todo esta solucion deberia desaparecer cuando se trabaje con una unica lista
 */
int VectorElementEditor::vectorCounter_ = 0;

/**
 * Constructor.
 * @param[in]	pViewer:	Viewer sobre el cual actua.
 * @param[in]	Type: tipo de Vector que crea.
 * @param[in]  CategoryName: categoria del vector.
 * @param[in]	ListGroup: Grupo al que pertenece.
 * @param[in]  pVectorEditor: Se usa para agregar puntos a vectores
 * @return instancia de la clase VectorElementEditor
 */
VectorElementEditor::VectorElementEditor(ViewerWidget *pViewer, Vector::VectorType Type,
      std::string CategoryName, int ListGroup, VectorEditor* pVectorEditor,
      DataViewManager* pDataViewManager) :
      pViewer_(pViewer), pCurrentVectorElement_(NULL), type_(Type),
      categoryName_(CategoryName), layers_(1), listGroup_(ListGroup),
      elementUrl_(wxEmptyString), elementName_(wxEmptyString),
      pVectorEditor_(pVectorEditor)
#ifdef __USE_MEMORY_VECTOR_DURING_EDITION__
      , elementFileName_(wxEmptyString) /*! Nombre de archivo a salvar */
#endif
      , pDataViewManager_(pDataViewManager)
{
   switch (type_) {
      case Vector::Point:
         elementName_ = _(label_POINT);
         break;
      case Vector::Line:
         elementName_ = _(label_LINE);
         break;
      case Vector::Polygon:
         elementName_ = _(label_POLYGON);
         break;
      default:
         elementName_ = _(label_UNKNOWN_VECTOR);
         break;
   }
   pDatasourceManager_ = pDataViewManager_->GetDatasourceManager();
}

/**
 * Destructor
 */
VectorElementEditor::~VectorElementEditor() {
}

/**
 * Indica si se termino de editar el elemento.
 * @return bool que indica si hay un vector editandose
 */
bool VectorElementEditor::IsVectorOpen() {
   // retorna true si estoy editando elemento y este esta abierto
   return ((pViewer_->GetList()->GetElement(pCurrentVectorElement_) != NULL)
         && (pVectorEditor_->IsVectorOpen()));
}

/**
 * Calcula el url en memoria del vector y lo guarda en elementUrl_.
 * Crea vector con VectorEditor. Llama a SetupElement para configurar la edicion.
 * Si SetupElement no creo un elemento falla.
 * Asigna el estilo al elemento.
 * @return true si pudo crear el vector
 * @return false si no se creo correctamente el elemento
 */
bool VectorElementEditor::CreateVectorElement() {
   bool isinlist = pViewer_->GetList()->GetElement(pCurrentVectorElement_) != NULL;

   if (isinlist) {
      return true;
   }

   // si no es un vector existente, lo creo
   if (!isinlist) {
      // Encuentro el nombre del archivo
      std::string tempvectordir = Configuration::GetParameter("app_temp_dir",
                                                              wxT("./tmp"));
#ifdef __USE_MEMORY_VECTOR_DURING_EDITION__
      /**
       * Se utiliza una variable estatica para formar el nombre del vector en memoria, para
       * asegurar que se crean nombres distintos y evitar que dos contextos editen el mismo
       * vector.
       */
      wxString vectorname;
      vectorname.sprintf("Elem_%d", vectorCounter_);
      elementUrl_ = tempvectordir + vectorname;
      vectorCounter_++;
#else
      elementUrl_ = GetNextPath(tempvectordir);
#endif
      if (elementUrl_.empty()) {
         SHOW_ERROR(message_ADD_ELEMENT_ERROR_s,
                    pCurrentVectorElement_->GetTypeAsString().c_str());
         pViewer_->GetList()->DelElement(pCurrentVectorElement_);
         pCurrentVectorElement_ = NULL;
         return false;
      }
      wxFileName fn(elementUrl_);
      if (fn.GetDirCount() == 0) {
         fn.PrependDir(".");
      }
      if (!fn.IsAbsolute()) {
         fn.MakeAbsolute();
      }
      if (!fn.IsDir()) {  // en caso de directorio, no quiero el ultimo /
         elementUrl_ = fn.GetFullPath(wxPATH_UNIX).c_str();
      } else {
         elementUrl_ = fn.GetPath(wxPATH_GET_VOLUME, wxPATH_UNIX).c_str();
      }

#ifdef __USE_MEMORY_VECTOR_DURING_EDITION__
      elementFileName_ = (fn.GetVolume() + fn.GetVolumeSeparator()).c_str()
            + elementUrl_;
      elementUrl_ = "shpmemory:" + elementUrl_;
#else
      elementUrl_=(fn.GetVolume()+fn.GetVolumeSeparator() ).c_str()+elementUrl_;
#endif
      // Abro el vector
      if (pVectorEditor_->OpenVector(elementUrl_)) {
         // HACK HORRIBLE : para que no destruya el vector en memoria por falta
         // de referencias, lo abro.
#ifdef __USE_MEMORY_VECTOR_DURING_EDITION__
         Vector *ptemp = Vector::Open(elementUrl_);
#endif
         // configura el elemento (campos del vector etc)
         SetupElement();
         // Si el SetupElement no creo el vector creo un VectorElement por default
         if (!pCurrentVectorElement_) {
            pCurrentVectorElement_ = VectorElement::Create(elementUrl_);
         }
         // HACK HORRIBLE - Ciero la referencia que obtuve para evitar el cierre
#ifdef __USE_MEMORY_VECTOR_DURING_EDITION__
         Vector::Close(ptemp);
#endif
         if (pCurrentVectorElement_) {
            pCurrentVectorElement_->SetName(elementName_);
            // obtengo el estilo del vector.
            VectorStyle * pvecstyle = VectorStyleManager::Instance().GetDefaultStyle(
                  categoryName_, type_);
            // Si no existe el estilo lo cargo.
            if (!pvecstyle) {
               LoadStyles();
               pvecstyle = VectorStyleManager::Instance().GetDefaultStyle(categoryName_,
                                                                          type_);

               if (!pvecstyle)  // este if no se deberia cumplir nunca.
               {
                  pViewer_->GetList()->DelElement(pCurrentVectorElement_);
                  pCurrentVectorElement_ = NULL;
                  REPORT_AND_FAIL_VALUE("D: no existe estilo para el vector.", false);
               }
            }
            pCurrentVectorElement_->SetStyle(pvecstyle->GetWkt(),
                                             pVectorEditor_->GetOpenLayer());
            pCurrentVectorElement_->Activate();
            pCurrentVectorElement_->SetEdited();
         }
      }
   } else {  // si no esta en la lista lo llevo a NULL porque el puntero no es valido
      pCurrentVectorElement_ = NULL;
   }
   return pCurrentVectorElement_ != NULL;
}

/**
 * Agrega el vector a la lista de elementos.
 * @return true si pudo agreagar el vector.
 * @return false C.O.C.
 */
bool VectorElementEditor::AddVectorElement() {
   // si agrega el vector, lo activa
   return pViewer_->GetList()->AddElement(pCurrentVectorElement_, listGroup_, pDataViewManager_);
}

/**
 * Despues de llamar este metodo se crea el elemento final con el nodo del
 * pVectorElement_.
 * El comportamiento default consiste en modificar el nombre del elemento al
 * nombre del archivo sin extension
 * \pre no se puede modificar el url de pCurrentVectorElemen_
 * \pre no se puede modificar el las dependencias de pCurrentVectorElemen_
 * @param[in] pVectorElement elemento que se debe configurar
 * @return bool que indica si pudo configurar el elemento
 */
bool VectorElementEditor::ConfigureSavedElement(VectorElement* pVectorElement) {
   wxFileName wxfilename(pVectorElement->GetUrl().c_str());
   pVectorElement->SetName(wxfilename.GetName());
   return true;
}

/**
 * Cierra el vector en edicion.
 * Elimina de la lista a pCurrentVectorElement_
 * \post pCurrentVectorElement_ se asigna a NULL
 */
bool VectorElementEditor::CloseVector() {
   // no esta en la lista
   if (pCurrentVectorElement_
         && !pViewer_->GetList()->GetElement(pCurrentVectorElement_)) {
      pCurrentVectorElement_ = NULL;
   }
   pVectorEditor_->CloseVector();
   // limpio datos de vector
   elementUrl_.clear();
   // Elimino el temporal luego de agregar el nuevo para que no titile al renderizar
   pViewer_->GetList()->DelElement(pCurrentVectorElement_);
   pCurrentVectorElement_ = NULL;
   return true;
}

/**
 * Presenta un dialogo para el ingreso del nombre del archivo. Si el archivo
 * existe, lo sobrescribe a el y todas sus dependencias. Si el elemento
 * eliminado esta en las listas lo destruye.
 * Agrega el elemento a todos los contextos
 * \post Sincroniza los cambios a disco.
 * \post Actualiza las dependencias del elemento asociado.
 * \post Llama a ConfigureSavedElement. (propagacion para el VectorElementEditor
 * y derivados)
 * \post Agrega el nuevo archivo a la lista de elementos.
 * @param[in] FileName Nombre del archivo donde quiero guardar el vector.
 * Si esta vacio le pide al usuario que ingrese uno.
 * @return true si pudo grabar y agregar a las listas.
 * @return false en otro caso
 */
bool VectorElementEditor::SaveVector(const std::string &FileName) {
   // no esta en la lista
   if (pCurrentVectorElement_
         && !pViewer_->GetList()->GetElement(pCurrentVectorElement_)) {
      pCurrentVectorElement_ = NULL;
   }
   // si por alguna razon pCurrentVectorElement_ == NULL salgo con error
   if (pCurrentVectorElement_) {
      std::string filename = FileName;

      // TODO(Ramiro - TCK 2566) - VectorElementEditor es una clase de core,
      // no deberia ser el responsable de pedir un nombre de archivo al usuario.
      if (filename.empty() && !SelectOutputFileName(filename))
         return false;

      if (filename.empty()) {
         REPORT_DEBUG("E: La ruta para guardar el vector esta vacia");
         return false;
      }
      if (wxFileName::FileExists(filename)) {
         if (!CanDeleteShapefile(filename)) {
            SHOW_ERROR(message_CANT_DELETE_VECTOR);
            return false;
         }

         // Prueba eliminarlo de la lista, por si ademas de existir esta abierto
         // Crea un elemento para obtener el UID
         Element *pelement = VectorElement::Create(filename);
         if (pelement) {
            Element::UidType uid = pelement->GetUid();
            Element::Destroy(pelement);
            // Elimina el vector de todas las listas
            ElementManager::Instance().DeleteElement(uid);
         }
         if (!DeleteShapefile(filename)) {
            SHOW_ERROR(message_VECTOR_DELETION_ERROR);
            return false;
         }
      }

      // Informo que el vector se dejo de editar
      pCurrentVectorElement_->SetEdited(false);
      // Actualizo
#ifdef __USE_MEMORY_VECTOR_DURING_EDITION__
      // Abro el vector en memoria(para no perderlo) y copio el nodo del elemento
      Vector *ptemp = Vector::Open(pCurrentVectorElement_->GetUrl().c_str());
      if (!ptemp) {
         REPORT_DEBUG("E: Fallo abrir el vector");
         return false;
      }
      wxXmlNode* peditedelementnode = new wxXmlNode(*pCurrentVectorElement_->GetNode());

      // Cierro el elemento editado y el VectorEditor
      pViewer_->GetList()->BlockViewerUpdate();
      CloseVector();

      // Copio el vector en memoria a disco y lo cierro
      if (!ptemp->Save(filename, "ESRI Shapefile")) {
         REPORT_DEBUG("E: Fallo guardar el vector");
         Vector::Close(ptemp);
         return false;
      }
      Vector::Close(ptemp);

      // creo un elemento temporal y obtengo las dependencias
      VectorElement *pdiskelement = VectorElement::Create(filename);
      if (!pdiskelement) {
         REPORT_DEBUG("E: Error al abrir el elemento despues de guardarlo");
         return false;
      }
      std::vector<wxString> dependencies = pdiskelement->GetDependency();

      // Piso nodo de elemento con el del vector editado y actualizo url y dependencias
      pdiskelement->AddNode(NULL, peditedelementnode, true);
      pdiskelement->DelDependency();
      for (size_t i = 0; i < dependencies.size(); i++)
         pdiskelement->AddDependency(dependencies.at(i).c_str());
      pdiskelement->SetUrl(filename);
      std::string elementuid = pdiskelement->GetUid();

      // Configuro el elemento antes de agregarlo a la lista
      ConfigureSavedElement(pdiskelement);

      // Agrego elemento editado a todos los contextos
      wxXmlNode tempelementnode(*pdiskelement->GetNode());
      std::string geomtype = "";
      pViewer_->GetList()->UnblockViewerUpdate();
      if (pDatasourceManager_) {
         // TODO(Gabriel - TCK #874): Agregado de nuevo elemento al dsmanager. Revisar!
         DatasourceInterface* pdatasource = DatasourceInterface::Create(
                            "VectorDatasource", pdiskelement->GetNode());
         if (!pdatasource)
            return false;

         AnotationElement* anoElement =
               dynamic_cast<AnotationElement*>(pdatasource->GetElement());
         if (!anoElement) {
            geomtype =
                  dynamic_cast<VectorElement*>(pdatasource->GetElement())->GetStringType();
            pdatasource->SetName(geomtype);
         } else {
            geomtype = anoElement->GetAnotationFieldValue().c_str();
            geomtype = wxString(wxConvUTF8.cMB2WC(geomtype.c_str()),
                              wxCSConv(suri::XmlElement::xmlEncoding_.c_str())).c_str();
            pdatasource->SetName(geomtype);
         }
         pDatasourceManager_->AddDatasourceWithNotification(pdatasource,
               pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontextId());
      } else {
         ElementManager::Instance().AddElement(pdiskelement->GetNode());  // Se agrega una copia
      }
      // Elimino elemento creado temporalmente
      Element* pdestroypointer = pdiskelement;
      Element::Destroy(pdestroypointer);

      // obtengo el elemento de la lista y lo activo
      Element *pelementnew = pViewer_->GetList()->GetElement(elementuid);
      if (!pelementnew) {
         return false;
      }

      /* Agrego el elemento a la fuente de datos */
      // Actualizamos el nombre del vectorelement.
      pelementnew->SetName(geomtype);
      pelementnew->Activate(true);


#else
      pVectorEditor_->CloseVector();
      ConfigureSavedElement(pCurrentVectorElement_);
      pCurrentVectorElement_->SetChanged();
      pCurrentVectorElement_->SendViewerUpdate();
#endif
   } else {
      return false;
   }
   return true;
}

/**
 * Abre una ventana de dialogo para seleccionar la ruta y nombre para guardar
 * un vector.
 * @param[out] Filename ruta y nombre del archivo a salvar.
 * @return false si el usuario cancelo
 */
bool VectorElementEditor::SelectOutputFileName(std::string &Filename) {
#ifdef __UNUSED_CODE__
   wxFileDialog dlg(NULL, _(caption_SAVE_VECTOR), wxT(""), wxT(""),
         _(filter_SHAPEFILE_FORMATS_SAVE), wxFD_SAVE
         | wxFD_OVERWRITE_PROMPT);
#else
   wxFileDialog dlg(NULL, _(caption_SAVE_VECTOR), wxT(""), wxT(""),
   filter_SAVE_VECTOR,
                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
#endif
   if (dlg.ShowModal() == wxID_OK) {
      Filename = dlg.GetPath();
      return true;
   }
   return false;
}

/**
 * Analiza si se puede eliminar el shapefile y sus archivos asociados
 * Trata de abrir el datasource con el filename recibido, si no lo puede abrir
 * retorna false.
 * Recorre las capas, si en alguna no tiene permiso de escritura retorna false
 * @param[in] Filename nombre del archivo que se desea eliminar
 * @return informa si se puede eliminar archivo
 */
bool VectorElementEditor::CanDeleteShapefile(const std::string &Filename) {
   // Verifico que sea un vector valido
   OGRSFDriver *pdriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
         "ESRI Shapefile");
   if (!pdriver || !pdriver->TestCapability(ODrCDeleteDataSource)) {
      return false;
   }

   OGRDataSource *pdatasource = OGRSFDriverRegistrar::Open(Filename.c_str());
   if (!pdatasource) {
      return false;
   }

   OGRDataSource::DestroyDataSource(pdatasource);

   // Verifico que el elemento no este abierto
   VectorEditor vectoreditor;
   vectoreditor.OpenVector(Filename);
   int layerindex = 0;
   while (vectoreditor.OpenLayer(layerindex)) {
      if (!vectoreditor.CanWrite()) {
         vectoreditor.CloseVector();
         return false;
      }
      layerindex++;
   }
   vectoreditor.CloseVector();

   return true;
}

/**
 * Elimina un shapefile y todos los archivos asociados.
 * @param[in] Filename url del shapefile que quiero eliminar
 * @return true si pudo borrar el datasource (todos los archivos asociados).
 * \todo este metodo deberia estar en vector.cpp
 */
bool VectorElementEditor::DeleteShapefile(const std::string &Filename) {
   /** Elimino shapefile */
   if (!CanDeleteShapefile(Filename)) {
      return false;
   }

   /** Elimino archivo */
   OGRSFDriver *pdriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
         "ESRI Shapefile");
   if (pdriver->DeleteDataSource(Filename.c_str()) != OGRERR_NONE) {
      return false;
   }

   return true;
}

/**
 * Devuelve el tipo de vector.
 *
 * @return Tipo de vector (punto, linea o poligono)
 */
Vector::VectorType VectorElementEditor::GetType() {
   return type_;
}

/**
 * Recibe notificaciones del control contenedor.
 */
void VectorElementEditor::Update(NotificationInterface* pNotification) {
   MeasureDeletionRequestedNotification* pmnot =
         dynamic_cast<MeasureDeletionRequestedNotification*>(pNotification);
   if (pmnot != NULL) {
      CloseFeature();
   }
}

/**
 * Si no se esta editando un vector crea un nuevo elemento vectorial y lo
 * agrega a la lista.
 * Transforma el sistema de cooredenadas del punto al del elemento
 * Llama a AddPointToVector para agregar punto.
 * @param[in] Point coordenadas donde quiero agregar el punto en sistema de mundo
 * @return informa si pudo agregar el punto
 */
bool VectorElementEditor::AddPoint(const Coordinates &Point) {
   // Si el elemento se elimino externamente
   if (pCurrentVectorElement_
         && !pViewer_->GetList()->GetElement(pCurrentVectorElement_)) {
      CloseVector();
   }

   Coordinates point(Point);

   // Agrega un punto (donde se hizo click)
   bool success = false;
   if (CreateVectorElement()
         && (pViewer_->GetList()->GetElement(pCurrentVectorElement_)
               || AddVectorElement()) && pCurrentVectorElement_ != NULL) {
      // Georreferencia
      ParameterCollection params;
        params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                     pViewer_->GetWorld()->GetSpatialReference());
        params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                     pCurrentVectorElement_->GetSpatialReference(
                                        pVectorEditor_->GetOpenLayer()).c_str());
        TransformationFactory* pfactory =
              TransformationFactoryBuilder::Build();
        CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                 params);
        TransformationFactoryBuilder::Release(pfactory);
      if (!pct) {
         REPORT_DEBUG(
               "D:Error al generar CoordinatesTransform desde  \
					sistema referencia del mundo al del vector.");
         return false;
      }
      pct->Transform(point);
      delete pct;

      if (AddPointToVector(point)) {
         // Si el vector sigue en edicion lo actualizo
         if (pCurrentVectorElement_) {
            pCurrentVectorElement_->SetChanged();
            pCurrentVectorElement_->SendViewerUpdate();
         }
         success = true;
      }
   }

   if (!success) {
      CloseVector();
      SHOW_STATUS(message_POINT_INSERT_ERROR);
      // No se traduce lo hace SHOW_
   }
   return success;
}

/**
 * Elimina el ultimo punto ingresado al vector.
 * @param[in] Position posicion(dentro del vector de puntos) donde esta
 * el punto a eliminar.
 * @return informa si se pudo eliminar el punto
 */
bool VectorElementEditor::DeletePoint(int Position) {
   bool returnvalue = false;

   // Si el elemento se elimino externamente
   if (pCurrentVectorElement_
         && !pViewer_->GetList()->GetElement(pCurrentVectorElement_)) {
      CloseVector();
   }

   // Si la cantidad de puntos > 0 puedo eliminar uno.
   if ((pVectorEditor_->GetOpenFeature() >= 0) && (GetPointCount() > 0)) {
      if (pVectorEditor_->DeletePoint(Position)) {
         pVectorEditor_->SyncToDisk();
         pCurrentVectorElement_->SetChanged();
         pCurrentVectorElement_->SendViewerUpdate();
         returnvalue = true;
      }
   }
   // Si se elimino el ultimo punto, se cierra el feature.
   if (GetPointCount() <= 0) {
      DeleteFeature(pVectorEditor_->GetFeatureCount() - 1);
   }

   return returnvalue;
}

/**
 * Retorna la cantidad de puntos
 * @return cantidad de puntos en vector en edicion. -1 si no hay ninguno
 */
int VectorElementEditor::GetPointCount() {
   if (IsVectorOpen()) {
      return pVectorEditor_->GetPointCount();
   } else {
      return -1;
   }
}

/**
 * Agrega la tabla de estilos que va a utilizar el vector al VectorStyleManager.
 * @return	false en caso de no poder agregar la tabla de estilos.
 * @return	true en caso de haber agregado la tabla de estilos
 * \attention	del puntero de la tabla de estilos creada se hace responsable
 * el VectorStyleManager. No debe ser eliminado.
 */
bool VectorElementEditor::LoadStyles() {
   if (categoryName_.empty() || styles_.size() == 0) {
      return false;
   }

   return VectorStyleManager::Instance().AddTable(
         categoryName_, new VectorStyleTable(type_, categoryName_, styles_));
}

/**
 * Cierra el feature abierto
 */
void VectorElementEditor::CloseFeature() {
   pVectorEditor_->CloseFeature();
}

/**
 * Elimina el feature de la posicion indicada. Si es el ultimo feature del
 * vector, cierra el vector.
 * @param[in] FeatureID identificador del feature que se quiere eliminar
 * @return true si pudo eliminar el feature, y si era el ultimo si pudo cerrar
 * el vector.
 */
bool VectorElementEditor::DeleteFeature(long FeatureID) {
   bool returnvalue = false;
   CloseFeature();
   returnvalue = pVectorEditor_->DeleteFeature(FeatureID);
   if (pVectorEditor_->GetFeatureCount() <= 0) {
      returnvalue = returnvalue && CloseVector();
   }
   return returnvalue;
}
}  // namespace suri
