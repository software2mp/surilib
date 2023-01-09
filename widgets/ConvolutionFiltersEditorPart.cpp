/**
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

#include "ConvolutionFiltersEditorPart.h"
#include "DefaultDatasourceManager.h"
#include "DefaultViewcontextManager.h"
#include "DefaultTable.h"
#include "TablePart.h"
#include "TablePartConfigurator.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/ConvolutionFilterItemAttribute.h"
#include "suri/LibraryItemAttributeFactory.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/MemoryDriver.h"

#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/wx.h>

#define TOKEN ","
#define OPEN_BRACKET "{"
#define CLOSE_BRACKET "}"
#define FILTER_ONE "Filtro 1"

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(ConvolutionFiltersEditorPartEventHandler, ConvolutionFiltersEditorPart)
      IMPLEMENT_EVENT_CALLBACK(OnRefreshClick, OnRefreshClick(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnEquationCheckClick, OnEquationCheckClick(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnAddFilterClick, OnAddFilterClick(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnDeleteFilterClick, OnDeleteFilterClick(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

/**
 * Esta clase representa un filtro que esta compuesto por un valor nominal y
 * una matriz de dimension variable.
 * Params @in: pAttr = Atributo complejo que contiene el valor nominal y la matriz
 * con sus valores.
 * Params @in: Dimension = Cantidad de filtros que contendra el item.
 */
class PageFilterWidget : public Widget {

public:
   PageFilterWidget(ConvolutionFilterItemAttribute* pAttr, int Dimension) :
         pFilterTable_(NULL), pattr_(pAttr), pTablePart_(NULL), dim_(Dimension) {
   }

   virtual ~PageFilterWidget() {
      delete pFilterTable_;
      delete pattr_;
   }

   /** Crea el Tool Window */
   bool CreateToolWindow() {
      if(!pToolWindow_)
         pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
      wxT("ID_FILTER_PAGE_PANEL"));
      if (!pToolWindow_)
         return false;
      SetInitialValues();
      return true;
   }

   void SetInitialValues() {
      // TODO(Gabriel): Falta encapsular la obtencion del valor nominal
      std::string tempnominal = "0";
      if ( pattr_ )
         tempnominal = pattr_->GetAttribute(0);

      XRCCTRL(*pToolWindow_, wxT("ID_NOMINAL_NUM"), wxTextCtrl)->SetValue(
            tempnominal);

      nominal_ = StringToNumber<int>(tempnominal);
      pFilterTable_ = new DefaultTable;
      MemoryDriver* pdriver = new MemoryDriver;
      for (int k = 0; k < dim_; ++k) {
         std::stringstream ss;
         ss << "A";
         ss << k;
         pdriver->AppendColumn(DataInfo<float>::Name, ss.str());
      }

      for (int j = 0; j < dim_; ++j) {
         // nueva fila
         pdriver->AppendRow();
         for (int k = 0; k < dim_; ++k) {
            int value = 0;
            if (pattr_)
               value = (float) pattr_->GetConvolutionFilterMatrixValues(j, k);
            pdriver->WriteFloat(k, j, value);
         }
      }
      pFilterTable_->SetDriver(pdriver);
      suri::TablePartConfigurator tableconf;
      suri::Option opt;
      pTablePart_ = tableconf.Initialize(NULL, pFilterTable_, opt);
      AddControl(pTablePart_->GetWidget(), "ID_TABLE_PANEL");
   }

   virtual bool CommitChanges() {
      nominal_ = StringToNumber<int>(XRCCTRL(*pToolWindow_, 
               wxT("ID_NOMINAL_NUM"), wxTextCtrl)->GetValue().c_str());
      return pTablePart_->CommitChanges();
   }

   bool HasChanged() {
      return pTablePart_->HasChanged() || nominal_ != StringToNumber<int>(XRCCTRL(*pToolWindow_,                                             
                wxT("ID_NOMINAL_NUM"), wxTextCtrl)->GetValue().c_str());
   }

   int GetNominal() const {
      return StringToNumber<int>(XRCCTRL(*pToolWindow_, 
               wxT("ID_NOMINAL_NUM"), wxTextCtrl)->GetValue().c_str());
   }

   void SetFilterId(int FilterId) { filterId_ = FilterId; }

   int GetFilterId() { return filterId_; }

private:
      friend class ConvolutionFiltersEditorPart;
      Table* pFilterTable_;
      ConvolutionFilterItemAttribute* pattr_;
      TablePart * pTablePart_;
      int nominal_;
      int dim_;
      int filterId_;
};

/** Nombre del XRC que representa la ventana. */
const std::string ConvolutionFiltersEditorPart::CONVOLUTIONFILTERS_PANEL = wxT(
      "ID_CONVOLUTION");

