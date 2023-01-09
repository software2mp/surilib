/*! \file DrawTool.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard

// Includes Suri
#include "suri/DrawTool.h"
#include "suri/Viewer2D.h"
#include "suri/World.h"
#include "suri/Vector.h"
#include "suri/Configuration.h"
#include "suri/VectorEditor.h"
#include "suri/FileManagementFunctions.h"
#include "suri/messages.h"
#include "resources.h"
#include "VectorElementEditor.h"
#include "AnotationElementEditor.h"
#include "SaveVectorButton.h"
#include "CloseVectorButton.h"
#include "PointElementEditor.h"
#include "LineElementEditor.h"
#include "PolygonElementEditor.h"
#include "PixelVectorButton.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/ElementManager.h"
#include "suri/DataViewManager.h"
#include "suri/DatasourceManagerInterface.h"
#include "VectorSelectionButton.h"
#include "VectorDeleteButton.h"
#include "VectorEditionButton.h"
#include "VectorCreationButton.h"
#include "suri/DynCommandButton.h"
#include "suri/Command.h"
#include "suri/ToolCommand.h"
#include "suri/StateChangeNotification.h"
#include "VectorEditionControllerCommand.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {

/** Ctor para clases que hereden de DrawTool. */
DrawTool::DrawTool(wxWindow *pParent, DataViewManager* pDataViewManager, const suri::SuriObject::UuidType& ViewcontextId) :
            Widget(pParent), pVectorEditor_(NULL), pSaveButton_(NULL), pCloseButton_(NULL),
            pDataViewManager_(pDataViewManager), viewcontextId_(ViewcontextId),
            anotationGroup_(0), vectorGroup_(0), pCoordsTransform_(NULL) {
   wantedHeight_ = BUTTON_SIZE;
   wantedWidth_ = -1;
}

/**
 * Constructor.
 * @param[in]	AnotationGroup: grupo de la lista para las anotaciones
 * @param[in]	VectorGroup: grupo de la lista para los vectores
 * @param[in]  pVectorEditor vector para edicion.
 * @param[in]	pParent: puntero a la ventana padre.
 */
DrawTool::DrawTool(int AnotationGroup, int VectorGroup, VectorEditor* pVectorEditor,
                   DataViewManager* pDataViewManager,
                   const suri::SuriObject::UuidType& ViewcontextId, wxWindow *pParent) :
      Widget(pParent), pVectorEditor_(pVectorEditor), pSaveButton_(NULL),
      pCloseButton_(NULL), pDataViewManager_(pDataViewManager),
      viewcontextId_(ViewcontextId), anotationGroup_(AnotationGroup),
      vectorGroup_(VectorGroup), pCoordsTransform_(NULL) {
   windowTitle_ = _(caption_DRAW_CREATION_TOOL);
   toolName_ = GetWindowTitle();
   wantedHeight_ = BUTTON_SIZE;
   wantedWidth_ = -1;
}

/**
 * Destructor
 */
DrawTool::~DrawTool() {
   if (pCoordsTransform_ != NULL)
      delete pCoordsTransform_;
}

/**
 * Genera un panel donde se insertan los botones de las herramientas de dibujo.
 * @return true.
 */
bool DrawTool::CreateToolWindow() {
   pToolWindow_ = new wxPanel(pParentWindow_, wxID_ANY);
   pToolWindow_->SetSizer(new wxBoxSizer(wxHORIZONTAL));
   if (!pViewer_ || !pToolWindow_) {
      REPORT_AND_FAIL_VALUE("D:No hay viewer, lista o ventana inicializado", false);
   }
   /** Parche para que los botones funcionen */
   pList_ = pViewer_->GetList();

   InitializeToolbar(pToolWindow_, pViewer_, pList_);
   InitializeVectorEditionButtons();

   pViewer_->Link(this);
   return true;
}

/**
 * Se ejecuta al modificar la activacion.
 * En la primera activacion crea los botones de medicion y los agrega a la
 * ventana.
 * Una vez creados los botones, para valor active_ = false busca el primer
 * ToggleButton del grupo que no pertenezca a la herramienta y lo activa.
 * Esto permite que si se cambia el combo de herramientas, seleccionando otra
 * no quede activa la funcionalidad de medicion.
 * @param[in] Active estado de activacion de la herramienta
 * @return informa si la operacion se pudo realizar
 * \todo La funcionalidad de deseleccion del boton es muy general, buscar donde
 *  puede ser mas util tenerla.
 * \todo eliminar primer if cuando los botones cierren los vectores
 */
