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
#include <sstream>
#include <utility>
#include <map>
#include <string>
#include <vector>
// Includes Suri
#include "BandMathRenderer.h"
#include "suri/DataTypes.h"
#include "suri/xmlnames.h"
#include "logmacros.h"
#include "suri/Element.h"
#include "MemoryCanvas.h"
#include "EquationParser.h"
// Includes Wx
#include "wx/xml/xml.h"
// Includes App
// Defines

/** Macro para registrar el renderer */
AUTO_REGISTER_RENDERER(suri::BandMathRenderer);

/** namespace suri */
namespace suri {

/**
 * param[in] pBands: bandas en imagen de entrada
 * param[in] Size: cantidad de pixeles en imagenes de entrada
 * param[in] Equation: ecuacion que quiero realizar sobre bandas
 * param[in] EquationVariables: mapa 'nombre de variable' => 'nro de banda'
 * param[out] pDest: banda con el resultado de aplicar la ecuacion a cada pixel.
 */
template<typename T>
void bandmathoperation(std::vector<void*> pBands, size_t Size, std::string Equation,
                                 std::map<std::string, int> EquationVariables,
                                 BAND_MATH_OUTPUT_DATA_TYPE* pDest) {

   // Inicializo vector 'inputvalues' con bandas de entrada de tipo correcto
   T** inputvalues = new T* [pBands.size()];
   for (size_t i = 0; i < pBands.size(); i++)
      inputvalues[i] = static_cast<T*>(pBands[i]);

   // Inicializo EquationParser con la ecuacion
   EquationParser eqparser;
   eqparser.SetEquation(Equation);
   std::vector<std::string> variablenames;
   eqparser.GetVariableNames(variablenames);
   int variablecount = variablenames.size();

   // Configura vectores con variables que usa parser para realizar calculos.
   double** pvariables = new double*[variablenames.size()];
   std::vector<int> passociatedband(variablenames.size());
   for (size_t i = 0; i < variablenames.size(); i++) {
      eqparser.GetVariableValuePointer(variablenames[i], pvariables[i]);
      passociatedband[i] = EquationVariables[variablenames[i]];
   }

   // Calcula banda de salida
   for (size_t i = 0; i < Size; i++) {
      for (int j = 0; j < variablecount; j++)
         *(pvariables[j]) = inputvalues[passociatedband[j]][i];
      pDest[i] = eqparser.EvaluateEquation();
   }
}
/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(BandMathFunctionType, bandmathoperation);

/** Ctor */
BandMathRenderer::BandMathRenderer() {
}

/** Dtor */
BandMathRenderer::~BandMathRenderer() {
}

/**
 * Genera un nodo XML a partir de los parametros
 * @param[in] RenderParameters parametros con los que se generara el nodo
 * @return palgebranode nodo algebra de bandas o NULL
 */
wxXmlNode *BandMathRenderer::GetXmlNode(
      const BandMathRenderer::Parameters &RenderParameters) {
   wxXmlNode *palgebranode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(ALGEBRA_NODE));

   std::vector<Parameters::Equation>::const_iterator itequations =
         RenderParameters.equations_.begin();
   while (itequations != RenderParameters.equations_.end()) {
      wxXmlNode *pequationnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                               wxT(EQUATION_NODE));

      // Guardo el nombre y valor de las bandas como propiedades
      std::map<std::string, int>::const_iterator itbandnames =
            itequations->bandNames_.begin();
      for (; itbandnames != itequations->bandNames_.end(); itbandnames++) {
         std::stringstream bandnumber;
         bandnumber << itbandnames->second;
         pequationnode->AddProperty(itbandnames->first, bandnumber.str().c_str());
      }

      // Guardo ecuacion
      new wxXmlNode(pequationnode, wxXML_TEXT_NODE, wxT(""), itequations->equation_);

      palgebranode->AddChild(pequationnode);

