/// get_cic_yields.C
/// ----------------
/// A simple example showing the use of the CiCCategoryReader in interpreted
/// code. Prints the CiC yields to standard outputs and creates
/// png plots of histogram of mass and leading photon energy.
/// USAGE: root -l -b -q get_cic_yields.C
{
  /// Make sure we can use the mithep::hgg::CiCCategoryReader class
  loadmylib("MitHgg", "PhotonTree");
  using namespace mithep;
  using namespace hgg;  // mithep::hgg
  
  /// Configuration
  TString inputBase = "/home/mingyang/cms/hist/hgg-2013Final8TeV/merged";
  TString inputFileName = "hgg-2013Final8TeV_r12d-dph-j22-v1_noskim.root";
  TString dirName = "PhotonTreeWriterCiC";
  TString treeName = "hPhotonTree";
  int beamEnergy = CiCCategoryReader::k8TeV;
  TString filterList = "/home/veverka/cms/cmssw/031/CMSSW_5_3_10_patch1/"
                       "src/MitHgg/Tools/data/"
                       "HCALLaser2012AllDatasets_and_ECALLaserFiltered.txt.gz";
  
  /// Book a histo to store the yields per category
  TH1F hcat   ("hcat"   , ";cat;events"          , 16 , -0.5, 15.5);
  
  /// Book a couple more histos for illustration
  TH1F hmass  ("hmass"  , ";mass;events"         , 160, 110 , 150 );
  TH1F hph1e  ("hph1e"  , ";lead pho E;events"   , 100, 0   , 500 );
  TH1F hph1eta("hph1eta", ";lead pho #eta;events", 120, -3  , 3   );
  
  
  /// Initiliaze the CiCCategoryReader
  TFile *ifile = TFile::Open(inputBase + "/" + inputFileName);
  TDirectory *dir = (TDirectory*) ifile->FindObjectAny(dirName);
  TTree *tree = (TTree*) dir->Get(treeName);
  CiCCategoryReader reader(tree, beamEnergy, filterList.Data());
    
  /// Loop over all events and fill the histos
  for (int i=0; i<reader.GetEntries(); ++i) {
    reader.GetEntry(i);
    hcat   .Fill(reader.cicCat         );
    /// Can access any PhotonTreeWriterDiphotonEvent member like this
    hmass  .Fill(reader.mass           );
    /// Can access any scalar PhotonTreeWriterPhoton member like this
    /// (This doesn't work for arrays)
    hph1e  .Fill(reader.ph1.GetF("e"  ));
    hph1eta.Fill(reader.ph1.GetF("eta"));
    /// The GetF("e") and GetF("eta") is a workaround for Cint because the
    /// PhotonTreeWriter members are private and Cint doesn't support
    /// references. In a compiled code insted, you can use the public
    /// references reader.ph1.e, reader.ph1.eta etc.
  }
  
  /// Print the yields
  for (int bin=1; bin<17; bin++) {
    printf("cat %2.f: %5.f\n", hcat.GetBinCenter(bin), hcat.GetBinContent(bin));
  }
  
  /// Plot the mass and lead photon energy
  TCanvas canvas;
  hmass.Draw();
  canvas.Print("mass.png");
  hph1e.Draw();
  canvas.Print("hp1e.png");
}