bool DrawTool::DoSetActive(bool Active) {
   if (!Active) {
#ifdef __UNUSED_CODE__
      // Eliminar cuando los botones cierren los vectores
      if ( toolButtons_.find(pViewer_->GetActiveButtonTool() )!=toolButtons_.end() )
      {
         VectorElementEditor* activebutton=
         dynamic_cast<VectorElementEditor*>(pViewer_->GetActiveButtonTool() );
         if ( activebutton->IsVectorOpen() ) {
            activebutton->CloseVector();
         }
      }
#else
      // Obtiene el boton activo de la herramienta
      if (GetActiveButtonTool(ToolGroupManager::Edition)) {
         VectorEditorButton* pactivebutton =
               dynamic_cast<VectorEditorButton*>(GetActiveButtonTool(
                     ToolGroupManager::Edition));
         if (pactivebutton) {
            /** Comienzo chequeando si es un GeometryElement y luego un VectorElement */
            GeometryElementEditor *pgeoelement =
                  pactivebutton->GetGeometryElementEditor();
            /** Si GeometryElement */
            if (pgeoelement) {
               // Si el vector esta abierto, pregunta si salvar o descartar el vector
               while (pgeoelement && pgeoelement->IsVectorOpen()) {
                  switch(SHOWQUERY(GetWindow(), _(question_APPLY_CHANGES),
                              wxYES|wxNO|wxICON_QUESTION))
                  {
                     case wxID_YES:
                        pgeoelement->SaveVector();
                     break;
                     case wxID_NO:
                        pgeoelement->CloseVector();
                     break;
                  }
               }
            /** Si VectorElement */
            } else {
               // Si pudo obtener el boton, obtiene el VectorElementEditor
               VectorElementEditor *pvectorelement =
               pactivebutton->GetVectorElementEditor();
               // Si el vector esta abierto, pregunta si salvar o descartar el vector
               while (pvectorelement && pvectorelement->IsVectorOpen()) {
                  switch(SHOWQUERY(GetWindow(), _(question_APPLY_CHANGES),
                              wxYES|wxNO|wxICON_QUESTION))
                  {
                     case wxID_YES:
                     pvectorelement->SaveVector();
                     break;
                     case wxID_NO:
                     pvectorelement->CloseVector();
                     break;
                  }
               }

            }
            // Desactiva el boton de la herramienta que estaba activa
            DeactivateButton(pactivebutton);
         }
      }
#endif
      EnableControls(true);
   }
   SetEnabled(Active);
   return true;
}

/**
 * Determina si la herramienta puede activarse o no dependiendo de si hay
 * elementos en la lista, si estan activos y si el mundo tiene Referencia
 * Espacial.
 * Tambien verifica el estado de los vectores que se dibujan para habilitar o no
 * la toolbar principal. Esto es para prevenir errores que se producen al salvar
 * un proyecto sin antes haber cerrado los vectores.
 * \pre pViewer_ y pList_ deben estar inicializados
 * \pre pViewer_->GetWorld() debe retornar un mundo valido.
 * @return true El sistema de referencia del mundo acepta una transformacion
 *  de coordenadas.
 * @return true Hay al menos un elemento activo en la lista.
 * @return false No se inicializaron la lista, el viewer o el mundo.
 */
bool DrawTool::IsEnabled() {
   if (pViewer_ && pViewer_->GetWorld()) {
      if (lastSpatialReference_.compare(pViewer_->GetWorld()->GetSpatialReference())) {
         delete pCoordsTransform_;
         pCoordsTransform_ = NULL;
      }
      if (!pCoordsTransform_) {
         lastSpatialReference_ = pViewer_->GetWorld()->GetSpatialReference();
         ParameterCollection params;
         params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                      lastSpatialReference_);
         params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                      lastSpatialReference_);
         TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
         pCoordsTransform_ = pfactory->Create(TransformationFactory::kExact, params);
         TransformationFactoryBuilder::Release(pfactory);
      }
   }

   bool hastransform = pCoordsTransform_ != NULL;
   // condicion para habilitar o no la herramienta.
   bool retval = hastransform && pList_ && pList_->GetActiveCount() != 0;
   /* si hay un boton de la herramienta presionado y hay un vector abierto:
    *  - deshabilito las ventanas dependientes*/
   bool foundvectoropened = false;
   if (retval) {
      std::set<VectorEditorButton *>::iterator it = drawVectorButtons_.begin();
      for (; it != drawVectorButtons_.end() && !foundvectoropened; it++)
         if ((*it)->GetValue() && (*it)->IsActive()) {
            foundvectoropened = true;
         }
   }
   // Se recorren los botones de dibujo y se los des/habilita junto con la herramienta
   std::set<VectorEditorButton *>::iterator it = drawVectorButtons_.begin();
   for (; it != drawVectorButtons_.end(); it++)
      (*it)->SetEnabled(retval);

   EnableControls(!foundvectoropened);
   return retval;
}

