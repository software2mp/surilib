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

#include "KMeansTest.h"

// Includes estandar
// Includes Suri
#include "KMeansAlgorithm.h"
#include "ClassificationRenderer.h"
#include "suri/Configuration.h"
#include "suri/xmlnames.h"
// Includes Wx
#include "wx/sstream.h"
#include "wx/string.h"
#include "wx/xml/xml.h"
// Includes App
// Defines

/** namespace suri */
namespace suri {
/**
 * Constructor
 */
KMeansTest::KMeansTest() {
}

/**
 * Destructor
 */
KMeansTest::~KMeansTest() {
}

/**
 * Analizo si el nodo del renderizador se genero correctamente
 */
void KMeansTest::TestClassificationRendererToNode() {
   // Genero KMeansAlgorithmClassData
   KMeansAlgorithm::KMeansAlgorithmClassData kmeansdata(0);
   kmeansdata.mean_.push_back(1);
   kmeansdata.mean_.push_back(2);
   kmeansdata.mean_.push_back(3);

   // Cargo informacion a algoritmo
   KMeansAlgorithm kmeansalgo;
   kmeansalgo.classes_.push_back(kmeansdata);

   // Le pido al algoritmo el nodo y verifico los datos
   wxXmlNode* kmeansnode = kmeansalgo.GetNode();

#ifndef __PRINT_CLASSIFICATION_RENDERER_XML_NODE__
   // Tranforma pXmlDocument a string
   wxXmlDocument* pxmldocument = new wxXmlDocument();
   pxmldocument->SetRoot(kmeansnode);
   wxStringOutputStream localxmloutput;
   pxmldocument->Save(localxmloutput);
   wxString xmlasstring = localxmloutput.GetString();
   REPORT_DEBUG("%s", xmlasstring.c_str());
   pxmldocument->SetRoot(NULL);
   delete pxmldocument;
#endif

   CPPUNIT_ASSERT_MESSAGE("Error nombre clase",
                          kmeansnode->GetName().IsSameAs(wxT(KMEANS_NODE) ));
   wxXmlNode *pchildnode = kmeansnode->GetChildren();
   CPPUNIT_ASSERT_MESSAGE("Error nombre clase",
                          pchildnode->GetName().IsSameAs(wxT(CLASS_NODE) ));
   if (pchildnode->GetName().IsSameAs(wxT(CLASS_NODE))) {
      // Cargo id clase
      wxString idstring;
      if (pchildnode->GetPropVal(wxT(CLASS_ID_PROPERTY), &idstring)) {
         int id = StringToNumber<int>(idstring.c_str());
         KMeansAlgorithm::KMeansAlgorithmClassData classdata(id);
         // Cargo propiedades clase
         wxXmlNode *pclassnode = pchildnode->GetChildren();
         while (pclassnode) {
            std::string nodecontent = pclassnode->GetNodeContent().c_str();
            // Obtiene el color de la clase
            if (pclassnode->GetName() == wxT(MEAN_NODE)) {
               classdata.mean_ = split<double>(nodecontent);
            }

            pclassnode = pclassnode->GetNext();
         }

         // Analizo resultado
         bool luok = TestVector(classdata.mean_, kmeansalgo.classes_[0].mean_);
         CPPUNIT_ASSERT_MESSAGE("Error al cargar media en nodo", luok);
         luok = (classdata.id_ == kmeansalgo.classes_[0].id_);
         CPPUNIT_ASSERT_MESSAGE("Error de id", luok);
      }
   }
}

/**
 * Analizo si MaxLikelihoodAlgorith::Update funciona correctamente
 */
void KMeansTest::TestNodeToClassificationRenderer() {
   // Cargo nodo
   std::string path = __TEST_PATH__ "nodosxml/validKMeansAlgorithmNode.xml";

   wxXmlDocument* pxmldocument = new wxXmlDocument(path);

#ifdef __PRINT_CLASSIFICATION_RENDERER_XML_NODE__
   // Tranforma pXmlDocument a string
   wxStringOutputStream localxmloutput;
   pxmldocument->Save(localxmloutput);
   wxString xmlasstring=localxmloutput.GetString();
   REPORT_DEBUG("%s", xmlasstring.c_str() );
#endif

   wxXmlNode* pclasifnode = pxmldocument->GetRoot();
   KMeansAlgorithm* pnewclassifalgorithm = new KMeansAlgorithm();
   if (!pnewclassifalgorithm->Update(pclasifnode)) {
      delete pxmldocument;
      delete pnewclassifalgorithm;
      CPPUNIT_ASSERT_MESSAGE("Error al cargar nodo de ejemplo", false);
      return;
   }

   // Analizo resultado
   std::vector<double> mean;
   mean.push_back(1);
   mean.push_back(2);
   mean.push_back(3);

   // Analizo resultado
   bool luok = TestVector(mean, pnewclassifalgorithm->classes_[0].mean_);
   CPPUNIT_ASSERT_MESSAGE("Error al cargar media de nodo", luok);
   luok = (0 == pnewclassifalgorithm->classes_[0].id_);
   CPPUNIT_ASSERT_MESSAGE("Error de id", luok);

   delete pxmldocument;
   delete pnewclassifalgorithm;
}

/**
 * Compara dos vectores, verifica que tengan el mismo tamanio, y luego los
 * recorre comparanto cada uno de sus componentes.
 * @param[in] Vector1 vector que se quiere comparar
 * @param[in] Vector2 vector que se quiere comparar
 * @return true si los vectores son del mismo tamanio y todos sus componentes
 *             son iguales, false en otro caso.
 */
bool KMeansTest::TestVector(std::vector<double> &Vector1,
                            std::vector<double> &Vector2) {
   // Comparo dimensiones
   if (Vector1.size() != Vector2.size()) {
      return false;
   }
   int vectorsize = Vector1.size();

   for (int i = 0; i < vectorsize; i++)
      if (Vector1[i] != Vector2[i]) {
         return false;
      }

   return true;
}
}