      itequations++;
   }

   return palgebranode;
}

/**
 * Genera los parametros a partir de un nodo. Busca el nodo del BandMath
 * en el nodo renderizacion del elemento y carga en parameters los
 * valores de la(s) ecuaciones.
 * @param[in] pNode del elemento
 */
BandMathRenderer::Parameters BandMathRenderer::GetParameters(const wxXmlNode *pNode) {
   BandMathRenderer::Parameters params;

   wxXmlNode *pnode = NULL;
   // Obtengo nodo de renderizacion
   wxXmlNode *pdnode = pNode->GetChildren();
   wxXmlNode *pcnode = NULL;
   while (pdnode != NULL) {
      // Obtengo nodo renderizacion
      if (pdnode->GetName() == wxT(RENDERIZATION_NODE)) {
         pcnode = pdnode->GetChildren();
      }
      pdnode = pdnode->GetNext();
   }
   // Obtengo nodo lut
   while (pcnode != NULL) {
      if (pcnode->GetName() == wxT(ALGEBRA_NODE)) {
         pnode = pcnode;
         pcnode = NULL;
      } else {
         pcnode = pcnode->GetNext();
      }
   }

   if (pnode) {
      // Recorro las tablas y las cargo
      pnode = pnode->GetChildren();
      int band;
      while (pnode != NULL) {
         if (pnode->GetName() == wxT(EQUATION_NODE)) {
            BandMathRenderer::Parameters::Equation equation;
            // Cargo la funcion en lista
            equation.equation_ = pnode->GetNodeContent().c_str();
            // Cargo los nombres/valores de las bandas
            wxXmlProperty* pproperties = pnode->GetProperties();

            while (pproperties != NULL) {
               std::stringstream valuesstream(pproperties->GetValue().c_str());
               valuesstream >> band;
               equation.bandNames_.insert(
                     std::pair<std::string, int>(pproperties->GetName().c_str(), band));
               pproperties = pproperties->GetNext();
            }
            // Agrego equation a parameters
            params.equations_.push_back(equation);
         }
         pnode = pnode->GetNext();
      }
   } else {
      REPORT_DEBUG("D: Se intento obtener parametros de LUT de elemento sin este nodo");
   }

   return params;
}

/**
 * Crea un renderizador a partir de un pElement
 * @param[in] pElement elemento a partir del cual se crea el renderizador
 * @param[in] pPreviousRenderer renderer anterior en el pipeline
 */
BandMathRenderer::Renderer *BandMathRenderer::Create(
      Element *pElement, Renderer *pPreviousRenderer) const {
   if (!pPreviousRenderer) {
      return NULL;
   }

   // Si no hay un nodo algebra con al menos una ecuacion falla
   wxString nodepath = wxString(wxT(RENDERIZATION_NODE)) + wxT(NODE_SEPARATION_TOKEN)
         + wxString(CreatedNode().c_str()) + wxT(NODE_SEPARATION_TOKEN)
         + wxString(wxT(EQUATION_NODE));

   wxXmlNode *pnode = pElement->GetNode(nodepath);
   if (!pnode) {
      REPORT_ERROR(
            "D:No se pudo encontrar el nodo XML en el BandMathRenderer\"%s\"",
            														pElement->GetName().c_str());
      return NULL;
   }

   // Leo parametros de nodo
   Parameters params;
   params = GetParameters(pElement->GetNode(wxT("")));

   // Creo nuevo renderizador
   BandMathRenderer *pbandmath = new BandMathRenderer;
   if (!pbandmath->LoadEquations(params, pPreviousRenderer)) {
      REPORT_ERROR("D:error en los datos del nodo");
      delete pbandmath;
      return NULL;
   }
   return pbandmath;
}

/**
 * Nombre del renderizador == al nombre del nodo
 * @return string con el nombre del renderizador
 */
std::string BandMathRenderer::CreatedNode() const {
   return wxT(ALGEBRA_NODE);
}

