#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/ForwardGeometry/interface/ZdcHardcodeGeometryLoader.h"
#include "Geometry/ForwardGeometry/interface/IdealZDCTrapezoid.h"
#include "Geometry/ForwardGeometry/interface/ZdcGeometry.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <algorithm>

typedef CaloCellGeometry::CCGFloat CCGFloat ;

ZdcHardcodeGeometryLoader::ZdcHardcodeGeometryLoader() :
theTopology ( new ZdcTopology ) ,
extTopology ( theTopology )
{
    init();
}

ZdcHardcodeGeometryLoader::ZdcHardcodeGeometryLoader(const ZdcTopology& ht) :
theTopology( 0   ) ,
extTopology( &ht )
{
    init();
}

void ZdcHardcodeGeometryLoader::init()
{
}

ZdcHardcodeGeometryLoader::ReturnType
ZdcHardcodeGeometryLoader::load(DetId::Detector /*det*/, int subdet)
{
    ReturnType hg(new ZdcGeometry( extTopology ) );
    if(subdet == HcalZDCDetId::SubdetectorId)
    {
        fill(HcalZDCDetId::EM  ,hg );
        fill(HcalZDCDetId::RPD ,hg );
        fill(HcalZDCDetId::HAD ,hg );
    }
    return hg;
}

ZdcHardcodeGeometryLoader::ReturnType
ZdcHardcodeGeometryLoader::load()
{
    ReturnType hg(new ZdcGeometry( extTopology ) );
    fill(HcalZDCDetId::EM  ,hg );
    fill(HcalZDCDetId::RPD ,hg );
    fill(HcalZDCDetId::HAD ,hg );
    return hg;
}

void ZdcHardcodeGeometryLoader::fill( HcalZDCDetId::Section section,
                                     ReturnType            geom     )
{
    // start by making the new HcalDetIds
    std::vector<HcalZDCDetId> zdcIds;
    HcalZDCDetId id;
    int firstCell = extTopology->firstCell(section);
    int lastCell = extTopology->lastCell(section);
    for(int ichannel = firstCell; ichannel <= lastCell; ++ichannel) {
        id = HcalZDCDetId(section, true, ichannel);
        if(extTopology->valid(id)) zdcIds.push_back(id);
        id = HcalZDCDetId(section, false, ichannel);
        if(extTopology->valid(id)) zdcIds.push_back(id);
    }
    if( geom->cornersMgr() == 0 ) geom->allocateCorners( HcalZDCDetId::kSizeForDenseIndexing ) ;
    if( geom->parMgr()     == 0 ) geom->allocatePar(
                                                    ZdcGeometry::k_NumberOfParametersPerShape*ZdcGeometry::k_NumberOfShapes,
                                                    ZdcGeometry::k_NumberOfParametersPerShape ) ;
    
    edm::LogInfo("ZdcHardcodeGeometry") << "Number of ZDC DetIds made: " << section << " " << zdcIds.size();
    
    // for each new HcalZdcDetId, make a CaloCellGeometry
    
    for(std::vector<HcalZDCDetId>::const_iterator zdcIdItr = zdcIds.begin();
        zdcIdItr != zdcIds.end(); ++zdcIdItr)
    {
        makeCell( *zdcIdItr, geom ) ;
    }
}

void
ZdcHardcodeGeometryLoader::makeCell(const HcalZDCDetId& detId,
                                    ReturnType          geom) const
{
    double zside ( detId.zside() ) ;
    
    const HcalZDCDetId::Section section ( detId.section() ) ;
    
    const int channel ( detId.channel() ) ;
    
//********* Here are all the hardcoded numbers you need to know, in **cm**
//********* Most are from the zdc.xml and zdclum.xml files ******
    
    static const double x0 ( 0 ) ; // these 3 are for the "mother" volume
    static const double y0 ( 0 ) ;
    static const double z0 ( 14000 ) ;
    
    static const double angEM  ( 0 ) ; // the angles of front face wrt vertical
    static const double angHAD ( atan( 1. ) ) ; // this is 45 deg
    static const double angRPD ( 0 ) ;
    
    // these dimensions are **half**-sizes
    
    static const double dxHAD ( 4.8 ) ;
    static const double dxEM  ( dxHAD/5. ) ;
    static const double dxRPD ( dxHAD/4. ) ;
    
    static const double dhEM  ( 6.25 ) ;
    static const double dhHAD ( 6.25 ) ;
    static const double dhRPD ( dhEM/4. ) ;
    
    static const double dzEM  ( 33.*0.15 ) ;
    static const double dzHAD ( 0.82*6./cos(angHAD) ) ;
    static const double dzRPD ( 2.5 ) ;
    
    // these are not half-dimensions, they are offsets from nominal
    // for the center-of-front-face points
    
    static const double xOffEM  ( -4.*dxEM ) ;
    static const double xOffHAD ( 0 ) ;
    static const double xOffRPD ( -3.*dxRPD ) ;
    
    static const double yOffEM  ( 0 ) ;
    static const double yOffHAD ( 0 ) ;
    static const double yOffRPD ( -3.*dxRPD ) ;
    
    static const double zOffEM  ( -49.85  - 0.15 ) ;
    static const double zOffHAD ( -29.00         ) ;
    static const double zOffRPD ( -34.85        ) ;
    
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    double dx, dh, dz, x, y, z, an ;
    
    if( section==HcalZDCDetId::EM )
    {
        dx = dxEM ;
        dh = dhEM ;
        dz = dzEM ;
        an = angEM ;
        x  = zside*( x0 + xOffEM + ( channel - 1.0 )*dxEM*2. ) ;
        y  = y0 + yOffEM ;
        z  = zside*( z0 + zOffEM ) ;
    }
    else
    {
        if( section==HcalZDCDetId::RPD )
        {
            dx = dxRPD ;
            dh = dhRPD ;
            dz = dzRPD ;
            an = angRPD ;
            x  = zside*( x0 + xOffRPD + ( channel - 1.0 )*dxRPD*2. ) ;
            y  = y0*( y0 + yOffRPD + ( channel - 1.0 )*dhRPD*2. ) ;
            z  = zside*( z0 + zOffRPD ) ;
        }
        else
        {
            assert( section==HcalZDCDetId::HAD ) ;
            dx = dxHAD ;
            dh = dhHAD ;
            dz = dzHAD ;
            an = angHAD ;
            x  = zside*( x0 + xOffHAD ) ;
            y  = y0 + yOffHAD ;
            z  = zside*( z0 + zOffHAD + ( channel - 1.0 )*dzHAD*2. ) ;
        }
    }
    
    const GlobalPoint fc ( x, y, z );
    
    const double dy ( dh*cos( an ) ) ;
    
    std::vector<CCGFloat> zz ;
    zz.reserve( ZdcGeometry::k_NumberOfParametersPerShape ) ;
    zz.push_back( an ) ;
    zz.push_back( dx ) ;
    zz.push_back( dy ) ;
    zz.push_back( dz ) ;
    
    geom->newCell( fc, fc, fc,
                  CaloCellGeometry::getParmPtr( zz,
                                               geom->parMgr(),
                                               geom->parVecVec() ),
                  detId ) ;
}