/** Nombre del panel donde estableceremos el CGU */
const std::string ConvolutionFiltersEditorPart::CONVOLUTIONFILTERS_CGU_PANEL = wxT(
      "ID_CGU_PANEL");

/**
 * Ctor.
 */
ConvolutionFiltersEditorPart::ConvolutionFiltersEditorPart(
      const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
      UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature) :
      LibraryItemEditorPart(LibraryId, pManager, CONVOLUTIONFILTERS_PANEL,
                            CONVOLUTIONFILTERS_CGU_PANEL, LibraryManager::READONLY,
                            Feature),
      NEW_EVENT_OBJECT(ConvolutionFiltersEditorPartEventHandler),
      pCurrentItem_(NULL),
      pNotebook_(NULL), matrixdimension_(1), filtersnumber_(-1) {
}

/**
 * Dtor.
 */
ConvolutionFiltersEditorPart::~ConvolutionFiltersEditorPart() {
   DELETE_EVENT_OBJECT;
}

/** Metodo que actualiza los datos cambiados en el part */
void ConvolutionFiltersEditorPart::UpdateFields(const LibraryItem* pItem,
                                                bool Modifiable) {
   if (!pToolWindow_)
      return;
   if (pItem) {
      const LibraryItemAttribute* pattr = pItem->GetAttribute("eq");
      std::string equation = pattr ? pattr->GetValue() : "";
      GET_CONTROL(*(this->GetWidget()->GetWindow()),
            "ID_EQ_TEXTCTRL", wxTextCtrl)->SetValue(equation);
      const LibraryItemAttribute* psizeattr = pItem->GetAttribute("size");
      matrixdimension_ = wxAtoi(psizeattr ? psizeattr->GetValue().c_str() : "1");
      GET_CONTROL(*(this->GetWidget()->GetWindow()),
            "ID_SPIN_SIZE", wxSpinCtrl)->SetValue(matrixdimension_);
      LoadItemToTable(pItem);
   }
}

/** Metodo que transforma los datos en atributos para ser guardados */
std::vector<LibraryItemAttribute*>* ConvolutionFiltersEditorPart::CreateAttributesFromFields() {
   /** Guardar los atributos como un nuevo filtro de convolucion.*/
   // TODO: Recorro las tablas y armo los filtro
   std::vector<LibraryItemAttribute *>* pattrs = new std::vector<LibraryItemAttribute* >;
   // Itero por las paginas del notebook
   std::string eq = XRCCTRL(*(this->GetWidget()->GetWindow()), 
                     "ID_EQ_TEXTCTRL", wxTextCtrl)->GetValue().c_str();
   int size = XRCCTRL(*(this->GetWidget()->GetWindow()), "ID_SPIN_SIZE", wxSpinCtrl)->GetValue();
   int dim = pNotebook_->GetPageCount();

   pattrs->push_back(new LibraryItemAttribute("size", NumberToString<int>(size)));
   pattrs->push_back(new LibraryItemAttribute("dim", NumberToString<int>(dim)));
   pattrs->push_back(new LibraryItemAttribute("eq", eq));
   
   std::string filtername = LibraryItemAttributeFactory::ConvolutionFilterString;
   for (size_t f = 0; f < filters_.size(); ++f) {
      filters_[f]->CommitChanges();
      std::string name = LibraryItemAttributeFactory::ConvolutionFilterString + NumberToString<int>(filters_[f]->GetFilterId());
      ComplexItemAttribute filter(name, "");
      Table* ptable = filters_[f]->pFilterTable_;
      filter.AddAttribute(new LibraryItemAttribute("nominal", NumberToString(filters_[f]->GetNominal())));
      for (int col = 0; col < ptable->GetColumns(); ++col)
         for (int row = 0; row < ptable->GetRows(); ++row) {
            std::string value;
            ptable->GetCellValue(col, row, value);
            filter.AddAttribute(new LibraryItemAttribute("cell", value));
         }
      pattrs->push_back(new LibraryItemAttribute(filter.GetName(), 
                        filter.GetSimpleAttributeValue()));
   }
   return pattrs;
}

/**
 * Agrega un nuevo filtro.
 */
void ConvolutionFiltersEditorPart::AddNewFilter(ConvolutionFilterItemAttribute* pAttr, int FilterId) {
   int fid = FilterId == -1 ? ++filtersnumber_ : FilterId;

   wxPanel* ppanel = new wxPanel(pNotebook_, wxID_ANY);
   ppanel->SetSizer(new wxBoxSizer(wxVERTICAL));

   PageFilterWidget* pfilterwidget = new PageFilterWidget(pAttr, matrixdimension_);
   pfilterwidget->SetFilterId(fid);
   AddControl(pfilterwidget, ppanel);

   pNotebook_->AddPage(ppanel, (std::string("Filtro ") + NumberToString<int>(fid + 1)).c_str());

   filters_.push_back(pfilterwidget);
}

