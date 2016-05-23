import FWCore.ParameterSet.Config as cms

XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring('Geometry/CMSCommonData/data/materials.xml', 
        'Geometry/ForwardCommonData/data/zdcmaterials.xml', 
        'Geometry/ForwardCommonData/data/zdcrotations.xml', 
        'Geometry/ForwardCommonData/data/zdcworld.xml', 
        'Geometry/ForwardCommonData/data/zdc.xml'), 
    rootNodeName = cms.string('zdcworld:ZDCWorld')
)


