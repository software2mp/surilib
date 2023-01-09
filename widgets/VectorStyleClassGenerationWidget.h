/*! \file VectorStyleClassGenerationWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORSTYLECLASSGENERATIONWIDGET_H_
#define VECTORSTYLECLASSGENERATIONWIDGET_H_

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "suri/DialogWidget.h"
#include "suri/VectorStyle.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class VectorLayer;

namespace ui {

class VectorStyleClassGenerationEventHandler;

class VectorStyleClassGenerationWidget : public DialogWidget {

   /** const copia **/
   VectorStyleClassGenerationWidget(const VectorStyleClassGenerationWidget&);

public:
   struct ClassDefinition {
      std::string name_;
      std::string condition_;
      std::string wktStyle_;
		/** tipo de clase: 1. rango, 2. valor unico, 0. custom **/
		int type_;
		ClassDefinition() : type_(0) {}
		bool IsRangeClass() {
			return type_ == 1;
		}
		bool IsUniqueValueClass() {
			return type_ == 2;
		}
   };

   /** constructor **/
   explicit VectorStyleClassGenerationWidget(VectorLayer *pVectorLayer);
   /** dtor **/
   virtual ~VectorStyleClassGenerationWidget();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Obtiene las clases generadas con el widget **/
   std::vector<ClassDefinition> GetClasses();
	/** Configuracion inicial de los datos del Part **/
	void SetInitialValues();
	/** Configura el estilo base para el widget **/
	void SetBaseStyle(const std::string& Style) {
		baseStyle_ = Style;
	}
	/** Configura el color initial **/
	void SetInitialColor(const VectorStyle::Color& InitialColor);
	/** Configura el color final **/
	void SetFinalColor(const VectorStyle::Color& FinalColor);
	/** Configura el tipo de generacion de clases. 1. Rango, 2. Valores unicos*/
	void SetClassGenerationType(int Type);
	int GetClassGenerationType() const;
	/** **/
	double GetMinValue() const;
	double GetMaxValue() const;
	void SetMinValue(double Min);
	void SetMaxValue(double Max);
	std::string GetField() const;
	void SetField(const std::string& Field);

protected:
   /** Metodo auxiliar para la actualizacion de las clases**/
   void UpdateClasses();
   /** Metodo que se llama cuando se presiona Cancel **/
   void OnCancelClick(wxCommandEvent& Event);
   /** Metodo que se llama cuando se presiona Aceptar **/
   void OnOkClick(wxCommandEvent& Event);
   /** Metodo que se llama cuando se cambia el choice de generacion de clases**/
   void OnGenMethodChanged(wxCommandEvent& Event);
   /** Metodo que se llama cuando se presiona el boton de definicion de estilo base **/
   void OnBaseStyleClicked(wxCommandEvent& Event);
	/** Metodo que se llama cuando cambia la seleccion del choice de campos para rango numerico **/
	void OnNumericFieldChange(wxCommandEvent& Event);
	/** Metodo auxiliar para el calculo de clases por rango numerico **/
   void GenerateNumericRangeClasses(const std::string& FieldName, double Min,
                                    double Max, int ClassCount);
   /** Metodo auxiliar para el calculo de clases por valores unicos **/
   void GenerateUniqueValueClasses(const std::string& FieldName);
   /** Metodo auxiliar que actualiza los controle min y max dependiendo el tipo de generacion de clases
	 *  que se indica
	 *  @param[in] ChoicePos posicion para seleccion del choice de generacion de clases 
	 **/
	void UpdateClassGenControls(int ChoicePos);
	friend class VectorStyleClassGenerationEventHandler;
   VectorStyleClassGenerationEventHandler* pEventHandler_;
   VectorLayer *pVectorLayer_;
   /** Estilo base para la generacion de clases **/
   std::string baseStyle_;
   std::vector<ClassDefinition> classes_;
	std::vector<double> fieldmins_;
	std::vector<double> fieldmaxs_;
};

} /** namespace ui */
} /** namespace suri */

#endif /* VECTORSTYLECLASSGENERATIONWIDGET_H_ */