/** Actualiza el estado del notebook widget. */
void ConvolutionFiltersEditorPart::UpdateNotebook() {
   pNotebook_->Refresh();
   pNotebook_->Update();
}

/** Obtiene un vector con los IDs de los filtros disponibles ordenados de menor a mayor. */
std::vector<int> ConvolutionFiltersEditorPart::GetFids(LibraryItem::MapAttribute& Attributes) {
   std::vector<int> fids;
   LibraryItem::MapAttribute::const_iterator itattr = Attributes.begin();
   for (; itattr != Attributes.end(); ++itattr) {
      if (!LibraryItemAttributeFactory::IsConvolutionFilterItemAttribute(itattr->second))
         continue;

      ConvolutionFilterItemAttribute* pfilter =
            LibraryItemAttributeFactory::CreateConvolutionFilterItemAttribute(
                  itattr->second, matrixdimension_);

      if (pfilter != NULL) {
         std::string strfid = pfilter->GetName().substr(
               LibraryItemAttributeFactory::ConvolutionFilterString.size());

         fids.push_back(StringToNumber<int>(strfid));
      }
   }
   std::stable_sort(fids.begin(), fids.end());
   return fids;
}

/** Indica si el Part tiene cambios para salvar */
bool ConvolutionFiltersEditorPart::HasChanged() {
   bool changed = false;
   for (size_t i = 0; !changed && i < filters_.size(); ++i)
      changed |= filters_[i]->HasChanged();
   return changed || (filtersToRemove_.size() > 0);
}

/**
 * Obtiene un vector con los atributos marcados para ser eliminados.
 */
std::vector<LibraryItemAttribute*>* ConvolutionFiltersEditorPart::GetAttributesToRemove() {
   std::vector<LibraryItemAttribute *>* pattrs = new std::vector<LibraryItemAttribute*>;

   for (int f = 0, fsize = filtersToRemove_.size(); f < fsize; ++f) {
      ComplexItemAttribute filter(filtersToRemove_[f], "");
      pattrs->push_back(new LibraryItemAttribute(filter.GetName(), ""));
   }
   return pattrs;
}

/**
 * Metodo para hacer limpieza. Este codigo esta puesto para cubrir una
 * emergencia. Es para salir del paso. Cuando los parts dejen de existir
 * esto va a desaparecer y no hay problema.
 */
bool ConvolutionFiltersEditorPart::CleanUp() {
   filtersToRemove_.clear();
   return true;
}

/**
 * Metodo auxiliar privado que configura el part en si para cada editor en particular.
 * Este metodo es el que realmente tienen que implementar las clases hijas para
 * lograr la especializacion a la hora de iniciar el part (metodo que se llama
 * en CreateToolWindow, a modo de Template Method).
 **/
bool ConvolutionFiltersEditorPart::ConfigureFeatures() {
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_REFRESH_BTN", wxBitmapButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(ConvolutionFiltersEditorPartEventHandler::OnRefreshClick),
                                                         NULL, pEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_CHECK_EQ_BTN", wxBitmapButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(ConvolutionFiltersEditorPartEventHandler::OnEquationCheckClick),
                                                         NULL, pEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_NEW_FILTER", wxBitmapButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(ConvolutionFiltersEditorPartEventHandler::OnAddFilterClick),
                                                         NULL, pEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_DELETE_FILTER", wxBitmapButton)->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
   wxCommandEventHandler(ConvolutionFiltersEditorPartEventHandler::OnDeleteFilterClick),
                                                         NULL, pEventHandler_);

   return true;
}

/**
 *
 */
