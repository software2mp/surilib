/*! \file PrincipalComponentAnalysisProcess.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <algorithm>
#include <complex>
#include <map>
#include <sstream>
#include <vector>

#include "PrincipalComponentAnalysisProcess.h"

#include "DataCastRenderer.h"
#include "FileRenderization.h"
#include "HtmlReportWidget.h"
#include "Matrix.h"
#include "StatisticsReport.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/StatisticsCalculator.h"
#include "suri/XmlFunctions.h"

#include "Eigen/Eigenvalues"

#include "wx/xml/xml.h"

// Esto es un arreglo para alineacion de punteros en GCC para Windows.
// En versiones gcc (GCC) 4.4.0 no deberia hacer falta.
#ifdef __WINDOWS__
#define EIGEN_SAFE __attribute__((force_align_arg_pointer))
#else
#define EIGEN_SAFE
#endif  // __WINDOWS__

namespace suri {

const std::string PrincipalComponentAnalysisProcess::kToken = " ";
/** Nombre del proceso **/
const std::string PrincipalComponentAnalysisProcess::kProcessName =
      "PrincipalComponentAnalysisProcess";

/**
 * Ctor.
 */
PrincipalComponentAnalysisProcess::PrincipalComponentAnalysisProcess(
      Element* pInputElement, Element*& pOutputElement, const Subset& WindowSubset,
      DataViewManager* pDataViewManager, HtmlReportWidget* pStatisticsReport) :
      FileExporterProcess(pInputElement, WindowSubset, pDataViewManager),
      pStatisticsReport_(pStatisticsReport) {
   SetProcessName(kProcessName);
   pRenderizationObject_ = new FileRenderization();
}

/**
 * Dtor.
 */
PrincipalComponentAnalysisProcess::~PrincipalComponentAnalysisProcess() {

}

/**
 * Pemite configurar el RasterElement que se va a renderizar.
 */
bool PrincipalComponentAnalysisProcess::ConfigureRaster(RasterElement* pRasterElement) {
   wxXmlNode* prnode = pRasterElement->GetNode(wxT(RENDERIZATION_NODE));

   std::vector<int> selectedbands = GetSelectedRasterBands();
   std::stringstream inputss;
   for (size_t ix = 0, lenix = selectedbands.size(); ix < lenix; ++ix) {
      if (inputss.str().length() > 0) inputss << kToken;
      inputss << selectedbands[ix];
   }
   std::string ibstr = inputss.str();

   pRasterElement->AddNode(prnode, BAND_COMBINATION_NODE, wxT(ibstr.c_str()));
   pRasterElement->AddNode(prnode, GetComponentsInfo());
   pRasterElement->AddNode(prnode, GetDataCastRenderizationNode());

   return true;
}

/**
 * Otiene el tamano del objeto a salvar.
 */
wxLongLong PrincipalComponentAnalysisProcess::GetNeededSpace() {
   int hight = 0, width = 0;

   World* pworld = (pOutputWorld_)? pOutputWorld_ : pInputWorld_;
   pworld->GetViewport(hight, width);

   return hight * width * sizeof(double) + 1000000;
}

/**
 * Retorna el nodo datacast configurado con el tipo de dato de salida.
 */
wxXmlNode* PrincipalComponentAnalysisProcess::GetDataCastRenderizationNode() {
   DataCastRenderer::Parameters parameters;
   parameters.destinationDataType_ = "double";
   if (parameters.destinationDataType_.empty())
      return NULL;
   return DataCastRenderer::GetXmlNode(parameters);
}

/**
 * Obtiene el nodo con los autovalores y autovectores.
 */