/**
 * Metodo que se ejecuta cuando se desactiva un boton de la herramienta.
 * Si el boton que se desactiva es uno de los botones de dibujo, asigna NULL
 * al VectorElementEditor de los botones de salvar y cerrar vector.
 * @param[in] pButton puntero al boton que de desactiva
 */
void DrawTool::ButtonDeactivated(Button *pButton) {
   std::string filename;
   VectorEditorButton* pactivebutton =
         dynamic_cast<VectorEditorButton*>(pButton);
   if (pactivebutton) {
      VectorElementEditor* pvectorelementeditor =
            pactivebutton->GetVectorElementEditor();
      if (!pvectorelementeditor)
         return;

      pvectorelementeditor->CloseFeature();
      std::string pathtempaux = suri::Configuration::GetParameter(
            "app_vector_base_dir_volatile", wxT(""));
      pathtempaux = suri::GetNextPath(pathtempaux);
      std::string filename =
            wxFileName::CreateTempFileName(pathtempaux).c_str();
      filename += ".shp";
      // si no se puede guardar algun mensaje de error
      pvectorelementeditor->SaveVector(filename);
   } else {
      pButton->SetValue(false);
   }

}

/**
 * Metodo que se ejecuta cuando se activa un boton de la herramienta.
 * Obtiene el VectorEditorButton del boton que se activa, y lo asigna a los
 * botones salvar y cerrar vector (asigna NULL si no lo pudo obtener).
 * @param[in] pButton puntero al boton que se esta activando.
 */
