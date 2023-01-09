/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "MaxLikelihoodTest.h"

// Includes estandar
// Includes Suri
#include "suri/StatisticsFunctions.h"
#include "ClassificationRenderer.h"
#include "MaxLikelihoodPart.h"
#include "suri/Configuration.h"
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
MaxLikelihoodTest::MaxLikelihoodTest() {
}

/**
 * Constructor
 */
MaxLikelihoodTest::~MaxLikelihoodTest() {
}

/**
 * Analiza si se calculo correctamente descomposicion LU
 */
void MaxLikelihoodTest::TestGetClassificationManager() {
   Statistics classstatistics;
   LoadStatistics(classstatistics);

   // Uso MaxLikelihoodPart para calcular LU y cte
   MaxLikelihoodPart maxlikelihood;
   /*
   INCIO CODIGO PARCHE:
   Es necesario castear el algoritmo a ClusterClassificationAlgorithm* para
   que se puedan configurar los recintos. Ver TCK #856
   */
   ClusterClassificationAlgorithm* pclusteralgorithm =
         dynamic_cast<ClusterClassificationAlgorithm*>(
         maxlikelihood.GetClassificationManager());
   /* FIN CODIGO PARCHE */
   pclusteralgorithm->GetClusters()->AddCluster(1, classstatistics);
   MaxLikelihoodAlgorithm* pclassifalgorithm =
         dynamic_cast<MaxLikelihoodAlgorithm*>(maxlikelihood.GetClassificationManager());

   std::vector<std::vector<double> > luresult;
   std::vector<double> filetable;
   filetable.push_back(6);
   filetable.push_back(3);
   filetable.push_back(3);
   luresult.push_back(filetable);
   filetable.clear();
   filetable.push_back(0.5);
   filetable.push_back(2.5);
   filetable.push_back(0.5);
   luresult.push_back(filetable);
   filetable.clear();
   filetable.push_back(0.5);
   filetable.push_back(0.2);
   filetable.push_back(5.4);
   luresult.push_back(filetable);

   // Analizo resultado
   std::vector<Clusters::ClusterData>& clusters =
         pclassifalgorithm->GetClusters()->GetClusterVector();
   CPPUNIT_ASSERT_MESSAGE("Error al calcular LU", clusters.size()>0);
}

/**
 * Analizo si el nodo se genero correctamente
 */
void MaxLikelihoodTest::TestClassificationRendererToNode() {
   // Cargo estadisticas
   Statistics classstatistics;
   LoadStatistics(classstatistics);

   // Creo nodo usando ClassificationRenderer
   MaxLikelihoodPart maxlikelihood;
   /*
   INCIO CODIGO PARCHE:
   Es necesario castear el algoritmo a ClusterClassificationAlgorithm* para
   que se puedan configurar los recintos. Ver TCK #856
   */
   ClusterClassificationAlgorithm* pclusteralgorithm =
         dynamic_cast<ClusterClassificationAlgorithm*>(
         maxlikelihood.GetClassificationManager());
   /* FIN CODIGO PARCHE */
   pclusteralgorithm->GetClusters()->AddCluster(1, classstatistics);
   MaxLikelihoodAlgorithm* pclassifalgorithm =
         dynamic_cast<MaxLikelihoodAlgorithm*>(maxlikelihood.GetClassificationManager());
   ClassificationRenderer::Parameters clasifparameters;
   clasifparameters.pAlgorithm_ = pclassifalgorithm;
   wxXmlNode* pclasifnode = ClassificationRenderer::GetXmlNode(clasifparameters);

   if (!pclasifnode) {
      CPPUNIT_ASSERT_MESSAGE("Error al crear clasificador", false);
      return;
   }

#ifdef __PRINT_CLASSIFICATION_RENDERER_XML_NODE__
   // Tranforma pXmlDocument a string
   wxXmlDocument* pxmldocument= new wxXmlDocument();
   pxmldocument->SetRoot(pclasifnode);
   wxStringOutputStream localxmloutput;
   pxmldocument->Save(localxmloutput);
   wxString xmlasstring=localxmloutput.GetString();
   REPORT_DEBUG("%s", xmlasstring.c_str() );
   pxmldocument->SetRoot(NULL);
   delete pxmldocument;
#endif

   // Verifico xml resultado
   CPPUNIT_ASSERT_MESSAGE("Error nombre Clasificacion",
                          pclasifnode->GetName().IsSameAs(wxT(CLASSIFICATION_NODE) ));
   CPPUNIT_ASSERT_MESSAGE(
         "Error nombre algoritmo",
         pclasifnode->GetChildren()->GetName().IsSameAs(wxT(MAX_LIKELIHOOD_NODE) ));
   wxXmlNode *pchildnode = pclasifnode->GetChildren()->GetChildren();
   CPPUNIT_ASSERT_MESSAGE("Error nombre clase",
                          pchildnode->GetName().IsSameAs(wxT(CLASS_NODE) ));

   if (pchildnode->GetName().IsSameAs(wxT(CLASS_NODE))) {
      wxXmlNode *pclassnode = pchildnode->GetChildren();
      while (pclassnode) {
         Statistics stats;
         // Obtiene las estadisticas de la clase
         if (pclassnode->GetName() == wxT(STATISTICS_NODE)) {
            bool luok = GetStats(pclassnode, stats);
            CPPUNIT_ASSERT_MESSAGE("Error al cargar estadisticas", luok);
         }
         pclassnode = pclassnode->GetNext();
      }
   }
}

