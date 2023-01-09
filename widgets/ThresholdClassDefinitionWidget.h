/*! \file ThresholdClassDefinitionWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef THRESHOLDCLASSDEFINITIONWIDGET_H_
#define THRESHOLDCLASSDEFINITIONWIDGET_H_

// Includes Estandar
// Includes Suri
#include "suri/DialogWidget.h"
#include "suri/ProcessAtributeProvider.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class DataViewManager;
class Element;
class Table;
class TablePart;
class ThresholdClassDefinitionEventHandler;

/** Clase encargada de realizar la definicion de umbrales para una clase **/
class ThresholdClassDefinitionWidget : public DialogWidget,
                                       public ProcessAtributeProvider {
   /** ctor copia **/
   ThresholdClassDefinitionWidget(const ThresholdClassDefinitionWidget&);

public:
   /** Estructura que contiene los umbrales definidos en una banda */
   struct ThresholdBandInformation {
      int bandindex_;
      std::string bandname_;
      double max_;
      double min_;
   };
   /** Estructura que contiene un vector con los umbrales de cada banda **/
   struct ThresholdClassInformation {
      int classindex_;
      std::string classname_;
      std::string classhexacolor_;
      std::vector<ThresholdBandInformation> bandsThreshold_;
   };

   ThresholdClassDefinitionWidget(DataViewManager* pDataView, Element* pInputElement,
                                  ThresholdClassInformation& ClassInfo);
   ~ThresholdClassDefinitionWidget();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();

private:
   /**
    *  Metodo privado que inicializa la tabla que corresponde a la definicion
    *  de umbrales para una clase
    *  @return true en caso de que pueda configurarse la tabla
    *  @return false en caso contrario
    */
   bool InitializeTable();
   /** Metodo que se llama cuando se presiona Cancel **/
   void OnCancelClick(wxCommandEvent& Event);
   /** Metodo que se llama cuando se presiona Aceptar **/
   void OnOkClick(wxCommandEvent& Event);
   /** Metodo que verifica la validez de la definicion de una clase **/
   bool HasValidClassDefinition();
   TablePart* pTablePart_;
   Table* pTable_;
   DataViewManager* pDataView_;
   Element* pInputElement_;
   friend class ThresholdClassDefinitionEventHandler;
   ThresholdClassDefinitionEventHandler* pEventHandler_;
   ThresholdClassInformation& currentClassInfo_;
};

} /** namespace suri */

#endif /* THRESHOLDCLASSDEFINITIONWIDGET_H_ */