void DrawTool::ButtonActivated(Button *pButton) {
   VectorElementEditor *pvectorelementeditor = NULL;
   GeometryElementEditor *pgeoelement = NULL;
   VectorEditorButton* pactivebutton = dynamic_cast<VectorEditorButton*>(pButton);

   if (pactivebutton) {
      pgeoelement = pactivebutton->GetGeometryElementEditor();

      if (pgeoelement)
      {
         /** Inicio el ciclo de edicion */
         if (pSaveButton_) {
            pSaveButton_->SetGeometryElementEditor(pgeoelement);
         }
         if (pCloseButton_) {
            pCloseButton_->SetGeometryElementEditor(pgeoelement);
         }
      }else {
         pvectorelementeditor = pactivebutton->GetVectorElementEditor();
         if (pSaveButton_) {
            pSaveButton_->SetVectorElementEditor(pvectorelementeditor);
         }
         if (pCloseButton_) {
            pCloseButton_->SetVectorElementEditor(pvectorelementeditor);
         }
      }

   }

}
/** Inicializa los botones para la edicion vectorial de diferentes geometrias **/
void DrawTool::InitializeVectorEditionButtons() {

   GuiGeometryCreator::GeometryType types[] = { GuiGeometryCreator::Polygon,
                                                GuiGeometryCreator::Line,
                                                GuiGeometryCreator::Point };

   GuiGeometryCreator::GeometryType geotype = GuiGeometryCreator::Polygon; // Default
   // configuracion de boton para inicio/finalizacion de edicion
   VectorEditionControllerCommand* pcommand = new VectorEditionControllerCommand;
   ParameterCollection* pparams = new ParameterCollection();
   pparams->AddValue<std::string>(DynCommandButton::kActivatedBitmap,
                                  button_STOP_ANIMATION);
   pparams->AddValue<std::string>(DynCommandButton::kDeactivatedBitmap,
                                  button_START_ANIMATION_SRC);
   pparams->AddValue<std::string>(DynCommandButton::kActivatedTooltip,
                                  tooltip_TASK_MENU);
   pparams->AddValue<std::string>(DynCommandButton::kDeactivatedTooltip,
                                  tooltip_TASK_MENU);
   pcommand->SetInputParameters(pparams);
   DynCommandButton* ptaskbutton = new DynCommandButton(pcommand);

   // Boton crear anotacion
   // Crea editor de elementos tipo anotacion y se lo pasa a boton de dibujo.
   VectorElementEditor *ptool = new AnotationElementEditor(pViewer_, anotationGroup_,
                                                           pVectorEditor_, pDataViewManager_);
   PixelVectorButton *ppixvecbuttton = new PixelVectorButton(pToolWindow_, pViewer_, wxT(
         tool_SIMUR_DRAW_ANNOTATION),
         ptool, tooltip_ANOTATION,
         BUTTON_LEFT_UP);

   pcommand->AddButtonToDeactivateGroup(ppixvecbuttton);

   AddButtonTool(ppixvecbuttton, Last,
                 ToolGroupManager::Edition | ToolGroupManager::Default);
   toolButtons_.insert(ppixvecbuttton);
   drawVectorButtons_.insert(ppixvecbuttton);
   // Botones de creacion:
   wxString bitmap = tool_SIMUR_DRAW_POLYGON;
   wxString tooltip = tooltip_POLYGON;

   for (unsigned int i = 0; i < ARRAY_SIZE(types); ++i) {
      switch (i) {
         case GuiGeometryCreator::Point:
            bitmap = tool_SIMUR_DRAW_POINT;
            tooltip = tooltip_POINT;
            geotype = GuiGeometryCreator::Point;
            break;
         case GuiGeometryCreator::Line:
            bitmap = tool_SIMUR_DRAW_LINE;
            tooltip = tooltip_LINE;
            geotype = GuiGeometryCreator::Line;
            break;
         default:
            bitmap = tool_SIMUR_DRAW_POLYGON;
            tooltip = tooltip_POLYGON;
            // geotype ya tiene por defecto el valor que le corresponde
            // a este caso.
            break;
      }

      VectorCreationButton *ppixvecbut = new VectorCreationButton(
            pToolWindow_, pViewer_, bitmap, geotype,
            tooltip, pDataViewManager_,
                     BUTTON_LEFT_UP | BUTTON_RIGHT_UP | BUTTON_MIDDLE_UP);
      AddButtonTool(ppixvecbut, Last,
                    ToolGroupManager::Edition | ToolGroupManager::Default);
      toolButtons_.insert(ppixvecbut);
      drawVectorButtons_.insert(ppixvecbut);
      pcommand->AddButtonToDeactivateGroup(ppixvecbut);
      ppixvecbut->Attach(pcommand);
      ppixvecbut->Attach(ptaskbutton);
   }

   AddButtonTool(ptaskbutton, Last,
                 ToolGroupManager::Edition | ToolGroupManager::Default);
   toolButtons_.insert(ptaskbutton);

   // Agrega geometrias a ya preexistente/s.
   VectorCreationButton *paddfeaturebut = new VectorCreationButton(
         pToolWindow_, pViewer_, tool_CREATE_FEATURE, geotype,
         tooltip_CREATE_FEATURE,
         pDataViewManager_,
         BUTTON_LEFT_UP | BUTTON_RIGHT_UP | BUTTON_MIDDLE_UP,
         false);
   paddfeaturebut->SetEnabled(false);
   AddButtonTool(paddfeaturebut, Last,
                 ToolGroupManager::Edition | ToolGroupManager::Default);
   toolButtons_.insert(paddfeaturebut);
   drawVectorButtons_.insert(paddfeaturebut);

   pcommand->AddButtonToActivateGroup(paddfeaturebut);

   // Boton de edicion
   VectorEditionButton *peditionbutton = new VectorEditionButton(
         pToolWindow_, pViewer_, tool_EDIT_VECTOR, GuiGeometryCreator::Polygon,
         tooltip_EDIT_VECTOR, pDataViewManager_,
         BUTTON_LEFT_UP | BUTTON_RIGHT_UP | BUTTON_MIDDLE_UP);

   peditionbutton->SetEnabled(false);
   pcommand->AddButtonToActivateGroup(peditionbutton);

   AddButtonTool(peditionbutton, Last,
                    ToolGroupManager::Edition | ToolGroupManager::Default);
   toolButtons_.insert(peditionbutton);
   drawVectorButtons_.insert(dynamic_cast<VectorEditorButton *>(peditionbutton));

   // Boton de borrar geometrias.
   VectorDeleteButton *pdelbutton = new VectorDeleteButton(
         pToolWindow_, pViewer_, button_DELETE_GEOMETRY, GuiGeometryCreator::Polygon,
         tooltip_REMOVE_GEOMETRY,
         pDataViewManager_,
         BUTTON_LEFT_UP | BUTTON_RIGHT_UP | BUTTON_MIDDLE_UP);
   pdelbutton->SetEnabled(false);
   AddButtonTool(pdelbutton, Last,
                 ToolGroupManager::Edition | ToolGroupManager::Default);
   toolButtons_.insert(pdelbutton);
   drawVectorButtons_.insert(dynamic_cast<VectorEditorButton *>(pdelbutton));
   pcommand->AddButtonToActivateGroup(pdelbutton);
}
}  // namespace suri

