#ifndef TRD_KALMAN_TRACKING
#define TRD_KALMAN_TRACKING

using namespace std;
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "TString.h"

#include "TObject.h"


#include<TMath.h>

//for generator
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TFile.h"

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TChain.h"
#include "TTree.h"
#include "TMath.h"
#include <Math/BinaryOperators.h>
#include "Math/MConfig.h"
#include <iosfwd>
#include "Math/Expression.h"
#include "Math/MatrixRepresentationsStatic.h"
#include "Math/SMatrix.h"
#include "Math/MatrixFunctions.h"
#include "Ali_TRD_ST.h"
#include "Ali_TRD_ST_LinkDef.h"

ClassImp(Ali_TRD_ST_Tracklets)

//#include "TRD_Kalman_Tracking_Source.cxx"

class TRD_Kalman_Trackfinder
{
 private:
  ROOT::Math::SMatrix<double, 5, 5> mCov;      //Covariance Matrix
  ROOT::Math::SMatrix<double, 4, 5> mObs;      //Observation Model Matrix
  ROOT::Math::SMatrix<double, 5, 5> mTau;      //Propagation uncertainty Matrix
  ROOT::Math::SMatrix<double, 4, 4> mSig;      //Measure uncertainty Matrix
  ROOT::Math::SMatrix<double, 5, 4> mKal_Gain; //Kalman Gain Matrix
  ROOT::Math::SMatrix<double, 4, 4> mCov_Res_Inv;   
  //Double_t curv;							//curvature
  ROOT::Math::SVector<double, 5> mMu;                       	//current estimate
  vector<ROOT::Math::SVector<double, 5>> mEstimate;   		//estimates of this track
  vector<vector<ROOT::Math::SVector<double, 5>>> mEstimates; 	//estimates of all tracks
  ROOT::Math::SVector<double, 4>* mMeasurements;
  Double_t mChi_2;
  Double_t mDist;
  Int_t mCurrent_Det;
  Int_t mPrimVertex;
  ROOT::Math::SVector<double, 4> mUnc;
  ROOT::Math::SVector<double, 4> mMu_red;
  ROOT::Math::SVector<double, 4> mRes;

  TH1D* h_layer_radii;

  Double_t b_field	=	0.5;

  vector<vector<Double_t>> mHelices; // Kalman helix parameters, based on AliHelix

  Double_t mTRD_layer_radii[6][3] = {
    { 0,297.5, 306.5 },
    { 0,310.0, 320.0 },
    { 0,323.0, 333.0 },
    { 0,336.0, 345.5 },
    { 0,348.0, 357.0 },
    { 0,361.0, 371.0 }
  };

  Double_t mTRD_layer_radii_all[540];

  vector<vector<Ali_TRD_ST_Tracklets*>> mSeed;
  vector< vector<Bool_t> >  mVisited;
  vector<vector<Ali_TRD_ST_Tracklets*>> mBins; //Bins for all Tracklets corresponding to each Module and Layer
                                               //how many entries there are per bin
  vector<Ali_TRD_ST_Tracklets*> mTrack;        //Array with the Tracklets of the current Track
  Int_t mNbr_tracklets;
  vector<vector<Ali_TRD_ST_Tracklets*>> mFound_tracks; //Array with the Tracklets of all found Tracks

  Bool_t mShow;
  Bool_t mSearch_tracklets;
  ROOT::Math::SVector<double, 4> measure(Ali_TRD_ST_Tracklets* tracklet);
  Bool_t fitting(Ali_TRD_ST_Tracklets* a, Ali_TRD_ST_Tracklets* b);
  void get_seed(Ali_TRD_ST_Tracklets** Tracklets, Int_t Num_Tracklets);
  Bool_t prediction(Double_t dist);
  void correction(ROOT::Math::SVector<double, 4> measure);
  //Bool_t fits(ROOT::Math::SVector<double, 4> measure);
  void Kalman(vector<Ali_TRD_ST_Tracklets*> start);

 public:
  vector< vector<Ali_TRD_ST_Tracklets*> > Kalman_Trackfit(vector< vector<Ali_TRD_ST_Tracklets*> > tracks,Int_t prim_vertex);
  vector<vector<Ali_TRD_ST_Tracklets*>> Kalman_Trackfind(Ali_TRD_ST_Tracklets** Tracklets, Int_t Num_Tracklets, Int_t prim_vertex);
  vector<vector<Double_t>> get_Kalman_helix_params();
  void set_layer_radii_hist(TH1D* h_layer_radii_in)
  {
      h_layer_radii = h_layer_radii_in;
      for(Int_t i_det = 0; i_det < 540; i_det++)
      {
          mTRD_layer_radii_all[i_det] = h_layer_radii ->GetBinContent(i_det+1);
		  //cout<<"Det:"<<i_det<<"Det_lay: "<<i_det%6<<" radii: "<<mTRD_layer_radii_all[i_det]<<endl;
      }
  }

};
	
#endif
