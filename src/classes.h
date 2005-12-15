#include "FWCore/EDProduct/interface/Wrapper.h"
#include "DataFormats/TrackReco/interface/HelixParameters.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/RecHit.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include <vector>

namespace {
  namespace {
    std::vector<reco::Track> v1;
    edm::Wrapper<std::vector<reco::Track> > c1;
    edm::Ref<std::vector<reco::Track> > r1;
    edm::RefVector<std::vector<reco::Track> > rv1;

    std::vector<reco::RecHit> v2;
    edm::Wrapper<std::vector<reco::RecHit> > c2;
    edm::Ref<std::vector<reco::RecHit> > r2;
    edm::RefVector<std::vector<reco::RecHit> > rv2;

    std::vector<reco::TrackExtra> v3;
    edm::Wrapper<std::vector<reco::TrackExtra> > c3;
    edm::Ref<std::vector<reco::TrackExtra> > r13;
    edm::RefVector<std::vector<reco::TrackExtra> > rv13;
  }
}