/**
 * Genera salida a partir de las bandas de entrada y de las ecuaciones cargadas.
 * Antes de llamar a pPreviousRenderer_->Render, cambia el tipo de dato
 * en el canvas al de la entrada.
 * Despues de pPreviousRenderer_->Render modifica el tipo de dato a
 * BAND_MATH_OUTPUT_DATA_TYPE y la
 * cantidad de bandas a la cantidad de ecuaciones.
 * Para cada ecuacion en el nodo parameters llama a GetEquation para encontrar
 * la funcion asociada. Luego la llama pasandole una banda de salida y un vector
 * con las bandas que obtuvo del cambas.
 * Actualiza el canvas.
 * @param[in] pWorldWindow mundo a renderizar
 * @param[in] pCanvas imagen sobre la que se aplica las ecuaciones
 * @param[in] pMask mascara que indica sobre que puntos se aplican las ecuaciones
 * @return True si no falla la renderizacion previa y se completa esta renderizacion
 * @return False si no existe el renderizador previo o falla la renderizacion
 *    previa, o si el canvas devuelto por la renderizacion previa es invalido.
 */
bool BandMathRenderer::Render(const World *pWorldWindow, Canvas* pCanvas,
                              Mask* pMask) {
   bool prevrenderizationstatus = true;
   // Cambio tipo de dato en canvas y llamo a Render
   Canvas *ptempcanvas = pCanvas;
   int canvassizex, canvassizey, originalbandcount, outputbandcount = 0;
   std::string datatype;
   if (pPreviousRenderer_ && pCanvas) {
      pPreviousRenderer_->GetOutputParameters(canvassizex, canvassizey,
                                              originalbandcount, datatype);

      pCanvas->SetDataType(datatype);
      // no puede cambiar el tipo de canvas
      if (pCanvas->GetDataType() != datatype) {
         ptempcanvas = new MemoryCanvas;
         REPORT_DEBUG("D:Se creo un canvas intermedio");
      }
      prevrenderizationstatus = pPreviousRenderer_->Render(pWorldWindow, ptempcanvas,
                                                           pMask);
   } else {
      REPORT_AND_FAIL_VALUE("D:Canvas no puede ser nulo al aplicar ecuacion.", false);
   }

   // Leo el tamanio del canvas retornado
   pCanvas->GetSize(canvassizex, canvassizey);
   if (canvassizex < 1 || canvassizey < 1) {
      REPORT_AND_FAIL_VALUE("D:Tamano de canvas erroneo", false);
   }

   // leo el canvas retornado
   std::vector<int> bands(originalbandcount);
   std::vector<void*> data(originalbandcount);
   for (int b = 0; b < originalbandcount; b++) {
      bands[b] = b;
      data[b] = new unsigned char[canvassizex * canvassizey * pCanvas->GetDataSize()];
   }
   pCanvas->Read(bands, data);

   // Si el canvas es el original cambio el tipo de dato al de la salida y el numero de bandas
   if (ptempcanvas == pCanvas) {
      pCanvas->SetDataType(DataInfo<BAND_MATH_OUTPUT_DATA_TYPE>::Name);
   }
   outputbandcount = parameters_.equations_.size();
   pCanvas->SetBandCount(outputbandcount);
   pCanvas->SetSize(canvassizex, canvassizey);
   int size = canvassizex * canvassizey;

   // Creo vector donde se guardan datos de salida temporales
   std::vector<void*> outdata(outputbandcount);
   for (int b = 0; b < outputbandcount; b++)
      outdata[b] =
            new unsigned char[canvassizex * canvassizey * pCanvas->GetDataSize()];

   // Agrego todas las bandas a un std::vector
   std::vector<void*> originalbands;
   for (int b = 0; b < originalbandcount; b++)
      originalbands.push_back(data[b]);

   // Aplico las operaciones a las bandas
   for (size_t i = 0; i < parameters_.equations_.size(); i++) {
      bandmathoperationTypeMap[datatype](originalbands, size,
                                  parameters_.equations_[i].equation_,
                                  parameters_.equations_[i].bandNames_,
                                  (BAND_MATH_OUTPUT_DATA_TYPE*)outdata[i]);
   }

   // Guardo cambios
   pCanvas->Write(bands, outdata);

   // Limpio el vector con bandas originales
   originalbands.clear();
   // libero el temporario
   for (int b = 0; b < originalbandcount; b++)
      delete[] static_cast<unsigned char*>(data[b]);
   for (int b = 0; b < outputbandcount; b++)
      delete[] static_cast<unsigned char*>(outdata[b]);

   return true && prevrenderizationstatus;
}

