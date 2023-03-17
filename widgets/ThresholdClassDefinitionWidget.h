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