void ConvolutionFiltersEditorPart::LoadItemToTable(const LibraryItem* pItem) {
   if (pCurrentItem_
         && pCurrentItem_->GetPrincipalAttribute()->GetValue().compare(
               pItem->GetPrincipalAttribute()->GetValue()) == 0) return;

   pCurrentItem_ = pItem;

   wxPanel* pnbpanel = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_CONVOLUTION_FILTERS_PANEL"), wxPanel);

   //
   if (pNotebook_)
      RemoveControl(pNotebook_);
   pNotebook_ = new wxNotebook(pnbpanel, wxID_ANY);
   AddControl(pNotebook_, pnbpanel);

   //
   for (size_t f = 0, flen = filters_.size(); f < flen; ++f)
      RemoveControl(filters_[f]);

   filters_.clear();
   filtersnumber_ = -1;
   std::string fid;

   const LibraryItemAttribute* psizeattr = pItem->GetAttribute("size");

   // Tomo valores de ambos atributos.
   int matrixdimension_ = psizeattr ? StringToNumber<int>(psizeattr->GetValue()) : 1;
   LibraryItem::MapAttribute attributes = pItem->GetAttributes();
   std::vector<int> fids = GetFids(attributes);
   for (int ix = 0, ixsize = fids.size(); ix < ixsize; ++ix) {
      std::string fkey = LibraryItemAttributeFactory::ConvolutionFilterString
            + NumberToString<int>(fids[ix]);

      LibraryItem::MapAttribute::iterator attr = attributes.find(fkey);
      if (attr != attributes.end()) {
         ConvolutionFilterItemAttribute* pfilter =
               LibraryItemAttributeFactory::CreateConvolutionFilterItemAttribute(
                     attr->second, matrixdimension_);

         if (pfilter != NULL) {
            AddNewFilter(pfilter, fids[ix]);
         }
      }
   }
   filtersnumber_ = (fids.size() > 0) ? fids[fids.size() - 1] : -1;
   UpdateNotebook();
   pToolWindow_->Layout();
}

/** Chequea la ecuacion ingresada */
void ConvolutionFiltersEditorPart::OnEquationCheckClick(wxCommandEvent& Event) {
   EquationParser eqparser;
   wxString eqstr = USE_CONTROL(*GetWidget()->GetWindow(),
         wxT("ID_EQ_TEXTCTRL"), wxTextCtrl, GetValue(), "");
   if(!eqparser.SetEquation(eqstr.c_str()))
      SHOW_ERROR("Ecuacion con formato erroneo - Revise por favor");
}

/** Reajusta la dimension de la matriz y resetea sus valores a 0. */
void ConvolutionFiltersEditorPart::OnRefreshClick(wxCommandEvent& Event) {
   // Antes que nada chequeo si hay varianza entre la vieja dimension
   // y "la nueva".
   int newdimension = USE_CONTROL(*GetWidget()->GetWindow(), wxT("ID_SPIN_SIZE"),
                                  wxSpinCtrl, GetValue(), -1);
   if (newdimension == matrixdimension_)
      return;

   matrixdimension_ = newdimension;

   // Remuevo notebook para crear uno nuevo.
   if (pNotebook_ != NULL) {
      for (int ix = 0, ixlen = pNotebook_->GetPageCount(); ix < ixlen; ++ix) {
         std::string pagename = pNotebook_->GetPageText(ix).c_str();
         std::string strfid = pagename.substr(std::string("Filtro ").size());
         std::string filtername = LibraryItemAttributeFactory::ConvolutionFilterString + NumberToString<int>(StringToNumber<int>(strfid) - 1);
         filtersToRemove_.push_back(filtername);
      }
      RemoveControl(pNotebook_);
   }

   filters_.clear();

   wxPanel* pnbpanel = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_CONVOLUTION_FILTERS_PANEL"), wxPanel);

   pNotebook_ = new wxNotebook(pnbpanel, wxID_ANY);
   AddControl(pNotebook_, pnbpanel);

   AddNewFilter();
   UpdateNotebook();
   pToolWindow_->Layout();
}

/** Agrega un nuevo filtro al item */
void ConvolutionFiltersEditorPart::OnAddFilterClick(wxCommandEvent& Event) {
   AddNewFilter();
   UpdateNotebook();
   pToolWindow_->Layout();
}

/** Elimina el filtro seleccionado */
void ConvolutionFiltersEditorPart::OnDeleteFilterClick(wxCommandEvent& Event) {
   if (pNotebook_->GetPageCount() == 0)
      return;

   int pagetodelete = pNotebook_->GetSelection();
   wxPanel* ppage = dynamic_cast<wxPanel*>(pNotebook_->GetPage(pagetodelete));

   pNotebook_->RemovePage(pagetodelete);

   std::vector<PageFilterWidget*>::iterator it = filters_.begin();
   for(int count = 0 ; it != filters_.end(); ++it, ++count) {
      if (pagetodelete == count) {
         std::string name = LibraryItemAttributeFactory::ConvolutionFilterString + NumberToString<int>((*it)->GetFilterId());
         filters_.erase(it);
         filtersToRemove_.push_back(name);
      }
   }

   // Esto hay que hacerlo asi porque el DeletePage no solo borra la pagina sino
   // que tambien borra al padre (una cosa de locos). El RemovePage borra la pagina
   // pero no borra el control e internamente las paginas apuntan a cualquier lado
   // (a los hijos de la pagina que se quito)
   if (ppage != NULL) {
      ppage->Reparent(NULL);
      ppage->Destroy();
   }

   // Acomodamos y ajustamos controles.
   UpdateNotebook();
   pToolWindow_->Layout();
}

}  // namespace suri
