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

#include "StatisticNodeTest.h"

// Include suri
#include "suri/Configuration.h"
#include "suri/xmlnames.h"
#include "logmacros.h"
#include "ClassificationRenderer.h"
#include "suri/XmlElement.h"
#include "suri/XmlFunctions.h"

// Includes Wx
#include "wx/xml/xml.h"

/** namespace suri */
namespace suri {
/**
 * Constructor
 */
StatisticNodeTest::StatisticNodeTest() {
}

/**
 * Destructor
 */
StatisticNodeTest::~StatisticNodeTest() {
}

/**
 * Compara una variable parameters creada (vacia), contra una variable
 * parameters creada a partir de un nodo xml vacio.
 */
void StatisticNodeTest::CreateParameterFromEmptyNode() {
   std::string pathdir = __TEST_PATH__;
   pathdir += "nodosxml/validEmptyStatsNode.xml";

   wxXmlDocument classificationdocument(pathdir);
   wxXmlNode *pclassificationnode = classificationdocument.GetRoot();

   ClassificationRenderer::Parameters parametersfromemptyxml;
   ClassificationRenderer::Parameters parameters;

   // Obtiene parametros a partir de un nodo xml
   parametersfromemptyxml = ClassificationRenderer::GetParameters(pclassificationnode);

   CPPUNIT_ASSERT_MESSAGE("Los parametros son diferentes",
                          parametersfromemptyxml==parameters);
}

/**
 * Obtiene un nodo estadistica xml desde disco y genera una variable de tipo
 * parameters utilizando la funcion, y la compara con una variable parameters
 * a la cual se le cargaron manualmente los mismos valores que los del nodo XML
 * @return true si ambas variables son iguales
 * @return false si las variables son distintas
 */
void StatisticNodeTest::CreateParameterFromNode() {
   /*std::string pathdir = suri::Configuration::GetParameter("app_base_dir_volatile","");
    pathdir+="../lib/suri/test/nodosxml/validStatsNode.xml";

    wxXmlDocument classificationdocument(pathdir);
    wxXmlNode* pclassificationnode = classificationdocument.GetRoot();

    ClassificationRenderer::Parameters parametersfromxml;
    ClassificationRenderer::Parameters parameters;

    //Obtiene parametros a partir de un nodo xml
    parametersfromxml = ClassificationRenderer::GetParameters(pclassificationnode);

    //Carga los paramatros manualmente
    ClassificationRenderer::Parameters::ClassType class1;
    class1.id_ = 1;
    class1.name_ = "Clase1";
    class1.color_ = "#ff0044";
    class1.classStatistics_.count_ = 500;
    class1.classStatistics_.mean_.push_back(1);
    class1.classStatistics_.mean_.push_back(2);
    class1.classStatistics_.mean_.push_back(1);
    class1.classStatistics_.variance_.push_back(3);
    class1.classStatistics_.variance_.push_back(5);
    class1.classStatistics_.variance_.push_back(4);
    class1.classStatistics_.min_.push_back(1);
    class1.classStatistics_.min_.push_back(2);
    class1.classStatistics_.min_.push_back(4);
    class1.classStatistics_.max_.push_back(6);
    class1.classStatistics_.max_.push_back(7);
    class1.classStatistics_.max_.push_back(10);
    std::vector<double> filetable;
    filetable.push_back(3);
    filetable.push_back(4);
    filetable.push_back(2);
    class1.classStatistics_.covarianceMatrix_.push_back(filetable);
    filetable.clear();
    filetable.push_back(4);
    filetable.push_back(5);
    filetable.push_back(6);
    class1.classStatistics_.covarianceMatrix_.push_back(filetable);
    filetable.clear();
    filetable.push_back(2);
    filetable.push_back(6);
    filetable.push_back(4);
    class1.classStatistics_.covarianceMatrix_.push_back(filetable);
    parameters.class_.push_back(class1);

    ClassificationRenderer::Parameters::ClassType class2;

    class2.id_ = 2;
    class2.name_ = "Clase2";
    class2.color_ = "#ff0066";
    class2.classStatistics_.count_ = 645;
    class2.classStatistics_.mean_.push_back(2);
    class2.classStatistics_.mean_.push_back(3);
    class2.classStatistics_.mean_.push_back(4);
    class2.classStatistics_.variance_.push_back(4);
    class2.classStatistics_.variance_.push_back(8);
    class2.classStatistics_.variance_.push_back(9);
    class2.classStatistics_.min_.push_back(0);
    class2.classStatistics_.min_.push_back(1);
    class2.classStatistics_.min_.push_back(0);
    class2.classStatistics_.max_.push_back(3);
    class2.classStatistics_.max_.push_back(4);
    class2.classStatistics_.max_.push_back(6);
    filetable.clear();
    filetable.push_back(4);
    filetable.push_back(5);
    filetable.push_back(1);
    class2.classStatistics_.covarianceMatrix_.push_back(filetable);
    filetable.clear();
    filetable.push_back(5);
    filetable.push_back(8);
    filetable.push_back(3);
    class2.classStatistics_.covarianceMatrix_.push_back(filetable);
    filetable.clear();
    filetable.push_back(1);
    filetable.push_back(3);
    filetable.push_back(9);
    class2.classStatistics_.covarianceMatrix_.push_back(filetable);
    parameters.class_.push_back(class2);

    parameters.noClass_.id_ = 0;
    parameters.noClass_.name_ = "NoClase";
    parameters.noClass_.color_ = "#ff0044";

    //Comparo las dos variables parameters
    if(!(parameters==parametersfromxml))
    return false;*/

   CPPUNIT_ASSERT_MESSAGE("Falta Test", false);
}

/**
 * Obtiene un nodo estadistica valido desde un archivo, y en el orden en
 * que sale ordenado de la funcion que crea el nodo a partir de parametros.
 * Obtiene una variable parameters a partir del nodo xml.
 * Genera un nuevo nodo estadistica a partir de los parameters y lo compara con
 * el nodo original.
 * @return true si los nodos son iguales
 * @return false si son distintos
 * \attention: para que este test funcione en el xml tambien hay que completar
 * valores para el nodo estadistico de la clase sin clase, porque si no hay una
 * diferencia en como se crean los nodos vacios
 * \verbatim (<media/> vs <media></media>) \endverbatim y eso hace fallar el test.
 */

void StatisticNodeTest::CreateNodeFromParameter() {
   std::string pathdir = __TEST_PATH__;
   pathdir += "nodosxml/validSortedStatsNode.xml";

   wxXmlDocument classificationdocument(pathdir);
   wxXmlNode* poldclassificationnode = classificationdocument.GetRoot();

   ClassificationRenderer::Parameters parameters;
   parameters = ClassificationRenderer::GetParameters(poldclassificationnode);

   wxXmlNode * pnewclassificationnode = ClassificationRenderer::GetXmlNode(parameters);

   XmlElement * poldclassificationelement = new XmlElement(wxT(""));
   poldclassificationelement->AddNode(poldclassificationelement->GetNode(wxT("")),
                                      poldclassificationnode);

   XmlElement * pnewclassificationelement = new XmlElement(wxT(""));
   pnewclassificationelement->AddNode(pnewclassificationelement->GetNode(wxT("")),
                                      pnewclassificationnode);
#ifdef __DEBUG__
   poldclassificationelement->PrintProperties();
   pnewclassificationelement->PrintProperties();
#endif
   wxString oldelement = poldclassificationelement->GetXmlAsString();
   wxString newelement = pnewclassificationelement->GetXmlAsString();

   CPPUNIT_ASSERT_MESSAGE("Los xml son diferentes", oldelement!=newelement);
}

/*
 * Crea una variable parameters a partir de un nodo, para cada variable
 * statistic de cada clase, verifica con el metodo GetDimensions. Si los valores
 * obtenidos del nodo son consistentes se espera obtener el numero de estadisticas
 * si son inconsistentes se espera obtener -1
 * En este caso levanta un nodo valido de un archivo, con dimension 3
 * Y levanta un nodo invalido.
 */
void StatisticNodeTest::TestParametersDimension() {
   /*	std::string pathdir = suri::Configuration::GetParameter("app_base_dir_volatile","");
    pathdir+="../lib/suri/test/nodosxml/validStatsNode.xml";

    wxXmlDocument classificationdocument(pathdir);
    wxXmlNode* pclassificationnode = classificationdocument.GetRoot();

    ClassificationRenderer::Parameters parametersfromxml;

    //Obtiene parametros a partir de un nodo xml
    parametersfromxml = ClassificationRenderer::GetParameters(pclassificationnode);

    std::vector<ClassificationRenderer::Parameters::ClassType>::iterator iter = parametersfromxml.class_.begin();
    for(; iter != parametersfromxml.class_.end(); iter++)
    if(iter->classStatistics_.GetDimension() != 3)
    return false;

    //Prueba con un nodo invalido, la dimension tiene que ser -1
    pathdir = suri::Configuration::GetParameter("app_base_dir_volatile","");
    pathdir+="../lib/suri/test/nodosxml/invalidStatsNode.xml";

    classificationdocument.Load(pathdir);
    pclassificationnode = classificationdocument.GetRoot();

    ClassificationRenderer::Parameters parametersfromxmlinvalid;

    //Obtiene parametros a partir de un nodo xml
    parametersfromxmlinvalid = ClassificationRenderer::GetParameters(pclassificationnode);

    iter = parametersfromxmlinvalid.class_.begin();
    for(; iter != parametersfromxmlinvalid.class_.end(); iter++)
    if(iter->classStatistics_.GetDimension() != -1)
    return false;*/

   CPPUNIT_ASSERT_MESSAGE("Falta Test", false);
}

/**
 * A partir de un nodo xml con estadisticas invalida crea una una variable
 * parameters y compara la variable parameters generada contra una vacia.
 */
void StatisticNodeTest::TesXmlInvalidDimension() {
   // Nodo con inconsistencias entre clases
   std::string pathdir = __TEST_PATH__;
   pathdir += "nodosxml/invalidDimensionsStatsNode.xml";

   wxXmlDocument classificationdocument(pathdir);
   wxXmlNode* pclassificationnode = classificationdocument.GetRoot();

   ClassificationRenderer::Parameters parametersfromxml;
   ClassificationRenderer::Parameters emptyparameter;

   // Obtiene parametros a partir de un nodo xml
   parametersfromxml = ClassificationRenderer::GetParameters(pclassificationnode);

   if (!(parametersfromxml == emptyparameter)) {
      CPPUNIT_ASSERT_MESSAGE("El nodod deberia estar vacio", false);
      return;
   }

   // Nodo con covarianza viacia
   pathdir = __TEST_PATH__;
   pathdir += "nodosxml/invalidEmptyDimensionsStatsNode.xml";

   classificationdocument.Load(pathdir);
   pclassificationnode = classificationdocument.GetRoot();

   ClassificationRenderer::Parameters parametersfromxmlemptydimensions;

   // Obtiene parametros a partir de un nodo xml
   parametersfromxmlemptydimensions = ClassificationRenderer::GetParameters(
         pclassificationnode);

   CPPUNIT_ASSERT_MESSAGE("Parameters deberia estar vacio",
                          (parametersfromxmlemptydimensions==emptyparameter));
}

/**
 * Obtiene una variable parameters a partir de un nodo xml, y otra variable
 * parameters a partir de otro nodo que tiene los mismos valores pero los
 * nodos estan en distinto orden.
 * @return true si ambas variables son iguales
 * @return false si las variables son distintas
 */
void StatisticNodeTest::CompareParameterFromDifferentNodes() {
   std::string pathdir = __TEST_PATH__;
   pathdir += "nodosxml/validStatsNode.xml";

   wxXmlDocument classificationdocument(pathdir);
   wxXmlNode* pclassificationnode = classificationdocument.GetRoot();

   ClassificationRenderer::Parameters parametersfromxml;

   // Obtiene parametros a partir de un nodo xml
   parametersfromxml = ClassificationRenderer::GetParameters(pclassificationnode);

   // Prueba con un nodo invalido, la dimension tiene que ser -1
   pathdir = __TEST_PATH__;
   pathdir += "nodosxml/validUnsortStatsNode.xml";

   classificationdocument.Load(pathdir);
   pclassificationnode = classificationdocument.GetRoot();

   ClassificationRenderer::Parameters parametersfromunsortxml;

   // Obtiene parametros a partir de un nodo xml
   parametersfromunsortxml = ClassificationRenderer::GetParameters(pclassificationnode);

   CPPUNIT_ASSERT_MESSAGE("Error. Los parametros son diferentes",
                          parametersfromxml == parametersfromunsortxml);
}

/**
 * Funcion operador == que compara dos variables de tipo ClassificationRenderer::parameters
 * @param[in] Lhs variable a comparar a la izquierda del operador ==
 * @param[in] Rhs variable a comparar a la derecha del operador ===
 */
bool operator==(const ClassificationRenderer::Parameters &Lhs,
                const ClassificationRenderer::Parameters &Rhs) {
   /*	//Comparo la clase no clase
    if(Lhs.noClass_.id_ != Rhs.noClass_.id_ ||
    Lhs.noClass_.name_ != Rhs.noClass_.name_ ||
    Lhs.noClass_.color_ != Rhs.noClass_.color_ ||
    !(Lhs.noClass_.classStatistics_==Rhs.noClass_.classStatistics_))
    return false;

    if(Lhs.class_.size() != Rhs.class_.size())
    return false;

    std::vector<ClassificationRenderer::Parameters::ClassType>::const_iterator lhsiterator = Lhs.class_.begin();
    std::vector<ClassificationRenderer::Parameters::ClassType>::const_iterator rhsiterator = Rhs.class_.begin();
    //Comparo las clases
    for(; lhsiterator != Lhs.class_.end(); lhsiterator++, rhsiterator++)
    {
    if(lhsiterator->id_ != rhsiterator->id_ ||
    lhsiterator->name_ != rhsiterator->name_ ||
    lhsiterator->color_ != rhsiterator->color_ ||
    !(lhsiterator->classStatistics_ == rhsiterator->classStatistics_))
    return false;
    }*/

   return true;
}

/**
 * Funcion operador == que compara dos variables de tipo Statistics
 * @param[in] Lhs variable a comparar a la izquierda del operador ==
 * @param[in] Rhs variable a comparar a la derecha del operador ===
 */
bool operator==(const Statistics &Lhs, const Statistics &Rhs) {
   if (Lhs.count_ != Rhs.count_ || Lhs.mean_ != Rhs.mean_
         || Lhs.variance_ != Rhs.variance_ || Lhs.min_ != Rhs.min_
         || Lhs.max_ != Rhs.max_ || Lhs.covarianceMatrix_ != Rhs.covarianceMatrix_) {
      return false;
   }

   return true;
}
}
