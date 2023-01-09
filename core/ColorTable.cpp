/*! \file ColorTable.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <utility>
#include <string>
#include <vector>

#include "ColorTable.h"

// Includes Suri
#include "suri/xmlnames.h"

// defines
/** Cantidad de bandas que debe tener a la salida para ser una tabla de colores */
#define COLORTABLE_OUTPUT_BAND_COUNT 3

/** namespace suri */
namespace suri {
/** Ctor */
ColorTable::ColorTable() {
   LookUpTable lookuptable;
   lut_.AddLookUpTable(lookuptable, 0);
   lut_.AddLookUpTable(lookuptable, 0);
   lut_.AddLookUpTable(lookuptable, 0);
}

/** Dtor */
ColorTable::~ColorTable() {
}

/**
 * Usa LutArray para leer el nodo y verifica que sea una tabla de colores.
 * @param[in] pColorTableNode Nodo xml que quiero leer
 * @param[out] CTable Clase ColorTable configurada con los datos internos del
 * renderizador.
 * @return bool que indica si se pudo generar ColorTable a partir del nodo.
 */
bool ColorTable::FromXml(const wxXmlNode *pColorTableNode, ColorTable &CTable) {
   if (!pColorTableNode) {
      return false;
   }
   if (pColorTableNode->GetName() != wxT(COLOR_TABLE)) {
      return false;
   }
   wxXmlNode tempnode = *pColorTableNode;
   LutArray lutarray;
   tempnode.SetName(wxT(LUT_NODE));
   if (!LutArray::FromXml(&tempnode, lutarray)) {
      REPORT_DEBUG("D: El nodo no es de una color table");
      return false;
   }
   if (!VerifyLutArray(lutarray)) {
      REPORT_DEBUG("D: El nodo no es de una color table");
      return false;
   }
   CTable.lut_ = lutarray;
   return true;
}

/**
 * Si la lutArray_ esta bien configurada, le configura tipo="tabla de colores"
 * y transforma el contenido en un nodo.
 * @param[in] CTable Clase ColorTable que quiero transformar a xml.
 * @param[out] pColorTableNode Nodo xml con las tablas. Este nodo pasa a ser
 * propiedad del que invoco este metodo.
 * @return indica si tubo exito la operacion
 */
bool ColorTable::ToXml(const ColorTable &CTable, wxXmlNode* &pColorTableNode) {
   if (!VerifyLutArray(CTable.lut_) || !LutArray::ToXml(CTable.lut_, pColorTableNode)) {
      REPORT_DEBUG("D: La color table esta mal configurada");
      return false;
   }
   pColorTableNode->SetName(wxT(COLOR_TABLE));
   return true;
}

/** Convierte un array a un Nodo xml */
bool ColorTable::ToXml(const ColorTable &CTable, wxXmlNode* &pColorTableNode,
                       std::vector<std::pair<std::string, std::string> > Labels) {
   // Ejecuto el procedimiento normal para bajar a XML
   ToXml(CTable, pColorTableNode);

   // Agrego las labels
   wxXmlNode* plabels = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(COLOR_TABLE_LABELS));
   for (int ix = 0, lenix = Labels.size(); ix < lenix; ++ix) {
      wxXmlNode* pcontent = new wxXmlNode(NULL, wxXML_TEXT_NODE, wxEmptyString,
                                          wxT(Labels[ix].second.c_str()));

      wxXmlProperty* pprop = new wxXmlProperty(wxT(COLOR_TABLE_BOUNDARY),
                                               wxT(Labels[ix].first.c_str()));

      wxXmlNode* plabel = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(COLOR_TABLE_LABEL),
                                        wxEmptyString, pprop);
      plabel->AddChild(pcontent);
      plabels->AddChild(plabel);
   }
   pColorTableNode->AddChild(plabels);

   return true;
}

/**
 * Agrega el comienzo de un intervalor y el color asociado a la tabla
 * de colores.
 * @param[in] LowerBound limite inferior del intervalo que agrego a la tabla
 * de colores
 * @param[in] Red Valor que toma la banda roja en el intervalo
 * @param[in] Green Valor que toma la banda verde en el intervalo
 * @param[in] Blue Valor que toma la banda azul en el intervalo
 */