/**
 * Analizo si MaxLikelihoodAlgorith::Update funciona correctamente
 */
void MaxLikelihoodTest::TestNodeToClassificationRenderer() {
   // Cargo estadisticas
   Statistics classstatistics;
   LoadStatistics(classstatistics);

   // Cargo nodo
   std::string path = __TEST_PATH__ "nodosxml/validClassificationRendererNode.xml";

   wxXmlDocument* pxmldocument = new wxXmlDocument(path);

#ifdef __PRINT_CLASSIFICATION_RENDERER_XML_NODE__
   // Tranforma pXmlDocument a string
   wxStringOutputStream localxmloutput;
   pxmldocument->Save(localxmloutput);
   wxString xmlasstring=localxmloutput.GetString();
   REPORT_DEBUG("%s", xmlasstring.c_str() );
#endif

   wxXmlNode* pclasifnode = pxmldocument->GetRoot();
   MaxLikelihoodAlgorithm* pnewclassifalgorithm = new MaxLikelihoodAlgorithm();
   if (!pnewclassifalgorithm->Update(pclasifnode->GetChildren())) {
      delete pnewclassifalgorithm;
      delete pxmldocument;
      CPPUNIT_ASSERT_MESSAGE("Error al cargar nodo de ejemplo", false);
      return;
   }

   // Analizo resultado
   std::vector<std::vector<double> > luresult;
   std::vector<double> filetable;
   filetable.push_back(6);
   filetable.push_back(3);
   filetable.push_back(3);
   luresult.push_back(filetable);
   filetable.clear();
   filetable.push_back(0.5);
   filetable.push_back(2.5);
   filetable.push_back(0.5);
   luresult.push_back(filetable);
   filetable.clear();
   filetable.push_back(0.5);
   filetable.push_back(0.2);
   filetable.push_back(5.4);
   luresult.push_back(filetable);

   std::vector<Clusters::ClusterData>& clusters =
         pnewclassifalgorithm->GetClusters()->GetClusterVector();

   bool luok = (clusters[0].classId_ == 1);
   CPPUNIT_ASSERT_MESSAGE("Error de id", luok);

   delete pxmldocument;
   delete pnewclassifalgorithm;
}

/**
 * Carga una variable de tipo Statistics
 * @param[out] ClassStatistics estadisticas
 */
void MaxLikelihoodTest::LoadStatistics(Statistics &ClassStatistics) {
   // Configuro estadisticas
   ClassStatistics.count_ = 500;
   ClassStatistics.mean_.push_back(1);
   ClassStatistics.mean_.push_back(2);
   ClassStatistics.mean_.push_back(1);
   ClassStatistics.variance_.push_back(6);
   ClassStatistics.variance_.push_back(4);
   ClassStatistics.variance_.push_back(7);
   ClassStatistics.min_.push_back(1);
   ClassStatistics.min_.push_back(2);
   ClassStatistics.min_.push_back(4);
   ClassStatistics.max_.push_back(6);
   ClassStatistics.max_.push_back(7);
   ClassStatistics.max_.push_back(10);
   std::vector<double> filetable;
   filetable.push_back(6);
   filetable.push_back(3);
   filetable.push_back(3);
   ClassStatistics.covarianceMatrix_.push_back(filetable);
   filetable.clear();
   filetable.push_back(3);
   filetable.push_back(4);
   filetable.push_back(2);
   ClassStatistics.covarianceMatrix_.push_back(filetable);
   filetable.clear();
   filetable.push_back(3);
   filetable.push_back(2);
   filetable.push_back(7);
   ClassStatistics.covarianceMatrix_.push_back(filetable);
}

/**
 * Compara dos vectores. Compara los tamanios y cada uno de los valores.
 * @param[in] Vector1 vector que se quiere comparar
 * @param[in] Vector2 vector que se quiere comparar
 * @return true si los vectores son iguales, false en otro caso.
 */
bool MaxLikelihoodTest::TestVector(std::vector<double> &Vector1,
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

/**
 * Compara dos matrices. Compara los tamanios y los valores en cada posicion.
 * @param[in] Matrix1 matriz que se quiere comparar
 * @param[in] Matrix2 matriz que se quiere comparar
 * @return true si las matrices son iguales, false en otro caso.
 */
bool MaxLikelihoodTest::TestMatrix(std::vector<std::vector<double> > &Matrix1,
                                   std::vector<std::vector<double> > &Matrix2) {
   // Comparo dimensiones
   if (Matrix1.size() != Matrix2.size()) {
      return false;
   }
   if (Matrix1.size() == 0) {
      return true;
   }
   size_t matrixsizex = Matrix1.size();
   size_t matrixsizey = Matrix1[0].size();

   for (size_t i = 0; i < matrixsizex; i++)
      if (Matrix1[i].size() != matrixsizey || Matrix2[i].size() != matrixsizey) {
         return false;
      }

   // Comparo contenido
   for (size_t i = 0; i < matrixsizex; i++)
      for (size_t j = 0; j < matrixsizey; j++)
         if (Matrix1[i][j] != Matrix2[i][j]) {
            return false;
         }

   return true;
}
}
