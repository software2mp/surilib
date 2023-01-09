/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#include "LutRendererTest.h"

// Includes standard
#include <sstream>
// Includes Suri
#include "suri/RasterElement.h"
#include "suri/Configuration.h"
#include "suri/xmlnames.h"
#include "RasterRenderer.h"
#include "suri/World.h"
#include "suri/Subset.h"
#include "MemoryCanvas.h"
#include "RenderPipeline.h"
#include "Mask.h"
#include "suri/DataTypes.h"
#include "LutRenderer.h"
#include "suri/Element.h"
#include "suri/Renderer.h"
// Includes Wx
#include "wx/xml/xml.h"
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Ctor */
LutRendererTest::LutRendererTest() {
}

/**
 * Test de crear renderizador a partir de elemento.
 * Falla si no se pudo crear renderizador
 */
void LutRendererTest::TestCreateOk() {
   // Creo parameters con dato nodo
   std::string pathdir = __TEST_PATH__ "imagenes/red.bmp";
   suri::Element* prasterelement = suri::RasterElement::Create(pathdir);
   wxXmlNode* renderernode = prasterelement->GetNode(wxT(RENDERIZATION_NODE));

   pathdir = __TEST_PATH__ "imagenes/lut.xml";

   wxXmlDocument lutdocument(pathdir);
   wxXmlNode* plutnode = new wxXmlNode(*(lutdocument.GetRoot()));

   wxString aux = wxString(wxT(RENDERIZATION_NODE)) + wxT("|")
         + wxString(wxT(LUT_NODE));
   wxXmlNode* poldlutnode = prasterelement->GetNode(aux.c_str());
   renderernode->InsertChildAfter(plutnode, poldlutnode);
   renderernode->RemoveChild(poldlutnode);
   delete poldlutnode;

   suri::RasterRenderer rasterrendaux;
   suri::Renderer* prasterrend = rasterrendaux.Create(prasterelement, NULL);

   suri::LutRenderer lutaux;
   suri::Renderer* plutrenderer = (suri::LutRenderer*) lutaux.Create(prasterelement,
                                                                     prasterrend);

   CPPUNIT_ASSERT_MESSAGE("Error al crear renderizador lut", plutrenderer!=NULL);

   suri::Renderer::Destroy(plutrenderer);
   suri::RasterRenderer::Destroy(prasterrend);
   suri::Element::Destroy(prasterelement);
}

/**
 * Falla si crea renderizador a pesar que en elemento no hay nodo LUT
 */
void LutRendererTest::TestCreateError() {
   // Creo parameters con dato nodo
   std::string pathdir = __TEST_PATH__ "imagenes/red.bmp";
   suri::Element* prasterelement = suri::RasterElement::Create(pathdir);

   wxString aux = wxString(wxT(RENDERIZATION_NODE)) + wxT("|")
         + wxString(wxT(LUT_NODE));
   wxXmlNode* oldlutnode = prasterelement->GetNode(aux.c_str());
   wxXmlNode* renderernode = prasterelement->GetNode(wxT(RENDERIZATION_NODE));
   renderernode->RemoveChild(oldlutnode);
   delete oldlutnode;

   suri::RasterRenderer rasterrendaux;
   suri::Renderer * prasterrend = rasterrendaux.Create(prasterelement, NULL);

   suri::LutRenderer lutaux;
   suri::Renderer* plutrenderer = (suri::LutRenderer*) lutaux.Create(prasterelement,
                                                                     prasterrend);

   CPPUNIT_ASSERT_MESSAGE("Error. No detecto que no hay nodo lut", plutrenderer!=NULL);
   suri::Renderer::Destroy(plutrenderer);
   suri::RasterRenderer::Destroy(prasterrend);
   suri::Element::Destroy(prasterelement);
   CPPUNIT_ASSERT_MESSAGE("Sin Test", false);
}

/**
 * Con xml leido de disco crea parametros. Utiliza los parametros para
 * crear otro nodo y los compara
 * Falla si no son iguales
 */
void LutRendererTest::TestGetXmlNode() {
   // Creo parameters con dato nodo
   std::string pathdir = __TEST_PATH__ "imagenes/red.bmp";
   suri::Element* prasterelement = suri::RasterElement::Create(pathdir);
   wxXmlNode* renderernode = prasterelement->GetNode(wxT(RENDERIZATION_NODE));

   if (!renderernode) {
      REPORT_DEBUG("D:FALLO TEST");
      return;
   }

   pathdir = __TEST_PATH__ "imagenes/lut.xml";

   wxXmlDocument *lutdocument = new wxXmlDocument(pathdir);
   wxXmlNode* lutnode = lutdocument->GetRoot();

   wxString aux = wxString(wxT(RENDERIZATION_NODE)) + wxT("|")
         + wxString(wxT(LUT_NODE));
   wxXmlNode* oldlutnode = prasterelement->GetNode(aux.c_str());
   renderernode->InsertChildAfter(lutnode, oldlutnode);
   renderernode->RemoveChild(oldlutnode);
   delete oldlutnode;

   std::string oldxml = prasterelement->GetXmlAsString().c_str();
   REPORT_DEBUG(prasterelement->GetXmlAsString().c_str());

   suri::LutRenderer::Parameters params = suri::LutRenderer::GetParameters(
         prasterelement->GetNode(wxT("")));

   // Creo nodo con dato parameters y lo remplazo en elemento y comparo
   wxXmlNode* newlutnode = suri::LutRenderer::GetXmlNode(params);
   renderernode->InsertChildAfter(newlutnode, lutnode);
   renderernode->RemoveChild(lutnode);

   CPPUNIT_ASSERT_MESSAGE("ERROR en xml nuevo",
                          oldxml.compare(prasterelement->GetXmlAsString() )==0);

   suri::Element::Destroy(prasterelement);
   delete lutdocument;
}