/**
 * Obtiene el "bounding box" del elemento renderizado
 * @param[in]	pWorld: puntero a la ventana del mundo.
 * @param[in]	Ulx: uper-left x
 * @param[in]	Uly: uper-left y
 * @param[in]	Lrx: lower-right x
 * @param[in]	Lry: lower-right y
 */
void BandMathRenderer::GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                                      double &Lrx, double &Lry) {
}

/**
 * Obtiene los parametros de entrada del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void BandMathRenderer::GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                          std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
}

/**
 * Obtiene los parametros de salida del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void BandMathRenderer::GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                           std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
   DataType = DataInfo<BAND_MATH_OUTPUT_DATA_TYPE>::Name;
   BandCount = parameters_.equations_.size();
}

/**
 * Actualizacion de parametros para representar un elemento
 * @param[in]	pElement: puntero al elemento.
 */
void BandMathRenderer::Update(Element *pElement) {
   // Leo parametros de nodo
   Parameters params;
   params = GetParameters(pElement->GetNode(wxT("")));
   if (!LoadEquations(params)) {
      REPORT_ERROR("D:error en los datos del nodo");
   }
}

/**
 * Controla que el contenido de Parametres sea valido y en caso
 * afirmativo actualiza parameters_(atributo de la clase).
 * Para analizar la validez parsea la ecuacion y verifica que las variables
 * esten asignadas dentro de Parameters.
 * Tambien verifica que el cambas tenga suficientes bandas(una ecuacion no
 * puede usar una banda con un indice superior a la cantidad de bandas que
 * se recibe del renderizador anterior).
 * @param[in] Parameters: parametros a controlar
 * @param[in] pPreviousRenderer: renderizador anterior. Si es null usa el
 * de la clase.
 * @return bool informa si la operacion tubo exito.
 */
bool BandMathRenderer::LoadEquations(BandMathRenderer::Parameters Parameters,
                                                  Renderer *pPreviousRenderer) {
   // Obtengo el tipo de dato a la entrada
   int sizex, sizey, bandcount;
   std::string datatype;
   if (pPreviousRenderer)
      pPreviousRenderer->GetOutputParameters(sizex, sizey, bandcount, datatype);
   else
      GetInputParameters(sizex, sizey, bandcount, datatype);

   for (size_t i = 0; i < Parameters.equations_.size(); i++) {
      // Valido la ecuacion
      EquationParser eqparser;
      if (!eqparser.SetEquation(Parameters.equations_[i].equation_))
         return false;

      // Valido que las variables en la ecuacion esten asignadas a bandas existentes
      std::map<std::string, int>::iterator it;
      std::vector<std::string> variablenames;
      eqparser.GetVariableNames(variablenames);
      for (size_t j = 0; j < variablenames.size(); j++) {
         it = Parameters.equations_[i].bandNames_.find(variablenames[j]);
         if (it == Parameters.equations_[i].bandNames_.end() ||
                                              it->second >= bandcount)
            return false;
      }
   }

   parameters_.equations_ = Parameters.equations_;
   return true;
}
}  // namespace