void ColorTable::AddColor(double LowerBound, int Red, int Green, int Blue) {
   LutArray newlut;
   newlut.SetName(lut_.GetName());
   newlut.active_ = lut_.active_;
   int inputband = 0;

   // Rojo
   LookUpTable lookuptable = lut_.GetLookUpTable(0, inputband);
   lookuptable.AddBin(LowerBound, Red);
   newlut.AddLookUpTable(lookuptable, inputband);
   // Verde
   lookuptable = lut_.GetLookUpTable(1, inputband);
   lookuptable.AddBin(LowerBound, Green);
   newlut.AddLookUpTable(lookuptable, inputband);
   // Azul
   lookuptable = lut_.GetLookUpTable(2, inputband);
   lookuptable.AddBin(LowerBound, Blue);
   newlut.AddLookUpTable(lookuptable, inputband);

   lut_ = newlut;
}

/** Elimina el contenido de la tabla */
void ColorTable::ResetColorTable() {
   lut_ = LutArray();
}

/**
 * Retorna el nombre de la tabla de colores
 * @return nombre de la tabla de colores.
 */
std::string ColorTable::GetName() const {
   return lut_.GetName();
}

/**
 * Modifica el nombre de la tabla de colores
 * @param[in] LutName Nombre de la tabla de colores
 */
void ColorTable::SetName(const std::string &LutName) {
   lut_.SetName(LutName);
}

/**
 * Informa si la tabla de colores esta activa
 * @return bool estado de la tabla de colores
 */
bool ColorTable::IsActive() const {
   return lut_.active_;
}

/**
 * Modifica el estado de activacion de la tabla de colores. Determina
 * si se debe renderizar o no.
 * @param[in] Active nuevo estado de la tabla de colores
 */
void ColorTable::Activate(bool Active) {
   lut_.active_ = Active;
}

/**
 * Verifica que tenga una banda de entrada y tres de salida.
 * @param[in] Lut LutArray que se desea analizar
 * @return informa si Lut es una tabla de colores
 */
bool ColorTable::VerifyLutArray(const LutArray &Lut) {
   // El numero de bandas de salida debe ser 3
   if (Lut.GetCount() != COLORTABLE_OUTPUT_BAND_COUNT) {
      return false;
   }

   // Analizo que no se modifique el orden de las bandas.
   int inputband = 0;
   for (int i = 0; i < Lut.GetCount(); i++) {
      Lut.GetLookUpTable(i, inputband);
      if (inputband != 0) {
         return false;
      }
   }

   return true;
}

/**
 * Compara valores del tipo ColorTable.
 * @param[in] OtherColorTable tabla de colores contra la que estoy comparando
 * la clase
 * @return bool que indica si son iguales
 */
bool ColorTable::operator==(const ColorTable &OtherColorTable) const {
   // El numero de bandas de salida debe ser 3
   if (lut_.GetCount() != OtherColorTable.lut_.GetCount()) {
      return false;
   }

   // Analizo que no se modifique el orden de las bandas.
   int inputband = 0;
   int otherinputband = 0;
   LookUpTable::LutType luttype;
   LookUpTable::LutType otherluttype;
   for (int i = 0; i < OtherColorTable.lut_.GetCount(); i++) {
      lut_.GetLookUpTable(i, inputband).GetTable(luttype);
      OtherColorTable.lut_.GetLookUpTable(i, otherinputband).GetTable(otherluttype);
      if (luttype != otherluttype || inputband != otherinputband) {
         return false;
      }
   }
   return true;
}

/**
 * Retorna la tabla y banda de entrada para una banda de salida dada
 * @param[in] OutputBand banda de salida cuya tabla se busca
 * @return LookUpTable de la banda de salida pedida. Si no se encontro se
 * retorna vacia
 */
LookUpTable ColorTable::GetLookUpTable(int OutputBand) const {
   if (OutputBand > 2) {
      return LookUpTable();
   }
   int inputband = 0;
   return lut_.GetLookUpTable(OutputBand, inputband);
}
}