/**
 * Test de crear objeto Parameters
 * Falla si parameters tiene bandas incorrectas o primer tabla de
 * esta mal
 */
void LutRendererTest::TestGetParameters() {
   std::string pathdir = __TEST_PATH__ "imagenes/red.bmp";
   suri::Element* prasterelement = suri::RasterElement::Create(pathdir);
   wxXmlNode* renderernode = prasterelement->GetNode(wxT(RENDERIZATION_NODE));

   if (!renderernode) {
      REPORT_DEBUG("D:FALLO TEST");
      return;
   }

   pathdir = __TEST_PATH__ "imagenes/lut.xml";

   wxXmlDocument lutdocument(pathdir);
   wxXmlNode* lutnode = new wxXmlNode(*(lutdocument.GetRoot()));

   wxString aux = wxString(wxT(RENDERIZATION_NODE)) + wxT("|")
         + wxString(wxT(LUT_NODE));
   wxXmlNode* oldlutnode = prasterelement->GetNode(aux.c_str());
   renderernode->InsertChildAfter(lutnode, oldlutnode);
   renderernode->RemoveChild(oldlutnode);
   delete oldlutnode;

   REPORT_DEBUG(prasterelement->GetXmlAsString().c_str());

   suri::LutRenderer::Parameters params = suri::LutRenderer::GetParameters(
         prasterelement->GetNode(wxT("")));

   // Verifico que vandas esten cargadas correctamente
   int inputband = 0;
   for (int i = 0; i < params.lut_.GetCount(); i++) {
      params.lut_.GetLookUpTable(i, inputband);
      if (inputband != i) {
         REPORT_DEBUG("D:Error en nro de bandas");
         return;
      }
   }

   // Verifico que tabla para banda 0 sea correcta
   std::map<double, double> table;
   params.lut_.GetLookUpTable(0, inputband).GetTable(table);
   std::stringstream keysstream;
   std::stringstream valuesstream;
   std::map<double, double>::const_iterator it;
   for (it = table.begin(); it != table.end(); it++) {
      keysstream << (*it).first;
      valuesstream << (*it).second;
      valuesstream << " ";
      keysstream << " ";
   }

   CPPUNIT_ASSERT_MESSAGE(
         "ERROR en datos de tabla",
         ( (keysstream.str().compare("3 15 600 1.79769e+308 ")==0) && (valuesstream.str().compare("1 2 3 3 ")==0) ));

   suri::Element::Destroy(prasterelement);
}

/**
 * Test de renderizado
 * Renderiza imagen de 4 pixel y le aplica mapa. Si el resultado
 * tiene valores incorrectos falla.
 */
void LutRendererTest::TestRender() {
   // Creo parameters con dato nodo
   std::string pathdir = __TEST_PATH__ "imagenes/red.bmp";
   suri::RasterElement* prasterelement = suri::RasterElement::Create(pathdir);
   wxXmlNode* renderernode = prasterelement->GetNode(wxT(RENDERIZATION_NODE));

   pathdir = __TEST_PATH__ "imagenes/lut.xml";

   wxXmlDocument lutdocument(pathdir);
   wxXmlNode* lutnode = new wxXmlNode(*(lutdocument.GetRoot()));

   wxString aux = wxString(wxT(RENDERIZATION_NODE)) + wxT("|")
         + wxString(wxT(LUT_NODE));
   wxXmlNode* oldlutnode = prasterelement->GetNode(aux.c_str());
   renderernode->InsertChildAfter(lutnode, oldlutnode);
   renderernode->RemoveChild(oldlutnode);
   delete oldlutnode;

   int width, height = 0;
   suri::Subset worldextent;
   prasterelement->GetElementExtent(worldextent);
   prasterelement->GetRasterSize(width, height);
   suri::World world;
   world.SetSpatialReference(prasterelement->GetSpatialReference().c_str());
   world.SetViewport(width, height);
   world.SetWorld(worldextent);
   world.SetWindow(worldextent);

   suri::Mask *pmask = NULL;
   suri::RenderPipeline* pipeline = suri::RenderPipeline::Create(prasterelement);
   suri::Canvas* mc = new suri::MemoryCanvas();
   pipeline->Render(&world, mc, pmask);

   // Leo los datos en el canvas
   std::vector<int> bands(3);
   std::vector<void*> data(3);
   for (int b = 0; b < 3; b++) {
      bands[b] = b;
      data[b] = new unsigned char[width * height * mc->GetDataSize()];
   }
   mc->Read(bands, data);

   // Defino streams para cargar datos
   std::stringstream valuesstream;
   // Recorro map y cargo nodos(no cargo el ultimo)
   for (int i = 0; i < 3; i++) {
      unsigned char* datachar = (unsigned char*) data[i];
      for (int j = 0; j < 4; j++) {
         valuesstream << wxString::Format("%d", datachar[j]).c_str() << " ";
      }
   }

   CPPUNIT_ASSERT_MESSAGE("ERROR en render",
                          valuesstream.str()!="2 3 1 1 4 4 4 6 9 7 9 7 ");

   suri::Element* pelement = prasterelement;
   suri::Element::Destroy(pelement);
}
}  // namespace suri