EIGEN_SAFE wxXmlNode* PrincipalComponentAnalysisProcess::GetComponentsInfo() {
   RasterElement* praster = dynamic_cast<RasterElement*>(GetInputElement());
   if (praster == NULL)
      return NULL;

   // Calculo estadisticas para obtener las matrices.
   suri::raster::data::StatisticsBase* pStatistics = NULL;
   suri::raster::data::StatisticsCalculator calc(praster);
   calc.CalculateStatistics(pStatistics, true, true);

   // Obtengo los datos de configuracion para saber que matriz usar.
   std::string matrixtype;
   GetProcessAdaptLayer()->GetAttribute<std::string>(
         ProcessAdaptLayer::MatrixTypeKeyAttr, matrixtype);

   int compcount = 0;
   GetProcessAdaptLayer()->GetAttribute<int>(ProcessAdaptLayer::ComponentsCountKeyAttr, compcount);

   // Tipo de matriz a utilizar.
   std::vector<std::vector<double> > selmtx =
         matrixtype.compare("correlation") == 0 ? pStatistics->GetCorrelationMatrix() :
                                                  pStatistics->GetCovarianceMatrix();

   pStatistics->SetEig(matrixtype.compare("correlation") == 0 ? EIG_CORRELATION : EIG_COVARIANCE);

   std::map < std::string, std::string > options;

   // Adapto al formato de la clase Matrix.
   int selmtxsize = selmtx.size();

   Eigen::MatrixXd comtx(selmtxsize, selmtxsize);
   for (int ix = 0; ix < selmtxsize; ++ix) {
      for (int jx = 0; jx < selmtxsize; ++jx) {
         comtx(ix, jx) = selmtx[ix][jx];
      }
   }

   Eigen::EigenSolver<Eigen::MatrixXd> solver(comtx);
   Eigen::EigenSolver<Eigen::MatrixXd>::EigenvectorsType eigenvecs = solver.eigenvectors();
   Eigen::EigenSolver<Eigen::MatrixXd>::EigenvalueType eigenvals = solver.eigenvalues();

   // Obtengo autovalores y autovectores.
   std::map<double, std::vector<double> > autosvv;
   std::vector<double> autoval;

   if (compcount == 0)
      compcount = selmtxsize;

   std::vector<int> selectedbands = GetSelectedRasterBands();
   for (size_t rx = 0; rx < selectedbands.size(); ++rx) {
      std::vector<double> autovec;
      for (size_t sx = 0; sx < selectedbands.size(); ++sx) {
         autovec.push_back(eigenvecs(selectedbands[sx], selectedbands[rx]).real());
      }
      autosvv[eigenvals(selectedbands[rx]).real()] = autovec;
      autoval.push_back(eigenvals(selectedbands[rx]).real());
   }

   std::sort(autoval.begin(), autoval.end());
   std::reverse(autoval.begin(), autoval.end());

   // Armo el nodo
   wxXmlNode* ppcanode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(PCANALYSIS_NODE));
   wxXmlNode* ppcacompsnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(PCANALYSIS_COMPONENTS_NODE));

   ppcanode->AddChild(ppcacompsnode);

   for (int tx = 0; tx < compcount; ++tx) {
      wxXmlNode* pcompnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(PCANALYSIS_COMPONENT_NODE));

      pcompnode->AddProperty(PCANALYSIS_ORDER_ATTR_NODE, NumberToString<int>(tx));

      std::string values;
      for (int ix = 0, lenix = autosvv[autoval[tx]].size(); ix < lenix; ++ix) {
         if (values.length() > 0)
            values.append(" ");

         values.append(NumberToString<double>(autosvv[autoval[tx]][ix]));
      }

      wxXmlNode* pavnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(PCANALYSIS_AUTOVECTOR_NODE));
      new wxXmlNode(pavnode, wxXML_TEXT_NODE, wxT(""), values.c_str());
      std::string autovector = "autovector";
      autovector.append(NumberToString(tx));
      options[autovector] = values;

      pcompnode->AddChild(pavnode);

      ppcacompsnode->AddChild(pcompnode);
   }

   if (pStatisticsReport_) {
      StatisticsReport* preport = new StatisticsReport();

      // Configuro opciones con datos de la imagen de entrada
      ConfigureOutputOptions(options);

      Statistics::StatisticsFlag flags = Statistics::Mean | Statistics::Variance
            | Statistics::MinValue | Statistics::MaxValue;

      if (matrixtype.compare("correlation") == 0) {
         flags |= Statistics::CorrelationMatrix;
      } else {
         flags |= Statistics::CovarianceMatrix;
      }

      preport->SetStatistics(pStatistics, NULL);
      preport->SetOutputOptions(options);
      preport->SetFlags(flags);
      pStatisticsReport_->SetReport(preport);
   }

   return ppcanode;
}

/** 
 * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
 * agreguen sus atributos especificos
 * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
 * @return vector vacio en caso de que no tenga parametros adicionales
 **/
std::vector<wxXmlNode*> PrincipalComponentAnalysisProcess::DoGetXmlParameters() const {
   std::vector<wxXmlNode*> params;   
   std::string matrixtype;
   pAdaptLayer_->GetAttribute<std::string>(ProcessAdaptLayer::MatrixTypeKeyAttr, matrixtype);

   int compcount = 0;
   pAdaptLayer_->GetAttribute<int>(ProcessAdaptLayer::ComponentsCountKeyAttr, compcount);

   params.push_back(CreateKeyValueNode("matriz", matrixtype));
   params.push_back(CreateKeyValueNode("componentes", NumberToString<int>(compcount)));
   return params;
}

/** Configura las opciones **/
void PrincipalComponentAnalysisProcess::ConfigureOutputOptions(
      std::map<std::string, std::string> &pOptions) {
   RasterElement* praster = dynamic_cast<RasterElement*>(GetInputElement());
   std::string bandids, bandnames, name;
   std::vector<int> selectedbands = GetSelectedRasterBands();
   for (size_t i = 0; i < selectedbands.size(); ++i) {
      praster->GetBandName(name, selectedbands[i]);
      if (i > 0) {
         bandids += "|";
         bandnames += "|";
      }
      bandids += NumberToString<int>(selectedbands[i]);
      bandnames += name;
   }
   pOptions["selectedbands"] = bandnames;
   pOptions["selectedbandids"] = bandids;
   int compcount = 0;
   pAdaptLayer_->GetAttribute<int>(ProcessAdaptLayer::ComponentsCountKeyAttr, compcount);
   compcount =  compcount > 0 ? compcount : selectedbands.size();
   pOptions["components"] = NumberToString<size_t>(compcount);
}

}  // namespace suri
