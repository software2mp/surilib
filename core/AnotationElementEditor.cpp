/*! \file AnotationElementEditor.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "AnotationElementEditor.h"

// Includes suri
#include "suri/ViewerWidget.h"
#include "suri/World.h"
#include "suri/Configuration.h"
#include "suri/VectorEditor.h"
#include "suri/CoordinatesTransform.h"
#include "suri/FileManagementFunctions.h"
#include "suri/messages.h"
#include "AnotationElement.h"
#include "resources.h"
#include "logmacros.h"

// Includes app

// Defines
/** Nombre para crar la capa en el vector */
#define ANOTATION_FILE_NAME "Anotation_1"
/** Longitud del campo con las anotaciones */
#define ANOTATION_FIELD_LENGTH 512

/** namespace suri */
namespace suri {
/**
 * Constructor.
 * @param[in]	pViewer:	Viewer sobre el cual actua.
 * @param[in]	ListGroup:	Grupo al que pertenece.
 * @param[in]  pVectorEditor: Permite editar vectores
 */
AnotationElementEditor::AnotationElementEditor(ViewerWidget *pViewer, int ListGroup,
                                               VectorEditor* pVectorEditor,
                                               DataViewManager* pDataViewManager) :
                         VectorElementEditor(pViewer, Vector::Point, ANOTATION_TABLE_NAME,
                         ListGroup, pVectorEditor, pDataViewManager) {
   /* cargo los estilos para que VectorElementEditor::CreateVectorElement cree la
    * tabla de estilo.
    * Limpio el vector de estilos para asegurarme de que la tabla se cargue
    * solo con los estilos que seteo.*/
   styles_.clear();
   styles_.push_back(ANOTATION_POINT_1);
   // windowTitle_=_(tooltip_ANOTATION);
   elementName_ = _(label_ANOTATION);
}

/**
 * Destructor.
 */
AnotationElementEditor::~AnotationElementEditor() {
}

/**
 * Setea pAnotationElement_ a NULL y llama a VectorElementEditor::CloseVector
 * @return bool que indica si pudo cerrar el vector.
 */
bool AnotationElementEditor::CloseVector() {
   pAnotationElement_ = NULL;
   return VectorElementEditor::CloseVector();
}

/**
 *	Crea el campo SURLabel en la base de datos del vector. Agrega nodo
 * SUR_ANOTATION_FIELD en elemento
 * \post Setea el flag de feature abierto.
 * \post El vector asociado tiene el campo SUR_ANOTATION_FIELD
 * de tipo string de longitud ANOTATION_FIELD_LENGTH caracteres.
 * \todo Considerar utilizar propiedad para el tamanio del campo.
 * \warning El nombre del campo debe ser menos a 10 caracteres ASCII-127
 */
void AnotationElementEditor::SetupElement() {
   // Creo capa (antes de crear elemento para que se cree archivo)
   pVectorEditor_->OpenLayer(ANOTATION_FILE_NAME, LATLON_SPATIAL_REFERENCE,
                             Vector::Point);
   // Agrego campo tipo anotacion a la capa
   if (!pVectorEditor_->CreateField(SUR_ANOTATION_FIELD, DataInfo<unsigned char>::Name,
                                    ANOTATION_FIELD_LENGTH)) {
      REPORT_AND_FAIL("D:No se pudo configurar el vector");
   }
   // Sincronizo a disco antes de crear AnotationElement
   pVectorEditor_->SyncToDisk();

   pAnotationElement_ = AnotationElement::Create(elementUrl_);
   if (pAnotationElement_) {
      // Guardo AnotationElement para que VectorElementEditor no cree VectorElement
      pCurrentVectorElement_ = pAnotationElement_;
      // si tiene campo SURAnotation, inserto el nodo "etiqueta" en el xml.
      if (pAnotationElement_->HasAnotationField()) {
         pAnotationElement_->AddLabelNode(SUR_ANOTATION_FIELD);
      }
   } else {
      REPORT_AND_FAIL("D:No se pudo configurar el vector");
   }
}

/**
 * Solicita el texto que contendra el label usando wxDialog
 * Agregar el punto recibido por parametro al vector en curso.
 * Agrega el valor de la anotacion en el campo SUR_ANOTATION_FIELD, cambia el
 * nombre del elemento al texto ingresado.
 * \post Llama a CloseFeature para cerrar la geometria, deja el vector abierto
 * para agregar mas puntos.
 * @param[in] Point: punto a agregar.
 * @return true en caso de haber podido agregar el punto
 * @return false pCurrentVectorElement_ es invalido.
 * @return false No se pudo agregar el punto
 */
bool AnotationElementEditor::AddPointToVector(const Coordinates &Point) {
   if (!pCurrentVectorElement_
         || (pVectorEditor_->GetOpenFeature() < 0 && pVectorEditor_->OpenFeature() < 0)) {
      return false;
   }REPORT_DEBUG("D:L1,%s", pViewer_->GetWindowTitle().c_str());

   // creo un dialogo para ingresar un txt
   wxDialog dialog(pViewer_->GetFrame(), wxID_ANY, _(caption_ANOTATION_INPUT),
                   wxDefaultPosition, wxDefaultSize, wxCAPTION | wxSYSTEM_MENU);
   dialog.SetSizer(new wxBoxSizer(wxVERTICAL));
   REPORT_DEBUG("D:L1.1");
   wxTextCtrl *ptxtctrl = new wxTextCtrl(&dialog, wxID_ANY,
                                         _(content_DEFAULT_ANOTATION),
                                         wxDefaultPosition, wxSize(250, -1));
   dialog.GetSizer()->Add(ptxtctrl, 0, wxEXPAND | wxALL, 5);
   dialog.GetSizer()->Add(dialog.CreateButtonSizer(wxOK), 0, wxEXPAND | wxALIGN_BOTTOM);
   dialog.Fit();
   // Valor por default
   anotation_ = _(content_DEFAULT_ANOTATION);
   ptxtctrl->SetValidator(wxTextValidator(wxFILTER_NONE, &anotation_));
   ptxtctrl->SelectAll();
   ptxtctrl->SetFocusFromKbd();
   // muestro el dialogo
   if (dialog.ShowModal() == wxID_OK) {
      dialog.TransferDataFromWindow();
      // Agrego punto a capa
      if (!pVectorEditor_->SetPoint(Point)) {
         REPORT_AND_FAIL_VALUE("D:No se pudo escribir el punto", false);
      }
      pVectorEditor_->SetField(SUR_ANOTATION_FIELD, anotation_.c_str());
      CloseFeature();
      // Asigna al nombre del elemento el mismo texto que la anotacion
      if (pAnotationElement_) {
         pAnotationElement_->SetName(anotation_);
      }
   }
   return true;
}

/**
 * Ver clase padre
 * Deja el nombre del elemento que ya viene configurado con el valor de la
 * ultima anotacion
 * @param[in] pVectorElement elemento que se debe configurar
 * @return bool que indica si pudo configurar el elemento
 */
bool AnotationElementEditor::ConfigureSavedElement(VectorElement* pVectorElement) {
   return true;
}
}  // namespace suri

